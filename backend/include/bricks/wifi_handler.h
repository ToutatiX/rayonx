#ifdef CONNECT_TO_WIFI

    #include <WiFi.h>
    #include <configs/wifi_config.h>

    namespace WifiHandler {

        bool activated = true;
        String ssid;
        String password;
        String hostname;
        bool staticIPConfig;

        IPAddress localIP;
        IPAddress gatewayIP;
        IPAddress subnetMask;
        IPAddress dnsIP1;
        IPAddress dnsIP2;

        namespace {
            HttpEndpoint<WiFiSettings> _httpEndpoint;
            FSPersistence<WiFiSettings> _fsPersistence;
            unsigned long _lastConnectionAttempt;
            bool _stopping;
        } 

        void read(WiFiSettings& settings, JsonObject& root) {
            // connection settings
            root["ssid"] = settings.ssid;
            root["password"] = settings.password;
            root["hostname"] = settings.hostname;
            root["static_ip_config"] = settings.staticIPConfig;

            // extended settings
            JsonUtils::writeIP(root, "local_ip", settings.localIP);
            JsonUtils::writeIP(root, "gateway_ip", settings.gatewayIP);
            JsonUtils::writeIP(root, "subnet_mask", settings.subnetMask);
            JsonUtils::writeIP(root, "dns_ip_1", settings.dnsIP1);
            JsonUtils::writeIP(root, "dns_ip_2", settings.dnsIP2);
        }

        static StateUpdateResult update(JsonObject& root, WiFiSettings& settings) {
            settings.ssid = root["ssid"] | FACTORY_WIFI_SSID;
            settings.password = root["password"] | FACTORY_WIFI_PASSWORD;
            settings.hostname = root["hostname"] | SettingValue::format(FACTORY_WIFI_HOSTNAME);
            settings.staticIPConfig = root["static_ip_config"] | false;

            // extended settings
            JsonUtils::readIP(root, "local_ip", settings.localIP);
            JsonUtils::readIP(root, "gateway_ip", settings.gatewayIP);
            JsonUtils::readIP(root, "subnet_mask", settings.subnetMask);
            JsonUtils::readIP(root, "dns_ip_1", settings.dnsIP1);
            JsonUtils::readIP(root, "dns_ip_2", settings.dnsIP2);

            // Swap around the dns servers if 2 is populated but 1 is not
            if (IPUtils::isNotSet(settings.dnsIP1) && IPUtils::isSet(settings.dnsIP2)) {
            settings.dnsIP1 = settings.dnsIP2;
            settings.dnsIP2 = INADDR_NONE;
            }

            // Turning off static ip config if we don't meet the minimum requirements
            // of ipAddress, gateway and subnet. This may change to static ip only
            // as sensible defaults can be assumed for gateway and subnet
            if (settings.staticIPConfig && (IPUtils::isNotSet(settings.localIP) || IPUtils::isNotSet(settings.gatewayIP) ||
                                            IPUtils::isNotSet(settings.subnetMask))) {
            settings.staticIPConfig = false;
            }
            return StateUpdateResult::CHANGED;
        }

        void scanNetworks(AsyncWebServerRequest* request) {
            if (WiFi.scanComplete() != -1) {
                WiFi.scanDelete();
                WiFi.scanNetworks(true);
            }
            request->send(202);
        }

        void listNetworks(AsyncWebServerRequest* request) {

            int numNetworks = WiFi.scanComplete();

            if (numNetworks > -1) {
                AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_WIFI_SCANNER_SIZE);
                JsonObject root = response->getRoot();
                JsonArray networks = root.createNestedArray("networks");

                for (int i = 0; i < numNetworks; i++) {

                    JsonObject network = networks.createNestedObject();
                    network["rssi"] = WiFi.RSSI(i);
                    network["ssid"] = WiFi.SSID(i);
                    network["bssid"] = WiFi.BSSIDstr(i);
                    network["channel"] = WiFi.channel(i);

                    network["encryption_type"] = (uint8_t)WiFi.encryptionType(i);
                }

                response->setLength();
                request->send(response);

            } else if (numNetworks == -1) {
                request->send(202);
            } else {
                scanNetworks(request);
            }
        }

        void WiFiSettingsService::begin() {
            _fsPersistence.readFromFS();
            reconfigureWiFiConnection();
        }

        void WiFiSettingsService::reconfigureWiFiConnection() {
            // reset last connection attempt to force loop to reconnect immediately
            _lastConnectionAttempt = 0;

            // disconnect and de-configure wifi
            if (WiFi.disconnect(true)) {
                _stopping = true;
            }
        }

        void WiFiSettingsService::manageSTA() {
            // Abort if already connected, or if we have no SSID
            if (WiFi.isConnected() || _state.ssid.length() == 0) {
                return;
            }
            // Connect or reconnect as required
            if ((WiFi.getMode() & WIFI_STA) == 0) {
                Serial.println(F("Connecting to WiFi."));
                if (_state.staticIPConfig) {
                // configure for static IP
                WiFi.config(_state.localIP, _state.gatewayIP, _state.subnetMask, _state.dnsIP1, _state.dnsIP2);
                }
                else {
                    // configure for DHCP
                    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
                    WiFi.setHostname(_state.hostname.c_str());
                }
                // attempt to connect to the network
                WiFi.begin(_state.ssid.c_str(), _state.password.c_str());
            }
        }

        void WiFiSettingsService::loop() {
            unsigned long currentMillis = millis();
            if (!_lastConnectionAttempt || (unsigned long)(currentMillis - _lastConnectionAttempt) >= WIFI_RECONNECTION_DELAY) {
                _lastConnectionAttempt = currentMillis;
                manageSTA();
            }
        }

        WiFiSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) {
            // We want the device to come up in opmode=0 (WIFI_OFF), when erasing the flash this is not the default.
            // If needed, we save opmode=0 before disabling persistence so the device boots with WiFi disabled in the future.
            if (WiFi.getMode() != WIFI_OFF) {
                WiFi.mode(WIFI_OFF);
            }

            // Disable WiFi config persistance and auto reconnect
            WiFi.persistent(false);
            WiFi.setAutoReconnect(false);
            #ifdef ESP32
            // Init the wifi driver on ESP32
            WiFi.mode(WIFI_MODE_MAX);
            WiFi.mode(WIFI_MODE_NULL);
            WiFi.onEvent(
                std::bind(&WiFiSettingsService::onStationModeDisconnected, this, std::placeholders::_1, std::placeholders::_2),
                WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
            WiFi.onEvent(std::bind(&WiFiSettingsService::onStationModeStop, this, std::placeholders::_1, std::placeholders::_2),
                        WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_STOP);

            addUpdateHandler([&](const String& originId) { reconfigureWiFiConnection(); }, false);

        }
    }

#else
    namespace WifiHandler {
        bool activated = false;
    }
#endif

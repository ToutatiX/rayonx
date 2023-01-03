
/* ***********************************************************************
 *  This is used to configure and establish a WLAN-connection
 *  It works the following way:
 *  - Loading cerdentials from flash
 *       - ssid
 *       - password
 *  - Try to establish a connection with these credentials
 *  - If there is noch connection possible, following will happen:
 *      - a change in AP-mode (NOT rebooting!!!) 
 *      - user calls http://192.168.4.1
 *      - displays HTML-code from wlanconf.h with filled in credentials
 *      - user puts correct ceredentials in it
 *      - when sending the form by submit, the credentials are updated in flash (ONLÝ THEN*)
 *      - reboots in station-mode and begins the next try to connect with new credentials
 *  => This happens infinite, until connection can be established
 *      
 *  It is used bei call of "makeWLAN()" (usaly in 'setup()')
 *  It returns "true" when connection successful
 *  
 *  Libraries which are used and included here (no need to include them otherwhere for this use)
 *    - Wifi.h
 *    - Preferences.h
 *    - ESPAsyncWebServer.h
 *  
 *  The webserver-instance (only when in AP-mode) is called "server"
 *  
 *  When TESTMODUS is true, after a successful connection a wrong password is stored in flash. So
 *  it won't connect after next reboot and starts the procedure from the beginning.
 *  
 *  *: When your WLAN is offline and that's why this device asks for new data, you can ignore it
 *     and reboot this device, when your WLAN is right back - the credentials won't change until
 *     you submit the form with new ones.
 */

#include <WiFi.h>
#include <DNSServer.h>
#include "ESPAsyncWebServer.h"
#include "interface.h"

namespace Wlan {

  /**
   * Private namespace
  */
  namespace {

    AsyncWebServer server(80);
    DNSServer dnsServer;
    IPAddress apIP(8,8,8,8); 

    String ssid;
    String password;

    unsigned long currentMillis;
    unsigned long previousMillis = 0; 

    enum MODE {
      STA,
      ERR,
      AP,
    };

    MODE mode;

    /**
     * @brief Handling lost of connection.
     * @param event: WifiEvent
     * @param info: WifiEventInfo
    */
    void wifiDisconnectionHandler(WiFiEvent_t event, WiFiEventInfo_t info){
      Log::log(Log::LEVEL_STATS, Log::WLAN, "Wifi Error code: " + String(info.wifi_sta_disconnected.reason));
      Feedback::status = Feedback::MALFUNCTION;
      mode = ERR;
      AudioPlayer::stop();
    }


    /**
     * @brief Connect to wifi
     * @param ssid: the ssid of the wifi network
     * @param pwd: the password of the wifi network
     * @return return true if successful, false otherwise
    */
    bool connectToWiFi(String ssid, String pwd) {

        Log::log(Log::LEVEL_STATS, Log::WLAN, "Connect to wifi: " + ssid + " using pwd: " + pwd);
        WiFi.disconnect(true);
        WiFi.softAPdisconnect(true);
        delay(100);
        WiFi.begin(ssid.c_str(), pwd.c_str());

        if (WiFi.waitForConnectResult(WIFI_TIMEOUT) == WL_CONNECTED) {
          Log::log(Log::LEVEL_GOOD_NEWS, Log::WLAN, "Wifi connection successfull!");
          Log::log(Log::LEVEL_VERBOSE, Log::WLAN, "IP Address: " + String(WiFi.localIP()));
          mode = STA;
          WiFi.onEvent(wifiDisconnectionHandler, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
          mode == STA;
          Feedback::status = Feedback::READY;
          return true;
        }
        else {
          Log::log(Log::LEVEL_ERROR, Log::WLAN, "Failed to connect to wifi");
        }
        return false;
    }
  }

  /**
   * @brief Find the number of available networks
   * @param nb_trials: number of trials
  */
  int scanNetworks(int nb_trials = 1) {

    int numberOfNetworks = -2;
    int tryCount = 1;
    WiFi.disconnect(true);


    while ((numberOfNetworks<1) && (tryCount<nb_trials)) {
      delay(WIFI_CHECK_DELAY);
      numberOfNetworks = WiFi.scanNetworks();
      tryCount++;
    }

    return numberOfNetworks;
  }

  /**
   * @brief Checking for connections on the DNS
  */
  void loopAP() {
    dnsServer.processNextRequest();
  }

  /**
   * @brief Checking for connection and reconnect if an error occured.
  */
  bool loopSTA() {
    if (mode == ERR) {
      Log::log(Log::LEVEL_VERBOSE, Log::WLAN, "Reconnecting to wifi");
      bool result = connectToWiFi(ssid, password);
      if (result) {
        AudioPlayer::play();
      }
      return result;
    }
    else {
      return true;
    }
  }

  /**
   * @brief Connect to the Wifi Network.
   * @return true is succesfull, false otherwise
  */
  bool makeWLAN() {
      Memory::read(&ssid, &password);

      WiFi.disconnect(true);
      WiFi.softAPdisconnect(true);
      WiFi.mode(WIFI_STA);

      if (ssid == "" || password == "") {
        return false;
      }

      return connectToWiFi(ssid, password);   
  }

  /**
   * @brief Create an AccessPoint and stream the HTML page defined in interface.h
  */
  void createAP() {

    Log::log(Log::LEVEL_STATS, Log::WLAN, "Starting Access Point with ssid RayonX");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("RayonX");
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    dnsServer.start(53, "*", apIP);
    mode = AP;

    int numberOfNetworks = scanNetworks(10);

    server.onNotFound([numberOfNetworks](AsyncWebServerRequest *request){
      int paramsNr = request->params();
      int paramCountOk = 0;
      int gotAnswer = false;
    
      for(int i=0;i<paramsNr;i++){
    
        AsyncWebParameter* p = request->getParam(i);

        if (p->name() == "ssid") {
          ssid = p->value();
          paramCountOk++;
        }
        if (p->name() == "password") {
          password = p->value();
          paramCountOk++;
        }

        if (paramCountOk >= paramsNr) {
          Log::log(Log::LEVEL_STATS, Log::FS, "Input from user. Got: " + ssid + " | " + password);
          gotAnswer = true;
        }
      }

      String s = MAIN_page; 

      Log::log(Log::LEVEL_STATS, Log::WLAN, "Found " + String(numberOfNetworks) + " newtorks");
      
      s.replace("*password*", password);

      if (numberOfNetworks < 1) {
        s.replace("*option*", "<option value='none'>No wifi found</option>");
      }
      else {
        String SSID_strings = "";

        for(int i =0; i<numberOfNetworks; i++){
            SSID_strings += "<option value='"+String(WiFi.SSID(i))+"'>"+String(WiFi.SSID(i))+"</option>" ;
        }

        s.replace("*option*", SSID_strings);
      }

      if (gotAnswer) {
        Log::log(Log::LEVEL_STATS, Log::WLAN, "Connect to wifi: " + ssid + " using pwd: " + password);

        WiFi.begin(ssid.c_str(), password.c_str());
        
        if (WiFi.waitForConnectResult(WIFI_CHECK_DELAY) == WL_CONNECTED) {
          Memory::update(ssid, password);
          delay(200);
          Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Restarting ESP.");
          ESP.restart();
        }
        else {
          Log::log(Log::LEVEL_WARNING, Log::WLAN, "User input not working :(");
        }
      }

      request->send(200, "text/html", s);
      Log::log(Log::LEVEL_WARNING, Log::WLAN, "Server.notfound triggered: " + String(request->url()));

    }); 

    server.begin();       
  }
}
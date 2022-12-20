
/* ***********************************************************************
 *  This is used to configure and establish a WLAN-connection
 *  It works the following way:
 *  - Loading cerdentials from flash
 *       - ssid
 *       - password
 *       - ntpserver
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
 *  The WLAN-instance is called "WiFi"
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
#include "ESPAsyncWebServer.h"

#include "wlanconf.h"  //HTML-code for WLAN-setup

  
  // Webserver on port 80
  AsyncWebServer server(80);

  // WLAN credentials
  String ssid;
  String password;
  String ntpserver;
  int    bootAsAP;
  #define MAXWLANTRY 6  // try to connect with stored credentials MAXWLANTRY times, before switching
                        // to AP-mode and asking for new credentials

                        // ### Here a simple loop to display status on //Serial, the webserver runs asynchronous
  void serverLoop() {
    //Serial.println("Entering server loop...");
    while (true) {
      delay(500);
    }
  }
  // ### Usally should be called in 'setup()'
  bool makeWLAN()
  {
    preferences.begin("credentials", false);

    ssid = preferences.getString("ssid", ""); 
    password = preferences.getString("password", "");

    preferences.end();
    //ntpserver = preferences.getString("ntpserver", "at.pool.ntp.org");
      
    delay(250);

    WiFi.disconnect();
    WiFi.softAPdisconnect(true);

    if (ssid == "" || password == "") {
      return false;
    }

    Log::log(Log::LEVEL_STATS, Log::WLAN, "Connect to wifi: " + String(ssid) + " using pwd: " + String(password));

    int tryCount = 0;

    if (tryCount < MAXWLANTRY) {
      // Mit Wi-Fi verbinden
      WiFi.mode(WIFI_AP_STA);
      WiFi.begin(ssid.c_str(), password.c_str());
      //Serial.print("Connecting to wifi..");
      while ((WiFi.status() != WL_CONNECTED) && (tryCount<MAXWLANTRY)) {
        Serial.print('.');
        delay(1000);
        tryCount++;
      }
      Serial.print('\n');
      IPAddress lip(0,0,0,0);
      if (WiFi.localIP() == lip) {
        tryCount = MAXWLANTRY;
      }
    }

    if (tryCount < MAXWLANTRY) {
      Log::log(Log::LEVEL_GOOD_NEWS, Log::WLAN, "Wifi connection successfull!");
      Log::log(Log::LEVEL_VERBOSE, Log::WLAN, "IF Addrese: " + String(WiFi.localIP()));
      //Serial.print(F("IP-Adresse is: "));
      //Serial.println(WiFi.localIP());  
      // Für den Automatik-Test
      return(true);   // when connection ok, stop here and return positive
    }

    //Serial.println("Could not connect to wifi");

    // when no connection possible, start the configuration website in AP-mode 
    // makeConfigAP();

    return(false);    // will never be reached, because 'makeConfigAP()' is infinite or
                      // reboots when user entered new credentilas
  }


    // ### Builds an own WLAN "WEBRADIO" and serves under http://192.168.4.1 a webinterface
  // This function is infinite, until new credentials ar given - in this case it reboots
  void makeConfigAP() {

    //Serial.print("Starting AP (Access Point)…");
    Log::log(Log::LEVEL_STATS, Log::WLAN, "Starting Access Point with ssid RayonX");
    WiFi.mode(WIFI_AP);
    delay(500);  
    WiFi.softAP("RayonX");

    IPAddress IP = WiFi.softAPIP();
    //Serial.print("AP IP address: ");
    //Serial.println(IP);
    
    server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){

      int paramsNr = request->params();
      //Serial.println(paramsNr);
      String ssid = "";
      String password = "";
      int paramCountOk = 0;
      int gotAnswer = false;
    
      for(int i=0;i<paramsNr;i++){
    
        AsyncWebParameter* p = request->getParam(i);
        //Serial.print("Param name: ");
        //Serial.println(p->name());
        //Serial.print("Param value: ");
        //Serial.println(p->value());
        //Serial.println("------");
        if (p->name() == "ssid") {
          ssid = p->value();
          paramCountOk++;
        }
        if (p->name() == "password") {
          password = p->value();
          paramCountOk++;
        }

        if (paramCountOk >= paramsNr) {
          Log::log(Log::LEVEL_STATS, Log::FS, "Input from user. Update memory with new parameters");
          preferences.begin("credentials", false);
          preferences.putString("ssid", ssid); 
          preferences.putString("password", password); 
          preferences.end();
          gotAnswer = true;
        }
      }

      String s = MAIN_page; 

      if (gotAnswer) {
        //Serial.println("Got new credentials from the user :)");
        // delay(3000);
        Log::log(Log::LEVEL_STATS, Log::WLAN, "Connect to wifi: " + String(ssid) + " using pwd: " + String(password));
        WiFi.begin(ssid.c_str(), password.c_str());

        delay(500);

        if (WiFi.status() == WL_CONNECTED) {
          Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Restarting ESP.");
          ESP.restart();
        }
        else {
          // s.replace("*mark1begin*", "");
          // s.replace("*mark1end*", "");
          // s.replace("*feedback*", "<div style='color:#CC0000'>Failed to connect using the given credentials...</div>");
          s.replace("*ssid*", ssid);
          s.replace("*password*", password);
          Log::log(Log::LEVEL_WARNING, Log::WLAN, "User input not working...");
          
        }
      }

      request->send(200, "text/html", s);

    }); 

    server.begin();       
    //Serial.println("HTTP-Server started");

    serverLoop();
  }
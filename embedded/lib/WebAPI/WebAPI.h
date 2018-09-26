/*
    WebAPI.h - Library for networkcommunication.
    Created by Glenn Huber, 26.04.2018
    Basecode by Felix Nyffenegger
*/
#ifndef WebAPI_h
#define WebAPI_h

#include "Arduino.h"
#include "WiFiClient.h"
#include "IPAddress.h"
#include "WiFi101.h"
#include "Configuration.h"
#include "ArduinoJson.h"
#include "Modular.h"
#include "Configuration.h"

class WebAPI : public Component
{
    public:
        WebAPI();
        void loop();
        String handleRequest(String req); 
        String handleGET(String url, String params);
        String handlePOST(String url, String content);
        String handlePUT(String url, String params);
        void sendRequest(String request, String requestType);
        void printJSONHeaders();
        void printHTMLHeaders();
        void sendGETRequest(String request);
        void sendPOSTRequest(String request);
        void sendPUTRequest(String request);
        void printWiFiStatus();

    ///////please enter your sensitive data in the Secret tab/arduino_secrets.h
    String ssid = SECRET_SSID;        // your network SSID (name)
    String pass = SECRET_PASS;       // your network password (use for WPA, or use as key for WEP)
    int keyIndex;   // your network key Index number (needed only for WEP)
    int status;
    byte serverIP[4] = { IP1, IP2, IP3, IP4 }; //Server at DigitalLab

    long rssi;
    String message;

    WiFiServer *server;
    WiFiClient client;
    IPAddress ip;
};

#endif
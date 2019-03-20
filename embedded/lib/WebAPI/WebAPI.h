/**
 * @file WebAPI.h
 * @brief Library for networkcommunication.
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * @author Felix Nyffenegger (felix.nyffenegger@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef WebAPI_h
#define WebAPI_h

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Configuration.h"
#include "IPAddress.h"
#include "LogConfiguration.h"
#include "Modular.h"
#include "WiFi101.h"
#include "WiFiClient.h"

/**
 * @brief 
 * 
 */
class WebAPI : public Component {
   public:
    /**
     * @brief Construct a new Web A P I object
     * 
     */
    WebAPI();

    /**
     * @brief 
     * 
     */
    void loop();

    /**
     * @brief 
     * 
     * @param req - 
     * @return String - 
     */
    String handleRequest(String req);

    /**
     * @brief 
     * 
     * @param url - 
     * @param params - 
     * @return String - 
     */
    String handleGET(String url, String params);

    /**
     * @brief 
     * 
     * @param url - 
     * @param content - 
     * @return String - 
     */
    String handlePOST(String url, String content);

    /**
     * @brief 
     * 
     * @param url - 
     * @param params - 
     * @return String - 
     */
    String handlePUT(String url, String params);

    /**
     * @brief 
     * 
     * @param request - 
     * @param requestType - 
     */
    void sendRequest(String request, String requestType);

    /**
     * @brief 
     * 
     */
    void printJSONHeaders();

    /**
     * @brief 
     * 
     */
    void printHTMLHeaders();

    /**
     * @brief 
     * 
     * @param request - 
     */
    void sendGETRequest(String request);

    /**
     * @brief 
     * 
     * @param request - 
     */
    void sendPOSTRequest(String request);

    /**
     * @brief 
     * 
     * @param request - 
     */
    void sendPUTRequest(String request);

    /**
     * @brief 
     * 
     */
    void printWiFiStatus();
    /**
     * 
    * @todo change secret data to .inti
    * 
    */
    ///////please enter your sensitive data in the Secret tab/arduino_secrets.h
    String ssid = SECRET_SSID;                ///< your network SSID (name)
    String pass = SECRET_PASS;                ///< your network password (use for WPA, or use as key for WEP)
    int keyIndex;                             ///< your network key Index number (needed only for WEP)
    int status;                               ///<
    byte serverIP[4] = {IP1, IP2, IP3, IP4};  ///<Server at DigitalLab

    long rssi;       ///<
    String message;  ///<

    WiFiServer *server;  ///<
    WiFiClient client;   ///<
    IPAddress ip;        ///<
};

#endif
// /**
//  * @file WebAPI.cpp
//  * @brief Library for networkcommunication.
//  *
//  * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
//  * @author Felix Nyffenegger (felix.nyffenegger@hsr.ch)
//  *
//  * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
//  * @version 1.0 - BA FTS FS 2018
//  *
//  * @date 2019-03-20
//  * @copyright Copyright (c) 2019
//  *
//  */

// #include "WebAPI.h"
// #include "Arduino.h"

// WebAPI::WebAPI() {
//     if (DEBUGGER == true) Serial.println("Initializing webAPI...");
//     keyIndex = KEY_INDEX;
//     status = WL_IDLE_STATUS;
//     server = new WiFiServer(LISTEN_PORT);

//     //Configure pins for Adafruit ATWINC1500 Feather
//     WiFi.setPins(WIFI_CS, WIFI_IRQ, WIFI_RST, WIFI_EN);

//     // check for the presence of the shield:
//     if (WiFi.status() == WL_NO_SHIELD) {
//         if (DEBUGGER == true) Serial.println("WiFi shield not present");
//         // don't continue:
//         while (true)
//             ;
//     }

//     // attempt to connect to WiFi network:
//     while (status != WL_CONNECTED) {
//         if (DEBUGGER == true) Serial.print("Attempting to connect to SSID: ");
//         if (DEBUGGER == true) Serial.println(ssid);
//         // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
//         status = WiFi.begin(ssid, pass);

//         // wait 10 seconds for connection:
//         delay(10000);
//     }

//     if (DEBUGGER == true) Serial.println("Connectet to wifi");
//     this->printWiFiStatus();
//     server->begin();
// }

// void WebAPI::printWiFiStatus() {
//     // print the SSID of the network you're attached to:
//     if (DEBUGGER == true) Serial.print("SSID: ");
//     if (DEBUGGER == true) Serial.println(WiFi.SSID());

//     // print your WiFi shield's IP address:
//     ip = WiFi.localIP();
//     if (DEBUGGER == true) Serial.print("IP Address: ");
//     if (DEBUGGER == true) Serial.println(ip);

//     // print the received signal strength:
//     rssi = WiFi.RSSI();
//     if (DEBUGGER == true) Serial.print("signal strength (RSSI):");
//     if (DEBUGGER == true) Serial.print(rssi);
//     if (DEBUGGER == true) Serial.println(" dBm");
// }

// void WebAPI::loop() {
//     if (!client) {
//         client = server->available();
//     } else {
//         if (client.available()) {
//             //read request
//             String req = client.readString();
//             //handle request
//             String response = handleRequest(req);
//             //send response
//             client.print(response.c_str());
//             client.stop();
//         }
//     }
// }

// void WebAPI::sendRequest(String request, String requestType) {
//     if (DEBUGGER == true) Serial.print("Ready to send request:");
//     if (DEBUGGER == true) Serial.println(request);
//     if (DEBUGGER == true) Serial.print("Requesttype:");
//     if (DEBUGGER == true) Serial.println(requestType);
//     if (client.connect(serverIP, SENDING_PORT)) {
//         if (requestType == "GET") {
//             if (DEBUGGER == true) Serial.println("Detected GET-Request");
//             sendGETRequest(request);
//             if (DEBUGGER == true) Serial.println("Request sended");
//         } else if (requestType == "PUT") {
//             sendPUTRequest(request);
//         } else {
//             sendPOSTRequest(request);
//         }
//         //Wait for incoming message
//         if (DEBUGGER == true) Serial.println("Waiting for incoming message");
//         //while(!client.available())
//         //{
//         //}
//         //If incoming message is available, print the message
//         while (client.available()) {
//             if (DEBUGGER == true) Serial.println("Message recieved");
//             message = client.readString();
//         }
//         client.stop();
//         if (DEBUGGER == true) Serial.println(message);
//     }
// }

// void WebAPI::sendGETRequest(String request) {
//     client.print("GET /");
//     client.print(request);
//     client.println(" HTTP/1.1");
//     client.println();
// }

// void WebAPI::sendPUTRequest(String request) {
//     client.print("PUT /");
//     client.print(request);
//     client.println(" HTTP/1.1");
//     client.println();
// }

// void WebAPI::sendPOSTRequest(String request) {
//     client.print("POST /");
//     client.print(request);
//     client.println(" HTTP/1.1");
//     client.println();
// }

// String WebAPI::handleRequest(String req) {
//     if (DEBUGGER == true) Serial.print("New request: ");
//     if (DEBUGGER == true) Serial.println(req);
//     int sep = req.indexOf("/");
//     String method = req.substring(0, sep - 1);
//     String fullUrl = req.substring(sep, req.indexOf("HTTP") - 1);
//     String url = fullUrl.substring(req.indexOf("?") - 1);
//     String params = fullUrl.substring(req.indexOf("?"));
//     if (method == "POST") {
//         if (DEBUGGER == true) Serial.println(req);
//         String content = req.substring(req.indexOf("\r\n\r\n"));
//         if (DEBUGGER == true) Serial.println(content);
//         return handlePOST(url, content);
//     } else if (method == "GET") {
//         return handleGET(url, params);
//     } else {
//         return handlePUT(url, params);
//     }
// }

// String WebAPI::handlePOST(String url, String content) {
//     size_t bufferSize = 3 * JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(4);
//     DynamicJsonBuffer jsonBuffer(bufferSize);
//     JsonObject& root = jsonBuffer.parseObject(content);
//     return "done";
// }

// String WebAPI::handleGET(String url, String params) {
//     printHTMLHeaders();
//     return "<html><h1>It works!</h1></html>";
// }

// String WebAPI::handlePUT(String url, String params) {
//     printHTMLHeaders();
//     return "<html><h1>IT works!</h1></html>";
// }

// void WebAPI::printJSONHeaders() {
//     //Set Headers for JSON-format
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: text/json");
//     client.println("");
// }

// void WebAPI::printHTMLHeaders() {
//     //Set Headers for HTML-format
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: text/html");
//     client.println("");
// }
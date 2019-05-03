// /**
//  * @file VehicleWebAPI.cpp
//  * @brief Library for specific networkcommunication from vehicle to other objects in network.
//  *
//  * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
//  *
//  * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
//  * @version 1.0 - BA FTS FS 2018
//  *
//  * @date 2019-03-20
//  * @copyright Copyright (c) 2019
//  *
//  */

// #include "VehicleWebAPI.h"
// #include "Arduino.h"

// VehicleWebAPI::VehicleWebAPI(ApiState *state) : WebAPI() {
//     if (DEBUGGER == true) Serial.println("Initializing webAPI for Vehicle...");
//     decideName();
//     registerVehicle();
//     apiState.state = "ready";
//     state->state = "ready";
//     if (DEBUGGER == true) Serial.println("System ready...");
// }

// void VehicleWebAPI::registerVehicle() {
//     if (client.connect(serverIP, SENDING_PORT)) {
//         if (DEBUGGER == true) Serial.println("Connectet to Server");
//         client.print("GET /register?id=");
//         client.print(apiState.vehicleID);
//         client.print("&ip=");
//         client.print(ip);
//         client.print("&state=");
//         client.print(apiState.state);
//         client.print("&task=");
//         client.print(apiState.task);
//         client.print("&sector=");
//         client.print(apiState.sector);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         while (client.available()) {
//             apiState.message = client.readString();
//             if (DEBUGGER == true) Serial.print("Recieved message: ");
//             if (DEBUGGER == true) Serial.println(apiState.message);
//         }
//         client.stop();
//     }
// }

// void VehicleWebAPI::decideName() {
//     IPAddress ipO = {IP1, IP2, IP3, IP4_Obelix};
//     IPAddress ipI = {IP1, IP2, IP3, IP4_Idefix};
//     if (ip == ipO) {
//         apiState.vehicleID = "Obelix";
//         if (DEBUGGER == true) Serial.print("ID: ");
//         if (DEBUGGER == true) Serial.println(apiState.vehicleID);
//     } else if (ip == ipI) {
//         apiState.vehicleID = "Idefix";
//         if (DEBUGGER == true) Serial.print("ID: ");
//         if (DEBUGGER == true) Serial.println(apiState.vehicleID);
//     } else {
//         apiState.vehicleID = "IP not known";
//         if (DEBUGGER == true) Serial.println("IP not known");
//     }
// }

// void VehicleWebAPI::loop(ApiState *state) {
//     if (!client) {
//         client = server->available();
//     } else {
//         if (DEBUGGER == true) Serial.println("listening");
//         if (client.available()) {
//             if (DEBUGGER == true) Serial.println("read request");
//             //read request
//             String req = client.readString();
//             String response = handleRequest(req);
//             //send response
//             client.print(response.c_str());
//             client.stop();
//             if (DEBUGGER == true) Serial.println("Request handled");
//         }
//     }
//     apiState.sectorChange = state->sectorChange;
//     apiState.stateChange = state->stateChange;
//     apiState.taskChange = state->taskChange;

//     if (DEBUGGER == true) Serial.print("Container api: ");
//     if (DEBUGGER == true) Serial.println(apiState.container);
//     if (DEBUGGER == true) Serial.print("port api: ");
//     if (DEBUGGER == true) Serial.println(apiState.unloadPort);
//     if (apiState.containerChange == true) {
//         state->container = apiState.container;
//         apiState.containerChange = false;
//     }

//     if (DEBUGGER == true) Serial.print("task api: ");
//     if (DEBUGGER == true) Serial.println(apiState.task);
//     if ((apiState.unloadPortChange == true) && ((apiState.task == "finished") || (apiState.task == "nothing"))) {
//         state->unloadPort = apiState.unloadPort;
//         Serial.println("Inside portchange api");
//         apiState.unloadPortChange = false;
//     }
//     if (state->unloadPortChange == true) {
//         apiState.unloadPort = state->unloadPort;
//         state->unloadPortChange = false;
//     }

//     if (state->containerChange == true) {
//         apiState.container = state->container;
//         state->containerChange = false;
//     }

//     if (apiState.workStateChange == true) {
//         state->workState = apiState.workState;
//         apiState.workStateChange = false;
//     }
//     if (state->workStateChange == true) {
//         apiState.workState = state->workState;
//         state->workStateChange = false;
//     }

//     //state->speed            = apiState.speed;
//     apiState.state = state->state;
//     apiState.task = state->task;
//     apiState.sector = state->sector;

//     if (apiState.state != apiState.previousState) {
//         apiState.stateChange = true;
//         apiState.previousState = apiState.state;
//     }
//     if (apiState.sector != apiState.previousSector) {
//         apiState.sectorChange = true;
//         apiState.previousSector = apiState.sector;
//     }
//     if (apiState.task != apiState.previousTask) {
//         apiState.taskChange = true;
//         apiState.previousTask = apiState.task;
//     }

//     state->stateChange = apiState.stateChange;
//     state->sectorChange = apiState.sectorChange;
//     state->taskChange = apiState.taskChange;

//     state->vehicleID = apiState.vehicleID;
//     apiState.jobCount = state->jobCount;
//     state->otherSector = apiState.otherSector;
//     state->otherTask = apiState.otherTask;
//     state->vehicleCount = apiState.vehicleCount;
// }

// void VehicleWebAPI::sendState() {
//     if (DEBUGGER == true) Serial.println("Inside sendState");
//     if (client.connect(serverIP, SENDING_PORT)) {
//         client.print("GET /vehicleState?state=");
//         client.print(apiState.state);
//         client.print("&id=");
//         client.print(apiState.vehicleID);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         while (!client.available()) {
//         }

//         if (DEBUGGER == true) Serial.println("Answer recieved");
//         apiState.message = client.readString();
//         if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// void VehicleWebAPI::sendTask() {
//     if (DEBUGGER == true) Serial.println("Inside sendTask");
//     if (client.connect(serverIP, SENDING_PORT)) {
//         if (DEBUGGER == true) Serial.println("Inside if-case");
//         client.print("GET /vehicleTask?task=");
//         client.print(apiState.task);
//         if (DEBUGGER == true) Serial.print("Task: ");
//         if (DEBUGGER == true) Serial.println(apiState.task);
//         client.print("&id=");
//         client.print(apiState.vehicleID);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         //while (!client.available()) {
//         //
//         //}
//         //if (DEBUGGER == true) Serial.println("Message recieved");
//         //apiState.message = client.readString();
//         //if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// void VehicleWebAPI::sendTaskToObelix() {
//     if (DEBUGGER == true) Serial.println("Inside sendTaskToObelix");
//     if (client.connect(serverIP_Obelix, LISTEN_PORT)) {
//         if (DEBUGGER == true) Serial.println("Inside if-case");
//         client.print("GET /vTask?task=");
//         client.print(apiState.task);
//         if (DEBUGGER == true) Serial.print("Task send obelix: ");
//         if (DEBUGGER == true) Serial.println(apiState.task);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         //while (!client.available()) {
//         //
//         //}
//         //if (DEBUGGER == true) Serial.println("Message recieved");
//         //apiState.message = client.readString();
//         //if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// void VehicleWebAPI::sendTaskToIdefix() {
//     if (DEBUGGER == true) Serial.println("Inside sendTaskToIdefix");
//     if (client.connect(serverIP_Idefix, LISTEN_PORT)) {
//         if (DEBUGGER == true) Serial.println("Inside if-case");
//         client.print("GET /vTask?task=");
//         client.print(apiState.task);
//         if (DEBUGGER == true) Serial.print("Task send idefix: ");
//         if (DEBUGGER == true) Serial.println(apiState.task);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         //while (!client.available()) {
//         //
//         //}
//         //if (DEBUGGER == true) Serial.println("Message recieved");
//         //apiState.message = client.readString();
//         //if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// void VehicleWebAPI::sendSector() {
//     if (DEBUGGER == true) Serial.println("Inside sendSector");
//     if (client.connect(serverIP, SENDING_PORT)) {
//         client.print("GET /vehicleSector?sector=");
//         client.print(apiState.sector);
//         if (DEBUGGER == true) Serial.print("Sector: ");
//         if (DEBUGGER == true) Serial.println(apiState.sector);
//         client.print("&id=");
//         client.print(apiState.vehicleID);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         //while (!client.available()) {
//         //
//         //}
//         //if (DEBUGGER == true) Serial.println("Answer recieved");
//         //apiState.message = client.readString();
//         //if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// void VehicleWebAPI::sendSectorToIdefix() {
//     if (DEBUGGER == true) Serial.println("Inside sendSectorToIdefix");
//     if (client.connect(serverIP_Idefix, LISTEN_PORT)) {
//         client.print("GET /vSector?sector=");
//         client.print(apiState.sector);
//         if (DEBUGGER == true) Serial.print("Sector send to idefix: ");
//         if (DEBUGGER == true) Serial.println(apiState.sector);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         //while (!client.available()) {
//         //
//         //}
//         //if (DEBUGGER == true) Serial.println("Answer recieved");
//         //apiState.message = client.readString();
//         //if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// void VehicleWebAPI::sendSectorToObelix() {
//     if (DEBUGGER == true) Serial.println("Inside sendSectorToObelix");
//     if (client.connect(serverIP_Obelix, LISTEN_PORT)) {
//         client.print("GET /vSector?sector=");
//         client.print(apiState.sector);
//         if (DEBUGGER == true) Serial.print("Sector send to obelix: ");
//         if (DEBUGGER == true) Serial.println(apiState.sector);
//         client.println(" HTTP/1.1");
//         client.println("Connection: close");
//         client.println();

//         //while (!client.available()) {
//         //
//         //}
//         //if (DEBUGGER == true) Serial.println("Answer recieved");
//         //apiState.message = client.readString();
//         //if (DEBUGGER == true) Serial.println(apiState.message);
//         client.stop();
//     }
// }

// String VehicleWebAPI::handleRequest(String req) {
//     if (DEBUGGER == true) Serial.print("New request: ");
//     if (DEBUGGER == true) Serial.println(req);
//     int sep = req.indexOf("/");
//     String method = req.substring(0, sep - 1);
//     if (DEBUGGER == true) Serial.print("Method: ");
//     if (DEBUGGER == true) Serial.println(method);
//     String fullUrl = req.substring(sep, req.indexOf("HTTP") - 1);
//     if (DEBUGGER == true) Serial.print("Full-URL: ");
//     if (DEBUGGER == true) Serial.println(fullUrl);
//     String url = fullUrl.substring(0, fullUrl.indexOf("?"));
//     if (DEBUGGER == true) Serial.print("URL: ");
//     if (DEBUGGER == true) Serial.println(url);
//     String params = fullUrl.substring(fullUrl.indexOf("?") + 1);
//     if (DEBUGGER == true) Serial.print("Parameters: ");
//     if (DEBUGGER == true) Serial.println(params);
//     if (method == "POST") {
//         if (DEBUGGER == true) Serial.println(req);
//         String content = req.substring(req.indexOf("\r\n\r\n"));
//         if (DEBUGGER == true) Serial.println(content);
//         return handlePOST(url, content);
//     } else {
//         //if (DEBUGGER == true) Serial.println(req);
//         return handleGET(url, params);
//     }
// }

// String VehicleWebAPI::handlePOST(String url, String content) {
//     size_t bufferSize = 3 * JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(4);
//     DynamicJsonBuffer jsonBuffer(bufferSize);
//     JsonObject &root = jsonBuffer.parseObject(content);
//     return "done";
// }

// String VehicleWebAPI::handleGET(String url, String params) {
//     if (DEBUGGER == true) Serial.print("Url: ");
//     if (DEBUGGER == true) Serial.println(url);
//     if (DEBUGGER == true) Serial.print("Parameters: ");
//     if (DEBUGGER == true) Serial.println(params);

//     if (url == "/state") {
//         printJSONHeaders();
//         return apiState.state;
//     } else if (url == "/task") {
//         printJSONHeaders();
//         return apiState.task;
//     } else if (url == "/vTask") {
//         String para = params.substring(params.lastIndexOf("=") + 1);
//         apiState.otherTask = para;
//         if (DEBUGGER == true) Serial.println("Inside vtask");
//         printJSONHeaders();
//         return "{'Answer':'Vehicle task recieved'}";
//     } else if (url == "/sector") {
//         printJSONHeaders();
//         return apiState.sector;
//     } else if (url == "/vSector") {
//         String para = params.substring(params.lastIndexOf("=") + 1);
//         if (DEBUGGER == true) Serial.println("Inside vsector");
//         apiState.otherSector = para;
//         printJSONHeaders();
//         return "{'Answer':'Vehicle sector recieved'}";
//     } else if (url == "/count") {
//         if (DEBUGGER == true) Serial.println("Inside countrequest");
//         String para = params.substring(params.lastIndexOf("=") + 1);
//         apiState.vehicleCount = para.toInt();
//         if (DEBUGGER == true) Serial.print("Parameter: ");
//         if (DEBUGGER == true) Serial.println(para);
//         printJSONHeaders();
//         return "{'Answer':'Vehicle count recieved'}";
//     } else if (url == "/go") {
//         String para1 = params.substring(params.indexOf("=") + 1, params.indexOf("&"));
//         apiState.container = para1;
//         apiState.containerChange = true;
//         if (DEBUGGER == true) Serial.print("Container request: ");
//         if (DEBUGGER == true) Serial.println(apiState.container);
//         String para2 = params.substring(params.lastIndexOf("=") + 1);
//         apiState.unloadPortChange = true;
//         apiState.unloadPort = para2;

//         printJSONHeaders();
//         return "{'Answer':'Task recieved'}";
//     } else if (url == "/start") {
//         String para = params.substring(params.lastIndexOf("=") + 1);
//         apiState.speed = para.toInt();
//         apiState.workState = true;
//         apiState.workStateChange = true;
//         printJSONHeaders();
//         return "{'Answer':'Starting vehicle'}";
//     } else if (url == "/stop") {
//         apiState.workState = false;
//         apiState.workStateChange = true;
//         printJSONHeaders();
//         return "{'Answer':'Stopping vehicle'}";
//     } else
//         printHTMLHeaders();
//     return "<html><h1>Request not known!</h1></html>";
// }

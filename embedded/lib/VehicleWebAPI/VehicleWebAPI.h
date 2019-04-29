// /**
//  * @file VehicleWebAPI.h
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

// #ifndef VehicleWebAPI_h
// #define VehicleWebAPI_h

// #include "Arduino.h"
// #include "WebAPI.h"

// /**
//  * @brief
//  *
//  */
// struct ApiState {
//     String message;
//     String vehicleID = "";
//     String state = "booting";
//     String previousState = "booting";
//     String previousSector = "10";
//     String previousTask = "nothing";
//     String task = "nothing";
//     String sector = "10";
//     String otherTask;
//     String otherSector;
//     String container;
//     String unloadPort;
//     bool containerChange = false;
//     bool unloadPortChange = false;
//     bool stateChange = false;
//     bool sectorChange = false;
//     bool taskChange = false;
//     bool workStateChange = false;
//     bool workState = false;
//     int jobCount = 0;
//     int speed;
//     int vehicleCount = 0;
// };

// /**
//  * @brief
//  *
//  */
// class VehicleWebAPI : public WebAPI {
//    public:
//     /**
//      * @brief Construct a new Vehicle Web A P I object
//      *
//      * @param state -
//      */
//     VehicleWebAPI(ApiState *state);

//     /**
//      * @brief
//      *
//      * @param state -
//      */
//     void loop(ApiState *state);

//     /**
//      * @brief
//      *
//      */
//     void registerVehicle();

//     /**
//      * @brief
//      *
//      * @param req -
//      * @return String -
//      */
//     String handleRequest(String req);

//     /**
//      * @brief
//      *
//      * @param url -
//      * @param params -
//      * @return String -
//      */
//     String handleGET(String url, String params);

//     /**
//      * @brief
//      *
//      * @param url -
//      * @param content -
//      * @return String -
//      */
//     String handlePOST(String url, String content);

//     /**
//      * @brief
//      *
//      */
//     void sendState();

//     /**
//      * @brief
//      *
//      */
//     void sendSector();

//     /**
//      * @brief
//      *
//      */
//     void sendSectorToIdefix();

//     /**
//      * @brief
//      *
//      */
//     void sendSectorToObelix();

//     /**
//      * @brief
//      *
//      */
//     void sendTask();

//     /**
//      * @brief
//      *
//      */
//     void sendTaskToObelix();

//     /**
//      * @brief
//      *
//      */
//     void sendTaskToIdefix();

//     /**
//      * @brief
//      *
//      */
//     void decideName();

//    private:
//     /**
//      * @brief
//      *
//      */
//     ApiState apiState;

//     /**
//      * @brief
//      *
//      */
//     byte serverIP_Obelix[4] = {IP1, IP2, IP3, IP4_Obelix};

//     /**
//      * @brief
//      *
//      */
//     byte serverIP_Idefix[4] = {IP1, IP2, IP3, IP4_Idefix};
// };

// #endif
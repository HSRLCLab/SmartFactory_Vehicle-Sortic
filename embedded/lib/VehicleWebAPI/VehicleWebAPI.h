/*
    VehicleWebAPI.h - Library for specific networkcommunication from vehicle to other objects in network.
    Created by Glenn Huber, 28.04.2018
*/
#ifndef VehicleWebAPI_h
#define VehicleWebAPI_h

#include "Arduino.h"
#include "WebAPI.h"

struct ApiState {
    String message;
    String vehicleID = "";
    String state = "booting";
    String previousState = "booting";
    String previousSector = "10";
    String previousTask = "nothing";
    String task = "nothing";
    String sector = "10";
    String otherTask;
    String otherSector;
    String container;
    String unloadPort;
    bool containerChange = false;
    bool unloadPortChange = false;
    bool stateChange = false;
    bool sectorChange = false;
    bool taskChange = false;
    bool workStateChange = false;
    bool workState = false;
    int jobCount = 0;
    int speed;
    int vehicleCount = 0;
};

class VehicleWebAPI : public WebAPI
{
    public:
        VehicleWebAPI(ApiState *state);
        void loop(ApiState *state);
        void registerVehicle();
        String handleRequest(String req); 
        String handleGET(String url, String params);
        String handlePOST(String url, String content);
        void sendState();
        void sendSector();
        void sendSectorToIdefix();
        void sendSectorToObelix();
        void sendTask();
        void sendTaskToObelix();
        void sendTaskToIdefix();
        void decideName();

    private:
        ApiState apiState;

        byte serverIP_Obelix[4] = {IP1, IP2, IP3, IP4_Obelix};
        byte serverIP_Idefix[4] = {IP1, IP2, IP3, IP4_Idefix};
};

#endif
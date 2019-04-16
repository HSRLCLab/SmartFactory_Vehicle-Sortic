/**
 * @file myMQTT.cpp
 * @brief Implementation of the myMQTT class
 * 
 * https://pubsubclient.knolleary.net
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - added MQTT-Connection - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-16
 * 
 * @date 2019-04-16
 * @copyright Copyright (c) 2019
 * 
 */

#include "myMQTT.h"

//=====PUBLIC====================================================================================

myMQTT::myMQTT() : myMQTT(DEFAULT_HOSTNAME,
                          DEFAULT_MQTT_BROKER_IP1,
                          DEFAULT_MQTT_BROKER_IP2,
                          DEFAULT_MQTT_BROKER_IP3,
                          DEFAULT_MQTT_BROKER_IP4,
                          DEFAULT_MQTT_PORT) {
}

myMQTT::myMQTT(String hostname) : myMQTT(hostname,
                                         DEFAULT_MQTT_BROKER_IP1,
                                         DEFAULT_MQTT_BROKER_IP2,
                                         DEFAULT_MQTT_BROKER_IP3,
                                         DEFAULT_MQTT_BROKER_IP4,
                                         DEFAULT_MQTT_PORT) {
}

myMQTT::myMQTT(String hostname,
               const int BrokerIP1,
               const int BrokerIP2,
               const int BrokerIP3,
               const int BrokerIP4,
               const int BrokerPort) : pHostname(hostname + String(random(0xffff), HEX)),
                                       pBrokerIP(BrokerIP1,
                                                 BrokerIP2,
                                                 BrokerIP3,
                                                 BrokerIP4),
                                       pBrokerPort(BrokerPort) {
// myMQTTclient.PubSubClient(pBrokerIP, pBrokerPort, pToCallback, *myClient);
}

void myMQTT::init(WiFiClient *myClient,void(*pToCallback)(char*, unsigned char*, unsigned int)) {
    DBFUNCCALLln("myMQTT::init()");
    myMQTTclient.setClient(*myClient);
    myMQTTclient.setServer(pBrokerIP, pBrokerPort);
    myMQTTclient.setCallback(pToCallback);
    connectToMQTT();  // connecting to MQTT-Broker
}

void myMQTT::connectToMQTT() {
    DBFUNCCALLln("myMQTT::connectToMQTT()");
    while (!myMQTTclient.connected()) {
        DBINFO1ln("Attempting MQTT connection...");
        DBINFO1ln("MQTT Client ID: " + pHostname);
        if (myMQTTclient.connect(pHostname.c_str())) {
            DBINFO1ln("MQTT connected");
            DBINFO1ln("Variable myMQTT has successfully connected with hostname: " + pHostname);
        } else {
            // MQTTConnectionFailed();
            DBINFO1ln("trying again in 3 seconds");
            delay(3000);
        }
    }
}

//=====PRIVATE====================================================================================

String myMQTT::decodeMQTTstate(int errorcode) {
    DBFUNCCALLln("myMQTT::decodeMQTTstate(int errorcode)");
    switch (errorcode) {
        case -4:
            return "MQTT_CONNECTION_TIMEOUT";
        case -3:
            return "MQTT_CONNECTION_LOST";
        case -2:
            return "MQTT_CONNECT_FAILED";
        case -1:
            return "MQTT_DISCONNECTED";
        case 0:
            return "MQTT_CONNECTED";
        case 1:
            return "MQTT_CONNECT_BAD_PROTOCOL";
        case 2:
            return "MQTT_CONNECT_BAD_CLIENT_ID";
        case 3:
            return "MQTT_CONNECT_UNAVAILABLE";
        case 4:
            return "MQTT_CONNECT_BAD_CREDENTIALS";
        case 5:
            return "MQTT_CONNECT_UNAUTHORIZED";
        default:
            return "Error";
    }
}

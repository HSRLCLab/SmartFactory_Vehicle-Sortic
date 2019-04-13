/**
 * @file myMQTT.cpp
 * @brief 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.1 - Description - {author} - {date}
 * 
 * @date 2019-04-13
 * @copyright Copyright (c) 2019
 * 
 */

#include "myMQTT.h"
// void callback(const char[] topic, byte* payload, unsigned int length) {
// }

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
}

void myMQTT::init() {
    DBFUNCCALLln("myMQTT::init()");
    // myMQTTclient = new PubSubClient(myClient);
    // myMQTTclient->setServer(brokerIP, pBrokerPort);
    // myMQTTclient->setCallback(callback2);
    // myMQTTclient = new PubSubClient(pBrokerIP, pBrokerPort, callback, pWiFiClient);
    // connectToMQTT();  // connecting to MQTT-Broker
}

void myMQTT::connectToMQTT() {
    DBFUNCCALLln("myMQTT::connectToMQTT()");
    // while (!myMQTTclient->connected()) {
    //     DBINFO1ln("Attempting MQTT connection...");
    //     DBINFO1ln("MQTT Client ID: " + pHostname);
    //     if (myMQTTclient->connect(pHostname.c_str())) {
    //         DBINFO1ln("MQTT connected");
    //         DBINFO1ln("Variable myMQTT has successfully connected with hostname: " + pHostname);
    //     } else {
    //         // MQTTConnectionFailed();
    //         DBINFO1ln("trying again in 3 seconds");
    //         delay(3000);
    //     }
    // }
}

//=====PRIVATE====================================================================================

String myMQTT::decodeMQTTstate(int errorcode) {
    DBFUNCCALLln("myMQTT::decodeMQTTstate(int errorcode)");
    // LOG4("NetworkManager::decodeMQTTstate(int errorcode)");
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

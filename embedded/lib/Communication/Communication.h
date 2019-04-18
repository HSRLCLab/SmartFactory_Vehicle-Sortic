/**
 * @file Communication.h
 * @brief The Connection-class is used as interface
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - added Connection-Implementation - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-16
 * 
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef COMMUNICATION_H__
#define COMMUNICATION_H__

#include "Arduino.h"

// #include "CommunicationConfiguration.h"
// #include "LogConfiguration.h"

#include "Network.h"
#include "myJSON.h"
#include "myMQTT.h"

class Communication {
   public:
    /**
    * @brief Construct a new Communication object
    * 
    */
    Communication();

    /**
    * @brief If the client is used to subscribe to topics,
    * a callback function must be provided in the constructor. 
    * his function is called when new messages arrive at the client.
    * 
    * Internally, the client uses the same buffer for both inbound and outbound messages.
    * After the callback function returns, or if a call to either publish or subscribe
    * is made from within the callback function,
    * the topic and payload values passed to the function will be overwritten.
    * The application should create its own copy of the values if they are required beyond this.
    * 
    * https://pubsubclient.knolleary.net/api.html#callback
    * 
    * 
    * @param topic - the topic the message arrived on (const char[])
    * @param payload - the message payload (byte array)
    * @param length - the length of the message payload (unsigned int)
    */
    static void MQTTcallback(char* topic, byte* payload, unsigned int length);

    //==INTERFACE===================================
    /**
     * @brief 
     * 
     */
    inline void init() {
        pNetwork.init();
        pMymqtt.init(&pClient, funcPointer);
    };

    //=======NETWORK============
    inline void connectToWiFi() {
        pNetwork.connectToWiFi();
    }

    inline void printNetworkInfo() {
        pNetwork.printNetworkInfo();
    }

    //=======myMQTT============
    inline bool subscribe(const String topic) {
        pMymqtt.subscribe(topic);
    }

    inline bool unsubscribe(const String topic) {
        pMymqtt.unsubscribe(topic);
    }

    inline bool publishMessage(const String topic, const String msg) {
        pMymqtt.publishMessage(topic, msg);
    }

    //=======myJSON============
   private:
    WiFiClient pClient;  ///< instance of WiFiClient
    Network pNetwork;    ///< instance of Network
    myMQTT pMymqtt;      ///< instance of myMQTT
    myJSON pMyjson;      ///< instance of myJSON

    void (*funcPointer)(char*, unsigned char*, unsigned int) = Communication::MQTTcallback;
};
#endif
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

#include "CircularBuffer.h"
#include "Network.h"
#include "myJSON.h"
#include "myMQTT.h"

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
void MQTTcallback(char* topic, byte* payload, unsigned int length);
extern myJSON _myjson;
extern CircularBuffer<myJSONStr, MAX_JSON_MESSAGES_SAVED> _buffer;

class Communication {
   public:
    /**
    * @brief Construct a new Communication object
    * 
    */
    Communication();

    //==INTERFACE===================================
    /**
     * @brief 
     * 
     */
    inline void init() {
        pNetwork.init();
        pMymqtt.init(&pClient, funcPointer);
    };

    inline bool loop() {
        return pMymqtt.loop();
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
        return pMymqtt.subscribe(topic);
    }

    inline bool unsubscribe(const String topic) {
        return pMymqtt.unsubscribe(topic);
    }

    inline bool publishMessage(const String topic, const String msg) {
        return pMymqtt.publishMessage(topic, msg);
    }

    //=======myJSON============
    inline myJSONStr shift() {
        return _buffer.shift();
    }

    inline myJSONStr pop() {
        return _buffer.pop();
    }

    inline bool isEmpty() {
        return _buffer.isEmpty();
    }

    inline bool size() {
        return _buffer.size();
    }

   private:
    WiFiClient pClient;  ///< instance of WiFiClient
    Network pNetwork;    ///< instance of Network
    myMQTT pMymqtt;      ///< instance of myMQTT
    void (*funcPointer)(char*, unsigned char*, unsigned int) = MQTTcallback;
};
#endif
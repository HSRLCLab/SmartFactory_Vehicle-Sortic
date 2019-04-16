/**
 * @file myMQTT.h
 * @brief This class handels the MQTT connection
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

#ifndef MYMQTT_H__
#define MYMQTT_H__

#include "Arduino.h"

#include "CommunicationConfiguration.h"
#include "LogConfiguration.h"

#include <Network.h>
// #include <WiFi101.h>
#include <PubSubClient.h>


class myMQTT {
    //=====PUBLIC====================================================================================
   public:
   /**
    * @brief Construct a new my M Q T T object
    * 
    * Sets hostname, BrokerIP and BrokerPort from Communication-Config file
    */
    myMQTT();

    /**
     * @brief Construct a new my M Q T T object
     * 
     * Sets BrokerIP and BrokerPort from Communication-Config file
     * 
     * @param hostname - the client ID to use when connecting to the server
     */
    myMQTT(String hostname);

    /**
     * @brief Construct a new my M Q T T object
     * 
     * @param hostname - the client ID to use when connecting to the server
     * @param BrokerIP1 - broker IP distributed in 4 values (192)
     * @param BrokerIP2 - broker IP distributed in 4 values (186)
     * @param BrokerIP3 - broker IP distributed in 4 values (1)
     * @param BrokerIP4 - broker IP distributed in 4 values (2)
     * @param BrokerPort - MQTT connection port
     */
    myMQTT(String hostname, const int BrokerIP1, const int BrokerIP2, const int BrokerIP3, const int BrokerIP4, const int BrokerPort);

    /**
     * @brief 
     * 
     * @param myClient - pointer to WiFiClient object
     * @param pToCallback - pointer to Callback-Function
     */
    void init(WiFiClient *myClient,void(*pToCallback)(char*, unsigned char*, unsigned int));

    /**
     * @brief Connects to the MQTT-server
     * 
     * If the server isn't already connected  a new connection will be established
     * If the connection fails then there will be a 3 Sec dealy.
     */
    void connectToMQTT();


//=====PRIVATE====================================================================================
   private:
    /**
     * @brief Decodes the Error Values from MQTT state() and returns a description
     * 
     * @param errorcode - errorvalue from client
     * @return String - Errordescription
     */
    String decodeMQTTstate(int errorcode);

    String pHostname;           ///< Contains Hostname/Client ID
    IPAddress pBrokerIP;        ///< the IP-address of the server/broker
    const int pBrokerPort;      ///< port to connect to MQTT
    PubSubClient myMQTTclient;  ///< instance of PubSubClient
};
#endif
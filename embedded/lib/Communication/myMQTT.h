/**
 * @file myMQTT.h
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

#ifndef MYMQTT_H__
#define MYMQTT_H__

#include "Arduino.h"

#include "CommunicationConfiguration.h"
#include "LogConfiguration.h"

#include <Network.h>
#include <PubSubClient.h>

// void callback(const char[] topic, byte* payload, unsigned int length);

class myMQTT {
   public:
    myMQTT();

    myMQTT(String hostname);

    myMQTT(String hostname, const int BrokerIP1, const int BrokerIP2, const int BrokerIP3, const int BrokerIP4, const int BrokerPort);

    void init();

    void connectToMQTT();

   private:
    String decodeMQTTstate(int errorcode);

    String pHostname;  ///< Contains Hostname
    IPAddress pBrokerIP;
    const int pBrokerPort;

    // PubSubClient* myMQTTclient;
    // WiFiClient pWiFiClient;  ///<
};
#endif
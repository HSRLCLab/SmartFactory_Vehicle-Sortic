/**
 * @file Communication.cpp
 * @brief Implementation of the Connection class
 *
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.0 - added Connection-Implementation - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-16
 *
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 *
 */

/*
Useful Links
https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
https://randomnerdtutorials.com/decoding-and-encoding-json-with-arduino-or-esp8266/
https://arduinodiy.wordpress.com/2017/07/30/json-mqtt-and-openhab/
https://assetwolf.com/learn/sending-data-from-arduino-to-cloud
*/

#include "Communication.h"

void Communication::MQTTcallback(char* topic, byte* payload, unsigned int length) {
    DBFUNCCALLln("Communication::callback(const char[] topic, byte* payload, unsigned int length)");
    String topic_str;
    for (int i = 0; topic[i] != '\0'; i++)  // iterate topic to topic_str
    {
        topic_str += topic[i];
    }
    String msg = "Message arrived [" + topic_str + "]: \t message:";
    for (unsigned int i = 0; i < length; i++)  // iterate message
    {
        msg += (char)payload[i];
    }
    DBINFO1ln(msg);
}

Communication::Communication() {
}

void Communication::init() {
    DBFUNCCALLln("Communication::init()");
    Network::init();
    myMQTT::init(&pClient, funcPointer);
}

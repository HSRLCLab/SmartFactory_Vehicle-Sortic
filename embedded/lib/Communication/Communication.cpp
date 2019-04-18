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

Communication::Communication() {
}

// void Communication::init() {
//     DBFUNCCALLln("Communication::init()");
//     pNetwork.init();
//     pMymqtt.init(&pClient, funcPointer);
// }

/**
 * @todo String is very inefficient
 * @todo Change way how pMyjson is generated...
 */
void Communication::MQTTcallback(char* topic, byte* payload, unsigned int length) {
    DBFUNCCALLln("Communication::callback(const char[] topic, byte* payload, unsigned int length)");
    myJSON pMyjson;
    String topic_str = String((char*)topic);
    // for (int i = 0; topic[i] != '\0'; i++) {// iterate topic to topic_str
    //     topic_str += topic[i];
    // }
    String msg = "Message arrived [" + topic_str + "]: \t message:";
    String payload_str;                          //= String((char*)payload); //need \0 termination
    for (unsigned int i = 0; i < length; i++) {  // iterate message till lentgh caus it's not 0-terminated
        payload_str += (char)payload[i];
    }
    DBINFO1ln(msg + payload_str);

    //https://stackoverflow.com/questions/1360183/how-do-i-call-a-non-static-method-from-a-static-method-in-c
    myJSONStr newMessage = pMyjson.parsingJSONToStruct((const char*)payload);
    DBINFO1("Sensor: ");
    DBINFO1ln(newMessage.sensor);
    DBINFO1("Time: ");
    DBINFO1ln(newMessage.time);
    DBINFO1("data_0: ");
    DBINFO1ln(newMessage.data[0]);
    DBINFO1("data_1: ");
    DBINFO1ln(newMessage.data[1]);
    // myJSON::stack.push(newMessage);
}

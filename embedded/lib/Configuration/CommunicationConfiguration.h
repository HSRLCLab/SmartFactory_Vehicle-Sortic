/**
 * @file NetworkConfiguration.h
 * @brief file that contains the default network configuration
 * 
 * 
 * @author Luciano Bettinaglio (luciano.bettinaglio@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - SA SmartFactroyBox HS 2018
 * 
 * @date 2019-03-07
 * 
 * @copyright Copyright (c) 2019
 * 
 * @todo Switch Password to .ini file \n
 * https://community.platformio.org/t/how-to-specify-ota-upload-password-outside-platformio-ini/3588 \n
 * http://docs.platformio.org/en/latest/envvars.html#building
 */

#ifndef COMMUNICATIONCONFIGURATION_H
#define COMMUNICATIONCONFIGURATION_H

//=============WLAN CONNECTION==================================
#define DEFAULT_WIFI_SSID "DigitalLab"        ///< SSID to connect to
#define DEFAULT_WIFI_PASSWORD "digital42HSR"  ///< Password to corresponding SSID

// Pins for Adafruit ATWINC1500 Feather https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/using-the-wifi-module
#define DEFAULT_WIFI_CS 8   ///<
#define DEFAULT_WIFI_IRQ 7  ///<
#define DEFAULT_WIFI_RST 4  ///<
#define DEFAULT_WIFI_EN 2   ///<

#define DEFAULT_HOSTNAME_SMARTBOX "SmartBox"  ///< for SmartBoxes, used for MQTT and WiFi, must be unique in Network
#define DEFAULT_HOSTNAME_VEHICLE "Vehicle"    ///< for Vehicles, used for MQTT and WiFi, must be unique in Network

//=============MQTT==================================
// * DEFAULT_MQTT_* are MQTT defaults
#define DEFAULT_MQTT_BROKER_IP1 192  ///< broker IP distributed in 4 values
#define DEFAULT_MQTT_BROKER_IP2 168  ///<
#define DEFAULT_MQTT_BROKER_IP3 1    ///<
#define DEFAULT_MQTT_BROKER_IP4 22   ///<
#define DEFAULT_MQTT_PORT 1883       ///< MQTT connection port

//=============JSON==================================
#define MAX_JSON_PARSE_SIZE 300  ///< max buffer size to parse JSON objects, size of the pool in bytes, can be calculated in https://arduinojson.org/v5/assistant/

#endif
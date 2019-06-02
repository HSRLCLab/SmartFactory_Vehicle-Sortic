/**
 * @file Configuration.h
 * @brief File that contains the configuratioindetails of the different modules
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

const unsigned int TIME_BETWEEN_PUBLISH = 300;
const unsigned int TIME_BETWEEN_PUBLISH_TOKEN = 200;
const unsigned int TIMEOUT_VACKS = 5 * 1000;
const unsigned int SORTIC_MAX_LINE = 3;

// #define DEFAULT_HOSTNAME_RAND String(random(0xffff), HEX)  ///< for Vehicles, used for MQTT and WiFi, must be unique in Network
const int DEFAUL_HOSTNAME_NUMBER = 1;                                     //defines also startline
#define DEFAULT_HOSTNAME (String("SV") + String(DEFAUL_HOSTNAME_NUMBER))  // + DEFAULT_HOSTNAME_RAND)

#endif
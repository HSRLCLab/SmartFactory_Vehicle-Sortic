/**
 * @file Configuration.h
 * @brief File that contains the configuratioindetails of the different modules
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Release BA FS19  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-06-10
 * @version 0.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 0.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

const unsigned int TIME_BETWEEN_PUBLISH = 300;        ///< Time in ms between publishs
const unsigned int TIME_BETWEEN_PUBLISH_TOKEN = 200;  ///< Time in ms between publish token
const unsigned int TIMEOUT_VACKS = 5 * 1000;          ///< How long the vehicle waits for acknoledgment from box
const unsigned int SORTIC_MAX_LINE = 3;               ///< How many lines are available. Needed atm to choose a free line without handshake with sortic

// #define DEFAULT_HOSTNAME_RAND String(random(0xffff), HEX)  ///< for Vehicles, used for MQTT and WiFi, must be unique in Network
const int DEFAUL_HOSTNAME_NUMBER = 2;                                     ///< defines also startline.
#define DEFAULT_HOSTNAME (String("SV") + String(DEFAUL_HOSTNAME_NUMBER))  ///< Hostname of the vehicle. Used for Communication and identification. Needs to be unique.

#endif
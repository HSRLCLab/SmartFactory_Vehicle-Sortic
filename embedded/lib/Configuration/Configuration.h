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

//Setup for chassis /////////////////
#define RIGHT_MOTOR 1     ///<
#define LEFT_MOTOR 2      ///<
#define PIN_SENSOR_0 13   ///<
#define PIN_SENSOR_1 12   ///<
#define PIN_SENSOR_2 11   ///<
#define PIN_SENSOR_3 10   ///<
#define PIN_SENSOR_4 9    ///<
#define SPEED 70          ///<
#define REDUCED_SPEED 60  ///<
#define PUSH_SPEED 80     ///<
/////////////////////////////////////

//Setup for PID /////////////////////
#define K_P 11   ///<
#define K_I 0.3  ///<
#define K_D 0    ///<
/////////////////////////////////////

//Setup for sonar ///////////////////
#define SONAR_SERVO_PIN 5        ///<
#define SONAR_TRIGGER_PIN 15     ///<
#define SONAR_ECHO_PIN 14        ///<
#define SONAR_MAX_DISTANCE 9999  ///<
#define MIN_ERROR -5             ///<
#define MAX_ERROR 5              ///<
#define MIN_TURN_ANGLE 180       ///<
#define MAX_TURN_ANGLE 0         ///<
////////////////////////////////////

//Setup for Hoist //////////////////
#define HOIST_SERVO_PIN 6       ///< Pin-Number for Servocontroll of the Hoist
#define HOIST_SERVO_DELAY 30    ///< Duration of the delay between the position updates of the Servo
#define HOIST_POSITION_MIN 168  ///< Minimal angle of the Servo
#define HOIST_POISITION_MAX 65  ///< Maximal angle of the Servo
////////////////////////////////////

//Setup for Vision /////////////////
#define VISION_SERVO_PIN 5          ///<
#define VISION_DELAY_FACTOR 6       ///<
#define VISION_TOLERANCE_LEFT 165   ///<
#define VISION_TOLERANCE_RIGHT 155  ///<
#define VISION_START_ANGLE 90       ///<
////////////////////////////////////

//Setup for webAPI /////////////////
#define SECRET_SSID "DigitalLab"    ///<
#define SECRET_PASS "digital42HSR"  ///<
#define KEY_INDEX 1                 ///<
//Ip-Address //////////////////////
#define IP1 192        ///<
#define IP2 168        ///<
#define IP3 1          ///<
#define IP4 4          ///<
#define IP4_Idefix 3   ///<
#define IP4_Obelix 27  ///<
//WiFi pins //////////////////////
#define WIFI_CS 8   ///<
#define WIFI_IRQ 7  ///<
#define WIFI_RST 4  ///<
#define WIFI_EN 2   ///<
//Ports //////////////////////////
#define LISTEN_PORT 80     ///<
#define SENDING_PORT 1880  ///<

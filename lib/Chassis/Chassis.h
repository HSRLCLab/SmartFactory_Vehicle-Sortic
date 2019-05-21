// /**
//  * @file Chassis.h
//  * @brief Library for mechatronic component chassis.
//  *
//  * The chassis contains 2 DC-motors which power the movement of the chassis
//  * and 5 IR-sensors which work as internal navigation components.
//  *
//  * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
//  * @author Robert Paly (robert.paly@hsr.ch)
//  *
//  * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
//  * @version 1.0 - BA FTS FS 2018
//  *
//  * @date 2019-03-20
//  * @copyright Copyright (c) 2019
//  *
//  */

// #ifndef Chassis_h
// #define Chassis_h

// #include "Adafruit_MotorShield.h"
// #include "Arduino.h"
// #include "Configuration.h"
// #include "LogConfiguration.h"
// #include "Modular.h"

// /**
//  * @brief
//  *
//  */
// struct ChassisState {
//     int speed;                ///<
//     int directionError = 0;   ///<
//     int virtualError = 0;     ///<
//     int sensor[5];            ///<
//     float PidValue;           ///<
//     bool actionDone = false;  ///<
//     bool IR = true;           ///<
//     bool fullLine = false;    ///<
//     String direction = "";    ///<
// };

// /**
//  * @brief
//  *
//  */
// class Chassis : public Component {
//    public:
//     /**
//     * @brief Construct a new Chassis object
//     *
//     * @param givenSpeed -
//     * @param kp -
//     * @param ki -
//     * @param kd -
//     * @param motorNumRight -
//     * @param motorNumLeft -
//     * @param sensorPin0 -
//     * @param sensorPin1 -
//     * @param sensorPin2 -
//     * @param sensorPin3 -
//     * @param sensorPin4 -
//     */
//     Chassis(int givenSpeed, float kp, float ki, float kd, int motorNumRight, int motorNumLeft, int sensorPin0, int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4);

//     /**
//      * @brief
//      *
//      * @param state -
//      */
//     void loop(ChassisState *state);

//     /**
//      * @brief
//      *
//      */
//     void readSensor();

//     /**
//      * @brief
//      *
//      */
//     void calculatePID();

//     /**
//      * @brief
//      *
//      * @param sonarFactor -
//      */
//     void motorControl(float sonarFactor);

//     /**
//      * @brief
//      *
//      * @param curveTime -
//      * @param startTime -
//      */
//     void turnLeftLimited(unsigned int curveTime, unsigned long startTime);

//     /**
//      * @brief
//      *
//      */
//     void turnLeft();

//     /**
//      * @brief
//      *
//      * @param curveTime -
//      * @param startTime -
//      */
//     void turnRightLimited(unsigned int curveTime, unsigned long startTime);

//     /**
//      * @brief
//      *
//      */
//     void turnRight();

//     /**
//      * @brief
//      *
//      * @param driveTime -
//      * @param startTime -
//      */
//     void driveStraightLimited(unsigned int driveTime, unsigned long startTime);

//     /**
//      * @brief
//      *
//      */
//     void driveStraight();

//     /**
//      * @brief
//      *
//      * @param sensor -
//      */
//     void turnAround(int sensor);

//     /**
//      * @brief
//      *
//      */
//     void driveBack();

//     /**
//      * @brief
//      *
//      * @param driveTime -
//      * @param startTime -
//      */
//     void driveBackLimited(unsigned int driveTime, unsigned long startTime);

//     /**
//      * @brief Get the Start Speed Left object
//      *
//      * @return int -
//      */
//     int getStartSpeedLeft();

//     /**
//      * @brief Get the Start Speed Right object
//      *
//      * @return int -
//      */
//     int getStartSpeedRight();

//     /**
//      * @brief
//      *
//      */
//     void stop();
//     ChassisState currentState;  ///<

//    protected:
//     //Motor objects
//     Adafruit_MotorShield AFMS;     ///<
//     Adafruit_DCMotor *motorRight;  ///<
//     Adafruit_DCMotor *motorLeft;   ///<
//     int numRight;                  ///<
//     int numLeft;                   ///<

//     //Sensor variables
//     int sensor0;  ///<
//     int sensor1;  ///<
//     int sensor2;  ///<
//     int sensor3;  ///<
//     int sensor4;  ///<

//     int previousSensorValue[5];  ///<

//     int sensorPin0;  ///<
//     int sensorPin1;  ///<
//     int sensorPin2;  ///<
//     int sensorPin3;  ///<
//     int sensorPin4;  ///<

//     //PID variables
//     float KP;                ///<
//     float KI;                ///<
//     float KD;                ///<
//     int error;               ///<
//     float PValue;            ///<
//     float IValue;            ///<
//     float DValue;            ///<
//     float PIDValue;          ///<
//     float previousPIDValue;  ///<
//     float previousError;     ///<
//     float previousI;         ///<
//     float korr;              ///<
//     float korr1;             ///<
//     float korr2;             ///<
//     float leftMotorSpeed;    ///<
//     float rightMotorSpeed;   ///<
//     float leftMotorSpeed1;   ///<
//     float rightMotorSpeed1;  ///<
//     float targetSpeedLeft;   ///<
//     float targetSpeedRight;  ///<

//     //Timevariables
//     unsigned long millisOfDriving;  ///<
//     unsigned long previousMillis;   ///<
//     int timeLoopCounter;            ///<

//     //Counter
//     int turnCounter;        ///<
//     int crossCounterLeft;   ///<
//     int crossCounterRight;  ///<
// };

// #endif
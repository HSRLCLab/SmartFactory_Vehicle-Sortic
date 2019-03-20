/**
 * @file Sonar.h
 * @brief Library for mechatronic component sonar.
 * 
 * The sonar module consists of a sonar sensor and a servo motor.
 * Based on the deviation of the sonar to the focus point an error is
 * given to the loop to turn the sonar to the right direction.
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * @author Robert Paly (robert.paly@hsr.ch)
 * @author Felix Nyffenegger (felix.nyffenegger@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef sonar_h
#define sonar_h

#include "Arduino.h"
#include "Configuration.h"
#include "Modular.h"
#include "NewPing.h"
#include "Servo.h"

/**
 * @brief 
 * 
 */
struct SonarState {
    int obstacleDistance = 9999;  ///<
    float sonarFactor = 1;        ///<
    bool detachServo = false;     ///<
    bool isAttached = false;      ///<
};

/**
 * @brief 
 * 
 */
class Sonar : public Component {
   public:
    /**
     * @brief Construct a new Sonar object
     * 
     * @param servoPin - 
     * @param triggerPin - 
     * @param echoPin - 
     * @param maxDistance - 
     * @param min_Error - 
     * @param max_Error - 
     * @param min_TurnAngle - 
     * @param max_TurnAngle - 
     */
    Sonar(int servoPin, int triggerPin, int echoPin, int maxDistance, int min_Error, int max_Error, int min_TurnAngle, int max_TurnAngle);

    /**
     * @brief 
     * 
     * @param state - 
     * @param directionError - 
     * @param servoActive - 
     */
    void loop(SonarState *state, int directionError, bool servoActive);

    /**
     * @brief 
     * 
     * @param directionError - 
     */
    void turnSonar(int directionError);

    /**
     * @brief 
     * 
     * @param state - 
     */
    void calculateSonarFactor(SonarState *state);

   protected:
    /**
    * @brief 
    * 
    */
    NewPing *sonar;

    /**
     * @brief 
     * 
     */
    Servo sonarServo;

    /**
     * @brief 
     * 
     */
    int minError;

    /**
     * @brief 
     * 
     */
    int maxError;

    /**
     * @brief 
     * 
     */
    int minTurnAngle;

    /**
     * @brief 
     * 
     */
    int maxTurnAngle;

    /**
     * @brief 
     * 
     */
    int _servoPin;
};

#endif
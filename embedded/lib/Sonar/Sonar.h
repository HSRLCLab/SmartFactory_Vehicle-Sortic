/**
 * @file Sonar.h
 * @brief Library for mechatronic component sonar.
 * 
 * The sonar module consists of a sonar sensor and a servo motor.
 * Based on the deviation of the sonar to the focus point an error is
 * given to the loop to turn the sonar to the right direction.
 * 
 * The HC-SR04 has a range from 2cm to 3mm with a resolution form 2mm. Max freq. is 50Hz
 * Voltage 5V
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
#include "LogConfiguration.h"
#include "Modular.h"
#include "NewPing.h"
#include "Servo.h"

/**
 * @brief 
 * 
 */
struct SonarState {
    int obstacleDistance = SONAR_MAX_DISTANCE;  ///<
    float sonarFactor = 1;                      ///<
    bool detachServo = false;                   ///<
    bool isAttached = false;                    ///<
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
     * @brief Gets distance to next obstacle in front of the vehicle
     * 
     * @return int - return distance in cm
     */
    int getDistanceToObstacle();

    /**
     * @brief Correct the sonar-orientation and turn it drivingdirection
     * 
     * Take the direction error and maps it into an turnangle (180 -0 degree).
     * Afterwards realigne the sonar so it faces the path correctly
     * 
     * @param directionError - deviation from the path -5 - +5
     * 
     * @todo smoother position change
     */
    void turnSonar(int directionError);

    /**
     * @brief 
     * 
     * @param state - 
     * @todo for what?? ->motorControl
     */
    void calculateSonarFactor(SonarState *state);

    /**
     * @brief Testfunction for Sonar-Class
     * 
     * 0 - run all tests \n
     * 1 - run test for servo \n
     * 2 - run test for obstacle detection \n
     * 
     * @param test - Choose which test to run
     */
    void Test(const int test);

   protected:
    NewPing *sonar;    ///<
    Servo sonarServo;  ///<
    int minError;      ///<
    int maxError;      ///<
    int minTurnAngle;  ///<
    int maxTurnAngle;  ///<
    int _servoPin;     ///<
};

#endif
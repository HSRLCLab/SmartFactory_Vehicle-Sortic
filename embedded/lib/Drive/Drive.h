/**
 * @file Drive.h
 * @brief 
 * 
 * DC MOTOR + STEPPER FEATHERWING ADD-ON FOR ALL FEATHER BOARDS
 * https://www.adafruit.com/product/2927 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Description - {author} - {date}
 * 
 * @date 2019-04-26
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DRIVE_H
#define DRIVE_H
/* https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/library-reference
 * https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
 */
#include <Adafruit_MotorShield.h>
#include <Arduino.h>

#include "Configuration.h"
#include "LogConfiguration.h"

/*void run(uint8_t)
FORWARD - Rotate in a forward direction
BACKWARD - Rotate in the reverse direction
RELEASE - Stop rotation
Also note that "RELEASE" simply cuts power to the motor. It does not apply any braking.
 */
/*void setSpeed(uint8_t);
The setSpeed() function controls the power level delivered to the motor.
The speed parameter is a value between 0 and 255.
*/

/**
 * @brief Class Drive handels the Driving-System
 * 
 */
class Drive {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief 
    * 
    */
    enum class Direction {
        Left,
        Right,
        Forward,
        Backward
    };

    /**
     * @brief Construct a new Drive object
     * 
     */
    Drive();

    /**
     * @brief Construct a new Drive object
     * 
     * @param MotorPinRight - 
     * @param MotorPinLeft - 
     * @param speed - 
     */
    Drive(const int MotorPortRight, const int MotorPortLeft);
    /**
     * @brief 
     * 
     * @param direction - 
     * @param speed - 0 and 255
     * @return true - 
     * @return false - 
     */
    void drive(Direction direction, unsigned int speed);

    /**
     * @brief 
     * 
     * @param direction - 
     * @param speed - 
     * @param time - 
     * @return true - 
     * @return false - 
     */
    bool drive(Direction direction, unsigned int speed, unsigned int time);

    /**
     * @brief 
     * 
     * @param direction - 
     * @param speed - 
     * @return true - 
     * @return false - 
     */
    void turn(Direction direction, unsigned int speed);

    /**
     * @brief 
     * 
     * @param direction - 
     * @param speed - 
     * @param time - 
     * @return true - 
     * @return false - 
     */
    bool turn(Direction direction, unsigned int speed, unsigned int time);

    void turnonpoint(Direction direction, unsigned int speed);

    /**
     * @brief 
     * 
     * @return true - 
     * @return false - 
     */
    void stop();

    //=====PRIVATE===================================================================================
   private:
    Adafruit_MotorShield pAFMS = Adafruit_MotorShield();  // Create the motor shield object with the default I2C address
    Adafruit_DCMotor *pMotorRight;                        ///<Pointer to right Motor Obj
    Adafruit_DCMotor *pMotorLeft;                         ///<Pointer to left Motor Obj
    unsigned int pSpeedLeft;                              ///< actual Speed of left Motor
    unsigned int pSpeedRight;                             ///< actual Speed of right Motor
    Direction pWay;                                       ///< Actual movingdirection Forward or Backward
    //============================================================================
    //==Aux-Function==============================================================
    /**
     * @brief Decodes the Direction-Enum and returns a description
     * 
     * @param direction - enum Direction
     * @return String - State as String
     */
    String decodeDirection(Direction direction);
};
#endif
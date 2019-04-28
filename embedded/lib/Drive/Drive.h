/**
 * @file Drive.h
 * @brief The Drive Class handles the activation of the dc-motors for the drive
 * 
 * DC MOTOR + STEPPER FEATHERWING ADD-ON FOR ALL FEATHER BOARDS
 * https://www.adafruit.com/product/2927 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Drive  - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-26
 * 
 * @date 2019-04-26
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DRIVE_H
#define DRIVE_H

#include <Adafruit_MotorShield.h>  //https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include <Arduino.h>

#include "Configuration.h"
#include "LogConfiguration.h"

/* https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/library-reference

void run(uint8_t)
FORWARD - Rotate in a forward direction
BACKWARD - Rotate in the reverse direction
RELEASE - Stop rotation
Also note that "RELEASE" simply cuts power to the motor. It does not apply any braking.

void setSpeed(uint8_t);
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
    * @brief Direction holds all possible directions
    * 
    */
    enum class Direction {
        Left,     ///< Left
        Right,    ///< Right
        Forward,  ///< Forward
        Backward  ///< Backward
    };

    /**
     * @brief Construct a new Drive object
     * 
     */
    Drive();

    /**
     * @brief Construct a new Drive object
     * https://www.adafruit.com/product/2927 
     * @param MotorPortRight - Port where the right Motor is connected on the MotorShield
     * @param MotorPortLeft - Port where the left Motor is connected on the MotorShield
     */
    Drive(const int MotorPortRight, const int MotorPortLeft);
    /**
     * @brief Drive straight Forwards or Backwards
     * 
     * @param direction - Movingdirection Forward or Backward
     * @param speed - Speed between 0 and 255
     */
    void drive(Direction direction, unsigned int speed);

    /**
     * @brief Turn left or right.
     * Speed will add to one Motor's actual speed and substract from the others depenig on wich side it turns
     * 
     * @param direction - left  or right
     * @param speed - Speed between 0 and 255
     */
    void turn(Direction direction, unsigned int speed);

    /**
     * @brief Turn on Point
     * Drive on Motor Forward and the other Backward with the given speed
     * 
     * @param direction - left or right
     * @param speed - Speed between 0 and 255
     */
    void turnonpoint(Direction direction, unsigned int speed);

    /**
     * @brief Power of the Motor. Does not apply breaking.
     */
    void stop();

    //=====PRIVATE===================================================================================
   private:
    Adafruit_MotorShield pAFMS = Adafruit_MotorShield();  // Create the motor shield object with the default I2C address
    Adafruit_DCMotor *pMotorRight;                        ///<Pointer to right Motor Obj
    Adafruit_DCMotor *pMotorLeft;                         ///<Pointer to left Motor Obj
    int pSpeedLeft;                                       ///< actual Speed of left Motor
    int pSpeedRight;                                      ///< actual Speed of right Motor
    int pTurnSpeedLeft;
    int pTurnSpeedRight;
    Direction pWayVehicle = Direction::Forward;  ///< Actual movingdirection Forward or Backward
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
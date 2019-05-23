/**
 * @file Hoist.h
 * @brief Library for mechatronic component hoist.
 * 
 * The hoist module consists of a lifting mechanism powered by a servo motor.
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 2.0 - Refactored to match FSM Funccall  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-04-23
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018 - Robert Paly (robert.paly@hsr.ch) Glenn Huber (glenn.patrick.huber@hsr.ch)
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 * @todo Delet Default Constructor and don't include configuration..
 * @todo make private variable const where necessary
 * @todo refactor private variable name -> pHoistServo
 * @todo add used servo-typ to documentation
 */

#ifndef Hoist_h
#define Hoist_h

#include "Configuration.h"
#include "LogConfiguration.h"

#include <Servo.h>
#include "Arduino.h"

/**
 * @brief Provides the basic functions to controll the Hoist
 * 
 */
class Hoist {
    //=====PUBLIC====================================================================================
   public:
    Hoist();
    /**
    * @brief Construct a new Hoist object
    * 
    * @param hoistServoPin - Pin-number on which the Servocontroll is connected
    * @param hoistServoDelay - Duration of the Delay in ms between the position-updates 
    * @param posMax - Upper limit for servo-positon in degree
    * @param posMin - Lower limit for servo-position in degree
    */
    Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin);

    /**
     * @brief Initialise the Hoist in the low position
     * 
     */
    void init();

    /**
     * @brief Raise the Hoist 1° per servodelay/call until it's at positionMax
     * 
     */
    bool raise();

    /**
     * @brief Lower the Hoist 1° per servodelay/call until it's at positionMin
     * 
     */
    bool lower();

    /**
     * @brief attache servo to servopin
     * 
     */
    inline void attach() {
        DBFUNCCALLln("Hoist::attach()");
        hoistServo.attach(servoPin);
    }

    /**
     * @brief detach servo from servopin
     * 
     */
    inline void detach() {
        DBFUNCCALLln("Hoist::detach()");
        hoistServo.detach();
    }
    //=====PRIVATE====================================================================================
   private:
    Servo hoistServo;  ///< Instance of servo object
    int servoPin;      ///< Servopin
    int position;      ///< actual servo-position
    int positionMin;   ///< minimal possible position (Hardware-Restriction)
    int positionMax;   ///< maximal possible position (Hardware-Restriction)
    int servoDelay;    ///< delay in ms between raise/lower per 1 degree. This sets the servospeed

    unsigned long currentMillis = 0;   ///< will store current time for non blocking delay
    unsigned long previousMillis = 0;  ///< will store last time called for non blocking delay
};

#endif
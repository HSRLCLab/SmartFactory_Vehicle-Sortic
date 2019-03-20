/**
 * @file Hoist.h
 * @brief Library for mechatronic component hoist.
 * 
 * The hoist module consists of a lifting mechanism powered by a servo motor.
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * @author Robert Paly (robert.paly@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef Hoist_h
#define Hoist_h

#include "Arduino.h"
#include "Configuration.h"
#include "Modular.h"
#include "Servo.h"

/**
 * @brief 
 * 
 */
struct HoistState {
    bool loaded = false;       ///<
    bool loading = false;      ///<
    bool detachServo = false;  ///<
    int targetAngle = 110;     ///<
};

/**
 * @brief 
 * 
 */
class Hoist : public Component {
   public:
    /**
    * @brief Construct a new Hoist object
    * 
    * @param hoistServoPin - 
    * @param hoistServoDelay - 
    * @param posMax - 
    * @param posMin - 
    */
    Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin);

    /**
     * @brief 
     * 
     * @param state - 
     */
    void loop(HoistState *state);

    /**
     * @brief 
     * 
     */
    void load();

    /**
     * @brief 
     * 
     */
    void unload();

   protected:
    HoistState currentState;  ///<
    Servo hoistServo;         ///<
    int servoPin;             ///<
    int position;             ///<
    int servoDelay;           ///<
    int positionMin;          ///<
    int positionMax;          ///<
};

#endif
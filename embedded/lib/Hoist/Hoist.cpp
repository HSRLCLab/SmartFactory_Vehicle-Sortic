/**
 * @file Hoist.cpp
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

#include "Hoist.h"
#include "Arduino.h"
Hoist::Hoist() : servoPin(HOIST_SERVO_PIN),
                 position(HOIST_POSITION_MIN),
                 positionMin(HOIST_POSITION_MIN),
                 positionMax(HOIST_POISITION_MAX),
                 servoDelay(HOIST_SERVO_DELAY) {
    DBFUNCCALLln("Hoist::Hoist()");
}

Hoist::Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin) : servoPin(hoistServoPin),
                                                                               position(posMin),
                                                                               positionMin(posMin),
                                                                               positionMax(posMax),
                                                                               servoDelay(hoistServoDelay) {
    DBFUNCCALLln("Hoist::Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin)");
}

//NEW FUNCTION DRAFT (LMA)
// bool Hoist::setServoPosition(unsigned int pos) {
//     hoistServo.attach(servoPin);
//     if (pos <= positionMax && pos >= positionMin) {
//         while (HoistServo.read(servoPin) != pos) {
//             hoistServo.write(pos);
//             delay(1);
//         }
//         if (position == positionMax) {
//             currentState.loaded = true;
//             else if (position == positionMin) {
//                 currentState.loaded = false;
//             }
//             hoistServo.detach();
//             state->detachServo = false;
//             return true
//         } else
//             return false;
//     }

/**
 * @todo non blocking delay 
 */
bool Hoist::load() {
    DBFUNCCALLln("Hoist::load()");
    hoistServo.attach(servoPin);  ///< Attach the Servo variable to a pin
    if (position > positionMax) {
        DBSTATUSln("Begin loading..");
        DBINFO1("Position: ");
        DBINFO1ln(position);
        position--;
        hoistServo.write(position);  //< Update Servo-position
        delay(servoDelay);
    }
    if (position == positionMax) {
        // currentState.loaded = true;
        DBSTATUSln("Target loaded!");
        hoistServo.detach();
        return true;
    }
    return false;
}

/**
 * @todo non blocking delay 
 */
bool Hoist::unload() {
    DBFUNCCALLln("Hoist::unload()");
    hoistServo.attach(servoPin);
    if (position < positionMin) {
        DBSTATUSln("Begin unloading...");
        position++;
        hoistServo.write(position);
        delay(servoDelay);
    }
    if (position == positionMin) {
        // currentState.loaded = false;
        DBSTATUSln("Target unloaded!");
        hoistServo.detach();
        return true;
    }
    return false;
}

// void Hoist::loop(HoistState* state) {
//     DBFUNCCALLln("Hoist::loop(HoistState* state)");
//     state->loaded = currentState.loaded;
//     if (state->detachServo == true) {
//         hoistServo.detach();
//         state->detachServo = false;
//     }
// }

// void Hoist::Test(const int test) {
//     DBFUNCCALLln("Hoist::Test()");
//     if (test == 0 || test == 1) {
//         load();
//         delay(1000);
//         unload();
//     }
// }
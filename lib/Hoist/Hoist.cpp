/**
 * @file Hoist.cpp
 * @brief Library for mechatronic component hoist.
 * 
 * The hoist module consists of a lifting mechanism powered by a servo motor.
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * @author Robert Paly (robert.paly@hsr.ch)
 * 
 * @version 2.0 - Refactored to match FSM Funccall  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-04-23
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#include "Hoist.h"

//=====PUBLIC====================================================================================
Hoist::Hoist() : Hoist(HOIST_SERVO_PIN,
                       HOIST_SERVO_DELAY,
                       HOIST_POISITION_MAX,
                       HOIST_POSITION_MIN) {
    DBFUNCCALLln("Hoist::Hoist()");
}

Hoist::Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin) : servoPin(hoistServoPin),
                                                                               position(posMin),
                                                                               positionMin(posMin),
                                                                               positionMax(posMax),
                                                                               servoDelay(hoistServoDelay) {
    DBFUNCCALLln("Hoist::Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin)");
    Hoist::init();
}

/*
 * hoistServo.read() Read the current angle of the servo (the value passed to the LAST call to write()).
 */
void Hoist::init() {
    Hoist::attach();
    position -= 5;
    while (!lower()) {
    }
    // DBINFO1ln(hoistServo.read());
    // position = hoistServo.read();
    // DBINFO1("Position: ");
    // DBINFO1ln(position);
    Hoist::detach();
}

bool Hoist::raise() {
    DBFUNCCALLln("Hoist::raise()");
    currentMillis = millis();
    if ((currentMillis - previousMillis) > servoDelay) {
        previousMillis = currentMillis;
        if (position > positionMax) {
            // DBSTATUSln("Begin raiseing..");
            // DBINFO1("Position: ");
            // DBINFO1ln(position);
            // DBINFO1("Position read: ");
            // DBINFO1ln(String(hoistServo.read()));
            position--;
            hoistServo.write(position);  //< Update Servo-position
            // delay(servoDelay);
        }
    }

    if (position == positionMax) {
        DBSTATUSln("Hoist raised!");
        return true;
    }
    return false;
}

bool Hoist::lower() {
    DBFUNCCALLln("Hoist::lower()");
    // hoistServo.attach(servoPin);
    currentMillis = millis();
    if ((currentMillis - previousMillis) > servoDelay) {
        previousMillis = currentMillis;
        if (position < positionMin) {
            // DBSTATUSln("Begin lowering...");
            // DBINFO1("Position: ");
            // DBINFO1ln(position);
            // DBINFO1("Position read: ");
            // DBINFO1ln(hoistServo.read());
            position++;
            hoistServo.write(position);
            // delay(servoDelay);
        }
    }
    if (position == positionMin) {
        DBSTATUSln("Hoist lowered!");
        return true;
    }
    return false;
}

//=====PRIVATE====================================================================================
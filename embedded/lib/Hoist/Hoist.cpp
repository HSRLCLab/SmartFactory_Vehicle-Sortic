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

Hoist::Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin) {
    if (DEBUGGER == true) Serial.print("Initializing hoist...");
    servoPin = hoistServoPin;
    position = posMin;
    positionMin = posMin;
    positionMax = posMax;
    servoDelay = hoistServoDelay;
    if (DEBUGGER == true) Serial.println("Initializing complete!");
}

void Hoist::load() {
    hoistServo.attach(servoPin);
    if (position > positionMax) {
        if (DEBUGGER == true) Serial.println("Begin loading..");
        position--;
        hoistServo.write(position);
        delay(servoDelay);
    }
    if (position == positionMax) {
        currentState.loaded = true;
        if (DEBUGGER == true) Serial.println("Target loaded!");
    }
}

void Hoist::unload() {
    hoistServo.attach(servoPin);
    if (position < positionMin) {
        if (DEBUGGER == true) Serial.println("Begin unloading...");
        position++;
        hoistServo.write(position);
        delay(servoDelay);
    }
    if (position == positionMin) {
        currentState.loaded = false;
        if (DEBUGGER == true) Serial.println("Target unloaded!");
    }
}

void Hoist::loop(HoistState *state) {
    state->loaded = currentState.loaded;
    if (state->detachServo == true) {
        hoistServo.detach();
        state->detachServo = false;
    }
}
/*
    Hoist.cpp - Library for mechatronic component hoist.
    The hoist module consists of a lifting mechanism powered by a servo motor.
    Created by Glenn Huber, 03.05.2018
*/

#include "Arduino.h"
#include "Hoist.h"

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
    if (state->detachServo == true){
        hoistServo.detach();
        state->detachServo = false;
    }
}
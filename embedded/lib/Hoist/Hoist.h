/*
    Hoist.h - Library for mechatronic component hoist.
    The hoist module consists of a lifting mechanism powered by a servo motor.
    Created by Glenn Huber, 03.05.2018
*/

#ifndef Hoist_h
#define Hoist_h

#include "Arduino.h"
#include "Modular.h"
#include "Servo.h"
#include "Configuration.h"

struct HoistState {
    bool loaded = false;
    bool loading = false;
    bool detachServo = false;
    int targetAngle = 110;
    
};

class Hoist : public Component
{
    public:
        Hoist(int hoistServoPin, int hoistServoDelay, int posMax, int posMin);
        void loop(HoistState *state);
        void load();
        void unload();

    protected:
        HoistState currentState;
        Servo hoistServo;
        int servoPin;
        int position;
        int servoDelay;
        int positionMin;
        int positionMax;
};

#endif
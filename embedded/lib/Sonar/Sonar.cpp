/**
 * @file Sonar.cpp
 * @brief Library for mechatronic component sonar.
 * 
 * The sonar module consists of a sonar sensor and a servo motor.
 * Based on the deviation of the sonar to the focus point an error is
 * given to the loop to turn the sonar to the right direction.
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

#include "Sonar.h"
#include "Arduino.h"

Sonar::Sonar(int servoPin, int triggerPin, int echoPin, int maxDistance, int min_Error, int max_Error, int min_TurnAngle, int max_TurnAngle) {
    if (DEBUGGER == true) Serial.print("Initializing sonar...");
    sonar = new NewPing(triggerPin, echoPin, maxDistance);
    minError = min_Error;
    maxError = max_Error;
    minTurnAngle = min_TurnAngle;
    maxTurnAngle = max_TurnAngle;
    _servoPin = servoPin;
    if (DEBUGGER == true) Serial.println(" complete!");
}

void Sonar::loop(SonarState *state, int directionError, bool servoActive) {
    if ((servoActive == true) && (state->isAttached == false)) {
        sonarServo.attach(_servoPin);
        state->isAttached = true;
    }
    int uSonar = sonar->ping_cm();
    if (DEBUGGER == true) Serial.print("distance: ");
    if (DEBUGGER == true) Serial.println(uSonar);
    state->obstacleDistance = uSonar;
    if (servoActive == true) {
        if (DEBUGGER == true) Serial.print("Turn sonar with directionError: ");
        if (DEBUGGER == true) Serial.println(directionError);
        turnSonar(directionError);
    }
    calculateSonarFactor(state);
    if (state->detachServo == true) {
        sonarServo.detach();
        state->detachServo = false;
        state->isAttached = false;
    }
}

void Sonar::turnSonar(int directionError) {
    int turnSonar = map(directionError, minError, maxError, minTurnAngle, maxTurnAngle);
    if (DEBUGGER == true) Serial.print("turnanglesonar: ");
    if (DEBUGGER == true) Serial.println(turnSonar);
    sonarServo.write(turnSonar);
}

void Sonar::calculateSonarFactor(SonarState *state) {
    if ((state->obstacleDistance > 10) || (state->obstacleDistance == 0)) {
        state->sonarFactor = 1;
        if (DEBUGGER == true) Serial.println("Factor1");
    } else if ((state->obstacleDistance <= 10) && (state->obstacleDistance >= 5)) {
        state->sonarFactor = 0.5;
        if (DEBUGGER == true) Serial.println("Factor0.5");
    } else {
        state->sonarFactor = 0;
        if (DEBUGGER == true) Serial.println("Factor0");
    }
}

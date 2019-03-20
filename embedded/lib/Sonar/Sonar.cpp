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
    DBFUNCCALLln("Sonar::Sonar(servoPin,triggerPin,echoPin,maxDistance,min_Error,max_Error,min_TurnAngle,max_TurnAngle)");
    DBINFO1("Initializing sonar...");
    sonar = new NewPing(triggerPin, echoPin, maxDistance);
    minError = min_Error;
    maxError = max_Error;
    minTurnAngle = min_TurnAngle;
    maxTurnAngle = max_TurnAngle;
    _servoPin = servoPin;
    DBINFO1ln(" complete!");
}

void Sonar::loop(SonarState *state, int directionError, bool servoActive) {
    DBFUNCCALLln("Sonar::loop(*state,directionError,servoActive)");
    if ((servoActive == true) && (state->isAttached == false)) {
        sonarServo.attach(_servoPin);
        state->isAttached = true;
    }

    state->obstacleDistance = getDistanceToObstacle();
    if (servoActive == true) {
        DBSTATUS("Turn sonar with directionError: ");
        DBSTATUSln(directionError);
        turnSonar(directionError);
    }
    calculateSonarFactor(state);
    if (state->detachServo == true) {
        sonarServo.detach();
        state->detachServo = false;
        state->isAttached = false;
    }
}

int Sonar::getDistanceToObstacle() {
    DBFUNCCALLln("Sonar::getDistanceToObstacle()");
    int uSonar = sonar->ping_cm();
    DBINFO1("Distance: ");
    DBINFO1ln(uSonar);
    return uSonar;
}

void Sonar::turnSonar(int directionError) {
    DBFUNCCALLln("Sonar::turnSonar(int directionError)");
    DBINFO1("directionError: ");
    DBINFO1ln(directionError);
    int turnSonar = map(directionError, minError, maxError, minTurnAngle, maxTurnAngle);  // map direction error (-5 to 5) to angle (180 to 0)
    DBINFO1("Turn anglesonar: ");
    DBINFO1ln(turnSonar);
    sonarServo.write(turnSonar);
}

void Sonar::calculateSonarFactor(SonarState *state) {
    DBFUNCCALLln("Sonar::calculateSonarFactor(SonarState *state)");
    if ((state->obstacleDistance > 10) || (state->obstacleDistance == 0)) {
        DBINFO1ln("Factor: 1");
        state->sonarFactor = 1;
    } else if ((state->obstacleDistance <= 10) && (state->obstacleDistance >= 5)) {
        DBINFO1ln("Factor: 0.5");
        state->sonarFactor = 0.5;
    } else {
        DBINFO1ln("Factor: 0");
        state->sonarFactor = 0;
    }
}

void Sonar::Test(const int test) {
    DBFUNCCALLln("Sonar::Test()");
    if (test == 0 || test == 1) {
        sonarServo.attach(_servoPin);
        for (size_t i = 0; i < 6; i++) {
            turnSonar(i);
            delay(1000);
        }
        for (int i = -6; i < 6; i++) {
            turnSonar(-i);
            delay(1000);
        }
        turnSonar(0);
        delay(1000);
        sonarServo.detach();
        delay(1000);
    } else if (test == 0 || test == 2) {
        for (size_t i = 0; i < 50; i++) {
            getDistanceToObstacle();
            delay(200);
        }
    }
}
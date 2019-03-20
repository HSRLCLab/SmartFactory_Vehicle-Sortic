/**
 * @file Chassis.cpp
 * @brief Library for mechatronic component chassis.
 * 
 * The chassis contains 2 DC-lego-motors which power the movement of the chassis
 * and 5 IR-sensors which work as internal navigation components.
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

#include "Chassis.h"
#include "Arduino.h"

Chassis::Chassis(int givenSpeed, float kp, float ki, float kd, int motorNumRight, int motorNumLeft, int sensorPin_0, int sensorPin_1, int sensorPin_2, int sensorPin_3, int sensorPin_4) {
    if (DEBUGGER == true) Serial.print("Initializing chassis...");
    //Initializing sensors.
    sensorPin0 = sensorPin_0;
    sensorPin1 = sensorPin_1;
    sensorPin2 = sensorPin_2;
    sensorPin3 = sensorPin_3;
    sensorPin4 = sensorPin_4;
    pinMode(sensorPin0, INPUT);
    pinMode(sensorPin1, INPUT);
    pinMode(sensorPin2, INPUT);
    pinMode(sensorPin3, INPUT);
    pinMode(sensorPin4, INPUT);
    sensor0 = 0;
    sensor1 = 0;
    sensor2 = 0;
    sensor3 = 0;
    sensor4 = 0;

    previousSensorValue[0] = 0;
    previousSensorValue[1] = 0;
    previousSensorValue[2] = 0;
    previousSensorValue[3] = 0;
    previousSensorValue[4] = 0;

    currentState.sensor[0] = 0;
    currentState.sensor[1] = 0;
    currentState.sensor[2] = 0;
    currentState.sensor[3] = 0;
    currentState.sensor[4] = 0;

    //Initializing motors.
    numRight = motorNumRight;
    numLeft = motorNumLeft;
    AFMS = Adafruit_MotorShield();
    motorRight = AFMS.getMotor(numRight);
    motorLeft = AFMS.getMotor(numLeft);
    AFMS.begin();
    currentState.speed = givenSpeed;

    //Initializing PID values.
    KP = kp;
    KI = ki;
    KD = kd;
    error = 0;
    PValue = 0;
    IValue = 0;
    DValue = 0;
    PIDValue = 0;
    previousPIDValue = 0;
    previousError = 0;
    previousI = 0;
    korr = -0.01;
    korr1 = 0;
    korr2 = 0;
    leftMotorSpeed = 0;
    rightMotorSpeed = 0;
    leftMotorSpeed1 = 0;
    rightMotorSpeed1 = 0;
    targetSpeedLeft = 0;
    targetSpeedRight = 0;

    //Initializing timevariables
    millisOfDriving = 0;
    previousMillis = 0;
    timeLoopCounter = 0;

    //Initializing counter
    turnCounter = 0;
    crossCounterLeft = 0;
    crossCounterRight = 0;
    if (DEBUGGER == true) Serial.println(" complete!");
}

void Chassis::loop(ChassisState *state) {
    if (state->IR == false) {
        currentState.IR = false;
        state->IR = true;
    }
    currentState.speed = state->speed;
    currentState.direction = state->direction;
    readSensor();

    state->directionError = currentState.directionError;
    state->sensor[0] = currentState.sensor[0];
    state->sensor[1] = currentState.sensor[1];
    state->sensor[2] = currentState.sensor[2];
    state->sensor[3] = currentState.sensor[3];
    state->sensor[4] = currentState.sensor[4];

    calculatePID();

    state->PidValue = currentState.PidValue;

    if (currentState.actionDone == true) {
        state->actionDone = currentState.actionDone;
        currentState.actionDone = false;
        if (DEBUGGER == true) Serial.println("Inside actiondone statement");
        if (DEBUGGER == true) Serial.print("currentState.actionDone: ");
        if (DEBUGGER == true) Serial.println(currentState.actionDone);
    }
}

void Chassis::readSensor() {
    //Read sensor values
    if (DEBUGGER == true) Serial.println("sensorreading");
    sensor0 = digitalRead(sensorPin0);
    sensor1 = digitalRead(sensorPin1);
    sensor2 = digitalRead(sensorPin2);
    sensor3 = digitalRead(sensorPin3);
    sensor4 = digitalRead(sensorPin4);

    previousSensorValue[0] = currentState.sensor[0];
    previousSensorValue[1] = currentState.sensor[1];
    previousSensorValue[2] = currentState.sensor[2];
    previousSensorValue[3] = currentState.sensor[3];
    previousSensorValue[4] = currentState.sensor[4];
    if (DEBUGGER == true) Serial.print("previousValues: ");
    if (DEBUGGER == true) Serial.print(previousSensorValue[0]);
    if (DEBUGGER == true) Serial.print(previousSensorValue[1]);
    if (DEBUGGER == true) Serial.print(previousSensorValue[2]);
    if (DEBUGGER == true) Serial.print(previousSensorValue[3]);
    if (DEBUGGER == true) Serial.println(previousSensorValue[4]);

    currentState.sensor[0] = sensor0;
    currentState.sensor[1] = sensor1;
    currentState.sensor[2] = sensor2;
    currentState.sensor[3] = sensor3;
    currentState.sensor[4] = sensor4;
    if (DEBUGGER == true) Serial.print("actualValues: ");
    if (DEBUGGER == true) Serial.print(currentState.sensor[0]);
    if (DEBUGGER == true) Serial.print(currentState.sensor[1]);
    if (DEBUGGER == true) Serial.print(currentState.sensor[2]);
    if (DEBUGGER == true) Serial.print(currentState.sensor[3]);
    if (DEBUGGER == true) Serial.println(currentState.sensor[4]);

    //If no fullline is detected let the sensors read the lines for normal motorcontrol
    if (currentState.fullLine == false) {
        if (DEBUGGER == true) Serial.println("Normal sensorreading");
        if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 0) && (sensor3 == 0) && (sensor4 == 1))
            error = 4;
        else if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 0) && (sensor3 == 1) && (sensor4 == 1))
            error = 3;
        else if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 0) && (sensor3 == 1) && (sensor4 == 0))
            error = 2;
        else if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 1) && (sensor3 == 1) && (sensor4 == 0))
            error = 1;
        else if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 1) && (sensor3 == 0) && (sensor4 == 0))
            error = 0;
        else if ((sensor0 == 0) && (sensor1 == 1) && (sensor2 == 1) && (sensor3 == 0) && (sensor4 == 0))
            error = -1;
        else if ((sensor0 == 0) && (sensor1 == 1) && (sensor2 == 0) && (sensor3 == 0) && (sensor4 == 0))
            error = -2;
        else if ((sensor0 == 1) && (sensor1 == 1) && (sensor2 == 0) && (sensor3 == 0) && (sensor4 == 0))
            error = -3;
        else if ((sensor0 == 1) && (sensor1 == 0) && (sensor2 == 0) && (sensor3 == 0) && (sensor4 == 0))
            error = -4;
        else if ((sensor0 == 1) && (sensor1 == 1) && (sensor2 == 1) && (sensor3 == 0) && (sensor4 == 0))  //90°-Kurve
            error = -10;
        else if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 1) && (sensor3 == 1) && (sensor4 == 1))  //90°-Kurve
            error = 10;
        else if ((sensor0 == 1) && (sensor1 == 1) && (sensor2 == 1) && (sensor3 == 1) && (sensor4 == 1)) {
            currentState.fullLine = true;
        }
        //Conditions for lost navigation lines
        else if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 0) && (sensor3 == 0) && (sensor4 == 0)) {
            if (error == -4)
                error = -5;  //If outer sensor loses line, turn hard.
            else if (error == 4)
                error = 5;
        }
    }
    //Conditions for fulllines
    if (currentState.fullLine == true) {
        if ((((sensor0 == 0) && (sensor4 == 0)) || (currentState.direction == "stop")) && (currentState.IR == true)) {
            currentState.fullLine = false;
            if (DEBUGGER == true) Serial.println("changing actiondone because of fulline");
            currentState.actionDone = true;
        }
        if (currentState.direction == "right") {
            error = 5;
            if (DEBUGGER == true) Serial.print("Error: Cross");
            if (DEBUGGER == true) Serial.println(error);
        }
        if (currentState.direction == "straight") {
            error = 0;
            if (DEBUGGER == true) Serial.print("Error: Cross");
            if (DEBUGGER == true) Serial.println(error);
        }
        if (currentState.direction == "left") {
            error = -5;
            if (DEBUGGER == true) Serial.print("Error: Cross");
            if (DEBUGGER == true) Serial.println(error);
        }
        if (currentState.direction == "stop") {
            error = 0;
            if (DEBUGGER == true) Serial.println("error = 0");
        }
    }
    currentState.directionError = error;
}

void Chassis::calculatePID() {
    //previousPIDValue = PIDValue;
    PValue = error;
    IValue = IValue + error;
    DValue = PIDValue - previousPIDValue;
    IValue = constrain(IValue, -300, 300);

    //Print out values of the controller
    if (DEBUGGER == true) Serial.print(" I= ");
    if (DEBUGGER == true) Serial.print(IValue);
    if (DEBUGGER == true) Serial.print(" P= ");
    if (DEBUGGER == true) Serial.print(PValue);
    if (DEBUGGER == true) Serial.print(" D= ");
    if (DEBUGGER == true) Serial.print(DValue);

    PIDValue = (KP * PValue) + (KI * IValue) * (KD * DValue);
    previousError = error;
    if ((sensor0 == 0) && (sensor1 == 0) && (sensor2 == 1) && (sensor3 == 0) && (sensor4 == 0)) {
        if ((IValue > 10) || (IValue < -10)) {
            IValue = 0.95 * IValue;
        } else {
            IValue = 0;
        }
    }
    currentState.PidValue = PIDValue;

    if (DEBUGGER == true) Serial.print("PID = ");
    if (DEBUGGER == true) Serial.println(currentState.PidValue);
}

void Chassis::motorControl(float sonarFactor) {
    if (currentState.speed == 0) {
        if (DEBUGGER == true) Serial.println("stopping inside motorcontrol");
        stop();
    } else {
        if (DEBUGGER == true) Serial.println("Motorcontrol");
        korr2 = 1 - korr;
        korr1 = 1 + korr;

        leftMotorSpeed = currentState.speed + currentState.PidValue;
        rightMotorSpeed = currentState.speed - currentState.PidValue;
        leftMotorSpeed1 = constrain(leftMotorSpeed, 0, currentState.speed + 50);
        rightMotorSpeed1 = constrain(rightMotorSpeed, 0, currentState.speed + 50);

        //Calculate the speed for both motors individualy
        if (DEBUGGER == true) Serial.print("SonarFactor inside MotorControl: ");
        if (DEBUGGER == true) Serial.println(sonarFactor);
        targetSpeedLeft = sonarFactor * korr2 * leftMotorSpeed1;
        targetSpeedRight = sonarFactor * korr1 * rightMotorSpeed1;

        motorLeft->run(FORWARD);
        motorRight->run(FORWARD);

        //Startspeed settings
        motorLeft->setSpeed(getStartSpeedLeft());
        motorRight->setSpeed(getStartSpeedRight());

        //Drivespeed settings
        motorLeft->setSpeed(targetSpeedLeft);
        motorRight->setSpeed(targetSpeedRight);

        if (DEBUGGER == true) Serial.println("Speed of motors: ");
        if (DEBUGGER == true) Serial.print(targetSpeedLeft);
        if (DEBUGGER == true) Serial.print(" || ");
        if (DEBUGGER == true) Serial.println(targetSpeedRight);
    }
}

int Chassis::getStartSpeedLeft() {
    if (targetSpeedLeft < 30) {
        return 0;
    } else if ((targetSpeedLeft >= 30) && (targetSpeedLeft < 60)) {
        return 100;
    } else {
        return 60;
    }
}

int Chassis::getStartSpeedRight() {
    if (targetSpeedRight < 30) {
        return 0;
    } else if ((targetSpeedRight >= 30) && (targetSpeedRight < 60)) {
        return 100;
    } else {
        return 60;
    }
}

void Chassis::stop() {
    motorLeft->setSpeed(0);
    motorRight->setSpeed(0);
    if (DEBUGGER == true) Serial.println("Vehicle stopped");
}

void Chassis::driveBack() {
    motorLeft->run(BACKWARD);
    motorRight->run(BACKWARD);
    motorLeft->setSpeed(PUSH_SPEED);
    motorRight->setSpeed(PUSH_SPEED);
    motorLeft->setSpeed(currentState.speed);
    motorRight->setSpeed(currentState.speed);
}

void Chassis::driveBackLimited(unsigned int driveTime, unsigned long startTime) {
    unsigned long actualMillis = millis();
    millisOfDriving = actualMillis - startTime;
    if (millisOfDriving > driveTime) {
        motorLeft->setSpeed(0);
        motorRight->setSpeed(0);
        currentState.actionDone = true;
        if (DEBUGGER == true) Serial.println("Changing actiondone because of driveback");
        currentState.IR = true;
    } else {
        currentState.IR = false;
        motorLeft->run(BACKWARD);
        motorRight->run(BACKWARD);
        motorLeft->setSpeed(PUSH_SPEED);
        motorLeft->setSpeed(currentState.speed);
        motorRight->setSpeed(PUSH_SPEED);
        motorRight->setSpeed(currentState.speed);
        if (DEBUGGER == true) Serial.println("Driving back");
    }
}

void Chassis::driveStraightLimited(unsigned int driveTime, unsigned long startTime) {
    unsigned long actualMillis = millis();
    millisOfDriving = actualMillis - startTime;
    if (DEBUGGER == true) Serial.print("millifOfDriving: ");
    if (DEBUGGER == true) Serial.println(millisOfDriving);
    if (millisOfDriving > driveTime) {
        motorRight->setSpeed(0);
        motorLeft->setSpeed(0);
        currentState.actionDone = true;
        if (DEBUGGER == true) Serial.println("Changing actiondone because of drivestraight");
        if (DEBUGGER == true) Serial.println("ifcase inside drive straight");
        currentState.IR = true;
    } else {
        currentState.IR = false;
        motorLeft->run(FORWARD);
        motorRight->run(FORWARD);
        motorLeft->setSpeed(PUSH_SPEED);
        motorRight->setSpeed(PUSH_SPEED);
        motorLeft->setSpeed(currentState.speed);
        motorRight->setSpeed(currentState.speed);
        if (DEBUGGER == true) Serial.println("elsecase inside drive straight");
    }
}

void Chassis::driveStraight() {
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
    motorLeft->setSpeed(PUSH_SPEED);
    motorRight->setSpeed(PUSH_SPEED);
    motorLeft->setSpeed(currentState.speed);
    motorRight->setSpeed(currentState.speed);
}

void Chassis::turnLeftLimited(unsigned int curveTime, unsigned long startTime) {
    motorLeft->setSpeed(0);
    unsigned long actualMillis = millis();
    millisOfDriving = actualMillis - startTime;
    if (DEBUGGER == true) Serial.print("millifOfDriving: ");
    if (DEBUGGER == true) Serial.println(millisOfDriving);
    if (millisOfDriving > curveTime) {
        motorRight->setSpeed(0);
        currentState.actionDone = true;
        if (DEBUGGER == true) Serial.println("Changing actiondone because of turnleft");
        if (DEBUGGER == true) Serial.println("ifcase inside turn left");
        currentState.IR = true;
    } else {
        currentState.IR = false;
        motorRight->run(FORWARD);
        motorRight->setSpeed(PUSH_SPEED);
        motorRight->setSpeed(currentState.speed);
        if (DEBUGGER == true) Serial.println("elsecase inside turn left");
    }
}

void Chassis::turnLeft() {
    motorLeft->run(RELEASE);
    motorRight->run(FORWARD);
    motorRight->setSpeed(PUSH_SPEED);
    motorRight->setSpeed(currentState.speed);
    if (DEBUGGER == true) Serial.println("Turnleft");
}

void Chassis::turnRightLimited(unsigned int curveTime, unsigned long startTime) {
    motorRight->setSpeed(0);
    unsigned long actualMillis = millis();
    millisOfDriving = actualMillis - startTime;
    if (DEBUGGER == true) Serial.print("millifOfDriving: ");
    if (DEBUGGER == true) Serial.println(millisOfDriving);
    if (millisOfDriving > curveTime) {
        motorLeft->setSpeed(0);
        currentState.actionDone = true;
        if (DEBUGGER == true) Serial.println("Changing actiondone because of turnright");
        if (DEBUGGER == true) Serial.println("ifcase inside turn right");
        currentState.IR = true;
    } else {
        currentState.IR = false;
        motorLeft->run(FORWARD);
        motorLeft->setSpeed(PUSH_SPEED);
        motorLeft->setSpeed(currentState.speed);
        if (DEBUGGER == true) Serial.println("elsecase inside turn right");
    }
}

void Chassis::turnRight() {
    motorRight->run(RELEASE);
    motorLeft->run(FORWARD);
    motorLeft->setSpeed(PUSH_SPEED);
    motorLeft->setSpeed(currentState.speed);
    if (DEBUGGER == true) Serial.println("Turnright");
}

void Chassis::turnAround(int sensor) {
    int i = sensor;
    if (DEBUGGER == true) Serial.println("Inside turnaround");
    if (currentState.actionDone == false) {
        motorLeft->run(BACKWARD);
        motorRight->run(FORWARD);
        motorLeft->setSpeed(PUSH_SPEED);
        motorLeft->setSpeed(currentState.speed);
        motorRight->setSpeed(PUSH_SPEED);
        motorRight->setSpeed(currentState.speed);
        if (DEBUGGER == true) Serial.println("Turnaround");
        if (DEBUGGER == true) Serial.print("turnCounter: ");
        if (DEBUGGER == true) Serial.println(turnCounter);
        if (DEBUGGER == true) Serial.print("currentsensor: ");
        if (DEBUGGER == true) Serial.println(currentState.sensor[i]);
        if (DEBUGGER == true) Serial.print("Sensor: ");
        if (DEBUGGER == true) Serial.println(i);
        currentState.IR = false;
    }
    if ((currentState.sensor[i] == 0) && (turnCounter == 0)) {
        turnCounter++;
        if (DEBUGGER == true) Serial.println("turncounter incrementing");
    }
    if ((previousSensorValue[i] == 0) && (currentState.sensor[i] == 1) && (turnCounter == 1)) {
        currentState.actionDone = true;
        if (DEBUGGER == true) Serial.println("Changing actiondone because of turnaround");
        currentState.IR = true;
        turnCounter = 0;
        motorLeft->setSpeed(0);
        motorRight->setSpeed(0);
        if (DEBUGGER == true) Serial.println("Turnaround complete");
    }
}
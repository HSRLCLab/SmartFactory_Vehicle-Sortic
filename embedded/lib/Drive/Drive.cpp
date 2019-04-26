/**
 * @file Drive.cpp
 * @brief 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Description - {author} - {date}
 * 
 * @date 2019-04-26
 * @copyright Copyright (c) 2019
 * 
 */
#include "Drive.h"

//=====PUBLIC====================================================================================

Drive::Drive() : Drive(RIGHT_MOTOR,
                       LEFT_MOTOR) {
    DBFUNCCALLln("Drive::Drive()");
}

Drive::Drive(const int MotorPortRight, const int MotorPortLeft) {
    DBFUNCCALLln("Drive::Drive(const int MotorPortRight, const int MotorPortLeft)");
    pMotorRight = pAFMS.getMotor(MotorPortRight);
    pMotorLeft = pAFMS.getMotor(MotorPortLeft);
    pAFMS.begin();  // create with the default frequency 1.6KHz
}

void Drive::drive(Direction direction, unsigned int speed) {
    DBFUNCCALLln("Drive::drive(Direction direction, unsigned int speed)");
    speed = constrain(speed, 0, 255);
    pWay = direction;
    pSpeedLeft = speed;
    pSpeedRight = speed;
    pMotorLeft->setSpeed(pSpeedLeft);
    pMotorRight->setSpeed(pSpeedRight);
    DBINFO3ln(decodeDirection(direction));
    DBINFO3("Speed L/R: ");
    DBINFO3(pSpeedLeft);
    DBINFO3(" / ");
    DBINFO3ln(pSpeedRight);
    switch (direction) {
        case Direction::Forward:
            pMotorLeft->run(FORWARD);
            pMotorRight->run(FORWARD);
            break;
        case Direction::Backward:
            pMotorLeft->run(BACKWARD);
            pMotorRight->run(BACKWARD);
            break;
        default:
            break;
    }
}

bool Drive::drive(Direction direction, unsigned int speed, unsigned int time) {
    DBFUNCCALLln("");
}

void Drive::turn(Direction direction, unsigned int speed) {
    DBFUNCCALLln("Drive::turn(Direction direction, unsigned int speed)");
    switch (direction) {
        case Direction::Left:
            pSpeedLeft -= speed;
            pSpeedRight += speed;
            break;
        case Direction::Right:
            pSpeedLeft += speed;
            pSpeedRight -= speed;
            break;
        default:
            break;
    }
    pSpeedLeft = constrain(pSpeedLeft, 0, 255);
    pSpeedRight = constrain(pSpeedRight, 0, 255);
    DBINFO3ln(decodeDirection(direction));
    DBINFO3("Speed L/R: ");
    DBINFO3(pSpeedLeft);
    DBINFO3(" / ");
    DBINFO3ln(pSpeedRight);
    pMotorLeft->setSpeed(pSpeedLeft);
    pMotorRight->setSpeed(pSpeedRight);
}

bool Drive::turn(Direction direction, unsigned int speed, unsigned int time) {
    DBFUNCCALLln("");
}

/**
 * @todo change direction if vehicle is moving forward or backward so the turndirectiomn still matches direction
 */
void Drive::turnonpoint(Direction direction, unsigned int speed) {
    DBFUNCCALLln("Drive::turn(Direction direction, unsigned int speed)");
    pSpeedLeft = constrain(pSpeedLeft, 0, 255);
    pSpeedRight = constrain(pSpeedRight, 0, 255);
    pMotorLeft->setSpeed(pSpeedLeft);
    pMotorRight->setSpeed(pSpeedRight);
    switch (direction) {
        case Direction::Left:
            pMotorLeft->run(BACKWARD);
            pMotorRight->run(FORWARD);
            break;
        case Direction::Right:
            pMotorLeft->run(FORWARD);
            pMotorRight->run(BACKWARD);
            break;
        default:
            break;
    }
}

void Drive::stop() {
    DBFUNCCALLln("Drive::stop()");
    pSpeedLeft = 0;
    pSpeedRight = 0;
    pMotorLeft->setSpeed(pSpeedLeft);
    pMotorRight->setSpeed(pSpeedRight);
    pMotorLeft->run(RELEASE);
    pMotorRight->run(RELEASE);
}
//=====PRIVATE===================================================================================

//============================================================================
//==Aux-Function==============================================================
String Drive::decodeDirection(Direction direction) {
    switch (direction) {
        case Direction::Left:
            return "Direction::Left";
            break;
        case Direction::Right:
            return "Direction::Right";
            break;
        case Direction::Forward:
            return "Direction::Forward";
            break;
        case Direction::Backward:
            return "Direction::Backward";
            break;
        default:
            return "ERROR: No matching event";
            break;
    }
}
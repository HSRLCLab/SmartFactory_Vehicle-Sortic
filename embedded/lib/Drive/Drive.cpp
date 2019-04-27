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
    stop();
}

void Drive::drive(Direction direction, unsigned int speed) {
    DBFUNCCALLln("Drive::drive(Direction direction, unsigned int speed)");
    speed = constrain(speed, 0, 255);
    pWayVehicle = direction;
    pSpeedLeft = speed;
    pSpeedRight = speed;
    pMotorLeft->setSpeed(pSpeedLeft);
    pMotorRight->setSpeed(pSpeedRight);
    DBINFO3ln(decodeDirection(direction));
    DBINFO3("Speed L/R: ");
    DBINFO3(pSpeedLeft);
    DBINFO3(" / ");
    DBINFO3ln(pSpeedRight);
    switch (pWayVehicle) {
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

/**
 * @todo change direction if vehicle is moving forward or backward so the turndirectiomn still matches direction
 */
void Drive::turn(Direction direction, unsigned int speed) {
    DBFUNCCALLln("Drive::turn(Direction direction, unsigned int speed)");
    switch (direction) {
        case Direction::Left:
            pTurnSpeedLeft = pSpeedLeft - speed;
            pTurnSpeedRight = pSpeedRight + speed;
            break;
        case Direction::Right:
            pTurnSpeedLeft = pSpeedLeft + speed;
            pTurnSpeedRight = pSpeedRight - speed;
            break;
        default:
            break;
    }
    pTurnSpeedLeft = constrain(pTurnSpeedLeft, 0, 255);
    pTurnSpeedRight = constrain(pTurnSpeedRight, 0, 255);
    DBINFO3ln(decodeDirection(direction));
    DBINFO3("Speed L/R: ");
    DBINFO3(pTurnSpeedLeft);
    DBINFO3(" / ");
    DBINFO3ln(pTurnSpeedRight);
    pMotorLeft->setSpeed(pTurnSpeedLeft);
    pMotorRight->setSpeed(pTurnSpeedRight);
    switch (pWayVehicle) {
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

void Drive::turnonpoint(Direction direction, unsigned int speed) {
    DBFUNCCALLln("Drive::turn(Direction direction, unsigned int speed)");
    speed = constrain(speed, 0, 255);
    pTurnSpeedLeft = speed;
    pTurnSpeedRight = speed;
    pMotorLeft->setSpeed(pTurnSpeedLeft);
    pMotorRight->setSpeed(pTurnSpeedRight);
    DBINFO3("Speed L/R: ");
    switch (direction) {
        case Direction::Left:
            pMotorLeft->run(BACKWARD);
            pMotorRight->run(FORWARD);
            DBINFO3("-");
            DBINFO3(pSpeedLeft);
            DBINFO3(" / ");
            DBINFO3ln(pSpeedRight);
            break;
        case Direction::Right:
            pMotorLeft->run(FORWARD);
            pMotorRight->run(BACKWARD);
            DBINFO3(pSpeedLeft);
            DBINFO3(" / ");
            DBINFO3("-")
            DBINFO3ln(pSpeedRight);
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
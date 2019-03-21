/**
 * @file Vision.cpp
 * 
 * @brief Library for mechatronic component vision.
 * 
 * The vision module consists of a pixy-cam and a servo motor.
 * The cam searches for target-objects and adjusts its position to that direction.
 * Afterwarts it sends information for course adaption.
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * 
 * @version 1.2 - Added Test-case - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-05 
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-06-05
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "Vision.h"
#include "Arduino.h"

Vision::Vision(int startAngle, int visionPin, int dddelay, int toleranceL, int toleranceR) {
    DBFUNCCALLln("Vision::Vision(int startAngle, int visionPin, int dddelay, int toleranceL, int toleranceR)");
    DBINFO1("Initializing vision...");
    pixy.init();
    _servoPin = visionPin;
    ddelay = dddelay;

    toleranceLeft = toleranceL;
    toleranceRight = toleranceR;

    visionState.servoAngle = startAngle;
    visionState.target = 0;
    target1 = 0;
    target2 = 0;
    target3 = 0;
    target4 = 0;
    target5 = 0;
    target6 = 0;
    target7 = 0;
    target8 = 0;
    targetFound = 0;
    targetFound1 = 0;
    targetFound2 = 0;
    targetFound3 = 0;
    targetFound4 = 0;
    failCounter = 0;

    DBINFO1ln("compelete!");
}

void Vision::loop(VisionState *state) {
    if (state->reset == true) {
        reset(state);
    }
    visionServo.attach(_servoPin);
    DBINFO1ln("Beginn visionloop");
    static int ii = 0;  //Cycle variable
    DBINFO1("I: ");
    DBINFO1ln(ii);
    int j;
    uint16_t blocks;
    char buf[32];
    DBINFO1("Target: ");
    visionState.target = state->target;
    DBINFO1ln(visionState.target);

    blocks = pixy.getBlocks();
    DBINFO1("ServoAngle: ");
    DBINFO1(visionState.servoAngle);
    visionServo.write(visionState.servoAngle);
    state->servoAngle = visionState.servoAngle;
    state->targetDetected = visionState.targetDetected;
    DBINFO1("targetdetected");
    DBINFO1ln(state->targetDetected);
    delay(13);  //Empirically testet value
    DBINFO1ln("before first if-case");
    if (blocks) {
        DBINFO1ln("inside first if-case");
        ii++;
        if ((ii % ddelay) == 0) {
            failCounter = 0;
            DBINFO1("Current angle: ");
            DBINFO1ln(visionState.servoAngle);

            sprintf(buf, "Detected %d:\n", blocks);
            DBINFO1(buf);
            for (j = 0; j < blocks; j++) {
                sprintf(buf, " block %d: ", j);
                DBINFO1(buf);
                pixy.blocks[j].print();
            }
            DBINFO1("The Target is = ");
            DBINFO1ln(visionState.target);
            DBINFO1("signature= ");
            DBINFO1(pixy.blocks[0].signature);
            DBINFO1(" | ");
            DBINFO1(pixy.blocks[1].signature);
            DBINFO1(" | ");
            DBINFO1(pixy.blocks[2].signature);
            DBINFO1(" | ");
            DBINFO1(pixy.blocks[3].signature);
            DBINFO1(" | ");
            DBINFO1(pixy.blocks[4].signature);
            DBINFO1(" | ");
            DBINFO1(pixy.blocks[5].signature);
            DBINFO1(" | ");
            DBINFO1(pixy.blocks[6].signature);
            DBINFO1(" | ");
            DBINFO1ln(pixy.blocks[7].signature);
            target1 = 0;
            target2 = 0;
            target3 = 0;
            target4 = 0;
            target5 = 0;
            target6 = 0;
            target7 = 0;
            target8 = 0;
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if ((pixy.blocks[0].signature == visionState.target) && (pixy.blocks[0].x > toleranceRight) && (targetFound1 == 0)) {
                target1 = 1;
                visionState.servoAngle--;
                DBINFO1ln("Target1 true");
            } else {
                target1 = 0;
            }

            if ((pixy.blocks[0].signature == visionState.target) && (pixy.blocks[0].x < toleranceLeft) && (targetFound1 == 0) && (target1 == 0)) {
                target2 = 1;
                visionState.servoAngle++;
                DBINFO1ln("Target2 true");
            } else {
                target2 = 0;
            }

            if ((pixy.blocks[1].signature == visionState.target) && (pixy.blocks[1].x > toleranceRight) && (targetFound2 == 0) && (target1 == 0) && (target2 == 0)) {
                target3 = 1;
                visionState.servoAngle--;
                DBINFO1ln("Target3 true");
            } else {
                target3 = 0;
            }

            if ((pixy.blocks[1].signature == visionState.target) && (pixy.blocks[1].x < toleranceLeft) && (targetFound2 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0)) {
                target4 = 1;
                visionState.servoAngle++;
                DBINFO1ln("Target4 true");
            } else {
                target4 = 0;
            }

            if ((pixy.blocks[2].signature == visionState.target) && (pixy.blocks[2].x > toleranceRight) && (targetFound3 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0)) {
                target5 = 1;
                visionState.servoAngle--;
                DBINFO1ln("Target5 true");
            } else {
                target5 = 0;
            }

            if ((pixy.blocks[2].signature == visionState.target) && (pixy.blocks[2].x < toleranceLeft) && (targetFound3 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0) && (target5 == 0)) {
                target6 = 1;
                visionState.servoAngle++;
                DBINFO1ln("Target6 true");
            } else {
                target6 = 0;  ///<@todo Fehler im sourcecode!?
            }

            if ((pixy.blocks[3].signature == visionState.target) && (pixy.blocks[3].x > toleranceRight) && (targetFound4 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0) && (target5 == 0) && (target6 == 0)) {
                target7 = 1;
                visionState.servoAngle--;
                DBINFO1ln("Target7 true");
            } else {
                target7 = 0;
            }

            if ((pixy.blocks[3].signature == visionState.target) && (pixy.blocks[3].x < toleranceLeft) && (targetFound4 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0) && (target5 == 0) && (target6 == 0) && (target7 == 0)) {
                target8 = 1;
                visionState.servoAngle++;
                DBINFO1ln("Target8 true");
            } else {
                target8 = 0;
            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if ((pixy.blocks[0].signature == visionState.target) && (pixy.blocks[0].x >= toleranceRight) && (pixy.blocks[0].x <= toleranceLeft)) {
                targetFound1 = 1;
            } else {
                targetFound1 = 0;
            }

            if ((pixy.blocks[1].signature == visionState.target) && (pixy.blocks[1].x >= toleranceRight) && (pixy.blocks[1].x <= toleranceLeft)) {
                targetFound2 = 1;
            } else {
                targetFound2 = 0;
            }

            if ((pixy.blocks[2].signature == visionState.target) && (pixy.blocks[2].x >= toleranceRight) && (pixy.blocks[2].x <= toleranceLeft)) {
                targetFound3 = 1;
            } else {
                targetFound3 = 0;
            }

            if ((pixy.blocks[3].signature == visionState.target) && (pixy.blocks[3].x >= toleranceRight) && (pixy.blocks[3].x <= toleranceLeft)) {
                targetFound4 = 1;
            } else {
                targetFound4 = 0;
            }

            targetFound = target1 + target2 + target3 + target4 + target5 + target6 + target7 + target8 + targetFound1 + targetFound2 + targetFound3 + targetFound4;

            DBINFO1ln();
            DBINFO1ln("All targets: ");
            DBINFO1(target1);
            DBINFO1(target2);
            DBINFO1(target2);
            DBINFO1(target3);
            DBINFO1(target4);
            DBINFO1(target5);
            DBINFO1(target6);
            DBINFO1(target7);
            DBINFO1ln(target8);
            DBINFO1ln("Found targets: ");
            DBINFO1(targetFound1);
            DBINFO1(targetFound2);
            DBINFO1(targetFound3);
            DBINFO1ln(targetFound4);

            if ((visionState.servoAngle > 179) && (targetFound == 0)) {
                visionState.servoAngle = 0;
                DBWARNINGln("Wrong object detected!!");
            }
            if ((visionState.servoAngle <= 179) && (targetFound == 0)) {
                visionState.servoAngle++;
                DBWARNINGln("Wrong object detected!!");
            }
            DBINFO1ln("------------------------END---------------------------");
        }
    } else {
        ii++;
        DBINFO1("I:");
        DBINFO1ln(ii);
        if ((ii % ddelay) == 0) {
            failCounter++;
            if (failCounter > 8) {
                targetFound = 0;
                targetFound1 = 0;
                targetFound2 = 0;
                targetFound3 = 0;
                targetFound4 = 0;
                DBINFO1ln("Nothing found");

                if ((visionState.servoAngle > 179) && (targetFound == 0)) {
                    visionState.servoAngle = 0;
                    DBINFO1ln("Adjust camera");
                }
                if ((visionState.servoAngle <= 179) && (targetFound == 0)) {
                    visionState.servoAngle = visionState.servoAngle + 1;
                    DBINFO1ln("Adjust camera");
                }

                DBINFO1ln("----------------------END-------------------------------");
            }
        }
    }
    // DBINFO1ln("before signaturechange");
    pixy.blocks[0].signature = 10;
    pixy.blocks[1].signature = 10;
    pixy.blocks[2].signature = 10;
    pixy.blocks[3].signature = 10;
    pixy.blocks[4].signature = 10;
    pixy.blocks[5].signature = 10;
    pixy.blocks[6].signature = 10;
    pixy.blocks[7].signature = 10;

    if (ii == 200) {
        ii = 0;
    }

    if ((targetFound1 == 1) || (targetFound2 == 1) || (targetFound3 == 1) || (targetFound4 == 1)) {
        DBINFO1ln("targetdetected inside loop");
        state->targetDetected = true;
    } else {
        state->targetDetected = false;
    }
    visionServo.detach();
}

void Vision::turnVision(int angle) {
    DBFUNCCALLln("Vision::turnVision(int angle)");
    DBINFO1("Angle: ");
    DBINFO1ln(angle);
    visionServo.attach(_servoPin);
    visionServo.write(angle);
    visionServo.detach();
}

void Vision::reset(VisionState *state) {
    DBFUNCCALLln("Vision::reset()");
    failCounter = 0;
    targetFound4 = 0;
    targetFound3 = 0;
    targetFound2 = 0;
    targetFound1 = 0;
    targetFound = 0;
    target8 = 0;
    target7 = 0;
    target6 = 0;
    target5 = 0;
    target4 = 0;
    target3 = 0;
    target2 = 0;
    target1 = 0;
    visionState.target = 0;
    visionState.servoAngle = 90;
    state->reset = false;
}
void Vision::Test(const int test) {
    DBFUNCCALLln("Vision::Test(const int test)");
    int testdelay = 10;
    if (test == 0 || test == 1) {  //Test Servo turnVision
        visionServo.attach(_servoPin);
        int maxinc = 90;  //Chose max increment range for left/ritgh-turn
        for (size_t i = 0; i < maxinc; i++) {
            turnVision(i + 90);
            delay(testdelay);
        }
        for (int i = -maxinc; i < maxinc; i++) {
            turnVision(-i + 90);
            delay(testdelay);
        }
        for (size_t i = 0; i < maxinc; i++) {
            turnVision(i);
            delay(testdelay);
        }
        turnVision(90);
        delay(testdelay);
        visionServo.detach();
        delay(testdelay);
    } else if (test == 0 || test == 2) {
        /*func*/
    } else {
        DBERROR("No vailid Test selected.");
        DBINFO1("Your Input: ");
        DBINFO1ln(test);
    }
}
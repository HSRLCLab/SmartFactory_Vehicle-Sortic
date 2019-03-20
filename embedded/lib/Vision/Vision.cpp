/**
 * @file Vision.cpp
 * 
 * @brief Library for mechatronic component vision.
 * 
 * The vision module consists of a pixy-cam and a servo motor.
 * The cam searches for target-objects and adjusts its position to that direction.
 * Afterwarts it sends information for course adaption.
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * 
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
    visionServo.attach(_servoPin);
    if (DEBUGGER == true) Serial.println("Beginn visionloop");
    static int ii = 0;  //Cycle variable
    if (DEBUGGER == true) Serial.print("I: ");
    if (DEBUGGER == true) Serial.println(ii);
    int j;
    uint16_t blocks;
    char buf[32];
    if (DEBUGGER == true) Serial.print("Target: ");
    visionState.target = state->target;
    if (DEBUGGER == true) Serial.println(visionState.target);

    blocks = pixy.getBlocks();
    if (DEBUGGER == true) Serial.print("ServoAngle: ");
    if (DEBUGGER == true) Serial.print(visionState.servoAngle);
    visionServo.write(visionState.servoAngle);
    state->servoAngle = visionState.servoAngle;
    state->targetDetected = visionState.targetDetected;
    if (DEBUGGER == true) Serial.print("targetdetected");
    if (DEBUGGER == true) Serial.println(state->targetDetected);
    delay(13);  //Empirically testet value
    if (DEBUGGER == true) Serial.println("before first if-case");
    if (blocks) {
        if (DEBUGGER == true) Serial.println("inside first if-case");
        ii++;
        if ((ii % ddelay) == 0) {
            failCounter = 0;
            if (DEBUGGER == true) Serial.print("Current angle: ");
            if (DEBUGGER == true) Serial.println(visionState.servoAngle);

            sprintf(buf, "Detected %d:\n", blocks);
            if (DEBUGGER == true) Serial.print(buf);
            for (j = 0; j < blocks; j++) {
                sprintf(buf, " block %d: ", j);
                if (DEBUGGER == true) Serial.print(buf);
                pixy.blocks[j].print();
            }
            if (DEBUGGER == true) Serial.print("The Target is = ");
            if (DEBUGGER == true) Serial.println(visionState.target);
            if (DEBUGGER == true) Serial.print("signature= ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[0].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[1].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[2].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[3].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[4].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[5].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.print(pixy.blocks[6].signature);
            if (DEBUGGER == true) Serial.print(" | ");
            if (DEBUGGER == true) Serial.println(pixy.blocks[7].signature);
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
                if (DEBUGGER == true) Serial.println("Target1 true");
            } else {
                target1 = 0;
            }

            if ((pixy.blocks[0].signature == visionState.target) && (pixy.blocks[0].x < toleranceLeft) && (targetFound1 == 0) && (target1 == 0)) {
                target2 = 1;
                visionState.servoAngle++;
                if (DEBUGGER == true) Serial.println("Target2 true");
            } else {
                target2 = 0;
            }

            if ((pixy.blocks[1].signature == visionState.target) && (pixy.blocks[1].x > toleranceRight) && (targetFound2 == 0) && (target1 == 0) && (target2 == 0)) {
                target3 = 1;
                visionState.servoAngle--;
                if (DEBUGGER == true) Serial.println("Target3 true");
            } else {
                target3 = 0;
            }

            if ((pixy.blocks[1].signature == visionState.target) && (pixy.blocks[1].x < toleranceLeft) && (targetFound2 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0)) {
                target4 = 1;
                visionState.servoAngle++;
                if (DEBUGGER == true) Serial.println("Target4 true");
            } else {
                target4 = 0;
            }

            if ((pixy.blocks[2].signature == visionState.target) && (pixy.blocks[2].x > toleranceRight) && (targetFound3 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0)) {
                target5 = 1;
                visionState.servoAngle--;
                if (DEBUGGER == true) Serial.println("Target5 true");
            } else {
                target5 = 0;
            }

            if ((pixy.blocks[2].signature == visionState.target) && (pixy.blocks[2].x < toleranceLeft) && (targetFound3 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0) && (target5 == 0)) {
                target6 = 1;
                visionState.servoAngle++;
                if (DEBUGGER == true) Serial.println("Target6 true");
            } else {
                target6 = 0;  //Fehler im sourcecode!!!!!!!!!!!!!!
            }

            if ((pixy.blocks[3].signature == visionState.target) && (pixy.blocks[3].x > toleranceRight) && (targetFound4 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0) && (target5 == 0) && (target6 == 0)) {
                target7 = 1;
                visionState.servoAngle--;
                if (DEBUGGER == true) Serial.println("Target7 true");
            } else {
                target7 = 0;
            }

            if ((pixy.blocks[3].signature == visionState.target) && (pixy.blocks[3].x < toleranceLeft) && (targetFound4 == 0) && (target1 == 0) && (target2 == 0) && (target3 == 0) && (target4 == 0) && (target5 == 0) && (target6 == 0) && (target7 == 0)) {
                target8 = 1;
                visionState.servoAngle++;
                if (DEBUGGER == true) Serial.println("Target8 true");
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

            if (DEBUGGER == true) Serial.println();
            if (DEBUGGER == true) Serial.println("All targets: ");
            if (DEBUGGER == true) Serial.print(target1);
            if (DEBUGGER == true) Serial.print(target2);
            if (DEBUGGER == true) Serial.print(target2);
            if (DEBUGGER == true) Serial.print(target3);
            if (DEBUGGER == true) Serial.print(target4);
            if (DEBUGGER == true) Serial.print(target5);
            if (DEBUGGER == true) Serial.print(target6);
            if (DEBUGGER == true) Serial.print(target7);
            if (DEBUGGER == true) Serial.println(target8);
            if (DEBUGGER == true) Serial.println("Found targets: ");
            if (DEBUGGER == true) Serial.print(targetFound1);
            if (DEBUGGER == true) Serial.print(targetFound2);
            if (DEBUGGER == true) Serial.print(targetFound3);
            if (DEBUGGER == true) Serial.println(targetFound4);

            if ((visionState.servoAngle > 179) && (targetFound == 0)) {
                visionState.servoAngle = 0;
                if (DEBUGGER == true) Serial.println("Wrong object detected!!");
            }
            if ((visionState.servoAngle <= 179) && (targetFound == 0)) {
                visionState.servoAngle++;
                if (DEBUGGER == true) Serial.println("Wrong object detected!!");
            }
            if (DEBUGGER == true) Serial.println("------------------------END---------------------------");
        }
    } else {
        ii++;
        if (DEBUGGER == true) Serial.print("I:");
        if (DEBUGGER == true) Serial.println(ii);
        if ((ii % ddelay) == 0) {
            failCounter++;
            if (failCounter > 8) {
                targetFound = 0;
                targetFound1 = 0;
                targetFound2 = 0;
                targetFound3 = 0;
                targetFound4 = 0;
                if (DEBUGGER == true) Serial.println("Nothing found");

                if ((visionState.servoAngle > 179) && (targetFound == 0)) {
                    visionState.servoAngle = 0;
                    if (DEBUGGER == true) Serial.println("Adjust camera");
                }
                if ((visionState.servoAngle <= 179) && (targetFound == 0)) {
                    visionState.servoAngle = visionState.servoAngle + 1;
                    if (DEBUGGER == true) Serial.println("Adjust camera");
                }

                if (DEBUGGER == true) Serial.println("----------------------END-------------------------------");
            }
        }
    }
    //if (DEBUGGER == true) Serial.println("before signaturechange");
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
        if (DEBUGGER == true) Serial.println("targetdetected inside loop");
        state->targetDetected = true;
    } else {
        state->targetDetected = false;
    }
    visionServo.detach();
}

void Vision::turnVision(int angle) {
    visionServo.attach(5);
    visionServo.write(angle);
    visionServo.detach();
    if (DEBUGGER == true) Serial.println("inside turnvision");
}

void Vision::Test(const int test) {
}
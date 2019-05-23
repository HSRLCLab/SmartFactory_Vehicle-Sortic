/**
 * @file LineDetector.cpp
 * @brief Implementation of LineDetector class
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * 
 * @date 2019-04-26
 * @copyright Copyright (c) 2019
 * 
 */

#include "LineDetector.h"

//=====PUBLIC====================================================================================

LineDetector::LineDetector(const int sensorPin_0, const int sensorPin_1,
                           const int sensorPin_2, const int sensorPin_3,
                           const int sensorPin_4)
    : pSensorPinLL(sensorPin_0), pSensorPinL(sensorPin_1), pSensorPinM(sensorPin_2), pSensorPinR(sensorPin_3), pSensorPinRR(sensorPin_4) {
    DBFUNCCALLln("LineDetector::LineDetector(...)");
    pinMode(pSensorPinLL, INPUT);
    pinMode(pSensorPinL, INPUT);
    pinMode(pSensorPinM, INPUT);
    pinMode(pSensorPinR, INPUT);
    pinMode(pSensorPinRR, INPUT);
}

/**
 * @todo use some Matrix Bit Magic
 * 
 * @todo line lost case
 */
const int LineDetector::deviation() {
    DBFUNCCALLln("LineDetector::deviation()");
    pSensorLL = digitalRead(pSensorPinLL);
    pSensorL = digitalRead(pSensorPinL);
    pSensorM = digitalRead(pSensorPinM);
    pSensorR = digitalRead(pSensorPinR);
    pSensorRR = digitalRead(pSensorPinRR);
    DBINFO3ln(String("LinePosition: ") + String(pSensorLL) + String(pSensorL) + String(pSensorM) + String(pSensorR) + String(pSensorRR));
    if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 0) && (pSensorR == 0) && (pSensorRR == 1)) {
        return 4;
    } else if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 0) && (pSensorR == 1) && (pSensorRR == 1)) {
        return 3;
    } else if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 0) && (pSensorR == 1) && (pSensorRR == 0)) {
        return 2;
    } else if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 1) && (pSensorR == 1) && (pSensorRR == 0)) {
        return 1;
    } else if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 1) && (pSensorR == 0) && (pSensorRR == 0)) {
        return 0;
    } else if ((pSensorLL == 0) && (pSensorL == 1) && (pSensorM == 1) && (pSensorR == 0) && (pSensorRR == 0)) {
        return -1;
    } else if ((pSensorLL == 0) && (pSensorL == 1) && (pSensorM == 0) && (pSensorR == 0) && (pSensorRR == 0)) {
        return -2;
    } else if ((pSensorLL == 1) && (pSensorL == 1) && (pSensorM == 0) && (pSensorR == 0) && (pSensorRR == 0)) {
        return -3;
    } else if ((pSensorLL == 1) && (pSensorL == 0) && (pSensorM == 0) && (pSensorR == 0) && (pSensorRR == 0)) {
        return -4;
    } else if ((pSensorLL == 1) && (pSensorL == 1) && (pSensorM == 1) && (pSensorR == 0) && (pSensorRR == 0)) {  //90°-Kurve
        DBINFO3ln("Possible 90 Degree turn left");
        return -10;
    } else if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 1) && (pSensorR == 1) && (pSensorRR == 1)) {  //90°-Kurve
        DBINFO3ln("Possible 90 Degree turn right");
        return 10;
    } else if ((pSensorLL == 1) && (pSensorL == 1) && (pSensorM == 1) && (pSensorR == 1) && (pSensorRR == 1)) {
        DBINFO3ln("Full line");
        return 180;
    } else if ((pSensorLL == 1) && (pSensorL == 1) && (pSensorM == 1) && (pSensorR == 1) && (pSensorRR == 0)) {
        DBINFO3ln("sloppy Full line");
        return 180;
    } else if ((pSensorLL == 0) && (pSensorL == 1) && (pSensorM == 1) && (pSensorR == 1) && (pSensorRR == 1)) {
        DBINFO3ln("sloppy Full line");
        return 180;
    } else if ((pSensorLL == 0) && (pSensorL == 1) && (pSensorM == 1) && (pSensorR == 1) && (pSensorRR == 0)) {
        DBINFO3ln("Line to wide");
        return 0;
    } else if ((pSensorLL == 0) && (pSensorL == 0) && (pSensorM == 0) && (pSensorR == 0) && (pSensorRR == 0)) {
        DBINFO3ln("No line detected");
        return 200;  ///@todo line lost
    } else {
        DBWARNINGln("LineDetector: Unexpected Results");
        DBWARNINGln(String("LinePosition: ") + String(pSensorLL) + String(pSensorL) + String(pSensorM) + String(pSensorR) + String(pSensorRR));

        return 200;  ///@todo wired results
    }
}

//=====PRIVATE=================================================================================
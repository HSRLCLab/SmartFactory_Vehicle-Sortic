/**
 * @file LineDetector.h
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

#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H
#include <Arduino.h>

#include "Configuration.h"
#include "LogConfiguration.h"

/**
 * @brief Class LineDetector handels the Line detection
 * 
 */
class LineDetector {
    //=====PUBLIC====================================================================================
   public:
    LineDetector(const int sensorPin_0, const int sensorPin_1, const int sensorPin_2, const int sensorPin_3, const int sensorPin_4);

    /**
     * @brief 
     * 
     * -90 90 Degree turn left
     * 90 90 Degree turn right
     * 180 full Line detected
     * 
     * @return const int - error
     */
    const int deviation();
    //=====PRIVATE===================================================================================
   private:
    //Use an Array?
    const int pSensorPinLL;
    const int pSensorPinL;
    const int pSensorPinM;
    const int pSensorPinR;
    const int pSensorPinRR;

    int pSensorLL;
    int pSensorL;
    int pSensorM;
    int pSensorR;
    int pSensorRR;
};
#endif
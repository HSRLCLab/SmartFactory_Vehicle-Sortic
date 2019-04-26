/**
 * @file EnviromentDetection.h
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

#ifndef ENVIRONMENTDETECTION_H
#define ENVIRONMENTDETECTION_H
#include <Arduino.h>

#include "Configuration.h"
#include "LogConfiguration.h"

#include "LineDetector.h"

/**
 * @brief 
 * 
 */
class EnvironmentDetection {
    //=====PUBLIC====================================================================================
   public:
    EnvironmentDetection(){};

    inline const int deviation() {
        return pLinedetector.deviation();
    }
    //=====PRIVATE===================================================================================
   private:
    LineDetector pLinedetector = LineDetector(PIN_SENSOR_0, PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4);
};
#endif
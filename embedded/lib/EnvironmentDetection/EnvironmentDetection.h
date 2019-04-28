/**
 * @file EnvirnomentDetection.h
 * @brief Class EnvirnomentDetection provides an Interface for all Enviroment detection Systems of the Vehilce. 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Interface  - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-26
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
 * @brief Provides an Interface for all Enviroment detection Systems of the Vehilce. 
 * 
 */
class EnvironmentDetection {
    //=====PUBLIC====================================================================================
   public:
    EnvironmentDetection(){};

    inline const int Linedeviation() {
        return pLinedetector.deviation();
    }
    //=====PRIVATE===================================================================================
   private:
    LineDetector pLinedetector = LineDetector(PIN_SENSOR_0, PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4);
};
#endif
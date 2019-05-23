/**
 * @file EnvironmentDetection.h
 * @brief Class EnvironmentDetection provides an Interface for all Enviroment detection Systems of the Vehilce. 
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
    /**
    * @brief Construct a new Environment Detection object
    * 
    */
    EnvironmentDetection(){};

    /**
     * @brief Recognise deviation from vehicle to line
     * 
     * -10 Degree turn left \n
     *  10 Degree turn right \n
     * 180 full Line detected \n
     * 200 Line Lost or ErrorValues \n
     *  \n
     *  4 Line is on the right side \n
     *  0 Line is in the middle \n
     * -4 Line is on the left side \n
     * 
     * @return const int - deviation from middle sensor
     */
    inline const int Linedeviation() {
        return pLinedetector.deviation();
    }
    //=====PRIVATE===================================================================================
   private:
    /**
    * @brief Linedetector Object with 5 Sensors
    * 
    */
    LineDetector pLinedetector = LineDetector(PIN_SENSOR_0, PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4);
};
#endif
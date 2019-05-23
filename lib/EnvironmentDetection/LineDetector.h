/**
 * @file LineDetector.h
 * @brief The Class LineDetector handels the Line detection-System 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation LineDetectiion System  - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-26
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
 * @brief Class LineDetector handels the Line detection-System 
 * 
 */
class LineDetector {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Construct a new Line Detector object
    * 
    * @param sensorPin_0 - Pin Sensor Far Left
    * @param sensorPin_1 - Pin Sensor  Left
    * @param sensorPin_2 - Pin Sensor Middle
    * @param sensorPin_3 - Pin Sensor  Right
    * @param sensorPin_4 - Pin Sensor Far Right
    */
    LineDetector(const int sensorPin_0, const int sensorPin_1, const int sensorPin_2, const int sensorPin_3, const int sensorPin_4);

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
    const int deviation();
    //=====PRIVATE===================================================================================
   private:
    //Use an Array?
    const int pSensorPinLL;  ///< Pin Sensor Far Left
    const int pSensorPinL;   ///< Pin Sensor  Left
    const int pSensorPinM;   ///< Pin Sensor Middle
    const int pSensorPinR;   ///< Pin Sensor  Right
    const int pSensorPinRR;  ///< Pin Sensor Far Right

    int pSensorLL;  ///< Value Sensor Far Left
    int pSensorL;   ///< Value Sensor  Left
    int pSensorM;   ///< Value Sensor Middle
    int pSensorR;   ///< Value Sensor  Right
    int pSensorRR;  ///< Value Sensor Far Right
};
#endif
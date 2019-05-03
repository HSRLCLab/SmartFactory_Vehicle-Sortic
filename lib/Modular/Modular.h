/**
 * @file Modular.h
 * @brief Library to define the basic structure of a modular component
 * 
 * @author Felix Nyffenegger (felix.nyffenegger@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef modular_h
#define modular_h

#include "Arduino.h"
#include "Modular.h"

/**
 * @brief 
 * 
 */
class Component {
   public:
    /**
    * @brief Construct a new Component object
    * 
    */
    Component();

    /**
     * @brief 
     * 
     */
    virtual void loop();

   protected:
};

#endif

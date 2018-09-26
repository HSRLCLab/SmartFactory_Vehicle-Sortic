/*
  Modular.h - Library to define the basic structure of a modular component.
  Created by Felix Nyffenegger
*/

#ifndef modular_h
#define modular_h

#include "Arduino.h"
#include "Modular.h"

class Component
{
  public:
    Component();
    virtual void loop();
  protected:

};

#endif

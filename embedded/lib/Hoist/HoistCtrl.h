/**
 * @file HoistCtrl.h
 * @brief The Hoist Controll class contains the FSM for the Hoist
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Hoist Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef HOISTCTRL_H__
#define HOISTCTRL_H__

#include "Hoist.h"

/**
 * @brief The Hoist Controll class contains the FSM for the Hoist
 * 
 */
class HoistCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Enum holds all possible events
    * 
    */
    enum class Event { Raise,       ///< Start Raise
                       Lower,       ///< Start Lower
                       PosReached,  ///< Position reached
                       Error,       ///< Error occured
                       Resume,      ///< Resume after Error occured
                       NoEvent      ///< No event generated
    };

    /**
    * @brief Enum holds all possible states
    * 
    */
    enum class State { low,        ///< Low State
                       raising,    ///< Raising State
                       high,       ///< High State
                       lowering,   ///< Lowering State
                       errorState  ///< Error State
    };
    /**
     * @brief Construct a new Hoist Ctrl object
     * and initailize the currentState with low state
     * 
     */
    HoistCtrl();

    /**
     * @brief Calls the do-function of the active state and hence generates Events
     * 
     */
    void loop();

    /**
     * @brief procceses the current Event and calls the do-function of the active state
     * 
     * @param currentEvent - Event
     */
    void loop(Event currentEvent);

    /**
     * @brief Get the current State
     * 
     * @return State - current State
     */
    const State getcurrentState();

    //=====PRIVATE====================================================================================
   private:
    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);

    State lastStateBevorError;  ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;         ///< holds the current state of the FSM
    Event currentEvent;         ///< holds the current event of the FSM

    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (HoistCtrl::*doActionFPtr)(void) = &HoistCtrl::doAction_low;

    Hoist pHoist;  ///< SensorArray Object

    //=====StateFunctions=====
    //=====low==========================================================
    /**
     * @brief executes the entry action of the low
     * 
     * Turns on the Loadindicator-LED
     * 
     */
    void entryAction_low();

    /**
     * @brief executes the main action of the low
     * 
     */
    HoistCtrl::Event doAction_low();

    /**
     * @brief executes the exit action of the low
     * 
     */
    void exitAction_low();

    //=====raising==========================================================
    /**
     * @brief executes the entry action of the raising 
     * 
     */
    void entryAction_raising();

    /**
     * @brief executes the main action of the raising
     * 
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_raising();

    /**
     * @brief executes the exit action of the raising
     * 
     */
    void exitAction_raising();

    //=====high==========================================================
    /**
     * @brief executes the entry action of the high 
     * 
     * Turns off the Loadindicator-LED
     */
    void entryAction_high();

    /**
     * @brief executes the main action of the high
     * 
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_high();

    /**
     * @brief executes the exit action of the high
     * 
     */
    void exitAction_high();

    //=====lowering==========================================================
    /**
     * @brief executes the entry action of the lowering 
     * 
     * Turns off the Loadindicator-LED
     */
    void entryAction_lowering();

    /**
     * @brief executes the main action of the lowering
     * 
     * 
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_lowering();

    /**
     * @brief executes the exit action of the lowering
     * 
     */
    void exitAction_lowering();

    //==errorState==========================================================
    /**
     * @brief entry action of the errorState
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState
     * 
     *  @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_errorState();

    /**
     * @brief exit action of the errorState
     * 
     */
    void exitAction_errorState();

    //============================================================================
    //==Aux-Function==============================================================
    /**
     * @brief Decodes the State-Enum and returns a description
     * 
     * @param state - enum State
     * @return String - State as String
     */
    String decodeState(State state);

    /**
     * @brief Decodes the Event-Enum and returns a description
     * 
     * @param event - enum Event
     * @return String - Event as String
     */
    String decodeEvent(Event event);
};
#endif

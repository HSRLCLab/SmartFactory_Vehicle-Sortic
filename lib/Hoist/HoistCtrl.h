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

#include "Configuration.h"
#include "LogConfiguration.h"

#include "Hoist.h"

/**
 * @brief Contains the FSM for the Hoist
 * 
 */
class HoistCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Enum holds all possible events
    * 
    */
    enum class Event { Raise,       ///< Ext: Start Raise
                       Lower,       ///< Ext: Start Lower
                       PosReached,  ///< Signal: Position reached
                       Error,       ///< Error occured
                       Resume,      ///< Ext: Resume after Error occured
                       NoEvent      ///< No event generated
    };

    /**
    * @brief Enum holds all possible states for the Hoist
    * 
    */
    enum class State { low,        ///< low State
                       raising,    ///< raising State
                       high,       ///< high State
                       lowering,   ///< lowering State
                       errorState  ///< error State
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
     * @brief Procceses the current Event and calls the do-function of the active state
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
    State lastStateBevorError;  ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;         ///< holds the current state of the FSM
    Event currentEvent;         ///< holds the current event of the FSM

    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (HoistCtrl::*doActionFPtr)(void) = &HoistCtrl::doAction_low;

    Hoist pHoist;  ///< Hoist Object

    //=====PrivateFunctions=========================================================================
    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);
    //=====StateFunctions=====
    //=====low==========================================================
    /**
     * @brief executes the entry action of the low state.
     * 
     */
    void entryAction_low();

    /**
     * @brief executes the main action of the low state.
     * 
     * This is an idle-state.
     * Return NoEvent.
     * 
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_low();

    /**
     * @brief executes the exit action of the low state.
     * 
     */
    void exitAction_low();

    //=====raising==========================================================
    /**
     * @brief executes the entry action of the raising state.
     * 
     * Attach Servo.
     */
    void entryAction_raising();

    /**
     * @brief executes the main action of the raising state.
     * 
     * Raise the Hoist till the Endposition is reached and generate an PosReached Event.
     * Else return NoEvent.
     * 
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_raising();

    /**
     * @brief executes the exit action of the raising state.
     * 
     * Detach Servo.
     */
    void exitAction_raising();

    //=====high==========================================================
    /**
     * @brief executes the entry action of the high state.
     * 
     */
    void entryAction_high();

    /**
     * @brief executes the main action of the high state.
     * 
     * This is an idle-state.
     * Return NoEvent.
     * 
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_high();

    /**
     * @brief executes the exit action of the high state.
     * 
     */
    void exitAction_high();

    //=====lowering==========================================================
    /**
     * @brief executes the entry action of the lowering  state.
     * 
     * Attach Servo.
     */
    void entryAction_lowering();

    /**
     * @brief executes the main action of the lowering state.
     * 
     * Lower the hoist till the lowposition is reached and then generate the event PosReached.
     * else return NoEvent.
     * @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_lowering();

    /**
     * @brief executes the exit action of the lowering state.
     * 
     * Detache the Servo.
     */
    void exitAction_lowering();

    //==errorState==========================================================
    /**
     * @brief entry action of the errorState state.
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState state.
     * 
     *  @return HoistCtrl::Event - generated Event
     */
    HoistCtrl::Event doAction_errorState();

    /**
     * @brief exit action of the errorState state.
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

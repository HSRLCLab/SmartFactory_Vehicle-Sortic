/**
 * @file HoistCtrl.cpp
 * @brief Implementation of the Hoist Controll-Class
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 -Implementation Hoist Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 */

#include "HoistCtrl.h"
//=====PUBLIC====================================================================================
HoistCtrl::HoistCtrl() : currentState(State::low), doActionFPtr(&HoistCtrl::doAction_low) {
}

void HoistCtrl::loop() {
    DBFUNCCALLln("HoistCtrl::loop()");
    process((this->*doActionFPtr)());  //do actions
}

void HoistCtrl::loop(Event currentEvent) {
    DBFUNCCALLln("HoistCtrl::loop(Event)");
    process(currentEvent);
    process((this->*doActionFPtr)());  //do actions
}

const HoistCtrl::State HoistCtrl::getcurrentState() {
    return currentState;
}
//=====PRIVATE====================================================================================
void HoistCtrl::process(Event e) {
    DBFUNCCALL("HoistCtrl::process ")
    DBEVENTln(decodeEvent(e));
    switch (currentState) {
        case State::low:
            if (Event::Raise == e) {
                exitAction_low();       // Exit-action current state
                entryAction_raising();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_low();          // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::raising:
            if (Event::PosReached == e) {
                exitAction_raising();  // Exit-action current state
                entryAction_high();    // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_raising();      // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::high:
            if (Event::Lower == e) {
                exitAction_high();       // Exit-action current state
                entryAction_lowering();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_high();         // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::lowering:
            if (Event::PosReached == e) {
                exitAction_lowering();  // Exit-action current state
                entryAction_low();      // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_lowering();     // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::errorState:
            if (Event::Resume == e) {
                exitAction_errorState();  // Exit-action current state
                switch (lastStateBevorError) {
                    case State::low:
                        entryAction_low();  // Entry-actions next state
                        break;
                    case State::raising:
                        entryAction_raising();  // Entry-actions next state
                        break;
                    case State::high:
                        entryAction_high();  // Entry-actions next state
                        break;
                    case State::lowering:
                        entryAction_lowering();  // Entry-actions next state
                        break;
                    default:
                        break;
                }
            }
            if (Event::Reset == e) {
                exitAction_errorState();   // Exit-action current state
                entryAction_resetState();  // Entry-actions next state
            }
            break;
        case State::resetState:
            if (Event::Resume == e) {
                exitAction_resetState();  // Exit-action current state
                entryAction_low();        // Entry-actions next state
            }
            break;
        default:
            break;
    }
}
//==low==========================================================
void HoistCtrl::entryAction_low() {
    DBSTATUSln("Hoist Entering State: low");
    currentState = State::low;  // state transition
    doActionFPtr = &HoistCtrl::doAction_low;
    //Entry-Action
}

HoistCtrl::Event HoistCtrl::doAction_low() {
    DBINFO1ln("Hoist State: low");
    //Generate the Event
    return Event::NoEvent;
}

void HoistCtrl::exitAction_low() {
    DBSTATUSln("Hoist Leaving State: low");
}

//==raising==========================================================
void HoistCtrl::entryAction_raising() {
    DBSTATUSln("Hoist Entering State: raising");
    currentState = State::raising;  // state transition
    doActionFPtr = &HoistCtrl::doAction_raising;
    //Entry-Action
    pHoist.attach();
}

HoistCtrl::Event HoistCtrl::doAction_raising() {
    DBINFO1ln("Hoist State: raising");
    //Generate the Event
    if (pHoist.raise()) {
        return Event::PosReached;
    }
    return Event::NoEvent;
}

void HoistCtrl::exitAction_raising() {
    DBSTATUSln("Hoist Leaving State: raising");
    pHoist.detach();
}

//==high==========================================================
void HoistCtrl::entryAction_high() {
    DBSTATUSln("Hoist Entering State: high");
    currentState = State::high;  // state transition
    doActionFPtr = &HoistCtrl::doAction_high;
    //Entry-Action
}

HoistCtrl::Event HoistCtrl::doAction_high() {
    DBINFO1ln("Hoist State: high");
    //Generate Event

    return Event::NoEvent;
}

void HoistCtrl::exitAction_high() {
    DBSTATUSln("Hoist Leaving State: high");
}

//==lowering==========================================================
void HoistCtrl::entryAction_lowering() {
    DBSTATUSln("Hoist Entering State: lowering");
    currentState = State::lowering;  // state transition
    doActionFPtr = &HoistCtrl::doAction_lowering;
    //Entry-Action
    pHoist.attach();
}

HoistCtrl::Event HoistCtrl::doAction_lowering() {
    DBINFO1ln("Hoist State: lowering");
    //Generate the Event
    if (pHoist.lower()) {
        return Event::PosReached;
    }
    return Event::NoEvent;
}

void HoistCtrl::exitAction_lowering() {
    DBSTATUSln("Hoist Leaving State: lowering");
    pHoist.detach();
}

//==errorState========================================================
void HoistCtrl::entryAction_errorState() {
    DBSTATUSln("Hoist Entering State: errorState");
    lastStateBevorError = currentState;
    currentState = State::errorState;  // state transition
    doActionFPtr = &HoistCtrl::doAction_errorState;
    //Entry-Action
}

HoistCtrl::Event HoistCtrl::doAction_errorState() {
    DBINFO1ln("Hoist State: errorState");
    //Generate the Event

    return Event::NoEvent;
}

void HoistCtrl::exitAction_errorState() {
    DBSTATUSln("Hoist Leaving State: errorState");
}

//==resetState========================================================
void HoistCtrl::entryAction_resetState() {
    DBERROR("Entering State: resetState");
    currentState = State::resetState;  // state transition
    doActionFPtr = &HoistCtrl::doAction_resetState;
    pHoist.attach();
}

HoistCtrl::Event HoistCtrl::doAction_resetState() {
    DBINFO1ln("State: resetState");
    //Generate the Event
    if (pHoist.lower()) {
        pHoist.detach();
    }
    return Event::NoEvent;
}

void HoistCtrl::exitAction_resetState() {
    DBSTATUSln("Leaving State: resetState");
}

//============================================================================
//==Aux-Function==============================================================
String HoistCtrl::decodeState(State state) {
    switch (state) {
        case State::low:
            return "State::low";
            break;
        case State::raising:
            return "State::raising";
        case State::high:
            return "State::high";
            break;
        case State::lowering:
            return "State::lowering";
            break;
        case State::errorState:
            return "State::errorState";
            break;
        case State::resetState:
            return "State::resetState";
            break;
        default:
            return "ERROR: No matching state";
            break;
    }
}

String HoistCtrl::decodeEvent(Event event) {
    switch (event) {
        case Event::Raise:
            return "Event::Raise";
            break;
        case Event::Lower:
            return "Event::Lower";
            break;
        case Event::PosReached:
            return "Event::PosReached";
            break;
        case Event::Error:
            return "Event::Error";
            break;
        case Event::Resume:
            return "Event::Resume";
            break;
        case Event::Reset:
            return "Event::Reset";
            break;
        case Event::NoEvent:
            return "Event::NoEvent";
            break;
        default:
            return "ERROR: No matching event";
            break;
    }
}
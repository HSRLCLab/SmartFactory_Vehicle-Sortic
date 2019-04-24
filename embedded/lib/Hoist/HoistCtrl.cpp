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
HoistCtrl::HoistCtrl() : currentState(State::low) {
    //call hoist.init() ?
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
    DBFUNCCALLln(decodeEvent(e));
    switch (currentState) {
        case State::low:
            if (Event::Loading == e) {
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
            if (Event::Unloading == e) {
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
        default:
            break;
    }
}
//==low==========================================================
void HoistCtrl::entryAction_low() {
    DBINFO2ln("Entering State: low");
    currentState = State::low;  // state transition
    doActionFPtr = &HoistCtrl::doAction_low;
}

HoistCtrl::Event HoistCtrl::doAction_low() {
    DBINFO2ln("State: low");
    //Generate the Event
    return Event::NoEvent;
}

void HoistCtrl::exitAction_low() {
    DBINFO2ln("Leaving State: low");
}

//==raising==========================================================
void HoistCtrl::entryAction_raising() {
    DBINFO2ln("Entering State: raising");
    currentState = State::raising;  // state transition
    doActionFPtr = &HoistCtrl::doAction_raising;
}

HoistCtrl::Event HoistCtrl::doAction_raising() {
    DBINFO2ln("State: raising");
    //Generate the Event
    if (pHoist.load()) {
        return Event::PosReached;
    }
    return Event::NoEvent;
}

void HoistCtrl::exitAction_raising() {
    DBINFO2ln("Leaving State: raising");
}

//==high==========================================================
void HoistCtrl::entryAction_high() {
    DBINFO2ln("Entering State: high");
    currentState = State::high;  // state transition
    doActionFPtr = &HoistCtrl::doAction_high;
}

HoistCtrl::Event HoistCtrl::doAction_high() {
    DBINFO2ln("State: high");
    //Generate the Event

    return Event::NoEvent;
}

void HoistCtrl::exitAction_high() {
    DBINFO2ln("Leaving State: high");
}

//==lowering==========================================================
void HoistCtrl::entryAction_lowering() {
    DBINFO2ln("Entering State: lowering");
    currentState = State::lowering;  // state transition
    doActionFPtr = &HoistCtrl::doAction_lowering;
}

HoistCtrl::Event HoistCtrl::doAction_lowering() {
    DBINFO2ln("State: lowering");
    //Generate the Event
    if (pHoist.unload()) {
        return Event::PosReached;
    }
    return Event::NoEvent;
}

void HoistCtrl::exitAction_lowering() {
    DBINFO2ln("Leaving State: lowering");
}

//==errorState========================================================
void HoistCtrl::entryAction_errorState() {
    DBINFO2ln("BL Entering State: errorState");
    lastStateBevorError = currentState;
    currentState = State::errorState;  // state transition
    doActionFPtr = &HoistCtrl::doAction_errorState;
}

HoistCtrl::Event HoistCtrl::doAction_errorState() {
    DBINFO2ln("BL State: errorState");
    //Generate the Event

    return Event::NoEvent;
}

void HoistCtrl::exitAction_errorState() {
    DBINFO2ln("BL Leaving State: errorState");
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
        default:
            return "ERROR: No matching state";
            break;
    }
}

String HoistCtrl::decodeEvent(Event event) {
    switch (event) {
        case Event::Loading:
            return "Event::Loading";
            break;
        case Event::Unloading:
            return "Event::Unloading";
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
        case Event::NoEvent:
            return "Event::NoEvent";
            break;
        default:
            return "ERROR: No matching event";
            break;
    }
}
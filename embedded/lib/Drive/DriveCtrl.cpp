/**
 * @file DriveCtrl.cpp
 * @brief Implementation of the Drive Controll-Class
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 -Implementation Drive Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 */

#include "DriveCtrl.h"
//=====PUBLIC====================================================================================
DriveCtrl::DriveCtrl() : currentState(State::idle) {
}

void DriveCtrl::loop() {
    DBFUNCCALLln("DriveCtrl::loop()");
    process((this->*doActionFPtr)());  //do actions
}

void DriveCtrl::loop(Event currentEvent) {
    DBFUNCCALLln("DriveCtrl::loop(Event)");
    process(currentEvent);
    process((this->*doActionFPtr)());  //do actions
}

const DriveCtrl::State DriveCtrl::getcurrentState() {
    return currentState;
}
//=====PRIVATE====================================================================================
void DriveCtrl::process(Event e) {
    DBFUNCCALL("DriveCtrl::process ")
    DBFUNCCALLln(decodeEvent(e));
    switch (currentState) {
        case State::idle:
            if (Event::TurnLeft == e) {
                exitAction_idle();          // Exit-action current state
                entryAction_turningLeft();  // Entry-actions next state
            } else if (Event::TurnLeft == e) {
                exitAction_idle();          // Exit-action current state
                entryAction_turningLeft();  // Entry-actions next state
            } else if (Event::TurnRight == e) {
                exitAction_idle();           // Exit-action current state
                entryAction_turningRight();  // Entry-actions next state
            } else if (Event::TurnAround == e) {
                exitAction_idle();            // Exit-action current state
                entryAction_turningAround();  // Entry-actions next state
            } else if (Event::FollowLine == e) {
                exitAction_idle();            // Exit-action current state
                entryAction_followingLine();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_idle();         // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::turningLeft:
            if (Event::LineAligned == e) {
                exitAction_turningLeft();  // Exit-action current state
                entryAction_idle();        // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_turningLeft();  // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::turningRight:
            if (Event::LineAligned == e) {
                exitAction_turningRight();  // Exit-action current state
                entryAction_idle();         // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_turningRight();  // Exit-action current state
                entryAction_errorState();   // Entry-actions next state
            }
            break;
        case State::turningAround:
            if (Event::LineAligned == e) {
                exitAction_turningAround();  // Exit-action current state
                entryAction_idle();          // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_turningAround();  // Exit-action current state
                entryAction_errorState();    // Entry-actions next state
            }
            break;
        case State::followingLine:
            if (Event::FullLineReached == e) {
                exitAction_followingLine();  // Exit-action current state
                entryAction_idle();          // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_followingLine();  // Exit-action current state
                entryAction_errorState();    // Entry-actions next state
            }
            break;
        case State::errorState:
            if (Event::Resume == e) {
                exitAction_errorState();  // Exit-action current state
                switch (lastStateBevorError) {
                    case State::idle:
                        entryAction_idle();  // Entry-actions next state
                        break;
                    case State::turningLeft:
                        entryAction_turningLeft();  // Entry-actions next state
                        break;
                    case State::turningRight:
                        entryAction_turningRight();  // Entry-actions next state
                        break;
                    case State::turningAround:
                        entryAction_turningAround();  // Entry-actions next state
                        break;
                    default:
                        break;
                }
            }
        default:
            break;
    }
}
//==idle==========================================================
void DriveCtrl::entryAction_idle() {
    DBINFO2ln("Drive Entering State: idle");
    currentState = State::idle;  // state transition
    doActionFPtr = &DriveCtrl::doAction_idle;
    //Entry-Action
}

DriveCtrl::Event DriveCtrl::doAction_idle() {
    DBINFO2ln("Drive State: idle");
    //Generate the Event
    return Event::NoEvent;
}

void DriveCtrl::exitAction_idle() {
    DBINFO2ln("Drive Leaving State: idle");
}

//==turningLeft==========================================================
void DriveCtrl::entryAction_turningLeft() {
    DBINFO2ln("Drive Entering State: turningLeft");
    currentState = State::turningLeft;  // state transition
    doActionFPtr = &DriveCtrl::doAction_turningLeft;
    //Entry-Action
}

DriveCtrl::Event DriveCtrl::doAction_turningLeft() {
    DBINFO2ln("Drive State: turningLeft");
    //Generate the Event
    return Event::NoEvent;
}

void DriveCtrl::exitAction_turningLeft() {
    DBINFO2ln("Drive Leaving State: turningLeft");
}

//==turningRight==========================================================
void DriveCtrl::entryAction_turningRight() {
    DBINFO2ln("Drive Entering State: turningRight");
    currentState = State::turningRight;  // state transition
    doActionFPtr = &DriveCtrl::doAction_turningRight;
    //Entry-Action
}

DriveCtrl::Event DriveCtrl::doAction_turningRight() {
    DBINFO2ln("Drive State: turningRight");
    //Generate Event
    return Event::NoEvent;
}

void DriveCtrl::exitAction_turningRight() {
    DBINFO2ln("Drive Leaving State: turningRight");
}

//==turningAround==========================================================
void DriveCtrl::entryAction_turningAround() {
    DBINFO2ln("Drive Entering State: turningAround");
    currentState = State::turningAround;  // state transition
    doActionFPtr = &DriveCtrl::doAction_turningAround;
    //Entry-Action
}

DriveCtrl::Event DriveCtrl::doAction_turningAround() {
    DBINFO2ln("Drive State: turningAround");
    //Generate the Event
    return Event::NoEvent;
}

void DriveCtrl::exitAction_turningAround() {
    DBINFO2ln("Drive Leaving State: turningAround");
}

//==followingLine==========================================================
void DriveCtrl::entryAction_followingLine() {
    DBINFO2ln("Drive Entering State: followingLine");
    currentState = State::followingLine;  // state transition
    doActionFPtr = &DriveCtrl::doAction_followingLine;
    //Entry-Action
}

DriveCtrl::Event DriveCtrl::doAction_followingLine() {
    DBINFO2ln("Drive State: followingLine");
    //Generate the Event
    return Event::NoEvent;
}

void DriveCtrl::exitAction_followingLine() {
    DBINFO2ln("Drive Leaving State: followingLine");
}

//==errorState========================================================
void DriveCtrl::entryAction_errorState() {
    DBINFO2ln("Drive Entering State: errorState");
    lastStateBevorError = currentState;
    currentState = State::errorState;  // state transition
    doActionFPtr = &DriveCtrl::doAction_errorState;
    //Entry-Action
}

DriveCtrl::Event DriveCtrl::doAction_errorState() {
    DBINFO2ln("Drive State: errorState");
    //Generate the Event
    return Event::NoEvent;
}

void DriveCtrl::exitAction_errorState() {
    DBINFO2ln("Drive Leaving State: errorState");
}

//============================================================================
//==Aux-Function==============================================================
String DriveCtrl::decodeState(State state) {
    switch (state) {
        case State::idle:
            return "State::idle";
            break;
        case State::turningLeft:
            return "State::turningLeft";
        case State::turningRight:
            return "State::turningRight";
            break;
        case State::turningAround:
            return "State::turningAround";
            break;
        case State::errorState:
            return "State::errorState";
            break;
        default:
            return "ERROR: No matching state";
            break;
    }
}

String DriveCtrl::decodeEvent(Event event) {
    switch (event) {
        case Event::TurnLeft:
            return "Event::TurnLeft";
            break;
        case Event::TurnRight:
            return "Event::TurnRight";
            break;
        case Event::TurnAround:
            return "Event::TurnAround";
            break;
        case Event::FollowLine:
            return "Event::FollowLine";
            break;
        case Event::LineAligned:
            return "Event::LineAligned";
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
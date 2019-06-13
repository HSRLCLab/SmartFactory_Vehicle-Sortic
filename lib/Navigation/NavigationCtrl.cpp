/**
 * @file NavigationCtrl.cpp
 * @brief Implementation of the Navigation Controll-Class
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 -Implementation Navigation Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 * @todo Write a function for positionupdates and don't scatter it all over the states
 * 
 */

#include "NavigationCtrl.h"
//=====PUBLIC====================================================================================
NavigationCtrl::NavigationCtrl() : currentState(State::endPoint), doActionFPtr(&NavigationCtrl::doAction_endPoint) {
}

void NavigationCtrl::loop() {
    DBFUNCCALLln("NavigationCtrl::loop()");
    process((this->*doActionFPtr)());  //do actions
}

void NavigationCtrl::loop(Event currentEvent) {
    DBFUNCCALLln("NavigationCtrl::loop(Event)");
    process(currentEvent);
    process((this->*doActionFPtr)());  //do actions
}

const NavigationCtrl::State NavigationCtrl::getcurrentState() {
    DBFUNCCALL("Current State: ");
    DBFUNCCALLln(decodeState(currentState));
    return currentState;
}

const NavigationCtrl::Sector NavigationCtrl::getcurrentSector() {
    DBFUNCCALL("Current Sector: ");
    return pActual.sector;
}

const int NavigationCtrl::getcurrentLine() {
    DBFUNCCALL("Current line: ");
    return pActual.line;
}

void NavigationCtrl::setTargetPosition(Sector sector, const int line) {
    DBFUNCCALLln("NavigationCtrl::setTargetPosition(Sector sector, const int line)");
    pTarget.sector = sector;
    pTarget.line = line;
}

void NavigationCtrl::setActualPosition(Sector sector, const int line) {
    DBFUNCCALLln("NavigationCtrl::setActualPosition(Sector sector, const int line)");
    pActual.sector = sector;
    pActual.line = line;
}

void NavigationCtrl::giveToken() {
    DBFUNCCALLln("NavigationCtrl::giveToken()");
    pActual.token = true;
}

String NavigationCtrl::decodeSector(Sector sector) {
    switch (sector) {
        case Sector::SorticHandover:
            return "SorticHandover";
            break;
        case Sector::SorticToHandover:
            return "SorticToHandover";
            break;
        case Sector::SorticWaitForGateway:
            return "SorticWaitForGateway";
            break;
        case Sector::SorticGateway:
            return "SorticGateway";
            break;
        case Sector::TransitWaitForGatewaySortic:
            return "TransitWaitForGatewaySortic";
            break;
        case Sector::TransitToSortic:
            return "TransitToSortic";
            break;
        case Sector::TransitToTransfer:
            return "TransitToTransfer";
            break;
        case Sector::Parking:
            return "Parking";
            break;
        case Sector::TransitWaitForGatewayTransfer:
            return "TransitWaitForGatewayTransfer";
            break;
        case Sector::TransferGateway:
            return "TransferGateway";
            break;
        case Sector::TransferWaitForGateway:
            return "TransferWaitForGateway";
            break;
        case Sector::TransferToHandover:
            return "TransferToHandover";
            break;
        case Sector::TransferHandover:
            return "TransferHandover";
            break;

        default:
            return "ERROR: No matching sector";
            break;
    }
}

NavigationCtrl::Sector NavigationCtrl::decodeSector(String sector) {
    if (String("SorticHandover") == sector) {
        return Sector::SorticHandover;
    } else if (String("SorticToHandover") == sector) {
        return Sector::SorticToHandover;
    } else if (String("SorticWaitForGateway") == sector) {
        return Sector::SorticWaitForGateway;
    } else if (String("TransitWaitForGatewaySortic") == sector) {
        return Sector::TransitWaitForGatewaySortic;
    } else if (String("TransitToSortic") == sector) {
        return Sector::TransitToSortic;
    } else if (String("TransitToTransfer") == sector) {
        return Sector::TransitToTransfer;
    } else if (String("Parking") == sector) {
        return Sector::Parking;
    } else if (String("TransitWaitForGatewayTransfer") == sector) {
        return Sector::TransitWaitForGatewayTransfer;
    } else if (String("TransferGateway") == sector) {
        return Sector::TransferGateway;
    } else if (String("TransferWaitForGateway") == sector) {
        return Sector::TransferWaitForGateway;
    } else if (String("TransferToHandover") == sector) {
        return Sector::TransferToHandover;
    } else if (String("TransferHandover") == sector) {
        return Sector::TransferHandover;
    } else {
        return Sector::error;
    }
}

//=====PRIVATE====================================================================================
void NavigationCtrl::process(Event e) {
    DBFUNCCALL("NavigationCtrl::process ")
    DBEVENTln(decodeEvent(e));
    switch (currentState) {
        case State::endPoint:
            if (Event::MoveToTargetPosition == e) {
                if ((pActual.sector == pTarget.sector) && (pActual.line == pTarget.line)) {
                    //Already in correct position?
                    DBSTATUSln("Navigation: Already in correct position");
                    break;
                }
                exitAction_endPoint();    // Exit-action current state
                entryAction_toGateway();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_endPoint();     // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::toGateway:
            if (Event::PosReached == e) {
                exitAction_toGateway();  // Exit-action current state
                entryAction_gateway();   // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_toGateway();    // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::gateway:
            if (Event::PosTransitReached == e) {
                exitAction_gateway();        // Exit-action current state
                entryAction_crossTransit();  // Entry-actions next state
            } else if (Event::PosEndPointReached == e) {
                exitAction_gateway();      // Exit-action current state
                entryAction_toEndPoint();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_gateway();      // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::crossTransit:
            if (Event::PosReached == e) {
                exitAction_crossTransit();  // Exit-action current state
                entryAction_gateway();      // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_crossTransit();  // Exit-action current state
                entryAction_errorState();   // Entry-actions next state
            }
            break;
        case State::toEndPoint:
            if (Event::PosEndPointReached == e) {
                exitAction_toEndPoint();  // Exit-action current state
                entryAction_endPoint();   // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_toEndPoint();   // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::errorState:
            if (Event::Resume == e) {
                exitAction_errorState();                    // Exit-action current state
                pDriveCtrl.loop(DriveCtrl::Event::Resume);  //Pass Event to DriveCtrl
                switch (lastStateBevorError) {
                    case State::endPoint:
                        entryAction_endPoint();  // Entry-actions next state
                        break;
                    case State::toGateway:
                        entryAction_toGateway();  // Entry-actions next state
                        break;
                    case State::gateway:
                        entryAction_gateway();  // Entry-actions next state
                        break;
                    case State::crossTransit:
                        entryAction_crossTransit();  // Entry-actions next state
                        break;
                    default:
                        break;
                }
            }
            if (Event::Reset == e) {
                exitAction_errorState();                   // Exit-action current state
                pDriveCtrl.loop(DriveCtrl::Event::Reset);  // Pass Event to DriveCtrl
                entryAction_resetState();                  // Entry-actions next state
            }
            break;
        case State::resetState:
            if (Event::Resume == e) {
                exitAction_resetState();                    // Exit-action current state
                pDriveCtrl.loop(DriveCtrl::Event::Resume);  // Pass Event to DriveCtrl
                entryAction_endPoint();                     // Entry-actions next state
            }
            break;
        default:
            break;
    }
}
//==endPoint==========================================================
void NavigationCtrl::entryAction_endPoint() {
    DBSTATUSln("Navigation Entering State: endPoint");
    currentState = State::endPoint;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_endPoint;
    //Entry-Action
    if (pActual.lastSector == Sector::SorticToHandover) {
        pActual.sector = Sector::SorticHandover;
    } else if (pActual.lastSector == Sector::TransferToHandover) {
        pActual.sector = Sector::TransferHandover;
    }

    pActual.sector = pTarget.sector;
    pActual.startSector = pActual.sector;
    DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
}

NavigationCtrl::Event NavigationCtrl::doAction_endPoint() {
    DBINFO1ln("Navigation State: endPoint");
    //Generate the Event
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_endPoint() {
    DBSTATUSln("Navigation Leaving State: endPoint");
    pActual.lastSector = pActual.sector;
}

//==toGateway==========================================================
void NavigationCtrl::entryAction_toGateway() {
    DBSTATUSln("Navigation Entering State: toGateway");
    currentState = State::toGateway;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_toGateway;
    //Entry-Action
    //Update position
    if (pActual.lastSector == Sector::SorticHandover) {
        pActual.sector = Sector::SorticWaitForGateway;
    } else if (pActual.lastSector == Sector::TransferHandover) {
        pActual.sector = Sector::TransferWaitForGateway;
    }
    DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
}

NavigationCtrl::Event NavigationCtrl::doAction_toGateway() {
    DBINFO1ln("Navigation State: toGateway");
    //Generate the Event
    switch (pCurrentSubState) {
        case 0:  //drive Backwards
            pDriveCtrl.loop(DriveCtrl::Event::FollowLineBackward);
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pCurrentSubState += 10;
            }
            break;
        case 10:  // turn Around
            pDriveCtrl.loop(DriveCtrl::Event::TurnAround);
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pCurrentSubState += 10;
            }
            break;
        case 20:  // drive forward once
            pDriveCtrl.loop(DriveCtrl::Event::FollowLineForward);
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pCurrentSubState = 0;
                return Event::PosReached;
            }
            break;
        default:
            DBINFO1ln("Error. No matching State");
            break;
    }
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_toGateway() {
    DBSTATUSln("Navigation Leaving State: toGateway");
    pActual.lastSector = pActual.sector;
}

//==gateway==========================================================
void NavigationCtrl::entryAction_gateway() {
    DBSTATUSln("Navigation Entering State: gateway");
    currentState = State::gateway;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_gateway;
    //Entry-Action
    if (pActual.lastSector == Sector::TransitToTransfer) {
        pActual.sector = Sector::TransitWaitForGatewayTransfer;
    } else if (pActual.lastSector == Sector::TransitToSortic) {
        pActual.sector = Sector::TransitWaitForGatewaySortic;
    }
    DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
}

/**
 * @todo Refactoring so it only checks the neares sector and does not relay on startsector
 */
NavigationCtrl::Event NavigationCtrl::doAction_gateway() {
    DBINFO2ln("Navigation State: gateway " + String(pCurrentSubState));
    //Generate Event
    switch (pCurrentSubState) {
        case 0:  // wait for Token
            DBINFO2ln("Substate: WaitForToken");
            DBINFO2ln("Token: " + String(pActual.token));
            DBINFO2ln("Sector: " + decodeSector(pActual.sector));
            if (pActual.token) {
                //Update position
                pActual.token = false; 
                pActual.lastSector = pActual.sector;
                if (pActual.lastSector == Sector::SorticWaitForGateway) {
                    pActual.sector = Sector::SorticGateway;
                    pCurrentSubState = 10;  //call next case
                } else if (pActual.lastSector == Sector::TransferWaitForGateway) {
                    pActual.sector = Sector::TransferGateway;
                    pCurrentSubState = 10;  //call next case
                } else if (pActual.lastSector == Sector::TransitWaitForGatewaySortic) {
                    pActual.sector = Sector::SorticGateway;
                    pCurrentSubState = 10;  //call next case
                } else if (pActual.lastSector == Sector::TransitWaitForGatewayTransfer) {
                    pActual.sector = Sector::TransferGateway;
                    pCurrentSubState = 10;  //call next case
                }
                DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
            }
            break;
        case 10:  // drive forward
            DBINFO2ln("Substate: drive forward once");
            pDriveCtrl.loop(DriveCtrl::Event::FollowLineForward);
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pCurrentSubState = 20;  //call next case
            }
            break;
        case 20:  // turn left/right depending on target and current orientation
            DBINFO2ln("Substate: turn left or right");
            if (pActual.line == pTarget.line) {
                pSubStateLoopInc = 0;   //reset back to zero
                pCurrentSubState = 50;  // jump direct to last substate -> driveForwardTwice
                break;
            }

            if (((pActual.line < pTarget.line) && (pActual.startSector != Sector::SorticHandover)) ||    //nach unten und von rechts
                ((pActual.line > pTarget.line) && (pActual.startSector != Sector::TransferHandover))) {  //nach oben und von links
                pLastGatewayTurn = DriveCtrl::Event::TurnLeft;
                pDriveCtrl.loop(pLastGatewayTurn);
            } else if (((pActual.line > pTarget.line) && (pActual.startSector != Sector::SorticHandover)) ||    //nach oben und von rechts
                       ((pActual.line < pTarget.line) && (pActual.startSector != Sector::TransferHandover))) {  //nach unten und von links
                pLastGatewayTurn = DriveCtrl::Event::TurnRight;
                pDriveCtrl.loop(pLastGatewayTurn);
            }
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pSubStateLoopInc = 0;   //reset back to zero
                pCurrentSubState = 30;  //call next case
            }
            break;
        case 30:  // drive forward n times
            DBINFO2("Substate: drive forward times: ");
            DBINFO2ln(abs(pTarget.line - pActual.line));
            DBINFO2ln(pSubStateLoopInc);
            pDriveCtrl.loop(DriveCtrl::Event::FollowLineForward);
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pSubStateLoopInc += 1;
                if (abs(pTarget.line - pActual.line) <= pSubStateLoopInc) {
                    pSubStateLoopInc = 0;         //reset back to zero
                    pCurrentSubState = 40;        //call next case
                    pActual.line = pTarget.line;  //update actual line
                }
            }
            break;
        case 40:  // turn left/right depending on target and current orientation
            DBINFO2ln("Substate: turn left or right");
            if (pTarget.sector == pActual.startSector) {
                pDriveCtrl.loop(pLastGatewayTurn);
            } else {
                if (pLastGatewayTurn == DriveCtrl::Event::TurnLeft) {
                    pDriveCtrl.loop(DriveCtrl::Event::TurnRight);
                } else {
                    pDriveCtrl.loop(DriveCtrl::Event::TurnLeft);
                }
            }
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pCurrentSubState = 50;  //call next case
            }
            break;
        case 50:  // drive forward twice
            DBINFO2ln("Substate: drive forward twice");
            pDriveCtrl.loop(DriveCtrl::Event::FollowLineForward);
            if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
                pSubStateLoopInc += 1;
                if (pSubStateLoopInc >= 2) {
                    pSubStateLoopInc = 0;  //reset back to zero
                    // pSubStateLoopInc+= 10;
                    pCurrentSubState = 50;
                    pActual.token = false;  //release token
                    DBPOSln(String("pTarget.sector: ") + decodeSector(pTarget.sector) + String(" pActual.startSector: ") + decodeSector(pActual.startSector));
                    if (pTarget.sector != pActual.startSector && tranistonce) {
                        tranistonce = false;
                        pCurrentSubState = 0;
                        return Event::PosTransitReached;
                    } else {
                        pCurrentSubState = 0;
                        tranistonce = true;
                        return Event::PosEndPointReached;
                    }
                }
            }
            break;
        default:
            DBINFO1ln("Error. No matching State");
            break;
    }
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_gateway() {
    DBSTATUSln("Navigation Leaving State: gateway");
    DBPOSln(String("lastSector: ") + decodeSector(pActual.lastSector) + String(" Actual: ") + decodeSector(pActual.sector));
    DBPOSln(String("targetSector: ") + decodeSector(pTarget.sector));
    pActual.lastSector = pActual.sector;
    // if ((pActual.lastSector == Sector::SorticGateway) &&
    //     (pTarget.sector == Sector::TransferHandover)) {
    //     pActual.sector = Sector::TransitToTransfer;
    // } else if ((pActual.lastSector == Sector::TransferGateway) &&
    //            (pTarget.sector == Sector::TransferHandover)) {
    //     pActual.sector = Sector::TransferToHandover;
    // } else if ((pActual.lastSector == Sector::SorticGateway) &&
    //            (pTarget.sector == Sector::SorticHandover)) {
    //     pActual.sector = Sector::SorticToHandover;
    // } else if ((pActual.lastSector == Sector::TransferGateway) &&
    //            (pTarget.sector == Sector::SorticToHandover)) {
    //     pActual.sector = Sector::TransitToSortic;
    // }
    DBPOSln(String("lastSector: ") + decodeSector(pActual.lastSector) + String(" Actual: ") + decodeSector(pActual.sector));
}

//==crossTransit==========================================================
void NavigationCtrl::entryAction_crossTransit() {
    DBSTATUSln("Navigation Entering State: crossTransit");
    currentState = State::crossTransit;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_crossTransit;
    //Entry-Action
    DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
    pActual.lastSector = pActual.sector;
    if (pActual.lastSector == Sector::SorticGateway) {
        pActual.sector = Sector::TransitToTransfer;
    } else if (pActual.lastSector == Sector::TransferGateway) {
        pActual.sector = Sector::TransitToSortic;
        DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
    }
}

NavigationCtrl::Event NavigationCtrl::doAction_crossTransit() {
    DBINFO1ln("Navigation State: crossTransit");
    //Generate the Event
    pDriveCtrl.loop(DriveCtrl::Event::FollowLineForward);
    if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
        DBINFO2ln(pSubStateLoopInc);
        pSubStateLoopInc += 1;
        if (pSubStateLoopInc >= 3) {
            pSubStateLoopInc = 0;  //reset back to zero
            pCurrentSubState = 0;
            return Event::PosReached;
        }
    }
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_crossTransit() {
    DBSTATUSln("Navigation Leaving State: crossTransit");
    pActual.lastSector = pActual.sector;
}

//==toEndPoint==========================================================
void NavigationCtrl::entryAction_toEndPoint() {
    DBSTATUSln("Navigation Entering State: toEndPoint");
    currentState = State::toEndPoint;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_toEndPoint;
    //Entry-Action
    DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
    pActual.lastSector = pActual.sector;
    if (pActual.lastSector == Sector::SorticGateway) {
        pActual.sector = Sector::SorticToHandover;
    } else if (pActual.lastSector == Sector::TransferGateway) {
        pActual.sector = Sector::TransferToHandover;
    }
    DBPOSln(decodeSector(pActual.sector) + String(" Line: ") + String(pActual.line));
}

NavigationCtrl::Event NavigationCtrl::doAction_toEndPoint() {
    DBINFO1ln("Navigation State: toEndPoint");
    //Generate the Event
    pDriveCtrl.loop(DriveCtrl::Event::FollowLineForward);
    if (pDriveCtrl.getcurrentState() == DriveCtrl::State::idle) {
        pSubStateLoopInc += 1;
        if (pSubStateLoopInc >= 1) {
            pSubStateLoopInc = 0;  //reset back to zero
            pCurrentSubState = 0;
            return Event::PosEndPointReached;
        }
    }
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_toEndPoint() {
    DBSTATUSln("Navigation Leaving State: toEndPoint");
    pActual.lastSector = pActual.sector;
}

//==errorState========================================================
void NavigationCtrl::entryAction_errorState() {
    DBSTATUSln("Navigation Entering State: errorState");
    lastStateBevorError = currentState;
    pLastSubStateBevorError = pCurrentSubState;
    currentState = State::errorState;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_errorState;
    //Entry-Action
    pDriveCtrl.loop(DriveCtrl::Event::Error);
}

NavigationCtrl::Event NavigationCtrl::doAction_errorState() {
    DBINFO1ln("Navigation State: errorState");
    //Generate the Event
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_errorState() {
    DBSTATUSln("Navigation Leaving State: errorState");
}

//==resetState========================================================
void NavigationCtrl::entryAction_resetState() {
    DBERROR("Entering State: resetState");
    currentState = State::resetState;  // state transition
    doActionFPtr = &NavigationCtrl::doAction_resetState;
}

NavigationCtrl::Event NavigationCtrl::doAction_resetState() {
    DBINFO1ln("State: resetState");
    //Generate the Event
    return Event::NoEvent;
}

void NavigationCtrl::exitAction_resetState() {
    DBSTATUSln("Leaving State: resetState");
    pActual = {};  //reset struct
    pTarget = {};  //reset struct
    tranistonce = true;
    pCurrentSubState = 0;
    pLastSubStateBevorError = 0;
    pSubStateLoopInc = 0;
}

//============================================================================
//==Aux-Function==============================================================
String NavigationCtrl::decodeState(State state) {
    switch (state) {
        case State::endPoint:
            return "State::endPoint";
            break;
        case State::toGateway:
            return "State::toGateway";
        case State::gateway:
            return "State::gateway";
            break;
        case State::crossTransit:
            return "State::crossTransit";
            break;
        case State::toEndPoint:
            return "State::toEndPoint";
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

String NavigationCtrl::decodeEvent(Event event) {
    switch (event) {
        case Event::MoveToTargetPosition:
            return "Event::MoveToTargetPosition";
            break;
        case Event::PosEndPointReached:
            return "Event::PosEndPointReached";
            break;
        case Event::PosTransitReached:
            return "Event::PosTransitReached";
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

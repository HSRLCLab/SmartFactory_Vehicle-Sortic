/**
 * @file VehicleCtrl.cpp
 * @brief Implementation of the Vehicle Controll-Class
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Vehicle Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 */
#include "VehicleCtrl.h"
//=====PUBLIC====================================================================================
VehicleCtrl::VehicleCtrl() : currentState(State::waitForBox) {
    DBFUNCCALLln("VehicleCtrl::VehicleCtrl()");
    delay(100);
    clearGui();
    publishState(currentState);
    publishPosition();
    pNavCtrl.setActualPosition(vehicle.actualSector, vehicle.actualLine);
    pComm.unsubscribe("#");
    pComm.subscribe("Vehicle/" + String(vehicle.id) + "/error");
    pComm.subscribe("Vehicle/error");
    pComm.subscribe("error");
    pComm.subscribe("Box/+/handshake");
}

void VehicleCtrl::loop() {
    DBFUNCCALLln("VehicleCtrl::loop()");
    process((this->*doActionFPtr)());  //do actions
}

void VehicleCtrl::loop(Event currentEvent) {
    DBFUNCCALLln("VehicleCtrl::loop(Event)");
    process(currentEvent);
    process((this->*doActionFPtr)());  //do actions
}

//=====PRIVATE====================================================================================
void VehicleCtrl::process(Event e) {
    DBFUNCCALL("VehicleCtrl::process ")
    DBEVENTln(String("VehicleCtrl ") + decodeEvent(e));
    switch (currentState) {
        case State::waitForBox:
            if (Event::AnswerReceived == e) {
                exitAction_waitForBox();  // Exit-action current state
                entryAction_handshake();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_waitForBox();   // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::handshake:
            if (Event::HSsucessful == e) {
                exitAction_handshake();     // Exit-action current state
                entryAction_loadVehicle();  // Entry-actions next state
            } else if (Event::NoAnswerReceived == e) {
                exitAction_handshake();    // Exit-action current state
                entryAction_waitForBox();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_handshake();    // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::loadVehicle:
            if (Event::PosReached == e) {
                exitAction_loadVehicle();     // Exit-action current state
                entryAction_unloadVehicle();  // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_loadVehicle();  // Exit-action current state
                entryAction_errorState();  // Entry-actions next state
            }
            break;
        case State::unloadVehicle:
            if (Event::PosReached == e) {
                exitAction_unloadVehicle();  // Exit-action current state
                entryAction_waitForBox();    // Entry-actions next state
            } else if (Event::Error == e) {
                exitAction_unloadVehicle();  // Exit-action current state
                entryAction_errorState();    // Entry-actions next state
            }
            break;
        case State::errorState:
            if (Event::Resume == e) {
                exitAction_errorState();                       // Exit-action current state
                pHoistCtrl.loop(HoistCtrl::Event::Resume);     // Resume State for HoistCtrl
                pNavCtrl.loop(NavigationCtrl::Event::Resume);  // Resume State for NavigationCtrl
                switch (lastStateBevorError) {
                    case State::waitForBox:
                        entryAction_waitForBox();  // Entry-actions next state
                        break;
                    case State::handshake:
                        entryAction_handshake();  // Entry-actions next state
                        break;
                    case State::loadVehicle:
                        entryAction_loadVehicle();  // Entry-actions next state
                        break;
                    case State::unloadVehicle:
                        entryAction_unloadVehicle();  // Entry-actions next state
                        break;
                    default:
                        break;
                }
            }
            if (Event::Reset == e) {
                exitAction_errorState();                      // Exit-action current state
                pHoistCtrl.loop(HoistCtrl::Event::Reset);     // Reset State for HoistCtrl
                pNavCtrl.loop(NavigationCtrl::Event::Reset);  // Reset State for NavigationCtrl
                entryAction_resetState();                     // Entry-actions next state
            }
            break;
        case State::resetState:
            if (Event::Resume == e) {
                exitAction_resetState();                       // Exit-action current state
                pHoistCtrl.loop(HoistCtrl::Event::Resume);     // Resume State for HoistCtrl
                pNavCtrl.loop(NavigationCtrl::Event::Resume);  // Resume State for NavigationCtrl
                entryAction_waitForBox();                      // Entry-actions next state
            }
            break;
        default:
            break;
    }
}
//==waitForBox========================================================
void VehicleCtrl::entryAction_waitForBox() {
    DBSTATUSln("Entering State: emptyState");
    currentState = State::waitForBox;  // state transition
    doActionFPtr = &VehicleCtrl::doAction_waitForBox;
    publishState(currentState);  //Update Current State and Publish
    publishPosition();           //Update Current Position and Publish for GUI
                                 //Clear handshake in Gui
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/handshake", "{\"ack\":\"\",\"req\":\"\"}");

    pComm.subscribe("Box/+/handshake");
    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;
}

VehicleCtrl::Event VehicleCtrl::doAction_waitForBox() {
    DBINFO1ln("State: emptyState");
    //Generate the Event
    pComm.loop();  //Check for new Messages
    if (checkForError()) {
        return Event::Error;
    }

    DBINFO2ln("Wait for request");
    if (!pComm.isEmpty()) {
        myJSONStr temp = pComm.pop();
        DBINFO2ln(String("temp.req: ") + String(temp.req) + String("==") + String("vehicle.id: ") + String(vehicle.id));
        if ((temp.req == vehicle.id)) {
            vehicle.req = temp.id;
            return Event::AnswerReceived;
        }
    }

    currentMillis = millis();
    if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
        previousMillisPublish = millis();
        pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/available", "{\"id\":\"" + String(vehicle.id) + "\",\"sector\":\"" + pNavCtrl.decodeSector(vehicle.actualSector) + "\",\"line\":\"" + String(vehicle.actualLine) + "\"}");
    }
    return Event::NoEvent;
}

void VehicleCtrl::exitAction_waitForBox() {
    DBSTATUSln("Leaving State: emptyState");
    pComm.unsubscribe("Box/+/handshake");
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/available", "{\"id\":\"" + String(vehicle.id) + "\",\"sector\":\"\",\"line\":\"\"}");  //clear available in GUI
}

//==handshake========================================================
void VehicleCtrl::entryAction_handshake() {
    DBSTATUSln("Entering State: handshake");
    currentState = State::handshake;  // state transition
    doActionFPtr = &VehicleCtrl::doAction_handshake;
    publishState(currentState);  //Update Current State and Publish
    publishPosition();           //Update Current Position and Publish for GUI
    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;
    substate = 0;
    pComm.subscribe("Box/" + String(vehicle.req) + "/handshake");
}

VehicleCtrl::Event VehicleCtrl::doAction_handshake() {
    DBINFO1ln("State: handshake");
    pComm.loop();  //Check for new Messages
    if (checkForError()) {
        return Event::Error;
    }
    currentMillis = millis();
    switch (substate) {
        case 0:  //Publish request for Box and check for ack
            //Check for Ack from box and Timeout
            if ((currentMillis - previousMillis) < TIMEOUT_VACKS) {
                DBINFO2ln("Check for ack. fron box");
                if (!pComm.isEmpty()) {
                    myJSONStr temp = pComm.pop();
                    DBINFO2ln(String("temp.ack: ") + String(temp.ack) + String("==") + String("vehicle.id: ") + String(vehicle.id));
                    if ((temp.ack == vehicle.id) && (temp.id == vehicle.req)) {
                        //if ack received update traget pos
                        vehicle.ack = temp.id;
                        vehicle.targetSector = pNavCtrl.decodeSector(temp.sector);
                        vehicle.targetLine = temp.line;
                        vehicle.cargo = temp.cargo;
                        substate = 10;
                        previousMillis = millis();
                        break;
                    }
                }
            } else {
                return Event::NoAnswerReceived;
            }

            //Publish request for Box
            if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                previousMillisPublish = millis();
                pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/handshake", "{\"id\":\"" + String(vehicle.id) + "\",\"req\":\"" + String(vehicle.req) + "\"}");
            }
            break;
        case 10:  //Publish ack for Box
            if ((currentMillis - previousMillis) < TIME_BETWEEN_PUBLISH * 5) {
                if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                    previousMillisPublish = millis();
                    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/handshake", "{\"id\":\"" + String(vehicle.id) + "\",\"ack\":\"" + String(vehicle.ack) + "\"}");
                }
            } else {
                substate = 0;
                return Event::HSsucessful;
            }
            return Event::NoEvent;
            break;
        default:
            break;
    }
    return Event::NoEvent;
}

void VehicleCtrl::exitAction_handshake() {
    DBSTATUSln("Leaving State: handshake");
    pComm.unsubscribe("Box/" + String(vehicle.req) + "/handshake");
}

//==loadVehicle========================================================
void VehicleCtrl::entryAction_loadVehicle() {
    DBSTATUSln("Entering State: loadVehicle");
    currentState = State::loadVehicle;  // state transition
    doActionFPtr = &VehicleCtrl::doAction_loadVehicle;
    publishState(currentState);  //Update Current State and Publish
    publishPosition();           //Update Current Position and Publish for GUI
    //update targetPosition
    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;

    pNavCtrl.setTargetPosition(vehicle.targetSector, vehicle.targetLine);
    pNavCtrl.loop(NavigationCtrl::Event::MoveToTargetPosition);
}

VehicleCtrl::Event VehicleCtrl::doAction_loadVehicle() {
    DBINFO1ln("State: loadVehicle");
    //Check all saved Messages and save the best Vehicles to an Array
    //Generate the Event
    pNavCtrl.loop();
    pComm.loop();  //Check for new Messages
    if (checkForError()) {
        return Event::Error;
    }

    currentMillis = millis();
    if ((currentMillis - previousMillisPublishPos) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
        previousMillisPublishPos = millis();
        publishPosition();        //Update Current Position and Publish for GUI
        publishTargetPosition();  //Publish TargetPosition so it will stay free
    }
    switch (substate) {
        case 0:  //check if already in TargetPosition and subscribe to Gateway if waiting for one
            if ((pNavCtrl.getcurrentSector() == vehicle.targetSector) &&
                (pNavCtrl.getcurrentLine() == vehicle.targetLine)) {
                substate = 40;
            }

            if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticWaitForGateway) {  //check in which sector and subscribe to actual gateway
                pComm.subscribe("Sortic/Gateway");
                substate = 10;
                previousMillis = millis();
            } else if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferWaitForGateway) {
                pComm.subscribe("Transfer/Gateway");
                substate = 10;
                previousMillis = millis();
            }
            break;
        case 10:  //check incomming messages. if gateway is free for some time take token
            DBINFO2ln("if gateway is free for some time take token");
            pComm.loop();               //Check for new Messages
            while (!pComm.isEmpty()) {  //check for message
                myJSONStr temp = pComm.pop();
                DBINFO2ln(String("vehicle.id: ") + String(vehicle.id) + String(": ") + String("temp.token: ") + String(temp.token));
                if (!temp.token && ((temp.topic == String("Sortic/Gateway")) || (temp.topic == String("Transfer/Gateway")))) {  //if token is not availabel reset time
                    previousMillis = millis();
                }
            }
            currentMillis = millis();
            if ((currentMillis - previousMillis) > TIME_BETWEEN_PUBLISH * 10) {  //if no message for some time take token
                if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticWaitForGateway) {
                    pComm.unsubscribe("Sortic/Gateway");
                } else if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferWaitForGateway) {
                    pComm.unsubscribe("Transfer/Gateway");
                }
                pNavCtrl.giveToken();
                substate = 20;
            }
            break;
        case 20:  //choose which gateway to block
            if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticGateway) {
                substate = 21;
            } else if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferGateway) {
                substate = 22;
            }
            break;
        case 21:                                                                   //publish token to gateway as long as you're blocking it
            if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                previousMillisPublish = millis();
                if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticGateway) {
                    pComm.publishMessage("Sortic/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":false}");
                } else {  //exit substate
                    pComm.publishMessage("Sortic/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":true}");
                    substate = 30;
                }
            }
        case 22:                                                                   //publish token to gateway as long as you're blocking it
            if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                previousMillisPublish = millis();
                if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferGateway) {
                    pComm.publishMessage("Transfer/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":false}");
                } else {  //exit substate
                    pComm.publishMessage("Transfer/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":true}");
                    substate = 30;
                }
            }
        case 30:  //check if in position
            if (pNavCtrl.getcurrentSector() == vehicle.targetSector &&
                pNavCtrl.getcurrentLine() == vehicle.targetLine) {
                substate = 40;
            }
            break;
        case 40:  //raise hoist and leave state
            if (pHoistCtrl.getcurrentState() != HoistCtrl::State::high) {
                pHoistCtrl.loop(HoistCtrl::Event::Raise);
            } else {
                substate = 0;
                return Event::PosReached;
            }
            break;
        default:
            break;
    }

    return Event::NoAnswerReceived;
    // return
}

void VehicleCtrl::exitAction_loadVehicle() {
    DBSTATUSln("Leaving State: loadVehicle");
    // pComm.clear();
}

//==unloadVehicle========================================================
void VehicleCtrl::entryAction_unloadVehicle() {
    DBSTATUSln("Entering State: unloadVehicle");
    currentState = State::unloadVehicle;  // state transition
    doActionFPtr = &VehicleCtrl::doAction_unloadVehicle;
    publishState(currentState);  //Update Current State and Publish
    publishPosition();           //Update Current Position and Publish for GUI
    previousMillis = millis();
    previousMillisPublish = previousMillis;
    currentMillis = previousMillis;
}

VehicleCtrl::Event VehicleCtrl::doAction_unloadVehicle() {
    DBINFO1ln("State: unloadVehicle " + String(substate));
    currentMillis = millis();
    pNavCtrl.loop();
    pComm.loop();  //Check for new Messages
    if (checkForError()) {
        return Event::Error;
    }

    if ((currentMillis - previousMillisPublishPos) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
        previousMillisPublishPos = millis();
        publishPosition();        //Update Current Position and Publish for GUI
        publishTargetPosition();  //Publish TargetPosition so it will stay free
    }

    switch (substate) {
        case 0:  //check cargo and choose targetpos
            if (vehicle.actualSector == NavigationCtrl::Sector::SorticHandover) {
                pComm.subscribe("Transfer/Handover");
                substate = 1;
                break;
            } else if (vehicle.actualSector == NavigationCtrl::Sector::TransferHandover) {
                pComm.subscribe("Sortic/Handover");
                substate = 2;
                for (int i = 0; i < SORTIC_MAX_LINE; i++) {
                    pSorticPark[i] = 0;  //reset
                }
                previousMillis = millis();
                break;
            }
            break;
        case 1:  // listen for matching cargo in Transfer
            if (!pComm.isEmpty()) {
                myJSONStr temp = pComm.pop();
                if ((temp.id == String("Transfer")) && (temp.cargo == vehicle.cargo)) {
                    vehicle.targetSector = NavigationCtrl::Sector::TransferHandover;
                    vehicle.targetLine = temp.line;
                    substate = 5;
                    pComm.unsubscribe("Transfer/Handover");
                    break;
                }
            }
            break;
        case 2:  // listen for empty place in Sortic
            DBINFO2ln("Substate 2: listen for empty place in Sortic");
            currentMillis = millis();
            DBINFO2ln(String(currentMillis - previousMillis) + String(" < ") + String(TIME_BETWEEN_PUBLISH * 10));
            if ((currentMillis - previousMillis) < TIME_BETWEEN_PUBLISH * 10) {  //wait some time and add message to array and evaluate afer timeout
                if (!pComm.isEmpty()) {
                    myJSONStr temp = pComm.pop();
                    DBINFO2ln(String("Sector: ") + String(temp.sector) + String(" line: ") + String(temp.line));
                    DBINFO2ln("Sector: " + String(pNavCtrl.decodeSector(temp.sector) == NavigationCtrl::Sector::SorticHandover));
                    DBINFO2ln("Line: " + String("0 < ") + String(temp.line) + String(" < ") + String(SORTIC_MAX_LINE + 1));
                    if ((pNavCtrl.decodeSector(temp.sector) == NavigationCtrl::Sector::SorticHandover) && (temp.line < SORTIC_MAX_LINE + 1) && (temp.line > 0)) {
                        pSorticPark[temp.line - 1] = 1;
                        DBINFO2ln(String("Line occupied: ") + String(temp.line));
                        DBINFO2ln(String("pSorticPark[temp.line - 1]: ") + String(pSorticPark[temp.line - 1]));
                        break;
                    }
                }
            } else {  // time reached
                DBINFO2ln("time reached");
                for (int i = 0; i < SORTIC_MAX_LINE; i++) {
                    Serial.println(pSorticPark[i]);
                }

                int line = 0;
                for (int i = 0; i < SORTIC_MAX_LINE; i++) {
                    if (pSorticPark[i] == 0) {
                        line = i + 1;  //update line with empty line
                    } else {
                        pSorticPark[i] = 0;  //reset arrayvalue at index
                    }
                }
                // int line = chooseLine(pSorticPark);
                DBINFO2("Line: ");
                DBINFO2ln(line);
                delay(10000);

                if (line != 0) {
                    vehicle.targetSector = NavigationCtrl::Sector::SorticHandover;
                    vehicle.targetLine = line;
                    substate = 5;
                    DBINFO2ln("Change Substate to 5");
                    previousMillis = millis();
                    pComm.unsubscribe("Sortic/Handover");
                    pComm.unsubscribe("Transfer/Handover");
                } else {  //start again if noavailable line found
                    previousMillis = millis();
                }
            }
            break;
        case 5:  //update target position
            pNavCtrl.setTargetPosition(vehicle.targetSector, vehicle.targetLine);
            pNavCtrl.loop(NavigationCtrl::Event::MoveToTargetPosition);
            substate = 10;
            break;
        case 10:  //check cargo and choose target position
            if ((pNavCtrl.getcurrentSector() == vehicle.targetSector) &&
                (pNavCtrl.getcurrentLine() == vehicle.targetLine)) {
                substate = 40;
            }
            if ((pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticWaitForGateway) ||
                (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransitWaitForGatewaySortic)) {  //check in which sector and subscribe to actual gateway
                pComm.subscribe("Sortic/Gateway");
                substate = 20;
                previousMillis = millis();
            } else if ((pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferWaitForGateway) ||
                       (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransitWaitForGatewayTransfer)) {
                pComm.subscribe("Transfer/Gateway");
                substate = 20;
                previousMillis = millis();
            }
            break;
        case 20:  // wait for gateway
            DBINFO2ln("if gateway is free for some time take token");
            pComm.loop();               //Check for new Messages
            while (!pComm.isEmpty()) {  //check for message
                myJSONStr temp = pComm.pop();
                DBINFO2ln(String("vehicle.id: ") + String(vehicle.id) + String(": ") + String("temp.token: ") + String(temp.token));
                if (!temp.token && ((temp.topic == String("Sortic/Gateway")) || (temp.topic == String("Transfer/Gateway")))) {  //if token is not availabel reset time
                    previousMillis = millis();
                }
            }
            currentMillis = millis();
            if ((currentMillis - previousMillis) > TIME_BETWEEN_PUBLISH * 10) {  //if no message for some time take token
                if ((pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticWaitForGateway) ||
                    (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransitWaitForGatewaySortic)) {
                    pComm.unsubscribe("Sortic/Gateway");
                    pNavCtrl.giveToken();
                    substate = 31;
                } else if ((pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferWaitForGateway) ||
                           (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransitWaitForGatewayTransfer)) {
                    pComm.unsubscribe("Transfer/Gateway");
                    pNavCtrl.giveToken();
                    substate = 32;
                }
            }
            break;
        case 30:  //choose which gateway to block
            if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticGateway) {
                substate = 31;
            } else if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferGateway) {
                substate = 32;
            }
            break;
        case 31:  //publish token to gateway as long as you're blocking it
            DBINFO2ln("Substate 31: Token Sortic");
            if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                previousMillisPublish = millis();
                if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::SorticGateway) {
                    pComm.publishMessage("Sortic/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":false}");
                } else {                                                                                               //exit substate
                    pComm.publishMessage("Sortic/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":true}");  //update for GUI
                    substate = 40;
                }
            }
            break;
        case 32:  //publish token to gateway as long as you're blocking it
            DBINFO2ln("Vehicle-Substate 32: Token Transfer");
            if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                previousMillisPublish = millis();
                if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransferGateway) {
                    pComm.publishMessage("Transfer/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":false}");
                } else {                                                                                                 //exit substate
                    pComm.publishMessage("Transfer/Gateway", "{\"id\":\"" + String(vehicle.id) + "\",\"token\":true}");  //update for GUI
                    substate = 40;
                }
            }
            break;
        case 40:  //check if in position waitforHandover or in targetpos
            DBINFO2ln("Vehicle-Substate 40: Check if in Position");
            if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransitWaitForGatewaySortic) {  //check in which sector and subscribe to actual gateway
                pComm.subscribe("Sortic/Gateway");
                substate = 20;
                previousMillis = millis();
            } else if (pNavCtrl.getcurrentSector() == NavigationCtrl::Sector::TransitWaitForGatewayTransfer) {
                pComm.subscribe("Transfer/Gateway");
                substate = 20;
                previousMillis = millis();
            }

            if (pNavCtrl.getcurrentSector() == vehicle.targetSector &&
                pNavCtrl.getcurrentLine() == vehicle.targetLine) {
                substate = 50;
            }
            break;
        case 50:  //lower hoist and leave state
            DBINFO2ln("Substate 50: Lower Hoist");
            if (pHoistCtrl.getcurrentState() != HoistCtrl::State::low) {
                pHoistCtrl.loop(HoistCtrl::Event::Lower);
            } else {
                substate = 60;
                previousMillis = millis();
            }
            break;
        case 60:  //publish position to box
            DBINFO2ln("Vehicle-Substate 60: Publish Position to Box");
            if ((currentMillis - previousMillis) < TIME_BETWEEN_PUBLISH * 5) {         //publish 5times
                if ((currentMillis - previousMillisPublish) > TIME_BETWEEN_PUBLISH) {  //only publish all xx seconds
                    previousMillisPublish = millis();
                    DBINFO2ln("Box/" + String(vehicle.ack) +
                              "/position"
                              "{\"id\":\"" +
                              String(vehicle.id) + "\",\"sector\":\"" + pNavCtrl.decodeSector(vehicle.actualSector) + "\",\"line\":\"" + String(vehicle.actualLine) + "\"}");
                    pComm.publishMessage("Box/" + String(vehicle.ack) + "/position", "{\"id\":\"" + String(vehicle.id) + "\",\"sector\":\"" + pNavCtrl.decodeSector(vehicle.actualSector) + "\",\"line\":\"" + String(vehicle.actualLine) + "\"}");
                }
            } else {
                substate = 0;
                return Event::PosReached;
            }
            break;

        default:
            break;
    }

    //Generate the Event
    return Event::NoEvent;
}

void VehicleCtrl::exitAction_unloadVehicle() {
    DBSTATUSln("Leaving State: unloadVehicle");
}

//==errorState========================================================
void VehicleCtrl::entryAction_errorState() {
    DBERROR("Entering State: errorState");
    lastStateBevorError = currentState;
    currentState = State::errorState;  // state transition
    doActionFPtr = &VehicleCtrl::doAction_errorState;
    publishState(currentState);  //Update Current State and Publish
    pComm.clear();
    pHoistCtrl.loop(HoistCtrl::Event::Error);
    pNavCtrl.loop(NavigationCtrl::Event::Error);
}

VehicleCtrl::Event VehicleCtrl::doAction_errorState() {
    DBINFO1ln("State: errorState");
    //Generate the Event
    pComm.loop();  //Check for new Messages
                   // if (!pComm.isEmpty()) {
    while (!pComm.isEmpty()) {
        // if (!pComm.first().error) {
        myJSONStr temp = pComm.pop();
        if (!temp.error && !temp.token) {
            // pComm.shift();
            pHoistCtrl.loop(HoistCtrl::Event::Resume);
            pNavCtrl.loop(NavigationCtrl::Event::Resume);
            return Event::Resume;
        } else if (temp.error && temp.token) {
            // pComm.shift();
            return Event::Reset;
        }
    }
    return Event::NoEvent;
}

void VehicleCtrl::exitAction_errorState() {
    DBSTATUSln("Leaving State: errorState");
    pComm.clear();
}

//==resetState========================================================
void VehicleCtrl::entryAction_resetState() {
    DBERROR("Entering State: resetState");
    lastStateBevorError = currentState;
    currentState = State::resetState;  // state transition
    doActionFPtr = &VehicleCtrl::doAction_resetState;
    clearGui();
    publishState(currentState);  //Update Current State and Publish
    pComm.clear();
}

VehicleCtrl::Event VehicleCtrl::doAction_resetState() {
    DBINFO1ln("State: resetState");
    //Generate the Event
    pComm.loop();  //Check for new Messages
    while (!pComm.isEmpty()) {
        myJSONStr temp = pComm.pop();
        if (!temp.error && !temp.token) {
            return Event::Resume;
        }
    }
    pHoistCtrl.loop();
    pNavCtrl.loop();
    return Event::NoEvent;
}

void VehicleCtrl::exitAction_resetState() {
    DBSTATUSln("Leaving State: resetState");
    pComm.clear();
    vehicle = {};  //reset struct
    pNavCtrl.setActualPosition(vehicle.actualSector, vehicle.actualLine);
    substate = 0;
    clearGui();
}

//============================================================================
//==Aux-Function==============================================================
String VehicleCtrl::decodeState(State state) {
    switch (state) {
        case State::waitForBox:
            return "waitForBox";
            break;
        case State::handshake:
            return "handshake";
            break;
        case State::loadVehicle:
            return "loadVehicle";
            break;
        case State::unloadVehicle:
            return "unloadVehicle";
            break;
        case State::errorState:
            return "errorState";
            break;
        case State::resetState:
            return "resetState";
            break;
        default:
            return "ERROR: No matching state";
            break;
    }
}

String VehicleCtrl::decodeEvent(Event event) {
    switch (event) {
        case Event::PosReached:
            return "Event::PosReached";
            break;
        case Event::HSsucessful:
            return "Event::HSsucessful";
            break;
        case Event::AnswerReceived:
            return "Event::AnswerReceived";
            break;
        case Event::NoAnswerReceived:
            return "Event::NoAnswerReceived";
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

// VehicleCtrl::Sector VehicleCtrl::decodeSector(String sector) {
//     if (String("SorticHandover") == sector) {
//         return Sector::SorticHandover;
//     } else if (String("SorticToHandover") == sector) {
//         return Sector::SorticToHandover;
//     } else if (String("SorticWaitForGateway") == sector) {
//         return Sector::SorticWaitForGateway;
//     } else if (String("TransitWaitForGatewaySortic") == sector) {
//         return Sector::TransitWaitForGatewaySortic;
//     } else if (String("TransitToSortic") == sector) {
//         return Sector::TransitToSortic;
//     } else if (String("TransitToTransfer") == sector) {
//         return Sector::TransitToTransfer;
//     } else if (String("Parking") == sector) {
//         return Sector::Parking;
//     } else if (String("TransitWaitForGatewayTransfer") == sector) {
//         return Sector::TransitWaitForGatewayTransfer;
//     } else if (String("TransferGateway") == sector) {
//         return Sector::TransferGateway;
//     } else if (String("TransferWaitForGateway") == sector) {
//         return Sector::TransferWaitForGateway;
//     } else if (String("TransferToHandover") == sector) {
//         return Sector::TransferToHandover;
//     } else if (String("TransferHandover") == sector) {
//         return Sector::TransferHandover;
//     } else {
//         return Sector::error;
//     }
// }

bool VehicleCtrl::checkForError() {
    DBFUNCCALLln("VehicleCtrl::checkForError()");
    if (!pComm.isEmpty()) {
        DBINFO3ln(pComm.size());
        DBINFO3ln(pComm.last().error);
        if (pComm.first().error) {
            pComm.shift();
            return true;
        }
    }
    return false;
}

void VehicleCtrl::publishState(State state) {
    vehicle.status = decodeState(state);
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/status", "{\"status\":\"" + String(vehicle.status) + "\"}");
}

void VehicleCtrl::publishPosition() {
    vehicle.actualSector = pNavCtrl.getcurrentSector();
    vehicle.actualLine = pNavCtrl.getcurrentLine();
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/position", "{\"sector\":\"" + pNavCtrl.decodeSector(vehicle.actualSector) + "\",\"line\":" + String(vehicle.actualLine) + "}");
}

void VehicleCtrl::publishTargetPosition() {  //Publish TargetPosition so it will stay free
    if (vehicle.targetSector == NavigationCtrl::Sector::TransferHandover) {
        pComm.publishMessage("Transfer/Handover", "{\"id\":\"" + String(vehicle.id) + "\",\"sector\":\"" + pNavCtrl.decodeSector(vehicle.targetSector) + "\",\"line\":" + String(vehicle.targetLine) + "}");
    }
}

// int VehicleCtrl::chooseLine() {
//     int size = sizeof(array) / sizeof(array[0]);
//     int line = 0;
//     for (int i = 0; i < SORTIC_MAX_LINE; i++) {
//         if (array[i] == 0) {
//             line = i + 1;  //update line with empty line
//         } else {
//             array[i] = 0;  //reset arrayvalue at index
//         }
//     }
//     return line;
// }

void VehicleCtrl::clearGui() {
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/status", "{\"status\":\"\"}");
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/position", "{\"sector\":\"\",\"line\":\"\"}");
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/available", "{\"sector\":\"\",\"line\":\"\"}");
    pComm.publishMessage("Vehicle/" + String(vehicle.id) + "/handshake", "{\"ack\":\"\",\"req\":\"\"}");
}
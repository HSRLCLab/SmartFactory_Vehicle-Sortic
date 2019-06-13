/**
 * @file VehicleCtrl.h
 * @brief The Vehicle Controll class contains the FSM for the Vehicle
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Vehicle Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 *  
 * @date 2019-04-03
 * @copyright Copyright (c) 2019
 * 
 * @todo Change pSorticPark[] array so its dynamic size
 * @todo Make a extra state after substate "checkposition" til "check if in pos" and use it in load and unload to minimize minimize code duplication
 * @todo Stop the vehicle if connection with mqtt-broker is lost.
 */

#ifndef BOXCTRL_H__
#define BOXCTRL_H__

#include <Arduino.h>

// own files:
#include "Configuration.h"

#include "HoistCtrl.h"
#include "MQTTCommunication.h"
#include "NavigationCtrl.h"

/**
 * @brief The Vehicle Controll class contains the FSM for the complete Vehicle
 * 
 * @image html VehicleCtrl.png width=1000
 */
class VehicleCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Holds all relevant infos for the Vehicle
    * 
    */
    struct Vehicle {
        String id = DEFAULT_HOSTNAME;                                                  ///< Vehiclename
        NavigationCtrl::Sector actualSector = NavigationCtrl::Sector::SorticHandover;  ///< actual sector initialise with Sortic handover
        int actualLine = DEFAUL_HOSTNAME_NUMBER;                                       ///< actual line initialised with 1
        NavigationCtrl::Sector targetSector = NavigationCtrl::Sector::SorticHandover;  ///< actual sector initialise with Sortic handover
        int targetLine = actualLine;                                                   ///< tartget line initialised with 1
        String cargo = "null";                                                         ///< cargo; not used atm
        String status = "null";                                                        ///< status of the Vehicle FSM
        String substate = "null";                                                      ///< substatus of the Vhicle
        String ack = "null";                                                           ///< ack for handshake vehicle
        String req = "null";                                                           ///< req for handshake vehicle
    } vehicle;

    /**
    * @brief Enum holds all possible events
    * 
    */
    enum class Event { PosReached,        ///< Vehicle is in position
                       HSsucessful,       ///< Handshake was sucessful
                       AnswerReceived,    ///< Answer received
                       NoAnswerReceived,  ///< No Answer received
                       Error,             ///< Error occured
                       Resume,            ///< Resume after Error occured
                       Reset,             ///< Reset after Error occured
                       NoEvent            ///< No event generated
    };

    /**
     * @brief Construct a new Vehicle Ctrl object
     * 
     * - Clears GUI
     * - Publishs current State and position
     * - sets start position in NavigationCtrl
     * - subscribe to "Vehicle/vehicle.id/error"
     * - subscribe to "Vehicle/error"
     * - subscribe to "error"
     * - subscribe to "Box/+/handshake"
     * 
     */
    VehicleCtrl();
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

    //=====PRIVATE====================================================================================
   private:
    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);

    /**
    * @brief Enum holds all possible state's
    * 
    * https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum
    */
    enum class State { waitForBox,     ///< wait for box to transport
                       handshake,      ///< handshake
                       loadVehicle,    ///< load box
                       unloadVehicle,  ///< unload box
                       resetState,     ///< reset state
                       errorState      ///< error state
    };

    State lastStateBevorError;                 ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;                        ///< holds the current state of the FSM
    Event currentEvent;                        ///< holds the current event of the FSM
    int substate = 0;                          ///< actual Substate of FSM
    int pSorticPark[SORTIC_MAX_LINE] = {0};    ///< Sortic with 10 diffrent lines
    int pTransferPark[SORTIC_MAX_LINE] = {1};  ///< Sortic with 10 diffrent lines
    unsigned int pRandomDelayFactor = 0;       ///< some sort of ALOHA stuff
    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (VehicleCtrl::*doActionFPtr)(void) = nullptr;

    NavigationCtrl pNavCtrl;                                ///< Navigation Controll object
    Communication pComm = Communication(DEFAULT_HOSTNAME);  ///< Communication object
    HoistCtrl pHoistCtrl;                                   ///< Hoist Controll object

    unsigned long currentMillis = 0;               ///< will store current time
    unsigned long previousMillis = 0;              ///< will store last time  published
    unsigned long previousMillisPublishToken = 0;  ///< will store last time token published
    unsigned long previousMillisPublish = 0;       ///< will store last publish time
    unsigned long previousMillisPublishPos = 0;    ///< will store last publish position time

    //=====StateFunctions===============================================================
    //==waitForBox==========================================================
    /**
     * @brief entry action of the waitForBox
     * 
     * - publish current state and position
     * - clear old message from circular buffer
     * - publish "Vehicle/vehicle.id/handshake" {ack:"",req:""} to clear GUI-Entry
     * - subscribe to "Box/+/handshake"
     * - publish to target sector to clear GUI-Entry
     */
    void entryAction_waitForBox();

    /**
     * @brief main action of the waitForBox
     * 
     * - check incomming message for error
     *   - return Event::Error
     * - check if a Box requests the vehicle
     *   - update vehicle.req with box.id
     *   - return Event::AnswerReceived
     * - publish all xx seconds "Vehicle/vehicle.id/availabel" {"sector": "actualSector", "line":number}
     * 
     * 
     * @return VehicleCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_waitForBox();

    /**
     * @brief exit action of the waitForBox
     * 
     * - unsubscribe "Box/+/handshake"
     * - publish "Vehicle/vehicle.id/available", {"id":"vehicle.id","sector":""","line":""} to clear GUI-Entry
     * 
     */
    void exitAction_waitForBox();

    //==handshake==========================================================
    /**
     * @brief entry action of the handshake
     * 
     * - publish current state and position
     * - subscribe to "Box/vehicle.req/handshake"
     */
    void entryAction_handshake();

    /**
     * @brief main action of the handshake
     * 
     * @image html VehicleCtrlHandshake.png width=600
     * - check incomming message for error
     *     - return Event::Error
     * 
     * 0 check for ack
     *   - check if Box ack vehicle
     *     - update targetSector and targetLine
     *     - switch to next state
     *   - if timeout
     *     - return Event:NoAnswerReceived
     * 
     * 10 publish ack for Box 5 times
     *     - "Vehicle/vehicle.id/handshake" {"id":"vehicle.id","ack":"vehicle.ack "}
     *       - return Event:HSsucessful
     * 
     *  @return VehicleCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_handshake();

    /**
     * @brief exit action of the handshake
     * 
     * - unsubscribe from "Box/vehicle.req/handshake"
     */
    void exitAction_handshake();

    //==loadVehicle==========================================================
    /**
     * @brief entry action of the loadVehicle
     * 
     * - publish current state and position
     * - publish empty targe pos to clear GUI-Entry
     * - update NavCtrls TargetPosition
     * - send NavigationCtrl::Event::MoveToTargetPosition
     */
    void entryAction_loadVehicle();

    /**
     * @brief main action of the loadVehicle
     * 
     * @image html VehicleCtrlLoad.png width=600
     * 
     * - check incomming message for error
     *     - return Event::Error
     * - publish position and targetposition all TIME_BETWEEN_PUBLISH seconds
     * 
     * 0 check position
     *  - if in positon switch to state 40 "raise hoist"
     *  - if in sector SorticWaitForGateway
     *      -   subscribe to "Sortic/Gateway"
     *  - if in sector TransferWaitForGateway
     *      -   subscribe to "Transfer/Gateway"
     * 
     * 10 wait for gateway
     *  -  listen if no token is published for current gateway for xx seconds + some random time
     *  
     * 11 listen if only one in gateway
     *  - publish token for gateway and check if the only one for xx seconds
     *  - if not available go back to 10 "wait for gateway"
     *  - else go to 20 "check which gateway to block"
     * 
     * 20 check which gateway to block
     *  - if in sector SorticGateway go to 21 "block Gateway Sortic"
     *  - if in sector TransitGateway go to 22 "block gateway Transfer"
     * 
     * 21 block Gateway Sortic
     *  - publish token to Gateway as long as in sector Gateway
     *  - go to substate 30 "chek if in position"
     * 
     * 22 block gateway Transfer
     *  - publish token to Gateway as long as in sector Gateway
     *  - go to substate 30 "chek if in position"
     * 
     * 30 chek if in position
     *  - if current sector = target sector go to 40 " raise hoist"
     * 
     * 40 raise hoist
     * 
     *  @return VehicleLevelCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_loadVehicle();

    /**
     * @brief exit action of the loadVehicle
     * 
     */
    void exitAction_loadVehicle();

    //==unloadVehicle==========================================================
    /**
     * @brief entry action of the unloadVehicle
     *
     * - publish current state and position
     * - publish empty targe pos to clear GUI-Entry
     */
    void entryAction_unloadVehicle();

    /**
     * @brief main action of the unloadVehicle
     * 
     * @image html VehicleCtrlUnload.png width=600
     * 
     * - check incomming message for error
     * - return Event::Error
     * - publish position and targetposition all TIME_BETWEEN_PUBLISH seconds
     * 
     * 0 check actual sector and subscribe to handover
     *  - if in SorticHandover subscirbe to "Transfer/Handover" and switch state to 10 "search suitable targetPos Transfer"
     *  - if in TransferHandover subscirbe to "Sortic/Handover" and switch state to 20 "search suitable targetPos Sortic"
     * 
     * 10 search suitable targetPos Transfer
     *  - listen for places with matching cargo till timeout is reached
     *  - listen to occupied places till timeout is reached
     *  - evaluate new targetposition
     *  - if targetposition is valid
     *      - publish targetposition and switch state to 11 "ensure valid target position Transfer"
     * 
     * 11 ensure valid target position Transfer
     *  - publish target position and listen if not occupied
     * 
     * 20 search suitable targetPos Sortic
     *  - listen to occupied places till timeout is reached
     *  - evaluate new targetposition
     *  - if targetposition is valid
     *      - publish targetposition and switch state to 21 "ensure valid target position Sortic"
     * 
     * 21 ensure valid target position Sortic
     *  - publish target position and listen if not occupied
     * 
     * 50 set targetposition
     *  - set Navigation TargetPosition
     *  - send Event MoveToTargetPosition to NavigationCtrl
     *  - switch to substate 100 "check position"
     * 
     * 100 check position
     *  - if already in position switcht to substate 400
     *  - check in which sector and subscribe to actual gateway
     *      - if waiting for gateway Sortic subscribe to "Sortic/Gateway"
     *      - if waiting for gateway Transfer subscribe to "Transfer/Gateway"
     *      - switch substate to 200 "wait for free gateway"
	 * 
     * 200 wait for free gateway
     *  -  listen if no token is published for current gateway for xx seconds + some random time
     *  
     * 210 listen if only one in gateway
     *  - publish token for gateway and check if the only one for xx seconds
     *  - if not available go back to 200 "wait for free gateway"
     *  - else go to 20 "check which gateway to block"
     * 
     * 300 check which gateway to block
     *  - if in sector SorticGateway go to 21 "block Gateway Sortic"
     *  - if in sector TransitGateway go to 22 "block gateway Transfer"
     * 
     * 310 block gateway Sortic
     *  - publish token to Gateway as long as in sector Gateway
     *  - go to substate 400 "chek if in position"
     * 
     * 320 block gateway Transfer
     *  - publish token to Gateway as long as in sector Gateway
     *  - go to substate 400 "chek if in position"
     * 
     * 
     * 400 chek if in position
     *  - if current sector = target sector
     *      - go to 500 "lower hoist"
     *  - if current sector = TransitWaitForGatewaySortic
     *      - subscribe to "Sortic/Gateway"
     *      - go to 200 "wait for free gateway"
     *  - if current sector = TransitWaitForGatewayTransfer
     *      - subscribe to "Transfer/Gateway"
     *      - go to 200 "wait for free gateway"
     *  
     * 500 lower hoist
     *  - if hoist is low go to 600 "publish position to Box"
     * 
     * 600 publish position to Box
     * 
     *  @return VehicleLevelCtrl::Event - generated Event 
     */
    VehicleCtrl::Event doAction_unloadVehicle();

    /**
     * @brief exit action of the unloadVehicle
     * 
     */
    void exitAction_unloadVehicle();

    //==errorState==========================================================
    /**
     * @brief entry action of the errorState
     * 
     * pass Errorevent to Hoist and NavCtrl
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState
     * 
     * check for reset message
     * 
     *  @return VehicleLevelCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_errorState();

    /**
     * @brief exit action of the errorState
     * 
     */
    void exitAction_errorState();

    //==resetState==========================================================
    /**
     * @brief entry action of the resetState
     * 
     */
    void entryAction_resetState();

    /**
     * @brief main action of the resetState
     * 
     * check for resume message
     * 
     *  @return VehicleLevelCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_resetState();

    /**
     * @brief exit action of the resetState
     * 
     */
    void exitAction_resetState();
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

    /**
     * @brief Update State and Publish actual state
     * - update vehicle state
     * - publishMessage "Vehicle/vehicle.id/status" {"status":"vehicle.status"}
     * @param state - state for update
     */
    void publishState(State state);

    /**
     * @brief Update position and Publish actual position
     * - update vehicle position
     * - publishMessage "Vehicle/vehicle.id/position" {"sector":vehicle.actualSector,"line":vehicle.actualLine"}
     */
    void publishPosition();

    /**
     * @brief Update position and Publish actual position
     * - publishMessage "Vehicle/vehicle.id/position" {"sector":vehicle.targetSector,"line":vehicle.targetLine"}
     */
    void publishTargetPosition();

    void publishTargetPositionBlockLine();

    /**
     * @brief Check if a new message with a error is received
     * 
     * @return true - error received
     * @return false - no error received
     */
    bool checkForError();

    /**
     * @brief clear node Red gui
     * 
     */
    void clearGui();
};
#endif

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
 */

#ifndef BOXCTRL_H__
#define BOXCTRL_H__

#include <Arduino.h>

// own files:

#include "Communication.h"
#include "HoistCtrl.h"
#include "NavigationCtrl.h"

/**
 * @brief The Vehicle Controll class contains the FSM for the complete Vehicle
 * 
 * @image html VehicleCtrl.png width=800
 */
class VehicleCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Holds all relevant infos for the vehicle
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

    State lastStateBevorError;               ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;                      ///< holds the current state of the FSM
    Event currentEvent;                      ///< holds the current event of the FSM
    int substate = 0;                        ///< actual Substate of FSM
    int pSorticPark[SORTIC_MAX_LINE] = {0};  ///< Sortic with 10 diffrent lines
    unsigned int pRandomDelayFactor = 0;     ///< some sort of ALOHA stuff
    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (VehicleCtrl::*doActionFPtr)(void) = nullptr;

    NavigationCtrl pNavCtrl;  ///< Navigation Controll object
    Communication pComm;      ///< Communication object
    HoistCtrl pHoistCtrl;     ///< Hoist Controll object

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
     * - publish "Vehicle/vehicle.id/handshake" {ack:"",req:""} to clear GUI-Entry
     * - subscirbe to "Box/+/handske"
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
     * - update NavCtrls TargetPosition
     * - send NavigationCtrl::Event::MoveToTargetPosition
     */
    void entryAction_loadVehicle();

    /**
     * @brief main action of the loadVehicle
     * 
     * - check incomming message for error
     *     - return Event::Error
     * - publish all position all xx seconds
     * 
     * 0 check position
     *  - if in positon switch to state 40
     *  - if in sector SorticWaitForGateway
     *      -   subscribe to "Sortic/Gateway"
     *  - if in sector TransferWaitForGateway
     *      -   subscribe to "Transfer/Gateway"
     * 
     * 10 wait for gateway
     *  - if gateway free for xx seconds
     *  
     * 20 block gateway
     * 
     * 30 chek if in position
     * 
     * 40 raise hoist
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
     */
    void entryAction_unloadVehicle();

    /**
     * @brief main action of the unloadVehicle
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

    void publishTargetPosition();

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

    // int chooseLine(int array[]);
};
#endif

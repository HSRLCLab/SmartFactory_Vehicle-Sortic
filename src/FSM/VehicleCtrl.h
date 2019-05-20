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
    struct Vehicle {
        String id = DEFAULT_HOSTNAME;                                                  ///< Vehiclename
        NavigationCtrl::Sector actualSector = NavigationCtrl::Sector::SorticHandover;  ///< actual sector initialise with Sortic handover
        int actualLine = 1;
        NavigationCtrl::Sector targetSector = NavigationCtrl::Sector::SorticHandover;  ///< actual sector initialise with Sortic handover
        int targetLine = 1;                                                            ///< actual line initialised with 1
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
                       NoEvent            ///< No event generated
    };

    /**
     * @brief Construct a new Vehicle Ctrl object
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
    enum class State { waitForBox,     ///<wait for box to transport
                       handshake,      ///<handshake
                       loadVehicle,    ///<load box
                       unloadVehicle,  ///<unload box
                       errorState };   ///< error state

    State lastStateBevorError;  ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;         ///< holds the current state of the FSM
    Event currentEvent;         ///< holds the current event of the FSM
    int substate = 0;           ///< actual Substate of FSM

    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (VehicleCtrl::*doActionFPtr)(void) = &VehicleCtrl::doAction_waitForBox;

    NavigationCtrl pNavCtrl;
    Communication pComm;
    HoistCtrl pHoistCtrl;

    // myJSONStr pTemp;

    unsigned long currentMillis = 0;          ///< will store current time
    unsigned long previousMillis = 0;         ///< will store last time
    unsigned long previousMillisPublish = 0;  ///< will store last publish time

    //=====StateFunctions===============================================================
    //==waitForBox==========================================================
    /**
     * @brief entry action of the waitForBox
     * 
     */
    void
    entryAction_waitForBox();

    /**
     * @brief main action of the waitForBox
     * 
     * 
     * @return VehicleCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_waitForBox();

    /**
     * @brief exit action of the waitForBox
     * 
     */
    void exitAction_waitForBox();

    //==handshake==========================================================
    /**
     * @brief entry action of the handshake
     * 
     */
    void entryAction_handshake();

    /**
     * @brief main action of the handshake
     * 
     *  @return VehicleCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_handshake();

    /**
     * @brief exit action of the handshake
     * 
     */
    void exitAction_handshake();

    //==loadVehicle==========================================================
    /**
     * @brief entry action of the loadVehicle
     * 
     */
    void entryAction_loadVehicle();

    /**
     * @brief main action of the loadVehicle
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
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState
     * 
     *  @return VehicleLevelCtrl::Event - generated Event
     */
    VehicleCtrl::Event doAction_errorState();

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

    /**
     * @brief 
     * 
     * @param state - 
     */
    void publishState(State state);

    /**
     * @brief 
     * 
     * @return true - 
     * @return false - 
     */
    bool checkForError();
};
#endif

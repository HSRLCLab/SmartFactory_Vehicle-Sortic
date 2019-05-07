/**
 * @file NavigationCtrl.h
 * @brief The Navigation Controll class contains the FSM for the Navigation
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Navigation Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef NAVIGATIONCTRL_H__
#define NAVIGATIONCTRL_H__

// #include "Configuration.h"
#include "LogConfiguration.h"

#include "DriveCtrl.h"

/**
 * @brief Contains the FSM for the Navigation
 * 
 * @image html NavigationCtrl.png width=1000
 */
class NavigationCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Enum holds all possible events
    * 
    */
    enum class Event { MoveToTargetPosition,  ///< Ext: Start MoveToTargetPosition
                       PosEndPointReached,    ///< Signal: PosEndPointReached
                       PosTransitReached,     ///< Signal: PosEndPointReached
                       PosReached,            ///< Signal: Position reached
                       Error,                 ///< Error occured
                       Resume,                ///< Ext: Resume after Error occured
                       NoEvent                ///< No event generated
    };

    /**
    * @brief Enum holds all possible states for the Navigation
    * 
    */
    enum class State { endPoint,      ///< endPoint State
                       toGateway,     ///< toGateway State
                       gateway,       ///< gateway State
                       crossTransit,  ///< crossTransit State
                       toEndPoint,    ///< toEndPoint State
                       errorState     ///< error State
    };

    enum class Sector {
        SorticHandover,
        SorticWaitForGateway,
        SorticGateway,
        TransitWaitForGatewaySortic,
        Transit,
        Parking,
        TransitWaitForGatewayTransfer,
        TransferGateway,
        TransferWaitForGateway,
        TransferHandover
    };

    enum class Orientation {
        North,
        East,
        South,
        West
    };

    /**
     * @brief Construct a new Navigation Ctrl object
     * and initailize the currentState with endPoint state
     * 
     */
    NavigationCtrl();

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

    /**
     * @brief Set the Target Position object
     * 
     * @param sector - 
     * @param line - 
     */
    void setTargetPosition(Sector sector, const int line);

    /**
     * @brief 
     * 
     */
    void giveToken();

    //=====PRIVATE====================================================================================
   private:
    State lastStateBevorError;  ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;         ///< holds the current state of the FSM
    Event currentEvent;         ///< holds the current event of the FSM
    int pCurrentSubState = 0;
    int pLastSubStateBevorError = 0;
    int pSubStateLoopInc = 0;

    struct ActualPos {
        Sector startSector = Sector::SorticHandover;
        Sector sector = startSector;  ///<
        int line = 1;                 ///<
        bool token = false;
    } pActual;

    struct TargetPos {
        Sector sector;  ///<
        int line;       ///<
    } pTarget;
    bool tranistonce = true;
    DriveCtrl::Event pLastGatewayTurn;

    /**
     * 
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverfendPoint.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (NavigationCtrl::*doActionFPtr)(void) = nullptr;

    DriveCtrl pDriveCtrl;  ///< Navigation Object

    //=====PrivateFunctions=========================================================================
    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);
    //=====StateFunctions=====
    //=====endPoint==========================================================
    /**
     * @brief executes the entry action of the endPoint state.
     * 
     */
    void entryAction_endPoint();

    /**
     * @brief executes the main action of the endPoint state.
     * 
     * @return NavigationCtrl::Event - generated Event
     */
    NavigationCtrl::Event doAction_endPoint();

    /**
     * @brief executes the exit action of the endPoint state.
     * 
     */
    void exitAction_endPoint();

    //=====toGateway==========================================================
    /**
     * @brief executes the entry action of the toGateway state.
     * 
     */
    void entryAction_toGateway();

    /**
     * @brief executes the main action of the toGateway state.
     * 
     * @return NavigationCtrl::Event - generated Event
     */
    NavigationCtrl::Event doAction_toGateway();

    /**
     * @brief executes the exit action of the toGateway state.
     */
    void exitAction_toGateway();

    //=====gateway==========================================================
    /**
     * @brief executes the entry action of the gateway state.
     * 
     */
    void entryAction_gateway();

    /**
     * @brief executes the main action of the gateway state.
     * 
     * @return NavigationCtrl::Event - generated Event
     */
    NavigationCtrl::Event doAction_gateway();

    /**
     * @brief executes the exit action of the gateway state.
     * 
     */
    void exitAction_gateway();

    //=====crossTransit==========================================================
    /**
     * @brief executes the entry action of the crossTransit  state.
     * 
     */
    void entryAction_crossTransit();

    /**
     * @brief executes the main action of the crossTransit state.
     * @return NavigationCtrl::Event - generated Event
     */
    NavigationCtrl::Event doAction_crossTransit();

    /**
     * @brief executes the exit action of the crossTransit state.
     */
    void exitAction_crossTransit();

    //=====toEndPoint==========================================================
    /**
     * @brief executes the entry action of the toEndPoint  state.
     * 
     */
    void entryAction_toEndPoint();

    /**
     * @brief executes the main action of the toEndPoint state.
     * @return NavigationCtrl::Event - generated Event
     */
    NavigationCtrl::Event doAction_toEndPoint();

    /**
     * @brief executes the exit action of the toEndPoint state.
     */
    void exitAction_toEndPoint();

    //==errorState==========================================================
    /**
     * @brief entry action of the errorState state.
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState state.
     * 
     *  @return NavigationCtrl::Event - generated Event
     */
    NavigationCtrl::Event doAction_errorState();

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

    /**
     * @brief Decodes the Sector-Enum and returns a description
     * 
     * @param sector - enum Sector
     * @return String - Sector as String
     */
    String decodeSector(Sector sector);

    /**
     * @brief Decodes the Orientation-Enum and returns a description
     * 
     * @param orientation - enum Orientation
     * @return String - orientation as String
     */
    String decodeOrientation(Orientation orientation);
};
#endif

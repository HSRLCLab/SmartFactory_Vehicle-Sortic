/**
 * @file DriveCtrl.h
 * @brief The Drive Controll class contains the FSM for the Drive
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - Implementation Drive Controll FSM - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-23
 * 
 * @date 2019-04-23
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DRIVECTRL_H__
#define DRIVECTRL_H__

#include "Configuration.h"

#include <PID_v1.h>

#include "Drive.h"
#include "EnvironmentDetection.h"

/**
 * @brief The Drive Controll class contains the FSM for the Drive
 * 
 */
class DriveCtrl {
    //=====PUBLIC====================================================================================
   public:
    /**
    * @brief Enum holds all possible events
    * 
    */
    enum class Event { TurnLeft,          ///< Ext.:
                       TurnRight,         ///< Ext.:
                       TurnAround,        ///< Ext.:
                       FollowLine,        ///< Ext.:
                       FullLineDetected,  ///< Signal:
                       LineAligned,       ///< Signal:
                       Error,             ///< Ext.: Error occured
                       Resume,            ///< Ext.: Resume after Error occured
                       NoEvent            ///< No event generated
    };

    /**
    * @brief Enum holds all possible states
    * 
    */
    enum class State { idle,           ///<  State
                       turningLeft,    ///<  State
                       turningRight,   ///<  State
                       turningAround,  ///<  State
                       followingLine,  ///<  State
                       errorState      ///< Error State
    };
    /**
     * @brief Construct a new Drive Ctrl object
     * and initailize the currentState with idle state
     * 
     */
    DriveCtrl();

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

    /**
     * @brief Get the current State
     * 
     * @return State - current State
     */
    const State getcurrentState();

    //=====PRIVATE====================================================================================
   private:
    /**
     * @brief changes the state of the FSM based on the event
     * 
     * @param e - Event
     */
    void process(Event e);

    State lastStateBevorError;  ///< holds the last state of the FSM so it's possible to resume after error
    State currentState;         ///< holds the current state of the FSM
    Event currentEvent;         ///< holds the current event of the FSM

    /**
     * @brief Functionpointer to call the current states do-function
     * 
     * https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
     */
    Event (DriveCtrl::*doActionFPtr)(void) = &DriveCtrl::doAction_idle;
    unsigned int loopcount;

    Drive pDrive = Drive(RIGHT_MOTOR, LEFT_MOTOR);  ///< Drive Object
    EnvironmentDetection pEnvdetect;                ///< EnviromentDetection Object

    double pRegler_Input = 0;
    double pRegler_Output = 0;
    double pRegler_Setpoint = 0;
    int pSampleTime = 50;

    double pVal_p = PID_KP;
    double pVal_i = PID_KI / pSampleTime;
    double pVal_d = PID_KD * pSampleTime;
    PID pRegler = PID(&pRegler_Input, &pRegler_Output, &pRegler_Setpoint, pVal_p, pVal_i, pVal_d, DIRECT);

    /*https://www.quora.com/What-would-be-appropriate-tuning-factors-for-PID-line-follower-robot
       1. Set all the gains to zero
       2. Increase only Kp (Proportional gain) to get an ultimate oscillating value Kp(max)
       3. Increase Kd (Derivative gain) until the oscillations disappear.
       4. Repeat steps 2 and 3 until increasing Kd does not dampen the oscillations
       5. Now increase Ki (Integral gain) to get a good system with desired number of oscillations (Ideally zero)
    */

    //=====StateFunctions=====
    //=====idle==========================================================
    /**
     * @brief executes the entry action of the idle
     * 
     * 
     */
    void entryAction_idle();

    /**
     * @brief executes the main action of the idle
     * 
     */
    DriveCtrl::Event doAction_idle();

    /**
     * @brief executes the exit action of the idle
     * 
     */
    void exitAction_idle();

    //=====turningLeft==========================================================
    /**
     * @brief executes the entry action of the turningLeft 
     * 
     */
    void entryAction_turningLeft();

    /**
     * @brief executes the main action of the turningLeft
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_turningLeft();

    /**
     * @brief executes the exit action of the turningLeft
     * 
     */
    void exitAction_turningLeft();

    //=====turningRight==========================================================
    /**
     * @brief executes the entry action of the turningRight 
     * 
     */
    void entryAction_turningRight();

    /**
     * @brief executes the main action of the turningRight
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_turningRight();

    /**
     * @brief executes the exit action of the turningRight
     * 
     */
    void exitAction_turningRight();

    //=====turningAround==========================================================
    /**
     * @brief executes the entry action of the turningAround 
     * 
     */
    void entryAction_turningAround();

    /**
     * @brief executes the main action of the turningAround
     * 
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_turningAround();

    /**
     * @brief executes the exit action of the turningAround
     * 
     */
    void exitAction_turningAround();

    //=====followingLine==========================================================
    /**
     * @brief executes the entry action of the followingLine 
     * 
     */
    void entryAction_followingLine();

    /**
     * @brief executes the main action of the followingLine
     * 
     * 
     * @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_followingLine();

    /**
     * @brief executes the exit action of the followingLine
     * 
     */
    void exitAction_followingLine();
    //==errorState==========================================================
    /**
     * @brief entry action of the errorState
     * 
     */
    void entryAction_errorState();

    /**
     * @brief main action of the errorState
     * 
     *  @return DriveCtrl::Event - generated Event
     */
    DriveCtrl::Event doAction_errorState();

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
};
#endif

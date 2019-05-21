/**
 * @file main.cpp
 * @brief Main programm to run the transport vehicle
 * 
 * @author Glenn Huber (glenn.patrick.huber@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 * @todo entkoppelung von configfiles
 */
#define TESTING 5  //0=run, 1=hoist, 2=sonar, 3=chassis, 4=vision, 5=network ,99= Motivation

// #include <WiFi101.h>
#include "Arduino.h"
#include "Chassis.h"
#include "Hoist.h"
#include "LogConfiguration.h"
// #include "SPI.h"
#include "Sonar.h"
// #include "VehicleWebAPI.h"
#include "Vision.h"

#include "Communication.h"
#include "DriveCtrl.h"
#include "EnvironmentDetection.h"
#include "HoistCtrl.h"
#include "NavigationCtrl.h"

#include "FSM/VehicleCtrl.h"

enum class TestCase {
    RUN,
    HOIST,
    HOISTCTRL,
    DRIVE,
    DRIVECTRL,
    ENVDETEC,
    NAVIGATIONCTRL,
    VISION,
    NETWORK,
    SONAR,
    MOTIVATION = 99
} Test = TestCase::RUN;

// /**
//  * @brief
//  *
//  */
// struct VehicleState {
//     ChassisState chassis;
//     SonarState sonar;
//     VisionState vision;
//     // HoistState hoist;
//     // ApiState api;
//     float curveFactor = 0;
//     int turnCount = 1;
//     int waitCount = 0;
//     int sonarCount = 0;
//     unsigned long timeStart = 0;
//     bool turnSonar = true;
//     bool waitForTurn = false;
//     bool visual = false;
//     String side = "";
// } state;

Sonar *vehicleSonar;
Vision *vehicleVision;
Hoist *vehicleHoist;
// Chassis *vehicleChassis;
// VehicleWebAPI *vehicleAPI;

// Network *network;
Communication *comm;
myJSONStr tmp_mess;
HoistCtrl *hoistctrl;
DriveCtrl *drivectrl;
NavigationCtrl *navctrl;

Drive *drive;
EnvironmentDetection *envdetect;

VehicleCtrl *vehictrl;

unsigned long currentMillis = 0;   ///< will store current time
unsigned long previousMillis = 0;  ///< will store last time
// /**
//  * @brief This shit is fucked up
//  *
//  */
// void strategy();

void (*FuncFPtr)(void) = &loop;
void run();
void test_communication();
void test_hardware();
void test_ctrl();

/**
 * @brief For initialisation of the Board
 * 
 * Use it to initialize variables, pin modes, start using libraries, etc.
 * The setup() function will only run once,
 * after each powerup or reset of the board
 */
void setup() {
    // Initialize serial and wait for port to open:
    // if (DEBUGGER == true) {
    //     Serial.begin(115200);
    //     while (!Serial) {
    //         ;  // wait for serial port to connect. Needed for native USB port only
    //     }
    // }

    DBFUNCCALLln("==setup()==");
    DBSTATUSln("Vehicle: booting...");
    switch (Test) {
        case TestCase::HOIST:
            vehicleHoist = new Hoist(HOIST_SERVO_PIN, HOIST_SERVO_DELAY, HOIST_POISITION_MAX, HOIST_POSITION_MIN);
            break;
        case TestCase::HOISTCTRL:
            hoistctrl = new HoistCtrl();
            break;
        case TestCase::DRIVE:
            drive = new Drive(RIGHT_MOTOR, LEFT_MOTOR);
            //     vehicleChassis = new Chassis(SPEED, K_P, K_I, K_D, RIGHT_MOTOR, LEFT_MOTOR, PIN_SENSOR_0, PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4);
            //     state.chassis.speed = SPEED;
            break;
        case TestCase::DRIVECTRL:
            drivectrl = new DriveCtrl();
            break;
        case TestCase::NAVIGATIONCTRL:
            navctrl = new NavigationCtrl();
            break;
        case TestCase::ENVDETEC:
            envdetect = new EnvironmentDetection();
            break;
        case TestCase::VISION:
            //     vehicleVision = new Vision(VISION_START_ANGLE, VISION_SERVO_PIN, VISION_DELAY_FACTOR, VISION_TOLERANCE_LEFT, VISION_TOLERANCE_RIGHT);
            break;
        case TestCase::NETWORK:
            //     vehicleAPI = new VehicleWebAPI(&state.api);

            comm = new Communication();
            // comm->init();
            comm->printNetworkInfo();
            comm->subscribe("Test");
            break;
        case TestCase::SONAR:
            // vehicleSonar = new Sonar(SONAR_SERVO_PIN, SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE, MIN_ERROR, MAX_ERROR, MIN_TURN_ANGLE, MAX_TURN_ANGLE);
            break;
        case TestCase::RUN:
            vehictrl = new VehicleCtrl();
            break;
        default:

            break;
    }
    DBSTATUSln("Booting complete!");
    // // you're connected now, so print out the data:
    // Serial.print("You're connected to the network");
}

/**
 * @brief 
 * 
 * After creating a setup() function, which initializes and sets the initial values,
 * the loop() function does precisely what its name suggests,
 * and loops consecutively, allowing your program to change and respond.
 * Use it to actively control the board.
 * 
 */
void loop() {
    DBINFO1ln("==loop()==");
    switch (Test) {
        case TestCase::RUN:
            FuncFPtr = &run;
            break;
        case TestCase::HOIST:
        case TestCase::DRIVE:
        case TestCase::ENVDETEC:
        case TestCase::VISION:
        case TestCase::SONAR:
            FuncFPtr = &test_hardware;
            break;
        case TestCase::NAVIGATIONCTRL:
        case TestCase::HOISTCTRL:
        case TestCase::DRIVECTRL:
            FuncFPtr = &test_ctrl;
            break;
        case TestCase::NETWORK:
            FuncFPtr = &test_communication;
            break;
        default:
            break;
    }
    FuncFPtr();
}

void run() {
    vehictrl->loop();
    //     vehicleAPI->loop(&state.api);
    //     if (state.api.workState == true) {
    //         state.sonarCount++;
    //         if ((state.api.sector == "20") && (state.visual == true)) {
    //             vehicleVision->loop(&state.vision);
    //         }
    //         vehicleChassis->loop(&state.chassis);
    //         if (state.sonarCount > 30) {
    //             state.sonarCount = 0;
    //             vehicleSonar->loop(&state.sonar, state.chassis.directionError, state.turnSonar);
    //         }
    //         vehicleHoist->loop(&state.hoist);
    //     }
    //     strategy();
    //     if (DEBUGGER == true) Serial.println("-----------------------------------------");
}
void test_communication() {
    // comm.publishMessage("Lizzy", "{Friend: Luca}");
    // delay(1000);
    previousMillis = millis();
    currentMillis = millis();
    while (((currentMillis - previousMillis) / 1000 < 10)) {
        currentMillis = millis();
        comm->loop();
    }

    DBINFO1ln("=====PRINT LAST MESSAGE=====");
    DBINFO1ln(comm->size());
    if (comm->isEmpty()) {
        DBINFO1ln("Buff empty ");
    } else {
        DBINFO1ln("Buff NOT empty ");
        myJSONStr newMessage1;
        while (!comm->isEmpty()) {
            DBINFO1ln(comm->size());
            tmp_mess = comm->pop();
            DBINFO1("Topic: ");
            DBINFO1ln(tmp_mess.topic);
            DBINFO1("Sensor: ");
            DBINFO1ln(tmp_mess.sensor);
            DBINFO1("Time: ");
            DBINFO1ln(tmp_mess.time);
            DBINFO1("data_0: ");
            DBINFO1ln(tmp_mess.data[0]);
            DBINFO1("data_1: ");
            DBINFO1ln(tmp_mess.data[1]);
        }
    }

    delay(1000);
}

void test_hardware() {
    switch (Test) {
        case TestCase::HOIST:
            DBSTATUSln("==Test HOIST==");
            //     vehicleHoist->Hoist::Test(0);
            while (!vehicleHoist->raise()) {
            };
            delay(1000);
            while (!vehicleHoist->lower()) {
            };
            delay(1000);
            break;
        case TestCase::DRIVE:
            DBSTATUSln("==Test DRIVE==");
            drive->drive(Drive::Direction::Forward, 250);
            delay(1000);
            drive->stop();
            delay(1000);
            drive->drive(Drive::Direction::Backward, 250);
            delay(1000);
            drive->stop();
            delay(5000);
            //     vehicleChassis->Chassis::Test();
            break;
        case TestCase::VISION:
            DBSTATUSln("==Test Vision==");
            //     vehicleVision->Vision::Test(1);
            break;
        case TestCase::ENVDETEC:
            envdetect->Linedeviation();
            break;
        case TestCase::SONAR:
            DBSTATUSln("==Test Sonar==");
            /*
             * 0 - run all tests
             * 1 - run test for servo
             * 2 - run test for obstacle detection
             * */
            vehicleSonar->Sonar::Test(1);
            break;

        default:
            break;
    }
}

void test_ctrl() {
    //https://www.arduino.cc/en/Tutorial/SwitchCase2
    int inByte;
    bool firstcall = true;
    switch (Test) {
        case TestCase::HOISTCTRL:
            DBSTATUSln("==Test Hoist CTRL==");
            //read Serial in and generate events
            Serial.println("Possible Events are:");
            Serial.println("R - Raise");
            Serial.println("L - Lower");
            Serial.println("E - Error");
            Serial.println("r - Resume");
            Serial.println("N - No Event");
            Serial.println("p - Position Reached");
            Serial.print("Choose Event: ");
            while (Serial.available() <= 0) {
            }
            inByte = Serial.read();
            Serial.print((char)inByte);
            Serial.println();
            switch (inByte) {
                case 'R':
                    DBINFO1ln("Event: Raise");
                    while (hoistctrl->getcurrentState() != HoistCtrl::State::high)
                        hoistctrl->loop(HoistCtrl::Event::Raise);
                    break;
                case 'L':
                    DBINFO1ln("Event: Lower");
                    while (hoistctrl->getcurrentState() != HoistCtrl::State::low)
                        hoistctrl->loop(HoistCtrl::Event::Lower);
                    break;
                case 'E':
                    DBINFO1ln("Event: Error");
                    hoistctrl->loop(HoistCtrl::Event::Error);
                    break;
                case 'r':
                    DBINFO1ln("Event: Resume");
                    hoistctrl->loop(HoistCtrl::Event::Resume);
                    break;
                case 'N':
                    DBINFO1ln("Event: No Event");
                    hoistctrl->loop(HoistCtrl::Event::NoEvent);
                    break;
                case 'p':
                    DBINFO1ln("Event: posPeached");
                    hoistctrl->loop(HoistCtrl::Event::PosReached);
                    break;
                default:
                    DBINFO1ln("Error: Unknown value entered");
                    break;
            }
            break;
        case TestCase::DRIVECTRL:
            DBSTATUSln("==Test Drive CTRL==");
            //read Serial in and generate events
            Serial.println("Possible Events are:");
            Serial.println("L - TurnLeft");
            Serial.println("R - TurnRight");
            Serial.println("A - TurnAround");
            Serial.println("F - FollowLineForward");
            Serial.println("B - FollowLineBackward");
            Serial.println("E - Error");
            Serial.println("r - Resume");
            Serial.println("N - No Event");
            Serial.println("l - LineAligned");
            Serial.println("f - FullLineDetected");
            Serial.print("Choose Event: ");
            while (Serial.available() <= 0) {
            }
            inByte = Serial.read();
            Serial.print((char)inByte);
            Serial.println();
            switch (inByte) {
                case 'L':
                    DBINFO1ln("Event: TurnLeft");
                    drivectrl->loop(DriveCtrl::Event::TurnLeft);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::turningLeft) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    break;
                case 'R':
                    DBINFO1ln("Event: TurnRight");
                    drivectrl->loop(DriveCtrl::Event::TurnRight);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::turningRight) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    break;
                case 'A':
                    DBINFO1ln("Event: TurnAround");
                    drivectrl->loop(DriveCtrl::Event::TurnAround);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::turningAround) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    break;
                case 'F':
                    DBINFO1ln("Event: FollowLineForward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineForward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    break;
                case 'B':
                    DBINFO1ln("Event: FollowLineBackward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineBackward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineBackward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    break;
                case 'E':
                    DBINFO1ln("Event: Error");
                    drivectrl->loop(DriveCtrl::Event::Error);
                    break;
                case 'r':
                    DBINFO1ln("Event: Resume");
                    drivectrl->loop(DriveCtrl::Event::Resume);
                    break;
                case 'N':
                    DBINFO1ln("Event: No Event");
                    drivectrl->loop(DriveCtrl::Event::NoEvent);
                    break;
                case 'l':
                    DBINFO1ln("Event: LineAligned");
                    drivectrl->loop(DriveCtrl::Event::LineAligned);
                    break;
                case 'f':
                    DBINFO1ln("Event: FullLineDetected");
                    drivectrl->loop(DriveCtrl::Event::FullLineDetected);
                    break;
                case 'x':
                    DBINFO1ln("AUTOSWITCH LANE");
                    DBINFO1ln("Event: FollowLineBackward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineBackward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineBackward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: TurnAround");
                    drivectrl->loop(DriveCtrl::Event::TurnAround);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::turningAround) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: FollowLineForward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineForward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: TurnRight");
                    drivectrl->loop(DriveCtrl::Event::TurnRight);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::turningRight) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: FollowLineForward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineForward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: TurnRight");
                    drivectrl->loop(DriveCtrl::Event::TurnRight);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::turningRight) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: FollowLineForward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineForward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    DBINFO1ln("Event: FollowLineForward");
                    drivectrl->loop(DriveCtrl::Event::FollowLineForward);
                    while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
                        drivectrl->loop();
                    }
                    break;
                default:
                    DBINFO1ln("Error: Unknown value entered");
                    break;
            }
            break;
        case TestCase::NAVIGATIONCTRL:
            DBSTATUSln("==Test Navigation CTRL==");
            //read Serial in and generate events
            Serial.println("Possible Events are:");
            Serial.println("S - MoveToTargetPosition Sortic 1");
            Serial.println("s - MoveToTargetPosition Sortic 3");
            Serial.println("T - MoveToTargetPosition Transfer 1");
            Serial.println("t - MoveToTargetPosition Transfer 3");
            Serial.println("E - Error");
            Serial.println("r - Resume");
            Serial.println("N - No Event");
            Serial.println("K - PosEndPointReached");
            Serial.println("k - PosTransitReached");
            Serial.println("p - Position Reached");
            Serial.print("Choose Event: ");
            while (Serial.available() <= 0) {
            }
            inByte = Serial.read();
            Serial.print((char)inByte);
            Serial.println();
            firstcall = true;
            switch (inByte) {
                case 'S':
                    DBINFO1ln("Event: MoveToTargetPosition Sortic 1");
                    navctrl->setTargetPosition(NavigationCtrl::Sector::SorticHandover, 1);
                    navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
                    while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
                        navctrl->loop();
                        // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
                        //     firstcall = false;
                        //     delay(3000);
                        //     navctrl->giveToken();
                        // }
                    }
                    break;
                case 's':
                    DBINFO1ln("Event: MoveToTargetPosition Sortic 3");
                    navctrl->setTargetPosition(NavigationCtrl::Sector::SorticHandover, 3);
                    navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
                    while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
                        navctrl->loop();
                        // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
                        //     firstcall = false;
                        //     delay(3000);
                        //     navctrl->giveToken();
                        // }
                    }
                    break;
                case 'T':
                    DBINFO1ln("Event: MoveToTargetPosition Transfer 1");
                    navctrl->setTargetPosition(NavigationCtrl::Sector::TransferHandover, 1);
                    navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
                    while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
                        navctrl->loop();
                        // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
                        //     firstcall = false;
                        //     delay(3000);
                        //     navctrl->giveToken();
                        // }
                    }
                    break;
                case 't':
                    DBINFO1ln("Event: MoveToTargetPosition Transfer 3");
                    navctrl->setTargetPosition(NavigationCtrl::Sector::TransferHandover, 3);
                    navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
                    while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
                        navctrl->loop();
                        // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
                        //     firstcall = false;
                        //     delay(3000);
                        //     navctrl->giveToken();
                        // }
                    }
                    break;
                case 'E':
                    DBINFO1ln("Event: Error");
                    navctrl->loop(NavigationCtrl::Event::Error);
                    break;
                case 'r':
                    DBINFO1ln("Event: Resume");
                    navctrl->loop(NavigationCtrl::Event::Resume);
                    break;
                case 'N':
                    DBINFO1ln("Event: No Event");
                    navctrl->loop(NavigationCtrl::Event::NoEvent);
                    break;
                case 'K':
                    DBINFO1ln("Event: PosEndPointReached");
                    navctrl->loop(NavigationCtrl::Event::PosEndPointReached);
                    break;
                case 'k':
                    DBINFO1ln("Event: PosTransitReached");
                    navctrl->loop(NavigationCtrl::Event::PosTransitReached);
                    break;
                case 'p':
                    DBINFO1ln("Event: posPeached");
                    navctrl->loop(NavigationCtrl::Event::PosReached);
                    break;
                default:
                    DBINFO1ln("Error: Unknown value entered");
                    break;
            }
            break;
        default:
            break;
    }
}

// void strategy() {
//     if (DEBUGGER == true) Serial.print("Sector begin strategy");
//     if (DEBUGGER == true) Serial.println(state.api.sector);
//     if (DEBUGGER == true) Serial.print("VehicleCount: ");
//     if (DEBUGGER == true) Serial.println(state.api.vehicleCount);
//     if (DEBUGGER == true) Serial.print("JobCount: ");
//     if (DEBUGGER == true) Serial.println(state.api.jobCount);
//     if (DEBUGGER == true) Serial.print("otherTask: ");
//     if (DEBUGGER == true) Serial.println(state.api.otherTask);
//     if (DEBUGGER == true) Serial.print("othersector: ");
//     if (DEBUGGER == true) Serial.println(state.api.otherSector);
//     if (DEBUGGER == true) Serial.print("Container: ");
//     if (DEBUGGER == true) Serial.println(state.api.container);
//     if (DEBUGGER == true) Serial.print("port: ");
//     if (DEBUGGER == true) Serial.println(state.api.unloadPort);

//     //After Task is finished, change the task
//     if ((state.api.state == "resting") && (state.api.task != "finished")) {
//         state.api.task = "finished";
//     }
//     //Sending the state if change happend
//     if (state.api.taskChange == true) {
//         if (DEBUGGER == true) Serial.println("Sending task");
//         vehicleAPI->sendTask();
//         if ((state.api.vehicleID == "Idefix") && (state.api.vehicleCount == 2)) {
//             if (DEBUGGER == true) Serial.println("send obelix task");
//             vehicleAPI->sendTaskToObelix();
//         }
//         if ((state.api.vehicleID == "Obelix") && (state.api.vehicleCount == 2)) {
//             if (DEBUGGER == true) Serial.println("send idefix task");
//             vehicleAPI->sendTaskToIdefix();
//         }
//         state.api.taskChange = false;
//     }
//     if (state.api.sectorChange == true) {
//         if (DEBUGGER == true) Serial.println("Sending sector");
//         vehicleAPI->sendSector();
//         if ((state.api.vehicleID == "Idefix") && (state.api.vehicleCount == 2) && ((state.api.sector == "20") || (state.api.sector == "24") || (state.api.sector == "31") || (state.api.sector == "40"))) {
//             if (DEBUGGER == true) Serial.println("send obelix sector");
//             vehicleAPI->sendSectorToObelix();
//         }
//         if ((state.api.vehicleID == "Obelix") && (state.api.vehicleCount == 2) && ((state.api.sector == "20") || (state.api.sector == "24") || (state.api.sector == "31") || (state.api.sector == "40"))) {
//             if (DEBUGGER == true) Serial.println("send idefix sector");
//             vehicleAPI->sendSectorToIdefix();
//         }
//         state.api.sectorChange = false;
//     }
//     if (state.api.stateChange == true) {
//         if (DEBUGGER == true) Serial.println("Sending state");
//         vehicleAPI->sendState();
//         state.api.stateChange = false;
//     }

//     //Sectorstrategy the workState is changed, dependend on the position of the other vehicle
//     if (((state.api.sector == "10") || (state.api.sector == "11")) && (state.api.otherSector == "20")) {
//         state.api.workState = false;
//         state.api.workStateChange = true;
//     } else if ((state.api.sector == "20") && (state.api.otherSector == "24")) {
//         state.api.workState = false;
//         state.api.workStateChange = true;
//     } else if ((state.api.sector == "31") && (state.api.otherSector == "31")) {
//         state.api.workState = false;
//         state.api.workStateChange = true;
//     }
//     //else if ((state.api.sector == "20") && (state.api.otherSector == "22")) {
//     //    state.api.workState = false;
//     //    state.api.workStateChange = true;
//     //}
//     //else if ((state.api.sector == "30") && (state.api.otherSector == "32")) {
//     //    state.api.workState = false;
//     //    state.api.workStateChange = true;
//     //}
//     //else if ((state.api.sector == "40") && (state.api.otherSector == "20") && (state.api.vehicleID == "Idefix")) {
//     //    state.api.workState = false;
//     //    state.api.workStateChange = true;
//     //}
//     else if (((state.api.sector == "10") || (state.api.sector == "11")) && (state.api.otherSector == "24")) {
//         state.api.workState = true;
//         state.api.workStateChange = true;
//     } else if ((state.api.sector == "20") && (state.api.otherSector == "31")) {
//         state.api.workState = true;
//         state.api.workStateChange = true;
//     } else if ((state.api.sector == "31") && (state.api.otherSector == "40")) {
//         state.api.workState = true;
//         state.api.workStateChange = true;
//     }
//     //else if ((state.api.sector == "20") && ((state.api.otherSector == "10") || (state.api.otherSector == "22") || (state.api.otherSector == "32") || (state.api.otherSector == "40"))) {
//     //    state.api.workState = true;
//     //    state.api.workStateChange = true;
//     //}
//     //else if ((state.api.sector == "30") && ((state.api.otherSector == "10") || (state.api.otherSector == "20") || (state.api.otherSector == "22") || (state.api.otherSector == "40"))) {
//     //    state.api.workState = true;
//     //    state.api.workStateChange = true;
//     //}
//     //else if ((state.api.sector == "40") && ((state.api.otherSector == "10") || (state.api.otherSector == "20") || (state.api.otherSector == "32") || (state.api.otherSector == "40")) && (state.api.vehicleID == "Idefix")) {
//     //    state.api.workState = true;
//     //    state.api.workStateChange = true;
//     //}
//     //Jobstrategy
//     if ((state.api.container == "yellow") && (state.api.unloadPort == "1") && (state.api.task != "RED_TO_PORT1") && (state.api.task != "RED_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT1") && (state.api.otherTask != "YELLOW_TO_PORT1")) {
//         state.api.task = "YELLOW_TO_PORT1";
//         state.api.state = "Starting_Task";
//         state.api.workState = true;
//         state.api.workStateChange = true;
//         state.vision.target = 2;
//     } else if ((state.api.container == "red") && (state.api.unloadPort == "1") && (state.api.task != "RED_TO_PORT1") && (state.api.task != "RED_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT1") && (state.api.otherTask != "RED_TO_PORT1")) {
//         state.api.task = "RED_TO_PORT1";
//         state.api.state = "Starting_Task";
//         state.api.workState = true;
//         state.api.workStateChange = true;
//         state.vision.target = 1;
//     } else if ((state.api.container == "yellow") && (state.api.unloadPort == "2") && (state.api.task != "RED_TO_PORT1") && (state.api.task != "RED_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT1") && (state.api.otherTask != "YELLOW_TO_PORT2")) {
//         state.api.task = "YELLOW_TO_PORT2";
//         state.api.state = "Starting_Task";
//         state.api.workState = true;
//         state.api.workStateChange = true;
//         state.vision.target = 2;
//     } else if ((state.api.container == "red") && (state.api.unloadPort == "2") && (state.api.task != "RED_TO_PORT1") && (state.api.task != "RED_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT2") && (state.api.task != "YELLOW_TO_PORT1") && (state.api.otherTask != "RED_TO_PORT2")) {
//         state.api.task = "RED_TO_PORT2";
//         state.api.state = "Starting_Task";
//         state.api.workState = true;
//         state.api.workStateChange = true;
//         state.vision.target = 1;
//     } else {
//         if (DEBUGGER == true) Serial.println("Container change in else");
//         state.api.container = "";
//         state.api.containerChange = true;
//     }

//     //Drive strategy without an active workstate, there is no sectorhandling
//     if (state.api.workState == true) {
//         //Navigation strategy
//         if (DEBUGGER == true) Serial.print("IR-status: ");
//         if (DEBUGGER == true) Serial.println(state.chassis.IR);
//         if (DEBUGGER == true) Serial.print("curveFactor before pid: ");
//         if (DEBUGGER == true) Serial.println(state.curveFactor);
//         state.curveFactor = state.chassis.PidValue;

//         //Sector 10 movement
//         if ((state.api.sector == "10") && (state.api.task != "nothing") && (state.api.task != "finished")) {
//             if (DEBUGGER == true) Serial.println("Sector 10");
//             if (state.chassis.direction != "right") {
//                 delay(1000);
//             }
//             state.chassis.direction = "right";
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 if (DEBUGGER == true) Serial.print("Sector 10 vehicleID");
//                 if (DEBUGGER == true) Serial.println(state.api.vehicleID);
//                 if (state.api.vehicleID == "Obelix") {
//                     state.api.sector = "11";
//                     if (DEBUGGER == true) Serial.println("Sector 10 order to Obelix");
//                     state.chassis.direction = "straight";
//                 } else if (state.api.vehicleID == "Idefix") {
//                     state.api.sector = "12";
//                     if (DEBUGGER == true) Serial.println("Sector 10 order to Idefix");
//                     state.chassis.direction = "stop";
//                 }
//                 if (DEBUGGER == true) Serial.println("end of scector10");
//                 state.chassis.actionDone = false;
//                 if (DEBUGGER == true) Serial.print("Sector 10 next sector: ");
//                 if (DEBUGGER == true) Serial.println(state.api.sector);
//             }
//         }
//         if (DEBUGGER == true) Serial.print("Between 10 and 11 sector: ");
//         if (DEBUGGER == true) Serial.println(state.api.sector);
//         //Sector 11 movement
//         if (state.api.sector == "11") {
//             if (DEBUGGER == true) Serial.println("Sector 11");
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "12";
//                 state.chassis.direction = "stop";
//                 if (DEBUGGER == true) Serial.println("end of sector11");
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 12 movement
//         if (state.api.sector == "12") {
//             if (DEBUGGER == true) Serial.println("Sector 12");
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 if (DEBUGGER == true) Serial.println("End of 12");
//                 state.api.sector = "20";
//                 state.chassis.actionDone = false;
//                 vehicleChassis->stop();
//                 state.chassis.direction = "straight";
//                 state.api.state = "Searching_box";
//                 state.visual = true;
//                 state.turnSonar = false;
//                 state.sonar.detachServo = true;
//             }
//         }

//         //Sector 20 movement
//         if (state.api.sector == "20") {
//             if (DEBUGGER == true) Serial.println("start of sector20");

//             if (((state.chassis.sensor[0] + state.chassis.sensor[1] + state.chassis.sensor[2] + state.chassis.sensor[3] + state.chassis.sensor[4]) == 0) || ((state.chassis.sensor[0] + state.chassis.sensor[1] + state.chassis.sensor[2] + state.chassis.sensor[3] + state.chassis.sensor[4]) == 5)) {
//                 if ((state.waitForTurn == false) && (state.waitCount == 0)) {
//                     if (DEBUGGER == true) Serial.println("changing wait for turn");
//                     state.waitForTurn = true;
//                 }
//             }
//             if (DEBUGGER == true) Serial.print("Targetdetected: ");
//             if (DEBUGGER == true) Serial.println(state.vision.targetDetected);
//             if ((state.vision.targetDetected == true) && (state.turnCount < 4)) {
//                 //First move dependend on which side the box is on
//                 if (DEBUGGER == true) Serial.println("before first turn");

//                 //Is the Box on the right side of the vehicle turn right
//                 if (state.vision.servoAngle < 90) {
//                     if (DEBUGGER == true) Serial.print("waitForturn: ");
//                     if (DEBUGGER == true) Serial.println(state.waitForTurn);
//                     if (DEBUGGER == true) Serial.print("TurnCount: ");
//                     if (DEBUGGER == true) Serial.println(state.turnCount);
//                     if ((state.waitForTurn == true) && (state.turnCount == 1)) {
//                         if (state.chassis.actionDone == true) {
//                             state.turnCount++;
//                             if (DEBUGGER == true) Serial.println("right turn accomplished");
//                             state.chassis.actionDone = false;
//                         }

//                         if (state.waitCount == 0) {
//                             if (DEBUGGER == true) Serial.println("starting timer");
//                             state.timeStart = millis();
//                             state.waitCount++;
//                         }

//                         if (state.turnCount == 1) {
//                             vehicleChassis->turnRightLimited(2800, state.timeStart);
//                             state.api.state = "Picking_up_right_box";
//                             state.side = "right";
//                             if (DEBUGGER == true) Serial.println("turnRightLimited");
//                             state.visual = false;
//                             state.turnSonar = true;
//                             //vehicleVision->turnVision(90);
//                             //if (DEBUGGER == true) Serial.println("after turnVision");
//                         }
//                     }

//                     //Second move is per default a drive straight for adjustment
//                     if (DEBUGGER == true) Serial.println("before second turn");
//                     if (state.turnCount == 2) {
//                         if (state.chassis.actionDone == true) {
//                             state.turnCount++;
//                             if (DEBUGGER == true) Serial.println("drive straight accomplished");
//                             state.chassis.actionDone = false;
//                         }

//                         if (DEBUGGER == true) Serial.println("turncount2");
//                         if (state.waitCount == 1) {
//                             state.timeStart = millis();
//                             if (DEBUGGER == true) Serial.println("starting second timer");
//                             state.waitCount++;
//                         }

//                         if (state.turnCount == 2) {
//                             vehicleChassis->driveStraightLimited(2800, state.timeStart);
//                             if (DEBUGGER == true) Serial.println("drivestraight");
//                         }
//                     }
//                     //Third move is a counterturn dependend on the first move
//                     if (DEBUGGER == true) Serial.println("before third turn");
//                     if (state.turnCount == 3) {
//                         if (DEBUGGER == true) Serial.println("turncount3");
//                         if (state.chassis.actionDone == true) {
//                             state.turnCount++;
//                             if (DEBUGGER == true) Serial.println("right turn accomplished");
//                             state.chassis.actionDone = false;
//                             state.visual = true;
//                             state.turnSonar = false;
//                             state.sonar.detachServo = true;
//                         }

//                         if (state.waitCount == 2) {
//                             state.timeStart = millis();
//                             if (DEBUGGER == true) Serial.println("starting third timer");
//                             state.waitCount++;
//                         }

//                         if (state.turnCount == 3) {
//                             vehicleChassis->turnLeftLimited(2600, state.timeStart);
//                             if (DEBUGGER == true) Serial.println("turnLeftLimited");
//                             state.waitForTurn = false;
//                         }
//                     }
//                 }
//                 //Is the Box on the left side of the vehicle turn left
//                 if (state.vision.servoAngle > 90) {
//                     if (DEBUGGER == true) Serial.print("waitForturn: ");
//                     if (DEBUGGER == true) Serial.println(state.waitForTurn);
//                     if (DEBUGGER == true) Serial.print("TurnCount: ");
//                     if (DEBUGGER == true) Serial.println(state.turnCount);
//                     if ((state.waitForTurn == true) && (state.turnCount == 1)) {
//                         if (state.chassis.actionDone == true) {
//                             state.turnCount++;
//                             if (DEBUGGER == true) Serial.println("left turn accomplished");
//                             state.chassis.actionDone = false;
//                         }

//                         if (state.waitCount == 0) {
//                             if (DEBUGGER == true) Serial.println("starting timer");
//                             state.timeStart = millis();
//                             state.waitCount++;
//                         }

//                         if (state.turnCount == 1) {
//                             vehicleChassis->turnLeftLimited(2600, state.timeStart);
//                             state.api.state = "Picking_up_left_box";
//                             state.side = "left";
//                             if (DEBUGGER == true) Serial.println("turnleftlimited");
//                             state.visual = false;
//                             state.turnSonar = true;
//                             if (DEBUGGER == true) Serial.println("after turnVision");
//                         }
//                     }

//                     //Second move is per default a drive straight for adjustment
//                     if (DEBUGGER == true) Serial.println("before second turn");
//                     if (state.turnCount == 2) {
//                         if (state.chassis.actionDone == true) {
//                             state.turnCount++;
//                             if (DEBUGGER == true) Serial.println("drive straight accomplished");
//                             state.chassis.actionDone = false;
//                         }

//                         if (DEBUGGER == true) Serial.println("turncount2");
//                         if (state.waitCount == 1) {
//                             state.timeStart = millis();
//                             if (DEBUGGER == true) Serial.println("starting second timer");
//                             state.waitCount++;
//                         }

//                         if (state.turnCount == 2) {
//                             vehicleChassis->driveStraightLimited(2400, state.timeStart);
//                             if (DEBUGGER == true) Serial.println("drivestraight");
//                         }
//                     }
//                     //Third move is a counterturn dependend on the first move
//                     if (DEBUGGER == true) Serial.println("before third turn");
//                     if (state.turnCount == 3) {
//                         if (DEBUGGER == true) Serial.println("turncount3");
//                         if (state.chassis.actionDone == true) {
//                             state.turnCount++;
//                             if (DEBUGGER == true) Serial.println("right turn accomplished");
//                             state.chassis.actionDone = false;
//                             state.visual = true;
//                             state.turnSonar = false;
//                             state.sonar.detachServo = true;
//                             state.vision.targetDetected = false;
//                         }

//                         if (state.waitCount == 2) {
//                             state.timeStart = millis();
//                             if (DEBUGGER == true) Serial.println("starting third timer");
//                             state.waitCount++;
//                         }

//                         if (state.turnCount == 3) {
//                             vehicleChassis->turnRightLimited(3200, state.timeStart);
//                             if (DEBUGGER == true) Serial.println("turnright");
//                             state.waitForTurn = false;
//                         }
//                     }
//                 }
//             }
//             //If the target is lost or never detected, the system has to stop and search for a box again
//             else if ((state.vision.targetDetected == false) && (state.turnCount < 4)) {
//                 state.visual = true;
//                 state.turnSonar = false;
//                 state.sonar.detachServo = true;
//                 vehicleChassis->stop();
//                 if (DEBUGGER == true) Serial.println("Stopping when No target");
//             }

//             //Before the box the vehicle first has to refind the line
//             if (DEBUGGER == true) Serial.println("before cameranavigation");
//             if (state.turnCount == 4) {
//                 if (DEBUGGER == true) Serial.print("Turn4 with servoAngle: ");
//                 if (DEBUGGER == true) Serial.println(state.vision.servoAngle);
//                 if (DEBUGGER == true) Serial.print("Visual: ");
//                 if (DEBUGGER == true) Serial.println(state.visual);
//                 if (((state.vision.targetDetected == true) || (state.visual == false)) && (state.hoist.loaded == false)) {
//                     if ((state.vision.servoAngle > 92) && (state.visual == true)) {
//                         vehicleChassis->turnLeft();
//                         if (DEBUGGER == true) Serial.println("after turnleft");
//                     }
//                     if ((state.vision.servoAngle < 88) && (state.visual == true)) {
//                         vehicleChassis->turnRight();
//                         if (DEBUGGER == true) Serial.println("after turnright");
//                     }
//                     if (((state.vision.servoAngle <= 92) && (state.vision.servoAngle >= 88)) && (state.visual == true)) {
//                         vehicleChassis->driveStraight();
//                         if (DEBUGGER == true) Serial.println("after drivestraight");
//                     }
//                     if (DEBUGGER == true) Serial.print(state.chassis.sensor[0]);
//                     if (DEBUGGER == true) Serial.print(state.chassis.sensor[1]);
//                     if (DEBUGGER == true) Serial.print(state.chassis.sensor[2]);
//                     if (DEBUGGER == true) Serial.print(state.chassis.sensor[3]);
//                     if (DEBUGGER == true) Serial.print(state.chassis.sensor[4]);
//                     if (((state.chassis.sensor[0] + state.chassis.sensor[1] + state.chassis.sensor[2] + state.chassis.sensor[3] + state.chassis.sensor[4]) != 0) && (state.visual == true)) {
//                         if (DEBUGGER == true) Serial.println("detected line");
//                         //state.chassis.IR = true;
//                         state.visual = false;
//                         state.turnSonar = true;
//                         state.api.sector = "21";
//                     }
//                 } else if (state.visual == true) {
//                     vehicleChassis->stop();
//                     if (DEBUGGER == true) Serial.println("stopping when no target available");
//                 }
//             }
//         }

//         //Sector 21 movement
//         if (state.api.sector == "21") {
//             if (DEBUGGER == true) Serial.println("following line again");
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "22";
//                 state.chassis.direction = "stop";
//                 if (DEBUGGER == true) Serial.println("end of sector21");
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 22 movement
//         if (state.api.sector == "22") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "23";
//                 if (DEBUGGER == true) Serial.println("end of sector22");
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 23 movement
//         if (state.api.sector == "23") {
//             state.vision.targetDetected = false;
//             if (DEBUGGER == true) Serial.println("Arrived at loadingpoint");
//             if (DEBUGGER == true) Serial.print("ActionDone 23: ");
//             if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//             if (state.hoist.loaded == false) {
//                 vehicleHoist->load();
//                 vehicleChassis->stop();
//                 state.chassis.direction = "straight";
//                 state.api.state = "Loading_box";
//                 state.chassis.IR = false;
//                 if (DEBUGGER == true) Serial.print("ActionDone 23 loading: ");
//                 if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//             }
//             //After loading process the system has to turn around
//             if (state.hoist.loaded == true) {
//                 state.hoist.detachServo = true;
//                 state.api.state = "Bringing_box_to_port";
//                 if (DEBUGGER == true) Serial.println("Loading complete");
//                 if (state.chassis.actionDone == false) {
//                     vehicleChassis->turnAround(2);
//                 }
//                 if (DEBUGGER == true) Serial.print("ActionDone: ");
//                 if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//                 if (state.chassis.actionDone == true) {
//                     state.api.sector = "24";
//                     state.chassis.direction = "right";
//                     state.chassis.actionDone = false;
//                     if (DEBUGGER == true) Serial.println("sector23 finished");
//                 }
//             }
//         }

//         //Start of sector 24 where the vehicle drives the container to the targeted port
//         if (state.api.sector == "24") {
//             if (DEBUGGER == true) Serial.println("Sector24");
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "30";
//                 if (state.side == "right") {
//                     state.chassis.direction = "left";
//                     if (DEBUGGER == true) Serial.println("direction from box at beginning was right");
//                 }
//                 if (state.side == "left") {
//                     state.chassis.direction = "straight";
//                     if (DEBUGGER == true) Serial.println("direction from box at beginning was left");
//                 }
//                 if (DEBUGGER == true) Serial.println("Sector24 actiondone");
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Start of sector 30 where the vehicle still drives the container to the targeted port
//         if (state.api.sector == "30") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (DEBUGGER == true) Serial.println("inside sector 30");
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "31";
//                 if (state.api.unloadPort == "1") {
//                     state.chassis.direction = "straight";
//                 } else if (state.api.unloadPort == "2") {
//                     state.chassis.direction = "left";
//                 }
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 31 movement
//         if (state.api.sector == "31") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (DEBUGGER == true) Serial.println("Inside sector 31");
//             if (state.chassis.actionDone == true) {
//                 if (state.api.unloadPort == "1") {
//                     state.chassis.direction = "stop";
//                     state.api.sector = "33";
//                 }
//                 if (state.api.unloadPort == "2") {
//                     state.chassis.direction = "right";
//                     state.api.sector = "32";
//                 }
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 32 movement
//         if (state.api.sector == "32") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (DEBUGGER == true) Serial.println("inside sector 32");
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "33";
//                 if (DEBUGGER == true) Serial.println("Finnishing sector 32");
//                 state.chassis.direction = "stop";
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 33 movement
//         if (state.api.sector == "33") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "34";
//                 if (DEBUGGER == true) Serial.println("Finnishing sector 33");
//                 state.chassis.direction = "straight";
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 34 movement
//         if (state.api.sector == "34") {
//             vehicleChassis->stop();
//             vehicleHoist->unload();
//             state.chassis.IR = false;
//             if (DEBUGGER == true) Serial.println("unload");
//             if (state.hoist.loaded == false) {
//                 state.hoist.detachServo = true;
//                 state.api.sector = "35";
//                 state.api.state = "Box_delivered";
//                 state.timeStart = millis();

//                 if (DEBUGGER == true) Serial.println("starting fourth timer");
//             }
//         }

//         //Sector 35 movement
//         if (state.api.sector == "35") {
//             if (state.chassis.actionDone == false) {
//                 vehicleChassis->driveBackLimited(2000, state.timeStart);
//                 state.chassis.IR = false;
//             }
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "36";
//                 if (DEBUGGER == true) Serial.println("Driveback home");
//                 state.api.state = "driving_back_home";
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 36 movement
//         if (state.api.sector == "36") {
//             if (DEBUGGER == true) Serial.print("Sector 36 Actiondone: ");
//             if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//             if (state.chassis.actionDone == false) {
//                 vehicleChassis->turnAround(2);
//                 if (DEBUGGER == true) Serial.println("Sector 36 after turnaround");
//             }
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "37";
//                 state.chassis.direction = "right";
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 37 movement
//         if (state.api.sector == "37") {
//             if (DEBUGGER == true) Serial.println("Sector 37");
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 if (state.api.unloadPort == "1") {
//                     state.api.sector = "38";
//                     state.chassis.direction = "straight";
//                 } else if (state.api.unloadPort == "2") {
//                     state.api.sector = "40";
//                     if (state.api.vehicleID == "Idefix") {
//                         state.chassis.direction = "left";
//                     } else if (state.api.vehicleID == "Obelix") {
//                         state.chassis.direction = "straight";
//                     }
//                     state.chassis.actionDone = false;
//                 }
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 38 movement
//         if (state.api.sector == "38") {
//             if (DEBUGGER == true) Serial.println("Sector 38");
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (state.chassis.actionDone == true) {
//                 state.api.sector = "40";
//                 if (state.api.vehicleID == "Idefix") {
//                     state.chassis.direction = "left";
//                 } else if (state.api.vehicleID == "Obelix") {
//                     state.chassis.direction = "straight";
//                 }
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 40 movement
//         if (state.api.sector == "40") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (DEBUGGER == true) Serial.println("Sector 40");
//             if (state.chassis.actionDone == true) {
//                 if (state.api.vehicleID == "Obelix") {
//                     if (DEBUGGER == true) Serial.println("Sector 40 Obelix finished");
//                     state.chassis.direction = "stop";
//                     state.api.sector = "42";
//                 } else if (state.api.vehicleID == "Idefix") {
//                     if (DEBUGGER == true) Serial.println("Sector 40 Idefix finished");
//                     state.chassis.direction = "right";
//                     state.api.sector = "41";
//                 }
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 41 movement
//         if (state.api.sector == "41") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (DEBUGGER == true) Serial.print("Sector 41 Actiondone: ");
//             if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//             if (state.chassis.actionDone == true) {
//                 state.chassis.direction = "stop";
//                 if (DEBUGGER == true) Serial.println("Sector 41 finished");
//                 state.api.sector = "42";
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 42 movement
//         if (state.api.sector == "42") {
//             vehicleChassis->motorControl(state.sonar.sonarFactor);
//             if (DEBUGGER == true) Serial.print("Sector 42 Actiondone: ");
//             if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//             if (state.chassis.actionDone == true) {
//                 state.chassis.direction = "stop";
//                 if (DEBUGGER == true) Serial.println("Sector 42 finished");
//                 vehicleChassis->stop();
//                 state.api.sector = "43";
//                 state.chassis.actionDone = false;
//             }
//         }

//         //Sector 43 movement reset the variables once the task is finished
//         if (state.api.sector == "43") {
//             if (DEBUGGER == true) Serial.print("Sector 43 Actiondone: ");
//             if (DEBUGGER == true) Serial.println(state.chassis.actionDone);
//             if (state.chassis.actionDone == false) {
//                 vehicleChassis->turnAround(2);
//                 if (DEBUGGER == true) Serial.println("Sector 43 after turnaround");
//             }
//             if (state.chassis.actionDone == true) {
//                 if (DEBUGGER == true) Serial.println("Sector 43 finished");
//                 state.api.sector = "10";
//                 state.api.state = "resting";
//                 state.api.task = "finished";
//                 state.api.workState = false;
//                 state.api.workStateChange = true;
//                 state.chassis.actionDone = false;
//                 state.waitCount = 0;
//                 state.turnCount = 1;
//                 state.waitForTurn = false;
//                 state.vision.targetDetected = false;
//                 state.api.jobCount++;
//                 state.vision.reset = true;
//                 state.api.container = "";
//                 state.api.unloadPort = "";
//                 state.api.containerChange = true;
//                 state.api.unloadPortChange = true;
//             }
//         }
//     } else {
//         vehicleChassis->stop();
//     }
// }
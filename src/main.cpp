/**
 * @file main.cpp
 * @brief Main programm to run the transport vehicle
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.1 - Added Doxygen-Documentation  - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * @version 1.0 - BA FTS FS 2018
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 * @todo Add a seperate testclass
 */

#include "Arduino.h"
#include "LogConfiguration.h"
#include "VehicleCtrl.h"

// #include "Hoist.h"

// #include "DriveCtrl.h"
// #include "EnvironmentDetection.h"
// #include "HoistCtrl.h"
// #include "MQTTCommunication.h"
// #include "NavigationCtrl.h"

// enum class TestCase {
//     RUN,
//     HOIST,
//     HOISTCTRL,
//     DRIVE,
//     DRIVECTRL,
//     ENVDETEC,
//     NAVIGATIONCTRL,
//     VISION,
//     NETWORK,
//     SONAR,
//     MOTIVATION = 99
// } Test = TestCase::RUN;

// Sonar *vehicleSonar;
// Vision *vehicleVision;
// Hoist *vehicleHoist;

// Communication *comm;
// myJSONStr tmp_mess;
// HoistCtrl *hoistctrl;
// DriveCtrl *drivectrl;
// NavigationCtrl *navctrl;
// Drive *drive;

// EnvironmentDetection *envdetect;

VehicleCtrl *vehictrl;

unsigned long currentMillis = 0;   ///< will store current time
unsigned long previousMillis = 0;  ///< will store last time

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
#ifdef DEBUGGER
    Serial.begin(115200);
    while (!Serial) {
        ;  // wait for serial port to connect. Needed for native USB port only
    }
#endif

    DBFUNCCALLln("==setup()==");
    DBSTATUSln("Vehicle: booting...");
    // switch (Test) {
    //     case TestCase::HOIST:
    //         vehicleHoist = new Hoist(HOIST_SERVO_PIN, HOIST_SERVO_DELAY, HOIST_POISITION_MAX, HOIST_POSITION_MIN);
    //         break;
    //     case TestCase::HOISTCTRL:
    //         hoistctrl = new HoistCtrl();
    //         break;
    //     case TestCase::DRIVE:
    //         drive = new Drive(RIGHT_MOTOR, LEFT_MOTOR);
    //         //     vehicleChassis = new Chassis(SPEED, K_P, K_I, K_D, RIGHT_MOTOR, LEFT_MOTOR, PIN_SENSOR_0, PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4);
    //         //     state.chassis.speed = SPEED;
    //         break;
    //     case TestCase::DRIVECTRL:
    //         drivectrl = new DriveCtrl();
    //         break;
    //     case TestCase::NAVIGATIONCTRL:
    //         navctrl = new NavigationCtrl();
    //         break;
    //     case TestCase::ENVDETEC:
    //         envdetect = new EnvironmentDetection();
    //         break;
    //     case TestCase::VISION:
    //         //     vehicleVision = new Vision(VISION_START_ANGLE, VISION_SERVO_PIN, VISION_DELAY_FACTOR, VISION_TOLERANCE_LEFT, VISION_TOLERANCE_RIGHT);
    //         break;
    //     case TestCase::NETWORK:
    //         //     vehicleAPI = new VehicleWebAPI(&state.api);

    //         comm = new Communication(DEFAULT_HOSTNAME);
    //         // comm->init();
    //         comm->printNetworkInfo();
    //         comm->subscribe("Sortic/Gateway");
    //         break;
    //     case TestCase::SONAR:
    //         // vehicleSonar = new Sonar(SONAR_SERVO_PIN, SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE, MIN_ERROR, MAX_ERROR, MIN_TURN_ANGLE, MAX_TURN_ANGLE);
    //         break;
    //     case TestCase::RUN:
    //         vehictrl = new VehicleCtrl();
    //         break;
    //     default:

    //         break;
    // }
    vehictrl = new VehicleCtrl();
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
    vehictrl->loop();
    // DBINFO1ln("==loop()==");
    // currentMillis = millis();
    // Serial.print("LoopTime [ms]: ");
    // Serial.println(currentMillis - previousMillis);
    // previousMillis = currentMillis;
    // // Serial.print("RandNumber: ");
    // // unsigned int randDelay = random(30) / 10.0 * 1000;
    // // Serial.println(randDelay);
    // switch (Test) {
    //     case TestCase::RUN:
    //         FuncFPtr = &run;
    //         break;
    //     case TestCase::HOIST:
    //     case TestCase::DRIVE:
    //     case TestCase::ENVDETEC:
    //     case TestCase::VISION:
    //     case TestCase::SONAR:
    //         FuncFPtr = &test_hardware;
    //         break;
    //     case TestCase::NAVIGATIONCTRL:
    //     case TestCase::HOISTCTRL:
    //     case TestCase::DRIVECTRL:
    //         FuncFPtr = &test_ctrl;
    //         break;
    //     case TestCase::NETWORK:
    //         FuncFPtr = &test_communication;
    //         break;
    //     default:
    //         break;
    // }
    // FuncFPtr();
}

// void run() {
//     vehictrl->loop();
// }

// void test_communication() {
//     previousMillis = millis();
//     currentMillis = millis();
//     while (((currentMillis - previousMillis) / 1000 < 10)) {
//         currentMillis = millis();
//         comm->loop();
//     }

//     DBINFO1ln("=====PRINT LAST MESSAGE=====");
//     DBINFO1ln(comm->size());
//     if (comm->isEmpty()) {
//         DBINFO1ln("Buff empty ");
//     } else {
//         DBINFO1ln("Buff NOT empty ");
//         myJSONStr newMessage1;
//         while (!comm->isEmpty()) {
//             DBINFO1ln(comm->size());
//             tmp_mess = comm->pop();
//             DBINFO1("Topic: ");
//             DBINFO1ln(tmp_mess.topic);
//             DBINFO1("token: ");
//             DBINFO1ln(tmp_mess.token);
//         }
//     }

//     delay(1000);
// }

// void test_hardware() {
//     switch (Test) {
//         case TestCase::HOIST:
//             DBSTATUSln("==Test HOIST==");
//             //     vehicleHoist->Hoist::Test(0);
//             while (!vehicleHoist->raise()) {
//             };
//             delay(1000);
//             while (!vehicleHoist->lower()) {
//             };
//             delay(1000);
//             break;
//         case TestCase::DRIVE:
//             DBSTATUSln("==Test DRIVE==");
//             drive->drive(Drive::Direction::Forward, 250);
//             delay(1000);
//             drive->stop();
//             delay(1000);
//             drive->drive(Drive::Direction::Backward, 250);
//             delay(1000);
//             drive->stop();
//             delay(5000);
//             //     vehicleChassis->Chassis::Test();
//             break;
//         case TestCase::VISION:
//             DBSTATUSln("==Test Vision==");
//             //     vehicleVision->Vision::Test(1);
//             break;
//         case TestCase::ENVDETEC:
//             envdetect->Linedeviation();
//             break;
//         case TestCase::SONAR:
//             DBSTATUSln("==Test Sonar==");
//             /*
//              * 0 - run all tests
//              * 1 - run test for servo
//              * 2 - run test for obstacle detection
//              * */
//             // vehicleSonar->Sonar::Test(1);
//             break;

//         default:
//             break;
//     }
// }

// void test_ctrl() {
//     //https://www.arduino.cc/en/Tutorial/SwitchCase2
//     int inByte;
//     bool firstcall = true;
//     switch (Test) {
//         case TestCase::HOISTCTRL:
//             DBSTATUSln("==Test Hoist CTRL==");
//             //read Serial in and generate events
//             Serial.println("Possible Events are:");
//             Serial.println("R - Raise");
//             Serial.println("L - Lower");
//             Serial.println("E - Error");
//             Serial.println("r - Resume");
//             Serial.println("N - No Event");
//             Serial.println("p - Position Reached");
//             Serial.print("Choose Event: ");
//             while (Serial.available() <= 0) {
//             }
//             inByte = Serial.read();
//             Serial.print((char)inByte);
//             Serial.println();
//             switch (inByte) {
//                 case 'R':
//                     DBINFO1ln("Event: Raise");
//                     // while (hoistctrl->getcurrentState() != HoistCtrl::State::high)
//                     hoistctrl->loop(HoistCtrl::Event::Raise);
//                     break;
//                 case 'L':
//                     DBINFO1ln("Event: Lower");
//                     // while (hoistctrl->getcurrentState() != HoistCtrl::State::low)
//                     hoistctrl->loop(HoistCtrl::Event::Lower);
//                     break;
//                 case 'E':
//                     DBINFO1ln("Event: Error");
//                     hoistctrl->loop(HoistCtrl::Event::Error);
//                     break;
//                 case 'r':
//                     DBINFO1ln("Event: Resume");
//                     hoistctrl->loop(HoistCtrl::Event::Resume);
//                     break;
//                 case 'N':
//                     DBINFO1ln("Event: No Event");
//                     hoistctrl->loop(HoistCtrl::Event::NoEvent);
//                     break;
//                 case 'p':
//                     DBINFO1ln("Event: posPeached");
//                     hoistctrl->loop(HoistCtrl::Event::PosReached);
//                     break;
//                 default:
//                     DBINFO1ln("Error: Unknown value entered");
//                     break;
//             }
//             break;
//         case TestCase::DRIVECTRL:
//             DBSTATUSln("==Test Drive CTRL==");
//             //read Serial in and generate events
//             Serial.println("Possible Events are:");
//             Serial.println("L - TurnLeft");
//             Serial.println("R - TurnRight");
//             Serial.println("A - TurnAround");
//             Serial.println("F - FollowLineForward");
//             Serial.println("B - FollowLineBackward");
//             Serial.println("E - Error");
//             Serial.println("r - Resume");
//             Serial.println("N - No Event");
//             Serial.println("l - LineAligned");
//             Serial.println("f - FullLineDetected");
//             Serial.print("Choose Event: ");
//             while (Serial.available() <= 0) {
//             }
//             inByte = Serial.read();
//             Serial.print((char)inByte);
//             Serial.println();
//             switch (inByte) {
//                 case 'L':
//                     DBINFO1ln("Event: TurnLeft");
//                     drivectrl->loop(DriveCtrl::Event::TurnLeft);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::turningLeft) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     break;
//                 case 'R':
//                     DBINFO1ln("Event: TurnRight");
//                     drivectrl->loop(DriveCtrl::Event::TurnRight);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::turningRight) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     break;
//                 case 'A':
//                     DBINFO1ln("Event: TurnAround");
//                     drivectrl->loop(DriveCtrl::Event::TurnAround);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::turningAround) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     break;
//                 case 'F':
//                     DBINFO1ln("Event: FollowLineForward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineForward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     break;
//                 case 'B':
//                     DBINFO1ln("Event: FollowLineBackward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineBackward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineBackward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     break;
//                 case 'E':
//                     DBINFO1ln("Event: Error");
//                     drivectrl->loop(DriveCtrl::Event::Error);
//                     break;
//                 case 'r':
//                     DBINFO1ln("Event: Resume");
//                     drivectrl->loop(DriveCtrl::Event::Resume);
//                     break;
//                 case 'N':
//                     DBINFO1ln("Event: No Event");
//                     drivectrl->loop(DriveCtrl::Event::NoEvent);
//                     break;
//                 case 'l':
//                     DBINFO1ln("Event: LineAligned");
//                     drivectrl->loop(DriveCtrl::Event::LineAligned);
//                     break;
//                 case 'f':
//                     DBINFO1ln("Event: FullLineDetected");
//                     drivectrl->loop(DriveCtrl::Event::FullLineDetected);
//                     break;
//                 case 'x':
//                     DBINFO1ln("AUTOSWITCH LANE");
//                     DBINFO1ln("Event: FollowLineBackward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineBackward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineBackward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: TurnAround");
//                     drivectrl->loop(DriveCtrl::Event::TurnAround);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::turningAround) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: FollowLineForward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineForward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: TurnRight");
//                     drivectrl->loop(DriveCtrl::Event::TurnRight);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::turningRight) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: FollowLineForward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineForward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: TurnRight");
//                     drivectrl->loop(DriveCtrl::Event::TurnRight);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::turningRight) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: FollowLineForward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineForward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     DBINFO1ln("Event: FollowLineForward");
//                     drivectrl->loop(DriveCtrl::Event::FollowLineForward);
//                     while ((drivectrl->getcurrentState() == DriveCtrl::State::followingLineForward) && (Serial.read() != 'E')) {
//                         drivectrl->loop();
//                     }
//                     break;
//                 default:
//                     DBINFO1ln("Error: Unknown value entered");
//                     break;
//             }
//             break;
//         case TestCase::NAVIGATIONCTRL:
//             DBSTATUSln("==Test Navigation CTRL==");
//             //read Serial in and generate events
//             Serial.println("Possible Events are:");
//             Serial.println("S - MoveToTargetPosition Sortic 1");
//             Serial.println("s - MoveToTargetPosition Sortic 3");
//             Serial.println("T - MoveToTargetPosition Transfer 1");
//             Serial.println("t - MoveToTargetPosition Transfer 3");
//             Serial.println("E - Error");
//             Serial.println("r - Resume");
//             Serial.println("N - No Event");
//             Serial.println("K - PosEndPointReached");
//             Serial.println("k - PosTransitReached");
//             Serial.println("p - Position Reached");
//             Serial.print("Choose Event: ");
//             while (Serial.available() <= 0) {
//             }
//             inByte = Serial.read();
//             Serial.print((char)inByte);
//             Serial.println();
//             firstcall = true;
//             switch (inByte) {
//                 case 'S':
//                     DBINFO1ln("Event: MoveToTargetPosition Sortic 1");
//                     navctrl->setTargetPosition(NavigationCtrl::Sector::SorticHandover, 1);
//                     navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
//                     while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
//                         navctrl->loop();
//                         // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
//                         //     firstcall = false;
//                         //     delay(3000);
//                         //     navctrl->giveToken();
//                         // }
//                     }
//                     break;
//                 case 's':
//                     DBINFO1ln("Event: MoveToTargetPosition Sortic 3");
//                     navctrl->setTargetPosition(NavigationCtrl::Sector::SorticHandover, 3);
//                     navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
//                     while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
//                         navctrl->loop();
//                         // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
//                         //     firstcall = false;
//                         //     delay(3000);
//                         //     navctrl->giveToken();
//                         // }
//                     }
//                     break;
//                 case 'T':
//                     DBINFO1ln("Event: MoveToTargetPosition Transfer 1");
//                     navctrl->setTargetPosition(NavigationCtrl::Sector::TransferHandover, 1);
//                     navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
//                     while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
//                         navctrl->loop();
//                         // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
//                         //     firstcall = false;
//                         //     delay(3000);
//                         //     navctrl->giveToken();
//                         // }
//                     }
//                     break;
//                 case 't':
//                     DBINFO1ln("Event: MoveToTargetPosition Transfer 3");
//                     navctrl->setTargetPosition(NavigationCtrl::Sector::TransferHandover, 3);
//                     navctrl->loop(NavigationCtrl::Event::MoveToTargetPosition);
//                     while ((navctrl->getcurrentState() != NavigationCtrl::State::endPoint) && (Serial.read() != 'E')) {
//                         navctrl->loop();
//                         // if (navctrl->getcurrentState() == NavigationCtrl::State::gateway && firstcall) {
//                         //     firstcall = false;
//                         //     delay(3000);
//                         //     navctrl->giveToken();
//                         // }
//                     }
//                     break;
//                 case 'E':
//                     DBINFO1ln("Event: Error");
//                     navctrl->loop(NavigationCtrl::Event::Error);
//                     break;
//                 case 'r':
//                     DBINFO1ln("Event: Resume");
//                     navctrl->loop(NavigationCtrl::Event::Resume);
//                     break;
//                 case 'N':
//                     DBINFO1ln("Event: No Event");
//                     navctrl->loop(NavigationCtrl::Event::NoEvent);
//                     break;
//                 case 'K':
//                     DBINFO1ln("Event: PosEndPointReached");
//                     navctrl->loop(NavigationCtrl::Event::PosEndPointReached);
//                     break;
//                 case 'k':
//                     DBINFO1ln("Event: PosTransitReached");
//                     navctrl->loop(NavigationCtrl::Event::PosTransitReached);
//                     break;
//                 case 'p':
//                     DBINFO1ln("Event: posPeached");
//                     navctrl->loop(NavigationCtrl::Event::PosReached);
//                     break;
//                 default:
//                     DBINFO1ln("Error: Unknown value entered");
//                     break;
//             }
//             break;
//         default:
//             break;
//     }
// }

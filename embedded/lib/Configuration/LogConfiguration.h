/**
 * @file LogConfiguration.h
 * @brief Contains Pre-Compiler directives for diffent Serialprints for Datalogin
 * 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 -  Implement diffrent debug functions - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef LOGCONFIGURATION_H
#define LOGCONFIGURATION_H

#define DEBUGGER true  ///< Option to activate the Serial.print global

#ifdef DEBUGGER
#define DEBUG_STATUS    ///< Define DEBUG_STATUS global to print all occuring status-updates via serial
#define DEBUG_ERROR     ///< Define DEBUG_ERROR global to print all  occuring errors via serial
#define DEBUG_WARNING   ///< Define DEBUG_WARNING global to print all occuring warnings via serial
#define DEBUG_FUNCCALL  ///< Define DEBUG_FUNCCALL global to print all occuring functioncalls via serial
#define DEBUG_INFO1     ///< Define DEBUG_INFO1 global to print all occuring infos via serial
#endif

#ifdef DEBUG_STATUS
#define DBSTATUS(x) Serial.print(x)
#define DBSTATUSln(x) Serial.println(x)
#else
#define DBSTATUS(x)
#define DBSTATUSln(x)
#endif

#ifdef DEBUG_ERROR
#define DBERROR(x)           \
    Serial.print("ERROR: "); \
    Serial.println(x);
#else
#define DBERROR(x)
#endif

#ifdef DEBUG_WARNING
#define DBWARNING(x) Serial.print(x)
#define DBWARNINGln(x) Serial.println(x)
#else
#define DBWARNING(x)
#define DBWARNINGln(x)
#endif

#ifdef DEBUG_FUNCCALL
#define DBFUNCCALL(x)    \
    if (Serial) {        \
        Serial.print(x); \
    }
#define DBFUNCCALLln(x)    \
    if (Serial) {          \
        Serial.println(x); \
    }
#else
#define DBFUNCCALL(x)
#define DBFUNCCALLln(x)
#endif

#ifdef DEBUG_INFO1
#define DBINFO1(x)       \
    if (Serial) {        \
        Serial.print(x); \
    };
#define DBINFO1ln(x)       \
    if (Serial) {          \
        Serial.println(x); \
    };
#else
#define DBINFO1(x)
#define DBINFOln(x)
#endif

#endif
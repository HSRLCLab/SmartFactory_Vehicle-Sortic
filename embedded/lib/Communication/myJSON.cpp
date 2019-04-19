/**
 * @file myJSON.cpp
 * @brief Implementation of the myJSON-class
 *
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.0 - added JSON-Conversion - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-17
 *
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 *
 */

#include "myJSON.h"

myJSON::myJSON() {
}

bool myJSON::StructIsEqual(const myJSONStr& lhs, const myJSONStr& rhs) {
    DBINFO1("sensor: ");
    DBINFO1(lhs.sensor);
    DBINFO1(" / ");
    DBINFO1ln(rhs.sensor);
    if (!(lhs.sensor == rhs.sensor)) {
        DBINFO1("sensor diffrent");
        return false;
    }

    DBINFO1("time: ");
    DBINFO1(lhs.time);
    DBINFO1(" / ");
    DBINFO1ln(rhs.time);
    if (!(lhs.time == rhs.time)) {
        DBINFO1ln("time diffrent");
        return false;
    }

    DBINFO1("data[0]: ");
    DBINFO1(lhs.data[0]);
    DBINFO1(" / ");
    DBINFO1ln(rhs.data[0]);
    if (!(lhs.data[0] == rhs.data[0])) {
        DBINFO1ln("data[0] diffrent");
        return false;
    }

    DBINFO1("data[1]: ");
    DBINFO1(lhs.data[1]);
    DBINFO1(" / ");
    DBINFO1ln(rhs.data[1]);
    if (!(lhs.data[1] == rhs.data[1])) {
        DBINFO1ln("data[1] diffrent");
        return false;
    }
    DBINFO1ln("==Structs are the same== ");
    return true;
}

myJSONStr myJSON::parsingJSONToStruct(const char* json) {
    DBFUNCCALLln("myJSON::parsingJSONToStruct(const char* json)");
    DBINFO1ln(json);
    myJSONStr tempStr;
    DynamicJsonDocument doc(pParsCapacity);

    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        DBWARNING("deserializeJson() failed: ");
        DBWARNINGln(error.c_str());
        tempStr.sensor = "Pars failed";
        return tempStr;
    } else {
        // DBINFO1("deserializeJson() success: ");
        /* use excplicit cast to prevent ambiguity
         * https://arduinojson.org/v6/doc/deserialization/ S67
         */
        tempStr.sensor = doc["sensor"].as<String>();   // "gps"
        tempStr.time = doc["time"].as<long>();         // 1351824120
        tempStr.data[0] = doc["data"][0].as<float>();  // 48.75608
        tempStr.data[1] = doc["data"][1].as<float>();  // 2.302038
        return tempStr;
    }
}

//Serializing program ===================================================
// String myJSON::serializ(myJSONStr tempStr) {
//     // DynamicJsonDocument doc(pSerializCapacity);

//     // doc["urgent"] = tempStr.urgent;
//     // doc["topic"] = tempStr.topic;
//     // doc["hostname"] = tempStr.hostname;
//     // doc["request"] = tempStr.request;
//     // doc["level"] = tempStr.level;

//     // JsonArray vehicleParams = doc.createNestedArray("vehicleParams");
//     // vehicleParams.add(tempStr.vehicleParams);
//     // // vehicleParams.add(tempStr.vehicleParams[0]);
//     // // vehicleParams.add(tempStr.vehicleParams[1]));
//     // // vehicleParams.add(tempStr.vehicleParams[2]));
//     // // vehicleParams.add(tempStr.vehicleParams[3]));
//     // // vehicleParams.add(tempStr.vehicleParams[4]));

//     // serializeJson(doc, Serial);
//     // // serializeJsonPretty(doc, Serial);
// }

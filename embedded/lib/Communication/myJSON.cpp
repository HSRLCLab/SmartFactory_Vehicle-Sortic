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

myJSONStr myJSON::parsingJSONToStruct(const char* json) {
    DBFUNCCALLln("myJSON::parsingJSONToStruct(const char* json)");
    DBINFO1ln(json);
    myJSONStr tempStr;
    DynamicJsonDocument doc(pParsCapacity);

    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        DBWARNING("deserializeJson() failed: ");
        DBWARNINGln(error.c_str());
        tempStr.sensor = "failed: ";
        return tempStr;
    } else {
        // DBINFO1("deserializeJson() success: ");
        tempStr.sensor = doc["sensor"];    // "gps"
        tempStr.time = doc["time"];        // 1351824120
        tempStr.data[0] = doc["data"][0];  // 48.75608
        tempStr.data[1] = doc["data"][1];  // 2.302038
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

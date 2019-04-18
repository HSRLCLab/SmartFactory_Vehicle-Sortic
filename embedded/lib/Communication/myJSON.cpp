/**
 * @file myJSON.cpp
 * @brief
 *
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.1 - Description - {author} - {date}
 *
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 *
 */

#include "myJSON.h"

myJSON::myJSON() {
}

//Parsing program ===================================================
myJSONStr myJSON::parsingJSONToStruct(const char* json) {
    DBFUNCCALLln("myJSON::parsingJSONToStruct(const char* json)");
    DBINFO1ln(json);
    // myJSONStr tempStr;

    // DynamicJsonDocument doc(pParsCapacity);

    // // const char* json =
    // //     "{  \"urgent\":     false,          \
    // //         \"topic\":      \"default\",    \
    // //         \"hostname\":   \"default\",    \
    // //         \"request\":    \"default\",    \
    // //         \"level\":      -5,             \
    // //         \"vehicleParams\":[ 48.7,       \
    // //                             2.3,        \
    // //                             2424.2,     \
    // //                             535.2,      \
    // //                             355.2]}";

    // tempStr.urgent = doc["urgent"];      // false
    // tempStr.topic = doc["topic"];        // "default"
    // tempStr.hostname = doc["hostname"];  // "default"
    // tempStr.request = doc["request"];    // "default"
    // tempStr.level = doc["level"];        // -5

    // tempStr.vehicleParams = doc["vehicleParams"];
    // // double vehicleParams_0 = vehicleParams[0];  // 48.7
    // // double vehicleParams_1 = vehicleParams[1];  // 2.3
    // // double vehicleParams_2 = vehicleParams[2];  // 2424.2
    // // double vehicleParams_3 = vehicleParams[3];  // 535.2
    // // double vehicleParams_4 = vehicleParams[4];  // 535.2
    // return tempStr;
    //======================================================================================
    // const size_t pParsCapacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + 30;
    myJSONStr tempStr;
    DynamicJsonDocument doc(pParsCapacity);

    // const char* json = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.75608,2.302038]}";

    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        DBINFO1("deserializeJson() failed: ");
        DBINFO1ln(error.c_str());
        tempStr.sensor = "failed: ";
        return tempStr;
    } else {
        // DBINFO1("deserializeJson() failed: ");
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

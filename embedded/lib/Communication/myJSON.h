/**
 * @file myJSON.h
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
#ifndef MYJSON_H__
#define MYJSON_H__

#include "Arduino.h"

#include "LogConfiguration.h"

#include <ArduinoJson.h>  //https://arduinojson.org/
#include "StackArray/StackArray.h"

/*
{
    "urgent":   false,
    "topic":    "default",
    "hostname": "default",
    "request":  "default",
    "level":    -5,
    "vehicleparams" :[0.1,1.1,2.2,3.3,4.4]
}
*/
// struct myJSONStr {
//     bool urgent = false;
//     String topic = "default";
//     String hostname = "default";
//     String request = "default";
//     int level = -5;  ///< describes Smart Box level states, -5 is default if not set!
//     /**
//      * @brief vehilce Params
//      *
//      * [0]: velocity v \n
//      * [1]: movingdirection vd \n
//      * [2]: distance d \n
//      * [3]: open tasks t \n
//      */
//     double vehicleParams[5];
// };

struct myJSONStr {
    const char* sensor = "default";
    long time;
    float data[2];
};

/**
 * @brief
 *
 */
class myJSON {  //}: public ArduinoJson {
   public:
    /**
     * @brief Construct a new myJSON object
     *
     */
    myJSON();

    myJSONStr parsingJSONToStruct(const char* json);

    String serialize(myJSONStr tempStr);

    static StackArray<myJSONStr> stack;

   private:
    const size_t pParsCapacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + 30;
    // const size_t pParsCapacity = JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(6) + 90;
    const size_t pSerializCapacity = JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(6);
};
#endif
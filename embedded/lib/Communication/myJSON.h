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

#include "CommunicationConfiguration.h"
#include "LogConfiguration.h"

#include <ArduinoJson.h>  //https://arduinojson.org/

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

   private:
    const size_t pParsCapacity = MAX_JSON_PARSE_SIZE;
};
#endif
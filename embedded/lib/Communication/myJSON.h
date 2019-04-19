/**
 * @file myJSON.h
 * @brief myJSON class handels the conversion from the JSON-Format into a struct
 * 
 *
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.0 - added JSON-Conversion - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-17
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
 * @brief myJSON class handels the conversion from the JSON-Format into a struct
 * 
 */
class myJSON {
   public:
    /**
     * @brief Construct a new myJSON object
     *
     */
    myJSON();

    /**
     * @brief Parsing JSON-Format into myJSONStr
     * 
     * Populates myJsonStr from a string. If an error occures, the default myJsonStr is returned.
     * If you change myJsonStr you also need to change this function.
     * 
     * @param json - pointer to a char-array in json-format
     * @return myJSONStr - Struct
     */
    myJSONStr parsingJSONToStruct(const char* json);

    // String serialize(myJSONStr tempStr);

   private:
    const size_t pParsCapacity = MAX_JSON_PARSE_SIZE;
};
#endif
#ifndef __JSON_HELPER_H__
#define __JSON_HELPER_H__

#include <json.hpp>

// for convenience
using json = nlohmann::json;

/**
* @brief utility function to quickly get a json element's data, used to check if correct def file
* @param def the json data to check for an element
* @param *element_name name of the element to look for
* @return json data of the element requested or NULL if the element doesn't exist in the data
*/
json get_element_data(json def, std::string element_name);

/**
* @brief loads json object from a given filepath
* @param *filename path to the file that will be opened
* @return json object or NULL if the file fails to open
*/
json load_from_def(std::string filename);

#endif

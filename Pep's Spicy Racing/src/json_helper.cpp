#include <fstream>
#include <simple_logger.h>

#include "json_helper.h"

/**
* @brief utility function to quickly get a json element's data, used to check if correct def file
* @param def the json data to check for an element
* @param *element_name name of the element to look for
* @return json data of the element requested or NULL if the element doesn't exist in the data
*/
json get_element_data(json def, char *element_name)
{
	if (def == NULL)
	{
		slog("json object is null");
		return NULL;
	}
	if (def.find(element_name) != def.end())
	{
		json element_data = def[element_name];
		return element_data;
	}
	else
	{
		slog("element cannot be found in the given def");
		return NULL;
	}
}

/**
* @brief loads json object from a given filepath
* @param *filename path to the file that will be opened
* @return json object or NULL if the file fails to open
*/
json load_from_def(char *filename)
{
	json def;
	char buf[512];
	std::ifstream file(filename);

	if (file.is_open())
	{
		file >> def;
	}
	else
	{
		slog("Failed to open %s", filename);
		return NULL;
	}

	return def;
}
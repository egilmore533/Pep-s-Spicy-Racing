#include <fstream>
#include <simple_logger.h>

#include "json_helper.h"

json get_element_data(json def, std::string element_name)
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

json load_from_def(std::string filename)
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
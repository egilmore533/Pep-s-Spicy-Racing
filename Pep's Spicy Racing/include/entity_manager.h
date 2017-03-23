#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#define MAX_ENTITIES	100

#include "entity.h"

class Entity_Manager
{
public:
	void create_entity(const char *json_filepath, glm::vec3 position);
	void delete_entity(int entity_id);

	void initialize();
	void close();
	void clear();

	void think_all();
	void update_all();
	void draw_all();
	void intersect_all();

private:
	std::array<class Entity*, MAX_ENTITIES> entity_list;
};

#endif

#include <string>
#include <simple_logger.h>

#include "json_helper.h"
#include "entity.h"
#include "graphics.h"

static Entity *entity_list = NULL;
static int entity_num = 0;
static int entity_max = 0;

/**
* @brief intializes the entity system to be used as a resource manager, sets all pointers to NULL
* @param num_entities the number of entities the game will allocated memory for
*/
void entity_initialize_system(int num_entities)
{
	int i;
	if (num_entities == 0)
	{
		slog("num entity == 0");
		return;
	}
	entity_list = (Entity *)malloc(sizeof(Entity) * num_entities);

	if (!entity_list)
	{
		slog("failed to initialize entity_list");
		return;
	}
	memset(entity_list, 0, sizeof(Entity) * entity_max);
	entity_num = 0;
	for (i = 0; i < num_entities; i++)
	{
		entity_list[i].in_use = false;
		entity_list[i].mesh = NULL;
	}
	entity_max = num_entities;
	atexit(entity_close_system);
}

/**
* @brief closes the entity system by freeing all entities in the entity_list and then resetting the entity_num and entity_max
*/
void entity_close_system()
{
	int i;
	Entity *entity;
	if (!entity_list)
	{
		slog("entity_list not initialized");
		return;
	}
	for (i = 0; i < entity_max; i++)
	{
		entity = &entity_list[i];
		entity_free(&entity);
	}
	entity_num = 0;
	free(entity_list);
	entity_list = NULL;
}

/**
* @brief frees the entity from the entity list (does not deallocate the memeory in the entity_list)
* @param entity	double pointer to the entity in the entity_list to be freed
*/
void entity_free(Entity **entity)
{
	Entity *self;
	if (!entity)
	{
		return;
	}
	else if (!*entity)
	{
		return;
	}
	self = *entity;
	self->in_use = false;

	if (self->mesh != NULL)
	{
		//free the mesh from the mesh_list (don't remove the mesh from memory let the mesh system handle that)
	}
	entity_num--;
	*entity = NULL;
}

/**
* @brief gets a pointer to the next free entity that we can use in the entity_list, and defines an entity using the given json file
* @param *json_file definition file for this entity to be created from
* @param default_shader_program	(temporary parameter) default shader program from the graphics class, only used if no shader defined
* @return a pointer to the next entity to be used
*/
Entity *entity_new(char *json_file, GLuint default_shader_program)
{
	int i;
	//makesure we have the room for a new entity
	if (entity_num + 1 > entity_max)
	{
		slog("Maximum Entity Reached.");
		exit(1);
	}
	for (i = 0; i < entity_max; i++)
	{
		if (entity_list[i].in_use)
		{
			continue;
		}

		//found the first entity in the list that is free to use, 
		// clear memory, fill out config data, increment entityNum, and return the new entity
		memset(&entity_list[i], 0, sizeof(Entity));
		entity_list[i].in_use = true;
		entity_num++;

		//create the entity using &entity_list[i]
		json def = load_from_def(json_file);
		json entity_def = get_element_data(def, "Entity");
		
		//we cannot count on this def file to contain the proper data
		if (entity_def == NULL)
		{
			return &entity_list[i];
		}

		std::string model_filepath = entity_def["model-filepath"];
		//since this json object we can count on being formated properly for an entity assume the data is there
		entity_list[i].mesh = new Mesh(model_filepath.c_str());
		entity_list[i].move_speed = (float)entity_def["move-speed"];
		entity_list[i].think_rate = (int)entity_def["think-rate"];

		if (entity_def["shader-program"] == "")
		{
			//for now if the shader-program isn't defined, use the standard one from the graphics class
			entity_list[i].shader_program = default_shader_program;
		}
		return &entity_list[i];
	}
	return NULL;
}

/**
* @brief draws every entity that is in use in the entity_list by running their draw function pointers
*/
void entitiy_draw_all()
{
	int i;
	Entity *cam = NULL; //get the camera
	for (i = 0; i < entity_max; i++)
	{
		if (!entity_list[i].in_use)
		{
			continue;
		}
		/*
		if (!entity_list[i].draw)
		{
			continue;
		}
		*/
		entity_list[i].draw();
	}
}


void Entity::draw()
{
	mesh->Mesh::draw(shader_program);
}

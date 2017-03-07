#include <simple_logger.h>

#include "entity.h"

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
* @brief gets a pointer to the next free entity that we can use in the entity_list
* @param think_rate the rate at which this new entity will think
* @param move_speed the movement speed of this new entity
* @return a pointer to the next entity to be used
*/
Entity *entity_new(int think_rate, float move_speed)
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
		entity_list[i].think_rate = think_rate;
		entity_list[i].move_speed = move_speed;
		entity_num++;

		return &entity_list[i];
	}
	return NULL;
}
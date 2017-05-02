#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "shader_manager.h"
#include "mesh_manager.h"
#include "texture_manager.h"
#include "camera.h"
#include "entity_manager.h"

static Entity_Manager *manager = NULL;

/**
* @brief initializes the entity manager by filling the entity_list with empty entities and setting the number of current entities to none
*/
void Entity_Manager::initialize()
{
	if (manager)
	{
		slog("manager currently exists");
		return;
	}

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		//memset
		Entity *ent = new Entity();
		this->entity_list[i] = ent;
	}
	num_entities = 0;
	manager = this;
}

/**
* @brief gets a pointer to the next free entity that we can use in the entity_list, and defines an entity using the given json file
* @param *entity_json_filepath definition file for this entity to be created from
* @param position  the position the entity is in the world space (model matrix)
* @return a pointer to the next entity to be used
*/
Entity *Entity_Manager::create_entity(std::string entity_json_filepath, glm::vec3 position)
{
	int i;

	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	//makesure we have the room for a new entity
	if (manager->num_entities + 1 > MAX_ENTITIES)
	{
		slog("Maximum Entity Reached.");
		exit(1);
	}
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		if (manager->entity_list[i]->in_use)
		{
			continue;
		}

		//create the entity using &entity_list[i]
		Entity *new_entity = manager->entity_list[i];

		new_entity->in_use = true;
		new_entity->id = i;
		new_entity->world_position = position;
		new_entity->draw = &default_draw;
		new_entity->update = &default_update;

		new_entity->model = glm::mat4(1.0f);
		new_entity->current_rotation = 0;
		glm::translate(new_entity->model, new_entity->world_position);
		manager->num_entities++;

		json def = load_from_def(entity_json_filepath);
		json entity_def = get_element_data(def, "Entity");

		//we cannot count on this def file to contain the proper data
		if (entity_def == NULL)
		{
			manager->num_entities++;
			manager->entity_list[i] = new_entity;
			return manager->entity_list[i];
		}

		//since this json object we can count on being formated properly for an entity assume the data is there
		std::string model_filepath = entity_def["model-filepath"];
		std::string texture_filepath = entity_def["texture-filepath"];
		new_entity->color_data = glm::vec4(entity_def["color"][0], entity_def["color"][1], entity_def["color"][2], entity_def["color"][3]);

		new_entity->texture = Texture_Manager::create_texture(texture_filepath.c_str(), true, true);
		new_entity->mesh = Mesh_Manager::create_mesh(model_filepath.c_str());

		new_entity->current_speed = (float)entity_def["move-speed"];
		new_entity->think_rate = (float)entity_def["think-rate"];
		new_entity->rotation_rate = (float)entity_def["rotation-speed"];

		std::string shader_filepath = entity_def["shader-program"];

		new_entity->shader = Shader_Manager::create_shader(shader_filepath);
		new_entity->model_location = glGetUniformLocation(new_entity->shader->program, "model");
		new_entity->color_location = glGetUniformLocation(new_entity->shader->program, "object_color");

		new_entity->view_location = glGetUniformLocation(new_entity->shader->program, "view");
		new_entity->projection_location = glGetUniformLocation(new_entity->shader->program, "projection");

		if (entity_def["shader-program"] != "json/shaders/light_shader.json")
		{
			new_entity->light_color_location = glGetUniformLocation(new_entity->shader->program, "light_color");
			new_entity->light_position_location = glGetUniformLocation(new_entity->shader->program, "light_position");
			new_entity->view_position_location = glGetUniformLocation(new_entity->shader->program, "view_position");
		}

		manager->entity_list[i] = new_entity;
		return manager->entity_list[i];
	}
	return NULL;
}

/**
* @brief deletes the given entity from the entity_list
* @param entity_id the id of the entity to delete
*/
void Entity_Manager::delete_entity(int entity_id)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (manager->entity_list[i]->in_use == false || manager->entity_list[i]->id != entity_id)
		{
			continue;
		}

		manager->entity_list[i]->in_use = false;
		manager->num_entities--;
		
		//any resources contained in the entity need to be dereferenced here
		Shader_Manager::dereference_shader(manager->entity_list[i]->shader->shader_def_file);
		Mesh_Manager::dereference_mesh(manager->entity_list[i]->mesh->filepath);
		Texture_Manager::dereference_texture(manager->entity_list[i]->texture->filepath);
		return;
	}
}

/**
* @brief empties the entity_list useful to load a new level
*/
void Entity_Manager::clear()
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (manager->entity_list[i]->in_use)
		{
			if (manager->entity_list[i]->in_use == false)
			{
				continue;
			}

			manager->entity_list[i]->in_use = false;
			manager->num_entities--;

			//any resources contained in the entity need to be dereferenced here
			if(manager->entity_list[i]->shader != NULL)
				Shader_Manager::dereference_shader(manager->entity_list[i]->shader->shader_def_file);

			if(manager->entity_list[i]->mesh != NULL)
				Mesh_Manager::dereference_mesh(manager->entity_list[i]->mesh->filepath);

			if(manager->entity_list[i]->texture != NULL)
				Texture_Manager::dereference_texture(manager->entity_list[i]->texture->filepath);

			if (manager->num_entities == 0)
			{
				return;
			}
		}
	}
}

/**
* @brief perform the think function of all the entities that are currently in use
*/
void Entity_Manager::think_all()
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}


	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!manager->entity_list[i]->in_use || !manager->entity_list[i]->think)
		{
			continue;
		}
		if (manager->entity_list[i]->next_think >= Graphics::get_delta_time().asSeconds())
		{
			manager->entity_list[i]->think(manager->entity_list[i]);
			manager->entity_list[i]->next_think = Graphics::get_delta_time().asSeconds() + manager->entity_list[i]->think_rate;
		}
	}
}

/**
* @brief perform the updates of all entities that are currently in use
*/
void Entity_Manager::update_all()
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!manager->entity_list[i]->in_use || !manager->entity_list[i]->update)
		{
			continue;
		}
		manager->entity_list[i]->update(manager->entity_list[i]);
	}
}

/**
* @brief draws all entities that are in use from the perspective of the given camera
* @param *camera the camera that will determine the perspective these entities will be drawn from
* @param *single_light (TODO make a light resource manager) temporary light in the world, there can only be one right now
*/
void Entity_Manager::draw_all(Camera *camera, Entity *single_light)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!manager->entity_list[i]->in_use || !manager->entity_list[i]->draw)
		{
			continue;
		}

		glUseProgram(manager->entity_list[i]->shader->program);
		glUniform4fv(manager->entity_list[i]->color_location, 1, &manager->entity_list[i]->color_data[0]);
		glUniformMatrix4fv(manager->entity_list[i]->model_location, 1, GL_FALSE, &manager->entity_list[i]->model[0][0]);

		glUniformMatrix4fv(manager->entity_list[i]->view_location, 1, GL_FALSE, &camera->Camera::get_view_matrix()[0][0]);
		glUniformMatrix4fv(manager->entity_list[i]->projection_location, 1, GL_FALSE, &camera->Camera::get_projection_matrix()[0][0]);

		if (manager->entity_list[i]->shader->shader_def_file != "json/shaders/light_shader.json")
		{
			glUniform4f(manager->entity_list[i]->light_color_location, single_light->color_data.x, single_light->color_data.y, single_light->color_data.z, single_light->color_data.w);
			glUniform3f(manager->entity_list[i]->light_position_location, single_light->world_position.x, single_light->world_position.y, single_light->world_position.z);
			glUniform3f(manager->entity_list[i]->view_position_location, camera->get_position().x, camera->get_position().y, camera->get_position().z);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, manager->entity_list[i]->texture->get_texture());
		glActiveTexture(GL_TEXTURE0);

		manager->entity_list[i]->draw(manager->entity_list[i]);
	}
}

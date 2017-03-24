#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "shader_manager.h"
#include "camera.h"
#include "entity_manager.h"

static Entity_Manager *manager = NULL;

/**
* @brief initializes the entity manager by filling the entity_list with empty entities and setting the number of current entities to none
*/
void Entity_Manager::initialize()
{
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
* @param *shader_json_filepath the filepath to the shader definition file
* @param default_shader_program	(temporary parameter) default shader program from the graphics class, only used if no shader defined
* @return a pointer to the next entity to be used
*/
Entity *Entity_Manager::create_entity(char *entity_json_filepath, glm::vec3 position)
{
	int i;
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

		Entity *new_entity = new Entity();

		new_entity->in_use = true;
		new_entity->id = i;
		new_entity->world_position = position;

		new_entity->model = glm::mat4(1.0f);
		new_entity->rotation_angle = 0;
		glm::translate(new_entity->model, new_entity->world_position);
		manager->num_entities++;

		//create the entity using &entity_list[i]
		json def = load_from_def(entity_json_filepath);
		json entity_def = get_element_data(def, "Entity");

		//we cannot count on this def file to contain the proper data
		if (entity_def == NULL)
		{
			manager->entity_list[i] = new_entity;
			return manager->entity_list[i];
		}

		//since this json object we can count on being formated properly for an entity assume the data is there
		std::string model_filepath = entity_def["model-filepath"];
		std::string texture_filepath = entity_def["texture-filepath"];
		new_entity->color_data = glm::vec4(entity_def["color"][0], entity_def["color"][1], entity_def["color"][2], entity_def["color"][3]);

		new_entity->mesh = new Mesh(model_filepath.c_str(), texture_filepath.c_str());
		new_entity->move_speed = (float)entity_def["move-speed"];
		new_entity->think_rate = (float)entity_def["think-rate"];
		new_entity->rotation_speed = (float)entity_def["rotation-speed"];

		std::string shader_filepath = entity_def["shader-program"];

		new_entity->shader = Shader_Manager::create_shader((char *)shader_filepath.c_str());
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
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (manager->entity_list[i]->id != entity_id)
		{
			continue;
		}

		manager->entity_list[i]->in_use = false;
		return;
	}
}

/**
* @brief empties the entity_list useful to load a new level
*/
void Entity_Manager::clear()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (this->entity_list[i]->in_use)
		{
			//TODO ASK BO IF THIS IS A BAD IDEA
			Entity *ent = new Entity();

			//specifically this, should I just set them all to false and let 
			//the create_entity replace the old automatically and would that 
			//make the old entity pointer go out of scope and get deleted or is 
			//that a memory leak or some shit, I NEED TO KNOW
			delete this->entity_list[i];

			this->entity_list[i] = ent;
			this->num_entities--;

			if (this->num_entities == 0)
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
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!manager->entity_list[i]->in_use)
		{
			continue;
		}
		if (manager->entity_list[i]->next_think >= Graphics::get_delta_time().asSeconds())
		{
			manager->entity_list[i]->think();
			manager->entity_list[i]->next_think = Graphics::get_delta_time().asSeconds() + manager->entity_list[i]->think_rate;
		}
	}
}

/**
* @brief perform the updates of all entities that are currently in use
*/
void Entity_Manager::update_all()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!manager->entity_list[i]->in_use)
		{
			continue;
		}
		manager->entity_list[i]->update();
	}
}

/**
* @brief draws all entities that are in use from the perspective of the given camera
* @param *camera the camera that will determine the perspective these entities will be drawn from
* @param *single_light (TODO make a light resource manager) temporary light in the world, there can only be one right now
*/
void Entity_Manager::draw_all(Camera *camera, Entity *single_light)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!manager->entity_list[i]->in_use)
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

		manager->entity_list[i]->draw();
	}
}

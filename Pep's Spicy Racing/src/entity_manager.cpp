#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "entity_manager.h"

void Entity_Manager::initialize()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		Entity *ent = new Entity();
		this->entity_list[i] = ent;
	}
	num_entities = 0;
}

Entity *Entity_Manager::create_entity(char *entity_json_filepath, glm::vec3 position, char *shader_json_filepath, GLuint temp_default_shader)
{
	int i;
	//makesure we have the room for a new entity
	if (num_entities + 1 > MAX_ENTITIES)
	{
		slog("Maximum Entity Reached.");
		exit(1);
	}
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		if (this->entity_list[i]->in_use)
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
		num_entities++;

		//create the entity using &entity_list[i]
		json def = load_from_def(entity_json_filepath);
		json entity_def = get_element_data(def, "Entity");

		//we cannot count on this def file to contain the proper data
		if (entity_def == NULL)
		{
			this->entity_list[i] = new_entity;
			return this->entity_list[i];
		}

		//since this json object we can count on being formated properly for an entity assume the data is there
		std::string model_filepath = entity_def["model-filepath"];
		std::string texture_filepath = entity_def["texture-filepath"];
		glm::vec4 color_data = glm::vec4(entity_def["color"][0], entity_def["color"][1], entity_def["color"][2], entity_def["color"][3]);

		new_entity->mesh = new Mesh(model_filepath.c_str(), texture_filepath.c_str(), color_data);
		new_entity->move_speed = (float)entity_def["move-speed"];
		new_entity->think_rate = (float)entity_def["think-rate"];
		new_entity->rotation_speed = (float)entity_def["rotation-speed"];

		//TODO implement a shader resource manager so this isn't so ugly
		if (entity_def["shader-program"] == "")
		{
			//for now if the shader-program isn't defined, use the standard one from the graphics class
			new_entity->shader_program = temp_default_shader;
			new_entity->model_location = glGetUniformLocation(new_entity->shader_program, "model");
			new_entity->color_location = glGetUniformLocation(new_entity->shader_program, "object_color");
		}
		this->entity_list[i] = new_entity;
		return this->entity_list[i];
	}
	return NULL;
}

void Entity_Manager::delete_entity(int entity_id)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (this->entity_list[i]->id != entity_id)
		{
			continue;
		}

		this->entity_list[i]->in_use = false;
		return;
	}
}

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

void Entity_Manager::think_all()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!this->entity_list[i]->in_use)
		{
			continue;
		}
		if (this->entity_list[i]->next_think >= Graphics::get_delta_time().asSeconds())
		{
			this->entity_list[i]->think();
			this->entity_list[i]->next_think = Graphics::get_delta_time().asSeconds() + this->entity_list[i]->think_rate;
		}
	}
}

void Entity_Manager::update_all()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!this->entity_list[i]->in_use)
		{
			continue;
		}
		this->entity_list[i]->update();
	}
}

void Entity_Manager::draw_all()
{
	int i;
	Entity *cam = NULL; //get the camera
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		if (!this->entity_list[i]->in_use)
		{
			continue;
		}
		this->entity_list[i]->draw();
	}
}

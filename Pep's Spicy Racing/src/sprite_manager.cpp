
#include <GL\glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <simple_logger.h>

#include "texture_manager.h"
#include "shader_manager.h"
#include "sprite_manager.h"

static Sprite_Manager *manager = NULL;

void Sprite_Manager::initialize()
{


	if (manager)
	{
		slog("manager currently exists");
		return;
	}

	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quad_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	for (int i = 0; i < MAX_SPRITES; i++)
	{
		//memset
		Sprite *sprite = new Sprite();
		this->sprite_list[i] = sprite;
	}
	num_sprites = 0;
	manager = this;
}

void Sprite_Manager::clear()
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_SPRITES; i++)
	{
		if (manager->sprite_list[i]->in_use)
		{
			if (manager->sprite_list[i]->in_use == false)
			{
				continue;
			}

			manager->sprite_list[i]->in_use = false;
			manager->num_sprites--;

			//any resources contained in the entity need to be dereferenced here
			if(manager->sprite_list[i]->shader != NULL)
				Shader_Manager::dereference_shader(manager->sprite_list[i]->shader->shader_def_file);

			if(manager->sprite_list[i]->texture != NULL)
				Texture_Manager::dereference_texture(manager->sprite_list[i]->texture->filepath);

			if (manager->num_sprites == 0)
			{
				return;
			}
		}
	}
}

void Sprite_Manager::draw(Camera *cam, int sprite_id)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	if (!manager->sprite_list[sprite_id]->in_use)
	{
		slog("sprite not in use");
		return;
	}
	
	glUseProgram(manager->sprite_list[sprite_id]->shader->program);
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(manager->sprite_list[sprite_id]->screen_position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finally translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * manager->sprite_list[sprite_id]->size.x, 0.5f * manager->sprite_list[sprite_id]->size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(manager->sprite_list[sprite_id]->rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * manager->sprite_list[sprite_id]->size.x, -0.5f * manager->sprite_list[sprite_id]->size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(manager->sprite_list[sprite_id]->size, 1.0f)); // Last scale

	glUniformMatrix4fv(manager->sprite_list[sprite_id]->model_location, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(manager->sprite_list[sprite_id]->projection_location, 1, GL_FALSE, &cam->Camera::get_ortho_projection_matrix()[0][0]);
	glUniform4f(manager->sprite_list[sprite_id]->sprite_color_location, manager->sprite_list[sprite_id]->color.x, manager->sprite_list[sprite_id]->color.y, manager->sprite_list[sprite_id]->color.z, manager->sprite_list[sprite_id]->color.w);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, manager->sprite_list[sprite_id]->texture->get_texture());
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(manager->quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Sprite *Sprite_Manager::create_sprite(std::string sprite_json_filepath)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	//makesure we have the room for a new sprite
	if (manager->num_sprites + 1 > MAX_SPRITES)
	{
		slog("Maximum Sprites Reached.");
		exit(1);
	}
	for (int i = 0; i < MAX_SPRITES; i++)
	{
		if (manager->sprite_list[i]->in_use)
		{
			continue;
		}

		//load json objects
		json def = load_from_def(sprite_json_filepath);
		json sprite_def = get_element_data(def, "Sprite");

		//we cannot count on this def file to contain the proper data
		//need to write it as == NULL because just using ! crashes the json object
		if (sprite_def == NULL)
		{
			slog("filepath given not a sprite defintion file");
			return NULL;
		}

		Sprite *new_sprite = manager->sprite_list[i];
		new_sprite->id = i;
		new_sprite->in_use = true;

		std::string texture_filepath = sprite_def["texture_filepath"];
		new_sprite->set_sprite_texture(texture_filepath);
		

		std::string shader_filepath = sprite_def["shader_def_filepath"];
		new_sprite->set_shader(shader_filepath);

		glm::vec2 position = glm::vec2(sprite_def["screen_position"][0], sprite_def["screen_position"][1]);
		glm::vec4 color_data = glm::vec4(sprite_def["color"][0], sprite_def["color"][1], sprite_def["color"][2], sprite_def["color"][3]);
		glm::vec2 size = glm::vec2(sprite_def["size"][0], sprite_def["size"][1]);
		float rotation = (float)sprite_def["rotation"];

		new_sprite->set_data(position, size, rotation, color_data);

		manager->num_sprites++;
		return manager->sprite_list[i];
	}
	return NULL;
}

void Sprite_Manager::delete_sprite(int sprite_id)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_SPRITES; i++)
	{
		if (manager->sprite_list[i]->in_use == false || manager->sprite_list[i]->id != sprite_id)
		{
			continue;
		}

		manager->sprite_list[i]->in_use = false;
		manager->num_sprites--;

		//any resources contained in the entity need to be dereferenced here
		Shader_Manager::dereference_shader(manager->sprite_list[i]->shader->shader_def_file);
		Texture_Manager::dereference_texture(manager->sprite_list[i]->texture->filepath);
		return;
	}
}

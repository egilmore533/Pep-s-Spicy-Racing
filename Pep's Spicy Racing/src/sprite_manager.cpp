
#include <GL\glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <simple_logger.h>

#include "texture_manager.h"
#include "shader_manager.h"
#include "sprite_manager.h"

static Sprite_Manager *manager = NULL;

/**
* @brief
*/
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


/**
* @brief performs the rendering of the given sprite
* @param *cam	pointer to the camera for using the projection
* @param the id of the sprite to draw
*/
void Sprite_Manager::draw(Camera *cam, int sprite_id)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_SPRITES; i++)
	{
		if (manager->sprite_list[i]->id == sprite_id)
		{
			glUseProgram(manager->sprite_list[i]->shader->program);
			glm::mat4 model;

			model = glm::translate(model, glm::vec3(manager->sprite_list[i]->screen_position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finally translation happens; reversed order)

			model = glm::translate(model, glm::vec3(0.5f * manager->sprite_list[i]->size.x, 0.5f * manager->sprite_list[i]->size.y, 0.0f)); // Move origin of rotation to center of quad
			model = glm::rotate(model, manager->sprite_list[i]->rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
			model = glm::translate(model, glm::vec3(-0.5f * manager->sprite_list[i]->size.x, -0.5f * manager->sprite_list[i]->size.y, 0.0f)); // Move origin back

			model = glm::scale(model, glm::vec3(manager->sprite_list[i]->size, 1.0f)); // Last scale

			glUniformMatrix4fv(manager->sprite_list[i]->model_location, 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(manager->sprite_list[i]->projection_location, 1, GL_FALSE, &cam->Camera::get_ortho_projection_matrix()[0][0]);
			glUniform4f(manager->sprite_list[i]->sprite_color_location, manager->sprite_list[i]->color.x, manager->sprite_list[i]->color.y, manager->sprite_list[i]->color.z, manager->sprite_list[i]->color.w);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, manager->sprite_list[i]->texture->get_texture());

			glBindVertexArray(manager->quad_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			break;
		}
	}
}

/**
* @brief gets a pointer to the next free sprite that we can use in the sprite_list, and defines an sprite using the given json file
* @param *sprite_json_filepath definition file for this entity to be created from
* @return a pointer to the next entity to be used
*/
Sprite *Sprite_Manager::create_sprite(char *sprite_json_filepath)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	//makesure we have the room for a new entity
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

		Sprite *new_sprite = manager->sprite_list[i];
		new_sprite->id = i;
		new_sprite->in_use = true;

		//load json objects
		json def = load_from_def(sprite_json_filepath);
		json sprite_def = get_element_data(def, "Sprite");

		//we cannot count on this def file to contain the proper data
		//need to write it as == NULL because just using ! crashes the json object
		if (sprite_def == NULL)
		{
			manager->num_sprites++;
			manager->sprite_list[i] = new_sprite;
			return manager->sprite_list[i];
		}

		std::string shader_filepath = sprite_def["shader_def_filepath"];
		std::string texture_filepath = sprite_def["texture_filepath"];

		new_sprite->set_sprite_texture(texture_filepath);
		new_sprite->set_shader(shader_filepath);

		glm::vec2 position = glm::vec2(sprite_def["screen_position"][0], sprite_def["screen_position"][1]);
		glm::vec4 color_data = glm::vec4(sprite_def["color"][0], sprite_def["color"][1], sprite_def["color"][2], sprite_def["color"][3]);
		glm::vec2 size = glm::vec2(sprite_def["size"][0], sprite_def["size"][1]);
		float rotation = (float)sprite_def["rotation"];

		new_sprite->set_data(position, size, rotation, color_data);

		manager->num_sprites++;
		manager->sprite_list[i] = new_sprite;
		return manager->sprite_list[i];
	}
	return NULL;
}

/**
* @brief deletes the given sprite from the sprite_list
* @param sprite_id the id of the sprite to delete
*/
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

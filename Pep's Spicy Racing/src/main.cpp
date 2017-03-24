#ifndef _DEBUG
#include <windows.hpp>		//TODO ASK BO WHY I CAN'T FIMD THIS FILE IN RELEASE MODE
#endif

#include <SFML\Graphics.hpp>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include <simple_logger.h>

#include "shader.h"
#include "shader_manager.h"
#include "graphics.h"
#include "camera.h"
#include "mesh.h"
#include "entity.h"
#include "entity_manager.h"

void initialize_systems();
void clear_systems();
void model_viewer_mode();


int main()
{
	model_viewer_mode();
	return 0;
}

void model_viewer_mode()
{
	int running = 1;

	init_logger("game_log.log");

	Shader_Manager shader_manager;
	shader_manager.initialize();

	Entity_Manager ent_manager;
	ent_manager.initialize();

	Graphics *graphics = new Graphics();

	Shader *toon_shader = shader_manager.create_shader("json/shaders/toon_shader.json");
	Shader *toon_shader2 = shader_manager.create_shader("json/shaders/toon_shader.json");
	Shader *light_shader = shader_manager.create_shader("json/shaders/light_shader.json");

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	GLuint  view_location, projection_location, light_color_location, light_posiiton_location, view_position_location;

	view_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "view");
	projection_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "projection");
	light_color_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "light_color");
	light_posiiton_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "light_position");
	view_position_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "view_position");

	//TODO Make a light class using implementing this code for lights
	//this will be our light temporarily, it will only be stationary we will move the monkey
	//Entity *test_cube = entity_new("json/entities/light-cube.json", glm::vec3(8, 1, -10), graphics->Graphics::get_shader_program());

	//test new entity system here
	Entity *test_cube = ent_manager.create_entity("json/entities/light-cube.json", glm::vec3(8, 1, -10), "", graphics->Graphics::get_shader_program());

	//this will be our "player" which we can move
	Entity *wood_monkey = ent_manager.create_entity("json/entities/wood-monkey.json", glm::vec3(0, 0, 0), "", graphics->Graphics::get_shader_program());

	while (running)
	{
		sf::Event event;
		while (graphics->Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				graphics->Graphics::get_game_window()->close();
				running = 0;
			}
		}
		camera->Camera::get_mouse_input();
		camera->Camera::get_keyboard_input();
		camera->Camera::update_view_matrix();

		graphics->Graphics::frame_begin();

		ent_manager.update_all();

		/*Drawing Code Start*/

		glUniformMatrix4fv(view_location, 1, GL_FALSE, &camera->Camera::get_view_matrix()[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &camera->Camera::get_projection_matrix()[0][0]);

		//TODO add this to the light code, for each draw call
		glUniform4f(light_color_location, 1.0f, 1.0f, 1.0f, 0.0f);
		glUniform3f(light_posiiton_location, test_cube->world_position.x, test_cube->world_position.y, test_cube->world_position.z);
		glUniform3f(view_position_location, camera->get_position().x, camera->get_position().y, camera->get_position().z);

		//entitiy_draw_all();
		ent_manager.draw_all();

		/*Drawing Code End*/

		graphics->Graphics::next_frame();
	}

	return;
}

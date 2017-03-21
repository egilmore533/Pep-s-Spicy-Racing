#ifndef _DEBUG
#include <windows.hpp>
#endif

#include <SFML\Graphics.hpp>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include <simple_logger.h>

#include "shader.h"
#include "graphics.h"
#include "camera.h"
#include "mesh.h"
#include "entity.h"

// Set up vertex data (and buffer(s)) and attribute pointers
GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

int main()
{
	int running = 1;
	
	init_logger("game_log.log");
	entity_initialize_system(10);

	Graphics *graphics = new Graphics;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	glm::mat4 model = glm::mat4(1.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	GLuint model_location, view_location, projection_location;
	glm::mat4 model_view_projection; // = projectionMatrix * view * model;

	model_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "model");
	view_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "view");
	projection_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "projection");
	
	Entity *test_cube = entity_new("json/entities/light-cube.json", glm::vec3(8, 1, -10), graphics->Graphics::get_shader_program());
	Entity *wood_monkey = entity_new("json/entities/wood-monkey.json", glm::vec3(0, 0, 0), graphics->Graphics::get_shader_program());

	/*
	//TODO Make a light class using implementing this code for lights
	GLuint light_shader = glCreateProgram();
	light_shader = build_shader_program("shader/vs1.glsl", "shader/light_fs.glsl");
	GLint light_color_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "light_color");
	
	Entity *light_entity = entity_new("json/entities/light-cube.json", glm::vec3(1.2f, 1.0f, 2.0f), graphics->Graphics::get_shader_program());
	light_entity->shader_program = light_shader;
	//END TODO
	*/
	while(running)
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

		entity_update_all();

		/*Drawing Code Start*/

		//model_view_projection = camera->Camera::get_projection_matrix() * camera->Camera::get_view_matrix() * model;
		
		//TODO move this uniform matrix stuff into the entity class 
		//glUniformMatrix4fv(model_location, 1, GL_FALSE, &wood_monkey->model[0][0]);
		glUniformMatrix4fv(view_location, 1, GL_FALSE, &camera->Camera::get_view_matrix()[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &camera->Camera::get_projection_matrix()[0][0]);

		//glUniform4f(light_color_location, 1.0f, 1.0f, 1.0f, 1.0f);

		entitiy_draw_all();

		/*Drawing Code End*/

		graphics->Graphics::next_frame();
	}

	return 0;
}

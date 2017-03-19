#ifndef _DEBUG
#include <windows.hpp>
#endif

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "camera.h"
#include "mesh.h"
#include "entity.h"

int main()
{
	int running = 1;
	
	init_logger("game_log.log");
	entity_initialize_system(10);

	Graphics *graphics = new Graphics;

	Mesh *mesh = new Mesh("models/cube.obj");

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	glm::mat4 model = glm::mat4(1.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	GLuint mvp_location;
	glm::mat4 model_view_projection; // = projectionMatrix * view * model;

	mvp_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "model_view_projection");
	//GLuint textureID = glGetUniformLocation(graphics->Graphics::get_shader_program(), "ourTexture");
	

	//Entity *my_entity = entity_new(100, 1.0f, graphics->Graphics::get_shader_program());

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

		/*Drawing Code Start*/

		model_view_projection = camera->Camera::get_projection_matrix() *
			camera->Camera::get_view_matrix() * 
			model;
		
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &model_view_projection[0][0]);

		//entitiy_draw_all();

		mesh->draw(graphics->Graphics::get_shader_program());

		/*Drawing Code End*/

		graphics->Graphics::next_frame();
	}

	return 0;
}

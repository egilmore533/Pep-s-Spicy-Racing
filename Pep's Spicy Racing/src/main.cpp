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

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	//TODO move all this stuff around so it isn't in the main function and is instead used in the entity or light stuff

	//this will be our light
	Entity *test_cube = ent_manager.create_entity("json/entities/light-cube.json", glm::vec3(8, 1, -10));

	//this will be our "player" which we can move
	Entity *wood_monkey = ent_manager.create_entity("json/entities/wood-monkey.json", glm::vec3(0, 0, 0));

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


		//TODO add camera to the draw all call that way we can pass this info for each draw all for each player :)
		
		//entitiy_draw_all();
		ent_manager.draw_all(camera, test_cube);

		/*Drawing Code End*/

		graphics->Graphics::next_frame();
	}

	return;
}

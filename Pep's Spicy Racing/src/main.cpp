#ifndef _DEBUG
#include <windows.hpp>		//TODO ASK BO WHY I CAN'T FIMD THIS FILE IN RELEASE MODE
#endif

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
#include "mesh_manager.h"
#include "texture_manager.h"
#include "sprite_manager.h"

void model_viewer_mode();


int main()
{
	init_logger("game_log.log");

	model_viewer_mode();
	return 0;

}

void model_viewer_mode()
{
	int running = 1;

	Graphics *graphics = new Graphics();

	Shader_Manager shader_manager;
	shader_manager.initialize();

	Mesh_Manager mesh_manager;
	mesh_manager.initialize();

	Texture_Manager texture_manager;
	texture_manager.initialize();

	Entity_Manager ent_manager;
	ent_manager.initialize();

	Sprite_Manager sprite_manager;
	sprite_manager.initialize();

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	//this will be our light
	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(8, 1, -10));

	//this will be our "player" which we can move
	Entity_Manager::create_entity("json/entities/wood-monkey.json", glm::vec3(0, 0, 0));

	Sprite *my_sprite = Sprite_Manager::create_sprite("json/sprites/joe_sprite.json");

	Sprite *my_sprite2 = Sprite_Manager::create_sprite("json/sprites/wood_sprite.json");

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

		Entity_Manager::update_all();

		/*Drawing Code Start*/

		Entity_Manager::draw_all(camera, test_cube);

		Sprite_Manager::draw(camera, my_sprite->id);

		Sprite_Manager::draw(camera, my_sprite2->id);

		/*Drawing Code End*/

		graphics->Graphics::next_frame();
	}

	return;
}

#ifndef _DEBUG
#include <windows.hpp>		//TODO ASK BO WHY I CAN'T FIMD THIS FILE IN RELEASE MODE
#endif

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include <simple_logger.h>

#include "shader_manager.h"
#include "graphics.h"
#include "camera.h"

#include "entity_manager.h"
#include "mesh_manager.h"
#include "texture_manager.h"
#include "sprite_manager.h"

#include "level_editor.h"

void singleplayer_mode();

void level_editor();


/*
?s

Why doesn't my entity system allow me to load entities after loading my sprites

Why doesn't my RenderTexture work with the sf::text stuff?
I cannot seem to render my text to this RenderTexture to use it.

Is this level editor a bad idea?
How else could I implement the stage itself?
How should I set up my level data for later use in the game, should i use the vertices 
of the track itself to determine where the ai and the  player are and keep track of the race?

*/

static bool pressed = false;

int main()
{
	int game_running = 1;

	init_logger("game_log.log");
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

	while (game_running)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if(!pressed)
				game_running = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			if (!pressed)
			{
				level_editor();
				sprite_manager.clear();
			}
				
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (!pressed)
			{
				singleplayer_mode();
				ent_manager.clear();
			}
		}
		else
		{
			pressed = false;
		}

		Graphics::frame_begin();

		Graphics::next_frame();
	}

	Graphics::get_game_window()->close();
	return 0;

}

void level_editor()
{
	int editor_running = 1;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	Level_Editor editor;

	editor.configure_editor(7, 10, glm::vec2(10, 20));

	//turn off depth test for UI/HUD elements
	glDisable(GL_DEPTH_TEST);

	//this should all be 2d so we dont need to enable this for sprites only and disable for 3d assets
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (editor_running)
	{
		sf::Event event;
		while (Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				editor_running = 0;
				pressed = true;
			}
		}
		
		camera->Camera::update_view_matrix();

		Graphics::frame_begin();

		editor.update_editor();

		//TODO ask bo/garry/google about alpha blending, am i thinking about this the wrong way or is there something else I should eb doing entirely?
		/*Drawing Code Start*/

		//because I don't understand alpha blending for now it doesn't matter so I'll just draw things in a weird order

		editor.draw_editor(camera);

		/*Drawing Code End*/

		Graphics::next_frame();
	}

	editor.save_and_exit("json/levels/created_level.json");

	return;
}

void singleplayer_mode()
{
	int singleplayer_running = 1;

	//start by loading up the level data
	json level_def = load_from_def("json/levels/created_level.json");
	json level = get_element_data(level_def, "Level");

	glm::vec2 start_position = glm::vec2(level["start_tile"][0], level["start_tile"][1]);
	int number_tiles = (int)level["number_tiles"];
	for (int i = 0; i < number_tiles; i++)
	{
		slog("tile #%d: %f %f", i+1, (float)level["tiles"][i][0], (float)level["tiles"][i][1]);
	}

	glm::vec3 cameraPosition = glm::vec3(start_position.x, 0.5f, start_position.y);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	//this will be our "player" which we can move
	Entity_Manager::create_entity("json/entities/wood-monkey.json", glm::vec3(0, 0, 0));

	//this will be our light
	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(8, 1, -10));

	Sprite *my_sprite = Sprite_Manager::create_sprite("json/sprites/joe_sprite.json");

	Sprite *my_sprite2 = Sprite_Manager::create_sprite("json/sprites/wood_sprite.json");

	//turn off depth test for UI/HUD elements
	glDisable(GL_DEPTH_TEST);

	//this should all be 2d so we dont need to enable this for sprites only and disable for 3d assets
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Sprite *my_text = Sprite_Manager::create_sprite("json/sprites/text_sprites/alphabet.json");


	while (singleplayer_running)
	{
		sf::Event event;
		while (Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				singleplayer_running = 0;
				pressed = true;
			}
		}
		camera->Camera::get_mouse_input();
		camera->Camera::get_keyboard_input();
		camera->Camera::update_view_matrix();

		Graphics::frame_begin();

		Entity_Manager::update_all();

		/*Drawing Code Start*/

		Entity_Manager::draw_all(camera, test_cube);

		//this should be enabled for 2d sprites to have their transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//this should be disabled for UI and HUD stuffs, but enabled for 3D
		glDisable(GL_DEPTH_TEST);

		Sprite_Manager::draw(camera, my_sprite->id);

		Sprite_Manager::draw(camera, my_sprite2->id);

		Sprite_Manager::draw(camera, my_text->id);

		//but this needs to be disabled after all sprites have been drawn so the 3d assets are drawn properly
		//opengl is hard :(
		glDisable(GL_BLEND);


		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/*Drawing Code End*/

		Graphics::next_frame();
	}

	return;
}

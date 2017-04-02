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
#include "stage.h"

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

	glm::vec4 menu_clear_color = glm::vec4(0.0f, 0.0f, 0.6f, 0.0f);

	Graphics::set_clear_color(menu_clear_color);

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
				Graphics::set_clear_color(menu_clear_color);
			}
				
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (!pressed)
			{
				singleplayer_mode();
				ent_manager.clear();
				//sprite_manager.clear(); 
				//so this the text only loads in the singleplayer mode if I have already run the singplayer mode
				//it also will not work if I load the singpleayer mode, then the editor, then the singplayer mode
				//it will however work if I run the editor, then the singpleayer mode several times, then I can switch between the two with no problems
				//alternatively If I disable the sprite list clear here I'll be able to switch between the two no problems, but I still have to reload the singleplayer mode atleast once
				
				//i will temporarily leave it without clearing the sprites just for testing code, but I need to solve this issue
				Graphics::set_clear_color(menu_clear_color);
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

	Graphics::set_clear_color(glm::vec4(0.0f, 0.0f, 0.6f, 1.0f));

	Level_Editor editor;

	editor.configure_editor(7, 10, glm::vec2(10, 20));

	//turn off depth test for UI/HUD elements
	glDisable(GL_DEPTH_TEST);

	//this should all be 2d so we dont need to switch in the loop
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

		/*Drawing Code Start*/

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
	Stage stage = Stage("json/levels/stage1.json");

	glm::vec3 cameraPosition = glm::vec3(-3, 20, 28);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	Graphics::set_clear_color(glm::vec4(stage.background_color.x, stage.background_color.y, stage.background_color.z, 1.0f));

	//this will be our "player" which we can move
	Entity_Manager::create_entity("json/entities/wood-monkey.json", glm::vec3(0, 0, 0));

	//this will be our light
	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(stage.start_position.x, stage.start_position.y + 4, stage.start_position.z));

	//test UI elements

	//this should be enabled for 2d sprites to have their transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//this should be disabled for UI and HUD stuffs, but enabled for 3D
	glDisable(GL_DEPTH_TEST);

	Sprite *my_sprite = Sprite_Manager::create_sprite("json/sprites/joe_sprite.json");
	Sprite *my_sprite2 = Sprite_Manager::create_sprite("json/sprites/wood_sprite.json");

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
		stage.draw_stage(camera, test_cube);

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

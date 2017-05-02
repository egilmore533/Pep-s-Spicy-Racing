#include <string>
#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "shader_manager.h"
#include "entity_manager.h"
#include "mesh_manager.h"
#include "texture_manager.h"
#include "sprite_manager.h"

#include "player.h"
#include "level_editor.h"
#include "stage.h"

#include "button.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>



void singleplayer_mode();

void level_editor();

int test_text();

Button *singleplayer_button;
Button *level_editor_button;

glm::vec4 menu_clear_color = glm::vec4(0.0f, 0.0f, 0.6f, 0.0f);

static bool pressed = false;

void reload_buttons()
{
	singleplayer_button = new Button("json/GUI/buttons/main_menu/singleplayer.json");
	singleplayer_button->callback = &singleplayer_mode;

	level_editor_button = new Button("json/GUI/buttons/main_menu/level_editor.json");
	level_editor_button->callback = &level_editor;
}

void clean_up_scene()
{
	Entity_Manager::clear();
	Sprite_Manager::clear();
	reload_buttons();
	Graphics::set_clear_color(menu_clear_color);
}

int main()
{
	int game_running = 1;

	int i;
	
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

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

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *cam = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	Graphics::set_clear_color(menu_clear_color);

	sf::Font hud_font;
	if (!hud_font.loadFromFile("fonts/Spicy.ttf"))
	{
		printf("dun goofed\n");
	}

	sf::Text menu_text;
	menu_text.setFont(hud_font);
	menu_text.setString("Pep's Spicy Racing");
	menu_text.setFillColor(sf::Color::Red);
	menu_text.setOutlineColor(sf::Color::Black);
	menu_text.setOutlineThickness(4.0f);
	menu_text.setCharacterSize(128);
	menu_text.setPosition((WINDOW_WIDTH / 2.0f) - (menu_text.getLocalBounds().width / 2.0f), 100);

	reload_buttons();

	while (game_running)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if(!pressed)
				game_running = 0;
		}
		else
		{
			pressed = false;
		}
		

		singleplayer_button->update();
		level_editor_button->update();

		Graphics::frame_begin();

		singleplayer_button->draw_background(cam);
		level_editor_button->draw_background(cam);

		Graphics::begin_draw_text();

		singleplayer_button->draw_text();
		level_editor_button->draw_text();


		Graphics::draw_text(menu_text);

		Graphics::end_draw_text();

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

	//this will be our light
	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(0, 0, 0));


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
		
		camera->Camera::view_matrix_look_forward();

		Graphics::frame_begin();

		editor.update_editor();

		/*Drawing Code Start*/

		editor.draw_editor(camera);

		/*Drawing Code End*/

		Graphics::next_frame();
	}

	editor.save_and_exit("json/levels/created_level.json");

	clean_up_scene();

	return;
}

void singleplayer_mode()
{
	int singleplayer_running = 1;

	Stage stage = Stage("json/levels/created_level.json");

	Graphics::set_clear_color(glm::vec4(stage.background_color.x, stage.background_color.y, stage.background_color.z, 1.0f));

	glm::vec3 cameraPosition = glm::vec3(-3, 20, 28);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	Player *player = new Player("json/racers/standard_racer.json", glm::vec3(stage.start_position.x, stage.start_position.y + 0.5f, stage.start_position.z));

	//this will be our light
	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(stage.start_position.x, stage.start_position.y + 4, stage.start_position.z));

	//test UI elements

	//this should be enabled for 2d sprites to have their transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//this should be disabled for UI and HUD stuffs, but enabled for 3D
	glDisable(GL_DEPTH_TEST);

	//TODO add to GUI all this junk
	Sprite *leader_1 = Sprite_Manager::create_sprite("json/GUI/sprites/joe_sprite.json");
	Sprite *item_backdrop = Sprite_Manager::create_sprite("json/GUI/sprites/wood_sprite.json");

	sf::Font hud_font;
	if (!hud_font.loadFromFile("fonts/Spicy.ttf"))
	{
		printf("dun goofed\n");
	}

	sf::Text mph;

	mph.setFont(hud_font);
	mph.setString("MPH");
	mph.setCharacterSize(32);
	mph.setFillColor(sf::Color::Red);
	mph.setOutlineColor(sf::Color::Black);
	mph.setOutlineThickness(2.0f);
	mph.setPosition(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 42);


	sf::Text speed;

	speed.setFont(hud_font);
	speed.setCharacterSize(64);
	speed.setFillColor(sf::Color::Red);
	speed.setOutlineColor(sf::Color::Black);
	speed.setOutlineThickness(4.0f);
	speed.setPosition(mph.getPosition().x + speed.getLocalBounds().width + 10.0f, WINDOW_HEIGHT - 74);

	//end TODO

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

		Entity_Manager::update_all();

		camera->Camera::follow_entity(player->entity_component);
		camera->Camera::view_matrix_look_at_target();

		Graphics::frame_begin();

		/*Drawing Code Start*/
		
		stage.draw_stage(camera, test_cube);
		//stage.draw_stage(player->player_cam, test_cube);

		Entity_Manager::draw_all(camera, test_cube);
		//Entity_Manager::draw_all(player->player_cam, test_cube);

		//this should be enabled for 2d sprites to have their transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//this should be disabled for UI and HUD stuffs, but enabled for 3D
		glDisable(GL_DEPTH_TEST);

		Sprite_Manager::draw(camera, leader_1->id);
		Sprite_Manager::draw(camera, item_backdrop->id);

		speed.setString(std::to_string((int)std::round(player->entity_component->current_speed / 10.0f)));
		speed.setPosition(mph.getPosition().x - speed.getLocalBounds().width - 10.0f, WINDOW_HEIGHT - 74);
		Graphics::begin_draw_text();
		Graphics::draw_text(speed); 
		Graphics::draw_text(mph);
		Graphics::end_draw_text();


		//but this needs to be disabled after all sprites have been drawn so the 3d assets are drawn properly
		//opengl is hard :(
		glDisable(GL_BLEND);


		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/*Drawing Code End*/

		Graphics::next_frame();
	}

	clean_up_scene();

	return;
}


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

#include "physics.h"

#include "ai_racer.h"
#include "player.h"
#include "level_editor.h"
#include "stage.h"

#include "button.h"

class BulletDebugDrawer_DeprecatedOpenGL : public btIDebugDraw {
public:
	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix) {
		glUseProgram(0); // Use Fixed-function pipeline (no shaders)
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&pViewMatrix[0][0]);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&pProjectionMatrix[0][0]);
	}
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		glColor3f(color.x(), color.y(), color.z());
		glBegin(GL_LINES);
		glVertex3f(from.x(), from.y(), from.z());
		glVertex3f(to.x(), to.y(), to.z());
		glEnd();
	}
	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
	virtual void reportErrorWarning(const char *) {}
	virtual void draw3dText(const btVector3 &, const char *) {}
	virtual void setDebugMode(int p) {
		m = p;
	}
	int getDebugMode(void) const { return 3; }
	int m;
};

void singleplayer_mode();

void level_editor();

Button *singleplayer_button;
Button *level_editor_button;

BulletDebugDrawer_DeprecatedOpenGL mydebugdrawer;

glm::vec4 menu_clear_color = glm::vec4(0.0f, 0.0f, 0.6f, 0.0f);

sf::Text fps;

static bool pressed = false;

void reload_buttons()
{
	singleplayer_button = new Button("json/GUI/buttons/main_menu/singleplayer.json");
	singleplayer_button->callback = &singleplayer_mode;

	level_editor_button = new Button("json/GUI/buttons/main_menu/level_editor.json");
	level_editor_button->callback = &level_editor;

	Graphics::set_clear_color(menu_clear_color);
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
	//opengl is hard :(
	int game_running = 1;

	int i;

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

	Physics physics;

	physics.world->setDebugDrawer(&mydebugdrawer);

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

	fps.setFont(hud_font);
	fps.setFillColor(sf::Color::Red);
	fps.setCharacterSize(24);
	fps.setPosition(10, 10);

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

		//fps.setString(std::to_string((int)1.0f / Graphics::get_delta_time().asSeconds()));

		singleplayer_button->update();
		level_editor_button->update();

		Graphics::frame_begin();

		singleplayer_button->draw_background(cam);
		level_editor_button->draw_background(cam);

		Graphics::begin_draw_text();

		singleplayer_button->draw_text();
		level_editor_button->draw_text();


		Graphics::draw_text(menu_text);
		Graphics::draw_text(fps);

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

	Level_Editor editor(7, 10, glm::vec2(10, 20));

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

	Entity_Manager::delete_entity(test_cube->id);

	return;
}

void singleplayer_mode()
{
	bool debug_rigid_bodies = false;
	int singleplayer_running = 1;

	Stage stage = Stage("json/levels/created_level.json");

	Graphics::set_clear_color(glm::vec4(stage.background_color.x, stage.background_color.y, stage.background_color.z, 1.0f));

	glm::vec3 cameraPosition = glm::vec3(0, 20,0);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	Player *player = new Player("json/racers/standard_racer.json", glm::vec3(stage.node_list[0].position.x, stage.node_list[0].position.y + 0.5, stage.node_list[0].position.z), stage.node_list[0].rotation);
	AI_Racer *ai_bot = new AI_Racer("json/racers/ai_racer.json", glm::vec3(stage.node_list[0].position.x + 9.0f, stage.node_list[0].position.y + 0.5, stage.node_list[0].position.z), stage.node_list[0].rotation);

	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(stage.start_position.x, stage.start_position.y + 4, stage.start_position.z));

	stage.add_racer(ai_bot);
	stage.add_racer(player);

	Racer *winner = NULL;
	sf::Text winner_text;

	//end TODO
	//just to reset the game delat timer

	Graphics::frame_begin();
	Graphics::next_frame();
	while (singleplayer_running && !winner)
	{
		bool debug_pressed = false;
		sf::Event event;

		if(Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				singleplayer_running = 0;
				pressed = true;
			}

			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
			//{
			//	//set path type to CenterOriented
			//	stage.path_type = CenterOriented;
			//}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
			//{
			//	//set path type to CutCorners
			//	stage.path_type = CutCorners;
			//}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
			//{
			//	//set path type to SmartTurns
			//	stage.path_type = SmartTurns;
			//}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			{
				if (!debug_pressed)
				{
					if (!debug_rigid_bodies)
						debug_rigid_bodies = true;
					else
						debug_rigid_bodies = false;

					debug_pressed = true;
				}
			}
			else
				debug_pressed = false;
		}

		/*camera->get_keyboard_input();
		camera->get_mouse_input();
		camera->view_matrix_look_forward();*/

		stage.update_stage();

		Entity_Manager::update_all();

		Physics::physics_step(Graphics::get_time().asMilliseconds());

		camera->Camera::follow_entity(player->entity_component);
		camera->Camera::view_matrix_look_at_target();

		winner = stage.get_winner();
		if (winner)
			break;

		Graphics::frame_begin();

		/*Drawing Code Start*/
		
		stage.draw_stage(camera, test_cube);

		Entity_Manager::draw_all(camera, test_cube);

		if (debug_rigid_bodies || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			mydebugdrawer.SetMatrices(camera->get_view_matrix(), camera->get_projection_matrix());
			Physics::world->debugDrawWorld();
		}

		//this should be enabled for 2d sprites to have their transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//this should be disabled for UI and HUD stuffs, but enabled for 3D
		glDisable(GL_DEPTH_TEST);

		player->draw_player_hud_sprites();
		
		Graphics::begin_draw_text();

		fps.setString(std::to_string(Graphics::get_delta_time().asMilliseconds()));
		Graphics::draw_text(fps);
		player->draw_player_hud_text();
		Graphics::end_draw_text();

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/*Drawing Code End*/

		Graphics::next_frame();

	}

	if (winner)
	{
		sf::Font hud_font;
		if (!hud_font.loadFromFile("fonts/Spicy.ttf"))
		{
			printf("dun goofed\n");
		}

		winner_text.setFont(hud_font);
		winner_text.setCharacterSize(128);
		winner_text.setFillColor(sf::Color::Red);
		winner_text.setOutlineColor(sf::Color::Black);
		winner_text.setOutlineThickness(6.0f);
		winner_text.setPosition((WINDOW_WIDTH / 2.0f) - (winner_text.getLocalBounds().width / 2.0f), (WINDOW_HEIGHT / 2.0f) - (winner_text.getLocalBounds().height / 2.0f));

		if (player->entity_component->id == winner->entity_component->id)
		{
			winner_text.setString("You Win!");
		}
		else
		{
			winner_text.setString("You Lose!");
		}

		singleplayer_running = 1;
		while (singleplayer_running)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				singleplayer_running = 0;
				pressed = true;
			}

			Graphics::frame_begin();

			//this should be enabled for 2d sprites to have their transparency
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//this should be disabled for UI and HUD stuffs, but enabled for 3D
			glDisable(GL_DEPTH_TEST);

			Graphics::begin_draw_text();
			Graphics::draw_text(winner_text);
			Graphics::end_draw_text();

			//but this needs to be disabled after all sprites have been drawn so the 3d assets are drawn properly
			//opengl is hard :(
			glDisable(GL_BLEND);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			Graphics::next_frame();
		}
	}

	Entity_Manager::delete_entity(test_cube->id);

	return;
}


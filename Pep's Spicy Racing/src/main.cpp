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

void singleplayer_mode();

void level_editor();


int main()
{
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

	level_editor();
	return 0;

}

void level_editor()
{
	int running = 1;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	Sprite *grid_tile_sprite = Sprite_Manager::create_sprite("json/sprites/grid_sprite.json");						//empty tile sprite
	Sprite *activated_tile_sprite = Sprite_Manager::create_sprite("json/sprites/activated_grid_sprite.json");		//tile that has been clicked on and added to the list of activated tiles (added to the stage)
	Sprite *highlighted_tile_sprite = Sprite_Manager::create_sprite("json/sprites/highlighted_grid_sprite.json");	//the tile that has the cursor over it

	//containers for our data
	sf::Vector2i mouse_position;				/**< stores the player's cursor position to determine which tile is highlighted and add that to the activated positions*/
	std::vector<glm::vec2> grid_positions;		/**< just standard grid used to determine where to place the tiles (highlighted, activated, empty) */
	std::vector<glm::vec2> activated_positions;	/**< contains all active positions */

	unsigned int max_grid_columns = 10;			/**< how many columns the grid contains total, used in creating standard grid, determining cursor position, and inserting activated tiles */
	unsigned int max_grid_rows = 7;				/**< how many rows the grid contains total, used in creating standard grid, determining cursor position, and inserting activated tiles */

	unsigned int x_offset = 10;					/**< x coordinate offset for the grid tiles (all tiles inserted, removed and highlighted should have this factored in) */
	unsigned int y_offset = 20;					/**< y coordinate offset for the grid tiles (all tiles inserted, removed and highlighted should have this factored in) */

	//lets fill the grid positions with the configuration we have
	for (int i = 0; i < max_grid_columns; i++)
	{
		for (int j = 0; j < max_grid_rows; j++)
		{
			grid_positions.push_back(glm::vec2((i * 100) + x_offset, j * 100 + y_offset));
		}

	}

	//this should all be 2d so we dont need to enable this for sprites only and disable for 3d assets
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (running)
	{
		sf::Event event;
		while (Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				Graphics::get_game_window()->close();
				running = 0;
			}
		}
		
		camera->Camera::update_view_matrix();

		mouse_position = sf::Mouse::getPosition(*Graphics::get_game_window());

		Graphics::frame_begin();

		//TODO ask bo/garry/google about alpha blending, am i thinking about this the wrong way or is there something else I should eb doing entirely?
		/*Drawing Code Start*/

		//because I don't understand alpha blending for now it doesn't matter so I'll just draw things in a weird order

		//draw highlighted tile
		for (int i = 0; i < max_grid_columns * max_grid_rows; i++)
		{
			if (mouse_position.x >= grid_positions[i].x && mouse_position.x <= grid_positions[i].x + 100)
			{
				if (mouse_position.y >= grid_positions[i].y && mouse_position.y <= grid_positions[i].y + 100)
				{
					highlighted_tile_sprite->screen_position = grid_positions[i];
					Sprite_Manager::draw(camera, highlighted_tile_sprite->id);

					//since we already have the tile here lets get input and add this tile to the activated tiles if the player left clicks
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						activated_positions.push_back(grid_positions[i]);
					}
				}
			}
		}

		//draw all activated tiles
		for (int i = 0; i < activated_positions.size(); i < i++)
		{
			activated_tile_sprite->screen_position = activated_positions[i];
			Sprite_Manager::draw(camera, activated_tile_sprite->id);
		}
		
		//draw all grid tiles
		for (int i = 0; i < max_grid_columns * max_grid_rows; i < i++)
		{
			grid_tile_sprite->screen_position = grid_positions[i];
			Sprite_Manager::draw(camera, grid_tile_sprite->id);
		}

		/*Drawing Code End*/

		Graphics::next_frame();
	}

	return;
}

void singleplayer_mode()
{
	int running = 1;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	//this will be our "player" which we can move
	Entity_Manager::create_entity("json/entities/wood-monkey.json", glm::vec3(0, 0, 0));

	Sprite *my_sprite = Sprite_Manager::create_sprite("json/sprites/joe_sprite.json");

	Sprite *my_sprite2 = Sprite_Manager::create_sprite("json/sprites/wood_sprite.json");

	Sprite *my_text = Sprite_Manager::create_sprite("json/sprites/text_sprites/alphabet.json");

	//this will be our light
	Entity *test_cube = Entity_Manager::create_entity("json/entities/light-cube.json", glm::vec3(8, 1, -10));


	while (running)
	{
		sf::Event event;
		while (Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				Graphics::get_game_window()->close();
				running = 0;
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

		Sprite_Manager::draw(camera, my_sprite->id);

		Sprite_Manager::draw(camera, my_sprite2->id);

		Sprite_Manager::draw(camera, my_text->id);

		//but this needs to be disabled after all sprites have been drawn so the 3d assets are drawn properly
		//opengl is hard :(
		glDisable(GL_BLEND);

		/*Drawing Code End*/

		Graphics::next_frame();
	}

	return;
}

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "sprite.h"
#include "camera.h"

class Level_Editor
{
public:

	/**
	* @brief configures the code for the editor, positioning the grid, and loading the sprites
	* @param rows		the number of rows the grid will have
	* @param columns	the number of columns in the grid
	* @param offsets	the offset of the tiles in the grid
	*/
	void configure_editor(unsigned int rows, unsigned int columns, glm::vec2 offsets);

	/**
	* @brief updates the data in the editor's activated positions and also determines whether to draw the highlighted tile or not (should only draw the highlighted sprite if the cursor is on the grid)
	*/
	void update_editor();

	/**
	* @brief draw the tile's onto the screen, then any other UI elements
	* @param *camera		the camera used by the sprite manager to draw the sprite
	*/
	void draw_editor(Camera *camera);

	/**
	* @param exit the editor and write a level definition file to the given filename
	* @param filename		the name of the file to be saved
	*/
	void save_and_exit(std::string filename);

private:
	Sprite *grid_tile_sprite;						/**< empty tile sprite */
	Sprite *activated_tile_sprite;					/**< tile that has been clicked on and added to the list of activated tiles (added to the stage) */
	Sprite *highlighted_tile_sprite;				/**< the tile that has the cursor over it */
	Sprite *start_tile;								/**< the start tile for the race */

	sf::Vector2i mouse_position;					/**< stores the player's cursor position to determine which tile is highlighted and add that to the activated positions*/
	std::vector<glm::vec2> grid_positions;			/**< just standard grid used to determine where to place the tiles (highlighted, activated, empty) */
	std::vector<glm::vec2> activated_positions;		/**< contains all active positions */

	unsigned int max_grid_columns = 10;			/**< how many columns the grid contains total, used in creating standard grid, determining cursor position, and inserting activated tiles */
	unsigned int max_grid_rows = 7;				/**< how many rows the grid contains total, used in creating standard grid, determining cursor position, and inserting activated tiles */

	glm::vec2 tile_offset;						/**< the x and y offset for the grid tiles (all tiles inserted, removed and highlighted should have this applied to them) */

	bool clicked;					/**< flag to help the update function, prevents from clicking every frame because thats not built into sfml */
	bool draw_highlighted_tile;		/**< flag to determine whether or not to draw the highlighted tile */
	bool start_tile_set;			/**< flag to determine if the start tile has been set */
};


#endif
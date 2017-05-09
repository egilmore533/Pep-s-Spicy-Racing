#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#include "sprite.h"
#include "camera.h"
#include "button.h"

class Level_Editor
{
public:

	/**
	* @brief configures the code for the editor, positioning the grid, and loading the sprites
	* @param rows		the number of rows the grid will have
	* @param columns	the number of columns in the grid
	* @param offsets	the offset of the tiles in the grid
	*/
	Level_Editor(unsigned int rows, unsigned int columns, glm::vec2 offsets);

	/**
	* @brief deletes the editors sprites
	*/
	~Level_Editor();

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

	/**
	* @brief adds a theme defintion file and sprite to the theme map and list
	* @param theme_filepath			the path to the defintion file for a theme
	* @param theme_sprite_filepath	the path to the definition file for a sprite that represents the theme
	*/
	void add_theme_to_list(std::string theme_filepath, std::string theme_sprite_filepath);

	/**
	* @brief adds a special sprite and defintion to the special map and list
	* @param special_def			the path to the defintion file for a special item
	* @param special_text			the text to display to represent a special item
	*/
	void add_special_to_list(std::string special_def, sf::Text special_text);

private:
	Sprite *grid_tile_sprite;						/**< empty tile sprite */
	Sprite *activated_tile_sprite;					/**< tile that has been clicked on and added to the list of activated tiles (added to the stage) */
	Sprite *highlighted_tile_sprite;				/**< the tile that has the cursor over it */
	Sprite *start_tile;								/**< the start tile for the race */
	Sprite *speed_pad;								/**< the speed pad sprite for the grid */
	Sprite *item_box;								/**< the item box sprite for the grid */

	sf::Vector2i mouse_position;					/**< stores the player's cursor position to determine which tile is highlighted and add that to the activated positions*/
	std::vector<glm::vec2> grid_positions;			/**< just standard grid used to determine where to place the tiles (highlighted, activated, empty) */
	std::vector<glm::vec2> activated_positions;		/**< contains all active positions */

	unsigned int max_grid_columns = 10;			/**< how many columns the grid contains total, used in creating standard grid, determining cursor position, and inserting activated tiles */
	unsigned int max_grid_rows = 7;				/**< how many rows the grid contains total, used in creating standard grid, determining cursor position, and inserting activated tiles */

	glm::vec2 tile_offset;						/**< the x and y offset for the grid tiles (all tiles inserted, removed and highlighted should have this applied to them) */

	bool clicked;					/**< flag to help the update function, prevents from clicking every frame because thats not built into sfml */
	bool draw_highlighted_tile;		/**< flag to determine whether or not to draw the highlighted tile */
	bool start_tile_set;			/**< flag to determine if the start tile has been set */

	std::map<std::string, Sprite *> theme_sprite_map;		/**< map of themes and their repsective sprites */
	std::vector<std::string> theme_key_list;				/**< list of the theme keys, used to index the theme sprite map */

	std::map<std::string, sf::Text> special_text_map;	/**< map of the special features and their respective text components */
	std::vector<std::string> special_list;				/**< list of all the speical features that can be added to the stage that */

	Button *theme_left_arrow_button;				/**< button that will cycle through to the previous theme */
	Button *theme_right_arrow_button;				/**< button that will cycle through to the next theme */

	Button *special_item_left_arrow;		/**< button that will cycle through to the last item */
	Button *special_item_right_arrow;		/**< button that will cycle through to the next item */

	sf::Font editor_font;		/**< the font all the GUI elements will use */
	sf::Text theme_text;		/**< the text above the theme settings */
	sf::Text special_text;		/**< the text above the special settings */

	sf::Text finish_line_text;		/**< text to display the finish line in the special itme in the special map */
	sf::Text speed_pad_text;		/**< text to display the speed pad in the special item in the special map */
	sf::Text item_box_text;			/**< text to display for the item box in the special items map */
};

/**
* @brief cycles backwards through the themes in the cpp file
*/
void theme_left_cycle();

/**
* @brief cycles forward through the themes in the cpp file
*/
void theme_right_cycle();

/**
* @brief cycles backward through the specials in the cpp file
*/
void special_right_cycle();

/**
* @brief cycles forward through the specials in the cpp file
*/
void special_left_cycle();

#endif

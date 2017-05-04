#ifndef __STAGE_H__
#define __STAGE_H__

#include "mesh_manager.h"
#include "shader_manager.h"
#include "texture_manager.h"
#include "camera.h"
#include "entity.h"

/**
* @struct defines a point on the track that will be used to determine a racer's progress through the map
*/
typedef struct
{
	glm::vec3 position;			/**< position of this node in world space */
	int node_num;				/**< this node's number in the node list, since I only load this once we don't have to worry about the number changing */
	bool finish_line_node;		/**< flag to know if this node is the finish line */
	float rotation;				/**< rotation of the node's plane to test position */
}Node;

/**
* @enum Cardinal Directions of the tiles, used to quickly and readibly determine which direction a tile is in
*/
enum CardinalDirection
{
	North = 1,
	South,
	East,
	West,
};

/**
* @enum	Type's of paths generated, used to know what type of path is in node_list already
*/
enum PathType
{
	CenterOriented = 1,
	CutCorners,
	SmartTurns,
	NoPath
};

class Stage {

public:
	/**
	* @brief loads a stage from the given defintion file
	* @param stage_def_filepath		filepath to a stage defintion file
	*/
	Stage(std::string stage_def_filepath);

	/**
	* @brief destructor for the stage that should delete the shaders, meshes and textures of the stage from their repsective managers 
	*/
	~Stage();

	/**
	* @brief loads the stage's theme into the stage's data
	* @param theme_filepath		filepath to the theme definition file
	*/
	void load_theme_data(std::string theme_filepath);

	/**
	* @brief loads the walls by determining if they are needed and positioning them around the stage
	*/
	void load_walls();

	/**
	* @brief gets an order for the track that will loop back to the start using neiboring tiles
	*		stores the track in tile_positions_in_order
	*/
	void get_track_in_order();

	/**
	* @brief draws the stage (should be one of if not the first thing drawn in the game loop)
	* @param camera the game camera
	* @param single_light	the single light in the game
	*/
	void draw_stage(Camera *camera, Entity *single_light);

	/**
	* @brief updates the stage's path, used to switch between the types of paths
	*/
	void update_path();

	/**
	* @brief sets the rotations of each node to face the next node in the list, in the case of the last node it faces the starting (finish) node
	*/
	void set_node_rotations();

	/**
	* @brief gets the next nodes in the path
	*			rigid and predictable, also not very optimized for speed
	*			does this by taking the current and next node to find the center point and the  point of the two tiles
	* @param current_tile		the current tile in the path we are working with
	* @param next_tile			the next_tile in the path
	* @param finish_line		is this current tile the finish line?
	*/
	void get_center_oriented_nodes(glm::vec3 current_tile, glm::vec3 next_tile, bool finish_tile);

	/**
	* @brief gets the next nodes in the path
	*			predictable, but rounded, not incredibly optimized or smooth but better
	*			does this by taking the current, and next tiles as well as the previous Node and finds the centroid of the next and previous node
	* @param current_tile		the current tile in the path we are working with
	* @param next_tile			the next tile in the path
	* @param previous_node		the previos node in the node list, used to find the centroid of it and the next Node
	* @param finish_tile		is this tile the finish tile?
	*/
	Node get_cut_corner_nodes(glm::vec3 current_tile, glm::vec3 next_tile, Node previous_node, bool finish_tile);

	/**
	* @brief uses get_center_oriented_nodes to populate node_list
	*/
	void get_center_oriented_path();

	/**
	* @brief populates the node_list using get_cut_corner_nodes
	*/
	void get_cut_corner_path();

	//std::string  name;
	//MusicPak *stage_music;

	Mesh *tile_mesh;						/**< mesh that this stage uses to create its tiles */
	glm::vec4 tile_color_data;
	Texture *tile_texture;					/**< the stage's texture for each tile */

	Mesh *wall_mesh;						/**< the mesh for the stage's walls */
	glm::vec4 wall_color_data;
	Texture *wall_texture;					/**< texture for the walls */

	Mesh *node_mesh;						/**< debug sphere to be drawn to see nodes in relation to the stage */

	glm::vec3 background_color;

	glm::vec3 tile_dimensions;					/**< the size of the tiles, used to draw, and determine node positions*/
	std::vector<glm::vec3> tile_positions;		/**< the positions of each tile in the stage */
	std::vector<glm::vec4> wall_position_data;	/**< locations of all the walls the stage will build, fourth value is the rotation of the wall */
	glm::vec3 start_position;					/**< the position of the starting tile */

	//shader uniform data 
	Shader *shader;				/**< the shader used to draw the stage */
	GLuint model_location;		/**< the location of the uniform model */
	GLuint color_location;		/**< location of the uniform color data */

	GLuint view_location;			/**< location of the view matrix uniform */
	GLuint projection_location;		/**< location of the projection matrix uniform */

	GLuint light_color_location;		/**< location of the light color uniform */
	GLuint light_position_location;		/**< location of the light position uniform */
	GLuint view_position_location;		/**< location of the camera position uniform */

	//PATHING DATA
	std::vector<glm::vec3> tile_positions_in_order;		/**< position data for the actual race track, these are the tiles in order that the players need to go through */
	PathType path_type;									/**< enumeration that determines what type of node list we have loaded here */
	std::vector<Node> node_list;						/**< list of all the nodes in the stage, in order, determines race position and will be used by the AI to determine how to move */

private:
	int node_id_num;				/**< current node id number, used to id the nodes, in order as they appear on the stage */
	PathType current_path_type;		/**< used to determine if the node list needs to be reloaded */
};

/**
* @brief finds the next neighboring tile position to the given tile posiiton, in the list given checking it against the list of already added tiles
* @param tile_positions				the list of all tiles in the stage
* @param already_added_positions	the list of tiles that have already been added to the order
* @param position					the position that we are checking for neigbors
* @param tile_dimensions			the x,y,z dimensions that we will be using to determine if tile's are touching (meaning they're neighbors)
*/
glm::vec3 find_next_neighbor_tile(std::vector<glm::vec3> tile_positions, std::vector<glm::vec3> already_added_positions, glm::vec3 position, glm::vec3 tile_dimensions);

/**
* @brief test to determine the direction of the next tile (cardinal directions only)
* @param current_tile		position of the tile we want to start from
* @param next_tile			posiiton of the tile we want to find a direction for
* @param dimensions			dimensions of the tiles
*/
CardinalDirection direction_to_next_tile(glm::vec3 current_tile, glm::vec3 next_tile, glm::vec3 dimensions);


#endif

#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>


#include "texture.h"

/**
 * @struct triangle_corner (vertex/point/corner), used to store the indexes for a vertex from an .obj file (one part of three for the face)
 */
typedef struct
{
	unsigned int v;		/**< vertex index for the corner */
	unsigned int vt;	/**< texel index for the corner */
	unsigned int vn;	/**< normal index for the corner */
}triangle_corner;

/**
* @struct vertex the actual data that each face's triangle_corner refers to
*/
typedef struct
{
	glm::vec3 position;	/**< vertex data for the corner */
	glm::vec3 normal;	/**< texel data for the corner */
	glm::vec2 texel;	/**< normal data for the corner */
}vertex;

/**
* @struct contains the data for a material, constructed when loading the mesh
*/
typedef struct
{
	glm::vec4 ambient_strength;
	glm::vec4 diffuse_strength;
	glm::vec4 specular_strength;
	int shininess;
}material;

class Mesh
{
public:
	/**
	 * @brief creates a mesh from a given .obj file, scans the file prelimnarily to allocate the correct storage for the mesh, then stores data
	 * @param filepath	the path to the .obj file from the working directory
	 */
	Mesh(std::string filename);

	/**
	* @brief creates a mesh to be used with all data set to default values
	*/
	Mesh();

	/**
	* @brief deconstructor for the mesh, destroys the data from the vertices, texels, normals, and triangles
	*/
	~Mesh();

	/**
	 * @brief draws the mesh
	 * @param shader_program	id of the shader program to use in drawing
	 */
	void draw(GLuint shader_program);

	/**
	 * @brief loads data from an obj file into this mesh, looking for vertices, uvs, normals, and faces
	 *			also loads indices and sets up the buffer_data for the mesh
	 * @param filename		the filepath to the obj file to load from
	 */
	void load_obj(std::string filename);

	/**
	 * @brief sets up the appropriate vertex array object and buffers for the mesh
	 */
	void setup_buffers();

/////////////////Data Structures////////////////////

public:
	//Mesh_Manager data
	std::string filepath;				/**< path to the mesh's file used with the texture_filepath to uniquely id each mesh */
	int reference_count;				/**< number of times this mesh is being used */

private:
	//data from the file, unchanged in order
	std::vector<glm::vec3> vertices;	/**< vertices from the obj file */
	std::vector<glm::vec2> uvs;			/**< uvs/texels from the obj file */
	std::vector<glm::vec3> normals;		/**< normals from the obj file */

	//indexing data and the final buffer to be sent to the graphics card
	std::vector<triangle_corner> unique_triplets;			/**< each unique vertex,uv,normal (v,vt,vn) indexes from one tri of the .obj face */
	std::vector<int> ind;									/**< the indexes of each unique */
	std::vector<vertex> buffer_data;						/**< the data being sent to the graphics card */

	Texture *myTexture;				/**< Mesh's texture(s) */

	//vertex buffer objects
	GLuint vao;		/**< vertex array object, stores the configurations of the buffer objects to allow rebinding of the same object without having to rebind and reconfigure the buffer */
	GLuint vbo;		/**< vertex buffer obejct, used to contain our vertices array in a buffer for OpenGL to use*/
	GLuint ebo;		/**< element buffer object, used to store what order we want to draw each unique vertex (index drawing) */
};

#endif

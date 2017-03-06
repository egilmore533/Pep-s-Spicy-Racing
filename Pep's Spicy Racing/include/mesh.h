#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

/**
 * @struct triangle_corner (vertex,point,corner), used to store data about a vertex from an .obj file
 */
typedef struct
{
	unsigned int v;		/**< vertex index for the corner */
	unsigned int vn;	/**< normal index for the corner */
	unsigned int vt;	/**< texel index for the corner */
}triangle_corner;

typedef struct
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texel;
}vertex;

/**
* @struct triangle, used to store data about a face/triangle from an .obj file, stores data from three points
*/
typedef struct
{
	triangle_corner corners[3];	/**< the three corners/vertices/points that this triangle is defined by */
}triangle;


class Mesh
{
public:
	/**
	 * @brief creates a mesh from a given .obj file, scans the file prelimnarily to allocate the correct storage for the mesh, then stores data
	 * @param	filepath	the path to the .obj file from the working directory
	 */
	Mesh(char *filename);

	/**
	* @brief deconstructor for the mesh, destroys the data from the vertices, texels, normals, and triangles
	*/
	~Mesh();

	/**
	 * @brief draws the mesh
	 * @param shader_program	id of the shader program to use in drawing
	 */
	void draw(GLuint shader_program);

	

private:
	/**
	 * @brief loads data from an obj file into this mesh, looking for vertices, uvs, normals, and faces
	 * @param filename		the filepath to the obj file to load from
	 */
	void load_obj(char *filename);

	/**
	 * @brief gets a vertex that is near if it already exists in the indexed data
	 * @param in_vertex		vertex to check if already indexed
	 * @param in_uv			uv to check if already indexed
	 * @param in_normal		normal to check if already indexed
	 * @return the index if found, else -1
	 */
	int Mesh::get_similar_vertex_index(glm::vec3 in_vertex, glm::vec2 in_uv, glm::vec3 in_normal);

	/**
	 * @brief index the vertices, uvs, and normals for use in a vertex buffer object
	 */
	void Mesh::index_data();

	/**
	 * @brief sets up the appropriate vertex array object and buffers for the mesh
	 */
	void setup_buffers();

	unsigned int get_num_indexed_vertices();

	unsigned int get_num_indexed_uvs();

	unsigned int get_num_indexed_normals();

	unsigned int get_num_indices();

	std::vector<glm::vec3> get_indexed_vertices();

	std::vector<glm::vec2> get_indexed_uvs();

	std::vector<glm::vec3> get_indexed_normals();

	std::vector<unsigned short> get_indices();

/////////////////Data Structures////////////////////
	//data from the file, unchanged
	std::vector<glm::vec3> vertices;	/**< vertices from the obj file */
	std::vector<glm::vec2> uvs;			/**< uvs from the obj file */
	std::vector<glm::vec3> normals;		/**< normals from the obj file */
	
	//data from the face section of the obj, unchanged
	std::vector<unsigned int> vertex_indices;	/**< unique identifier (not the line numbers) of the vertices from the obj */
	std::vector<unsigned int> uv_indices;		/**< unique identifier of the uvs from the obj */
	std::vector<unsigned int> normal_indices;	/**< unique identifier of the normals from the obj */

	//this is the indexed section, used to make drawing easier
	std::vector<unsigned short> indices;		/**< the unique indices for the mesh data */
	std::vector<glm::vec3> indexed_vertices;    /**< all the unique vertices in the order they are needed for referencing through indexed rendering */
	std::vector<glm::vec2> indexed_uvs;			/**< all the unique uvs in the order they are needed for referencing through indexed rendering */
	std::vector<glm::vec3> indexed_normals;		/**< all the unique normals in the order they are needed for referencing through indexed rendering */

	//vertex buffer objects
	GLuint vao;		/**< vertex array object, stores the configurations of the buffer objects to allow rebinding of the same object without having to rebind and reconfigure the buffer */
	GLuint vbo;		/**< vertex buffer obejct, used to contain our vertices array in a buffer for OpenGL to use*/
	GLuint ebo;		/**< element buffer object, used to store what order we want to draw each unique vertex (index drawing) */
};

/**
 * @brief utility function; 
 *		 determines if two floats are close enough together to be considered equal
 * @param v1	first float
 * @param v2	second float
 * @return bool true if near, else false
 */
bool is_near(float v1, float v2);

#endif

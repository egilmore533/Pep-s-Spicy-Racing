#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>

/**
 * @struct triangle_corner (vertex,point,corner), used to store data about a vertex from an .obj file
 */
typedef struct
{
	unsigned int v; /**< vertex index for the corner */
	unsigned int vn; /**< normal index for the corner */
	unsigned int vt; /**< texel index for the corner */
}triangle_corner;

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
	* @brief gets the number of vertices in this mesh, used for formatting the buffer in glBufferData
	* @return int	the num_vertices member from the Mesh class
	*/
	int get_num_vertices();

	/**
	* @brief gets the number of texels in this mesh, used for formatting the buffer in glBufferData
	* @return int	the num_texels member from the Mesh class
	*/
	int get_num_texels();

	/**
	* @brief gets the number of normals in this mesh, used for formatting the buffer in glBufferData
	* @return int	the num_normals member from the Mesh class
	*/
	int get_num_normals();

	/**
	* @brief gets the number of faces (tris/triangles) in this mesh, used for formatting the buffer in glBufferData
	* @return int	the num_faces member from the Mesh class
	*/
	int get_num_faces();

	/**
	* @brief gets the pointer to the vertex data
	* @return glm::vec3 pointer to the Mesh's vertices member
	*/
	glm::vec3 *get_vertices();

	/**
	* @brief gets the pointer to the texel data
	* @return glm::vec3 pointer to the Mesh's texels member
	*/
	glm::vec2 *get_texels();

	/**
	* @brief gets the pointer to the normals data
	* @return glm::vec3 pointer to the Mesh's normals member
	*/
	glm::vec3 *get_normals();

	/**
	* @brief gets the pointer to the triangle data
	* @return glm::vec3 pointer to the Mesh's tris member
	*/
	triangle *get_tris();

private:
	glm::vec3 *vertices;	/**< a pointer to all the vertices that this mesh has */
	glm::vec2 *texels;		/**< a pointer to all the texels that this mesh has */
	glm::vec3 *normals;		/**< a pointer to all the normals that this mesh has */
	triangle *tris;			/**< a pointer to all the triangles this mesh has */

	int  num_vertices = 0;	/**< number of vertices this mesh has allocated for and is storing */
	int  num_normals = 0;	/**< number of normals this mesh has allocated for and is storing */
	int  num_texels = 0;	/**< number of texels this mesh has allocated for and is storing */
	int  num_faces = 0;		/**< number of faces this mesh has allocated for and is storing */
};

#endif

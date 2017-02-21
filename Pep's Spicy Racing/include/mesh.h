#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>

typedef struct
{
	unsigned int v; //vertex index for the corner
	unsigned int vn; //normal index for the corner
	unsigned int vt; //texel index for the corner
}triangle_corner;

typedef struct
{
	triangle_corner corners[3];
}triangle;

class Mesh
{
public:
	Mesh(char *filename);
	~Mesh();

private:
	glm::vec3 *vertices;
	glm::vec2 *texels;
	glm::vec3 *normals;
	triangle *tris;

	int  num_vertices = 0;
	int  num_normals = 0;
	int  num_texcoords = 0;
	int  num_faces = 0;
};

#endif

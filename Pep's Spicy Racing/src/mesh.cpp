#include <stdlib.h>
#include <stdio.h>

#include <simple_logger.h>

#include "mesh.h"

Mesh::Mesh(char *filename)
{
	FILE *file;
	char buf[512];
	int v, t, n, f;

	if (!filename)
	{
		slog("no filename provided");
		return;
	}

	file = fopen(filename, "r");
	if (!file)
	{
		slog("failed to open file: %s", filename);
		return;
	}

	while (fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case 'v':
			switch (buf[1])
			{
			case '\0':
				fgets(buf, sizeof(buf), file);
				num_vertices++;
				break;
			case 'n':
				fgets(buf, sizeof(buf), file);
				num_normals++;
				break;
			case 't':
				fgets(buf, sizeof(buf), file);
				num_texels++;
				break;
			default:
				break;
			}
			break;
		case 'f':
			fgets(buf, sizeof(buf), file);
			num_faces++;
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	//allocate memory for the data structures we need
	tris = (triangle *)malloc(sizeof(triangle) * num_faces);
	normals = (glm::vec3 *)malloc(sizeof(glm::vec3) * num_normals);
	texels = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_texels);
	vertices = (glm::vec3 *)malloc(sizeof(glm::vec3) * num_vertices);

	rewind(file);
	v = t = n = f = 0;
	while (fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case 'v':
			switch (buf[1])
			{
			case '\0':
				fscanf(file, "%f %f %f", &vertices[v].x, &vertices[v].y, &vertices[v].z);
				v++;
				break;
			case 'n':
				fscanf(file, "%f %f %f", &normals[n].x, &normals[n].y, &normals[n].z);
				n++;
				break;
			case 't':
				fscanf(file, "%f %f", &texels[t].x, &texels[t].y);
				t++;
				break;
			default:
				break;
			}
			break;
		case 'f':
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&tris[f].corners[0].v, &tris[f].corners[0].vt, &tris[f].corners[0].vn,
				&tris[f].corners[1].v, &tris[f].corners[1].vt, &tris[f].corners[1].vn,
				&tris[f].corners[2].v, &tris[f].corners[2].vt, &tris[f].corners[2].vn
			);
			f++;
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}	

	if (file)
	{
		fclose(file);
	}
	
}


Mesh::~Mesh()
{
	if (vertices)
	{
		free(vertices);
	}
	if (texels)
	{
		free(texels);
	}
	if (normals)
	{
		free(normals);
	}
	if (tris)
	{
		free(tris);
	}
}

/**
* @brief gets the number of vertices in this mesh, used for formatting the buffer in glBufferData
*/
int Mesh::get_num_vertices()
{
	return num_vertices;
}

/**
* @brief gets the number of texels in this mesh, used for formatting the buffer in glBufferData
*/
int Mesh::get_num_texels()
{
	return num_texels;
}

/**
* @brief gets the number of normals in this mesh, used for formatting the buffer in glBufferData
*/
int Mesh::get_num_normals()
{
	return num_normals;
}

/**
* @brief gets the number of tris (triangles) in this mesh, used for formatting the buffer in glBufferData
*/
int Mesh::get_num_faces()
{
	return num_faces;
}

/**
* @brief gets the pointer to the vertex data
* @return glm::vec3 pointer to the Mesh's vertices member
*/
glm::vec3 *Mesh::get_vertices()
{
	return vertices;
}

/**
* @brief gets the pointer to the texel data
* @return glm::vec3 pointer to the Mesh's texels member
*/
glm::vec2 *Mesh::get_texels()
{
	return texels;
}

/**
* @brief gets the pointer to the normals data
* @return glm::vec3 pointer to the Mesh's normals member
*/
glm::vec3 *Mesh::get_normals()
{
	return normals;
}

/**
* @brief gets the pointer to the triangle data
* @return glm::vec3 pointer to the Mesh's tris member
*/
triangle *Mesh::get_tris()
{
	return tris;
}

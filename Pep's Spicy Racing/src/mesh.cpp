#include <stdlib.h>
#include <stdio.h>

#include <string>

#include <simple_logger.h>

#include "mesh.h"

Mesh::Mesh(char *filename)
{
	vertex_indices.reserve(1000);
	uv_indices.reserve(1000);
	normal_indices.reserve(1000);

	vertices.reserve(1000);
	uvs.reserve(1000);
	normals.reserve(1000);

	unique_triplets.reserve(3000);
	buffer_data.reserve(1000);

	this->load_obj(filename);
	this->index_data();
	this->setup_buffers();
}

/**
 * @brief loads data from an obj file into this mesh, looking for vertices, uvs, normals, and faces
 * @param filename		the filepath to the obj file to load from
 */
void Mesh::load_obj(char *filename)
{
	FILE *file;
	char buf[512];
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	temp_vertices.reserve(1000);
	temp_uvs.reserve(1000);
	temp_normals.reserve(1000);

	int current_index = 0;

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
			{
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
				break;
			}
			case 'n':
			{
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
				break;
			}
			case 't':
			{
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				uv.y = -uv.y;
				uvs.push_back(uv);
				break;
			}
			default:
				break;
			}
			break;
		case 'f':
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertex_index[3], uv_index[3], normal_index[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0], &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1], &vertex_index[2], &uv_index[2], &normal_index[2]);
			if (matches != 9) {
				slog("File can't be read by our simple parser Try exporting with other options\n");
				return;
			}
			
			for (unsigned int trip = 0; trip < 3; trip++)
			{
				bool unique_flag = true;
				int index;
				triangle_corner triplet;
				triplet.v = vertex_index[trip];
				triplet.vt = uv_index[trip];
				triplet.vn = normal_index[trip];
				for (unsigned int i = 0; i < unique_triplets.size(); i++)
				{
					if (unique_triplets[i].v == triplet.v &&
						unique_triplets[i].vt == triplet.vt &&
						unique_triplets[i].vn == triplet.vn)
					{
						unique_flag = false;
						index = i;
						break;
					}
				}
				if (unique_flag)
				{
					unique_triplets.push_back(triplet);
					ind.push_back(current_index++);

					vertex vertex_data;

					vertex_data.position = vertices[(triplet.v) - 1];
					vertex_data.texel = uvs[(triplet.vt) - 1];
					vertex_data.normal = normals[(triplet.vn) - 1];

					buffer_data.push_back(vertex_data);
				}
				else
				{
					ind.push_back(index);
					unique_flag = true;
				}
			}

			vertex_indices.push_back(vertex_index[0]);
			vertex_indices.push_back(vertex_index[1]);
			vertex_indices.push_back(vertex_index[2]);
			uv_indices.push_back(uv_index[0]);
			uv_indices.push_back(uv_index[1]);
			uv_indices.push_back(uv_index[2]);
			normal_indices.push_back(normal_index[0]);
			normal_indices.push_back(normal_index[1]);
			normal_indices.push_back(normal_index[2]);
			break;
		}

		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	if (file)
	{
		fclose(file);
	}

	/*
	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertex_indices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertex_indices[i];
		unsigned int uvIndex = uv_indices[i];
		unsigned int normalIndex = normal_indices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in the actual buffers
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
	}
	*/
}

/**
 * @brief gets a vertex that is near if it already exists in the indexed data
 * @param in_vertex		vertex to check if already indexed
 * @param in_uv			uv to check if already indexed
 * @param in_normal		normal to check if already indexed
 * @return int the index if found, else -1
 */
int Mesh::get_similar_vertex_index(glm::vec3 in_vertex, glm::vec2 in_uv, glm::vec3 in_normal)
{
	// Lame linear search
	for (int i = 0; i<indexed_vertices.size(); i++) 
	{
		if (
			//similar means same position, same UV, and same normal
			is_near(in_vertex.x, indexed_vertices[i].x) &&
			is_near(in_vertex.y, indexed_vertices[i].y) &&
			is_near(in_vertex.z, indexed_vertices[i].z) &&
			is_near(in_uv.x, indexed_uvs[i].x) &&
			is_near(in_uv.y, indexed_uvs[i].y) &&
			is_near(in_normal.x, indexed_normals[i].x) &&
			is_near(in_normal.y, indexed_normals[i].y) &&
			is_near(in_normal.z, indexed_normals[i].z)
			) 
		{
			return i;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return -1;
}

/**
 * @brief index the vertices, uvs, and normals for use in a vertex buffer object
 */
void Mesh::index_data()
{
	
}

/**
* @brief sets up the appropriate vertex array object and buffers for the mesh
*/
void Mesh::setup_buffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// ..:: Initialization code :: ..
	// 1. Bind Vertex Array Object
	glBindVertexArray(vao);
		// 2. Copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer_data.size() * sizeof(vertex), &buffer_data[0], GL_STATIC_DRAW);
		// 3. Copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
		// 3. Then set the vertex attributes pointers
		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
	// 4. Unbind VAO (NOT the EBO)
	glBindVertexArray(0);


	/*
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, get_num_indexed_uvs() * sizeof(glm::vec2), &get_indexed_uvs()[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, get_num_indexed_normals() * sizeof(glm::vec3), &get_indexed_normals()[0], GL_STATIC_DRAW);

	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_num_indices() * sizeof(unsigned short), &get_indices()[0], GL_STATIC_DRAW);
	*/
}

/**
* @brief draws the mesh
* @param shader_program	id of the shader program to use in drawing
*/
void Mesh::draw(GLuint shader_program)
{
	glUseProgram(shader_program);

	// Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	/*
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
	
	/*
	//for (int i = 0; i < vertices.size(); i += 3)
	//	glDrawArrays(GL_LINE_LOOP, i, 3);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	

	glDisableVertexAttribArray(0);
	*/
}


Mesh::~Mesh()
{
	
}

/**
* @brief gets the number of indexed vertices for use in drawing
* @return unsigned int the size of the indexed_vertices
*/
unsigned int Mesh::get_num_indexed_vertices()
{
	return indexed_vertices.size();
}

/**
* @brief gets the number of indexed uvs for use in drawing
* @return unsigned int the size of the indexed_uvs
*/
unsigned int Mesh::get_num_indexed_uvs()
{
	return indexed_uvs.size();
}

/**
* @brief gets the number of indexed normals for use in drawing
* @return unsigned int the size of the indexed_normals
*/
unsigned int Mesh::get_num_indexed_normals()
{
	return indexed_normals.size();
}

/**
* @brief gets the number of indices for use in drawing
* @return unsigned int the size of the indices
*/
unsigned int Mesh::get_num_indices()
{
	return indices.size();
}

/**
* @brief gets the indexed vertices for use in drawing
* @return std::vector<glm::vec3> the indexed_vertices
*/
std::vector<glm::vec3> Mesh::get_indexed_vertices()
{
	return indexed_vertices;
}

/**
* @brief gets the indexed uvs for use in drawing
* @return std::vector<glm::vec2> the indexed_uvs
*/
std::vector<glm::vec2> Mesh::get_indexed_uvs()
{
	return indexed_uvs;
}

/**
* @brief gets the indexed normals for use in drawing
* @return std::vector<glm::vec3> the indexed_normals 
*/
std::vector<glm::vec3> Mesh::get_indexed_normals()
{
	return indexed_normals;
}

/**
 * @brief gets the indices for use in drawing
 * @return std::vector<unsigned short> the indices
 */
std::vector<unsigned short> Mesh::get_indices()
{
	return indices;
}

/**
* @brief determines if two floats are close enough together to be considered equal
* @param v1	first float
* @param v2	second float
* @return bool true if near, else false
*/
bool is_near(float v1, float v2) {
	return fabs(v1 - v2) < 0.01f;
}

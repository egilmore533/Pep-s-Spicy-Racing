#include <stdlib.h>
#include <stdio.h>

#include <string>

#include <simple_logger.h>

#include "graphics.h"
#include "mesh.h"

Mesh::Mesh(std::string filename)
{
	vertices.reserve(1000);
	uvs.reserve(1000);
	normals.reserve(1000);

	unique_triplets.reserve(3000);
	ind.reserve(1000);
	buffer_data.reserve(1000);

	load_obj(filename);
	setup_buffers();
}

Mesh::Mesh()
{
	filepath = "";
	reference_count = 0;

	vertices.reserve(1000);
	uvs.reserve(1000);
	normals.reserve(1000);

	unique_triplets.reserve(3000);
	ind.reserve(1000);
	buffer_data.reserve(1000);

	myTexture = NULL;
	vao = 0;
	vbo = 0;
	ebo = 0;
}

void Mesh::load_obj(std::string filename)
{
	FILE *file;
	char buf[512];

	int current_index = 0;

	file = fopen(filename.c_str(), "r");
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
}

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
		// UV/Tex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, texel));
		// Normal attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
	// 4. Unbind VAO
	glBindVertexArray(0);

	//don't forget to unbind the array buffer and the element array buffer you silly goose
	//oh well only wasted an eternity figuring that one out
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(GLuint shader_program)
{
	glUseProgram(shader_program);
	// Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	
}

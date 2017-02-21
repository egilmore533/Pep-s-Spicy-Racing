#include <stdlib.h>
#include <stdio.h>

#include <simple_logger.h>

#include "mesh.h"

Mesh::Mesh(char *filename)
{
	FILE *file;
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


	char buf[256];

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
				num_texcoords++;
				break;
			default:
				break;
			}
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
	texels = (glm::vec2 *)malloc(sizeof(glm::vec2) * num_texcoords);
	vertices = (glm::vec3 *)malloc(sizeof(glm::vec3) * num_vertices);

	rewind(file);

	while (fscanf(file, "%s", buf) != EOF)
	{

	}

	fclose(file);
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

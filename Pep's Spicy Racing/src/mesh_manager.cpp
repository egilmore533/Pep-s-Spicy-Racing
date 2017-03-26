#include <simple_logger.h>

#include "mesh_manager.h"

static Mesh_Manager *manager = NULL;

/**
* @brief check mesh_list to see if the mesh has already been loaded if so
*		increment the reference count and return that mesh else build the new
*		mesh in the first available mesh position, also check if room
*		available in the mesh_list, if not exit program
* @param mesh_filepath path to the mesh obj file
* @param texture_filepath path to the texture file
* @return a pointer to the mesh that is defined by the given files
*/
Mesh *Mesh_Manager::create_mesh(std::string mesh_filepath, std::string texture_filepath)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	int first_empty = -1;
	for (int i = 0; i < MAX_MESHES; i++)
	{
		//if both the mesh file and the texture file match then just reuse this mesh 
		//(TODO move texture into entity and make a resource manager for that to allow me to reuse mesh's without having to have matching textures)
		if (manager->mesh_list[i]->filepath == mesh_filepath && manager->mesh_list[i]->texture_filepath == texture_filepath)
		{
			if (manager->mesh_list[i]->reference_count == 0)
			{
				manager->num_meshes++;
			}
			manager->mesh_list[i]->reference_count++;
			return manager->mesh_list[i];
		}

		//while searching for this mesh in the list, save the position of the first unused shader in the list
		if (first_empty == -1 && manager->mesh_list[i]->reference_count == 0)
		{
			first_empty = i;
		}
	}

	//now it is necessary to make a new mesh, so check if there is room for another
	if (manager->num_meshes + 1 > MAX_MESHES)
	{
		slog("Maximum Meshes Reached.");
		exit(1);
	}
	if (first_empty == -1)
	{
		slog("error: couldn't find an empty slot and there is apparently still space");
		return NULL;
	}

	Mesh *new_mesh = new Mesh(mesh_filepath.c_str(), texture_filepath.c_str());

	//save filepaths here because const char are used in the mesh constructors and we want strings
	new_mesh->filepath = mesh_filepath;
	new_mesh->texture_filepath = texture_filepath;

	new_mesh->reference_count = 1;					//referenced once
	manager->mesh_list[first_empty] = new_mesh;		//make this new mesh the mesh at this position
	manager->num_meshes++;							//increment the number of total meshes
	return manager->mesh_list[first_empty];			//return the mesh at this position in the mesh_list
}

/**
* @brief decrement the reference count of the given mesh, enabling it
*		(if the reference count has reached 0) to be replaced by the creation
*		of another mesh
* @param mesh_filepath the file of the mesh to be dereferenced
* @param texture_filepath the texture file of the mesh to be dereferenced
*/
void Mesh_Manager::dereference_mesh(std::string mesh_filepath, std::string texture_filepath)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_MESHES; i++)
	{
		if (manager->mesh_list[i]->filepath == mesh_filepath && manager->mesh_list[i]->texture_filepath == texture_filepath)
		{
			manager->mesh_list[i]->reference_count--;
			if (manager->mesh_list[i]->reference_count == 0)
			{
				manager->num_meshes--;
			}
		}
	}
}

/**
* @brief allocates and sets default data for the meshes in the mesh_manager's list
*			then sets the mesh manager to be the global mesh manager
*/
void Mesh_Manager::initialize()
{
	if (manager)
	{
		slog("manager already exists");
		return;
	}

	//cannot use this->mesh_list = {0}; because we need all the data to be initialized so we can check

	//these don't compile
	//std::fill(this->mesh_list, this->mesh_list + sizeof(this->mesh_list), 0);
	//memset(this->mesh_list, 0, sizeof(std::array<Mesh *, MAX_MESHES>));

	for (int i = 0; i < MAX_MESHES; i++)
	{
		Mesh *mesh = new Mesh();
		this->mesh_list[i] = mesh;
	}

	this->num_meshes = 0;
	manager = this;
}

/**
* @brief clears the mesh_list by setting the reference_count to 0 on each mesh
*/
void Mesh_Manager::clear()
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_MESHES; i++)
	{
		manager->mesh_list[i]->reference_count = 0;
	}
	num_meshes = 0;
}

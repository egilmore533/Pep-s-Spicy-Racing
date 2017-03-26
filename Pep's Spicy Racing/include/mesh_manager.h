#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__

#include <array>
#include "mesh.h"

#define MAX_MESHES		1000			

class Mesh_Manager
{
public:

	/**
	* @brief check mesh_list to see if the mesh has already been loaded if so 
	*		increment the reference count and return that mesh else build the new 
	*		mesh in the first available mesh position, also check if room 
	*		available in the mesh_list, if not exit program
	* @param mesh_filepath path to the mesh obj file
	* @param texture_filepath path to the texture file
	* @return a pointer to the mesh that is defined by the given files
	*/
	static Mesh *create_mesh(std::string mesh_filepath, std::string texture_filepath);

	/**
	* @brief decrement the reference count of the given mesh, enabling it 
	*		(if the reference count has reached 0) to be replaced by the creation 
	*		of another mesh
	* @param mesh_filepath the file of the mesh to be dereferenced
	* @param texture_filepath the texture file of the mesh to be dereferenced
	*/
	static void dereference_mesh(std::string mesh_filepath, std::string texture_filepath);

	/**
	* @brief allocates and sets default data for the meshes in the mesh_manager's list 
	*			then sets the mesh manager to be the global mesh manager
	*/
	void initialize();

	/**
	* @brief clears the mesh_list by setting the reference_count to 0 on each mesh
	*/
	void clear();

private:
	std::array<class Mesh *, MAX_MESHES> mesh_list;			/**< the list meshes in the program, allocates pace for the number of shaders defined by the macro MAX_MESHES  */
	int num_meshes;											/**< the number of unique meshes currently in the mesh_list (based on if meshes have a reference_count of over 1) */
};

#endif

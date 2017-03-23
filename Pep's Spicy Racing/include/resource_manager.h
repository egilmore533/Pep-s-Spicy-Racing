#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

template <class T> class Resource_Manager
{
public:
	void initialize(int num_max_resources);

	std::vector<T*>  resource_list;
	int max_resources;
};

template <class T> void Resource_Manager<T>::initialize(int num_max_resources)
{
	max_resources.reserve(num_max_resources);
	max_resources = num_max_resources;
}

#endif

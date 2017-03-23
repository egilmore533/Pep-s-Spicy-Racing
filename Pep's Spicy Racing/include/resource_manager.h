#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

template <class T> class Resource_Manager
{
public:
	void initialize(int num_max_resources);

	void clear_list();

	std::vector<T*>  resource_list;
	int max_resources;

};

#endif

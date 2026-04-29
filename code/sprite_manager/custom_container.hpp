#pragma once
#include <base/includes.hpp>

// please implement
// int GetNext(const TYPE& val);
// void RemoveToListHead(TYPE& removed_item, int current_head);
template<typename TYPE>
class CustomContainer
{
public:
	// for Stupid RAII people - there is no resource allocation
	CustomContainer() : data(NULL), last_index(0), capacity(0), increment(0), list_header(0) {}
	~CustomContainer() {}

	// for actual use - C style manual resource management
	void Create(int initial_size, int inc = 100);
	void Release();
	void Clear()
	{
		last_index = 0;
		list_header = -1;
	}

	// return sprite/component index
	int AddElement(const TYPE& elem);

	// return hole index
	int RemoveById(int component_id);

	// inlines
	const TYPE* GetData() const { return data; }
	TYPE* GetDataRAW() { return data; }
	int NumData() const { return last_index; }
	
	void SetDataAt(const TYPE& elem, int ind);
	const TYPE& GetDataAt(int ind) const
	{
		return data[ind];
	}
	TYPE& GetDataAtRaw(int ind)
	{
		return data[ind];
	}
private:
	void AllocData(int new_capacity);
	void ReleaseData();
	void ReallocData(int new_capacity);


	TYPE* data;
	int last_index;
	int capacity;
	int increment;
	int list_header;
};

template<typename TYPE>
inline void CustomContainer<TYPE>::Create(int initial_size, int inc)
{
	AllocData(initial_size);
	last_index = 0;
	increment = inc;
	list_header = -1;
}
template<typename TYPE>
inline void CustomContainer<TYPE>::Release()
{
	ReleaseData();
}

template<typename TYPE>
inline int CustomContainer<TYPE>::AddElement(const TYPE& elem)
{
	if(-1 == list_header)
	{
		if(capacity == last_index)
		{
			ReallocData(capacity + increment);
			if(capacity == last_index)
				return -1; // failed
		}

		SetDataAt(elem, last_index);

		return last_index++;
	}
	else
	{
		// refill hole
		int ind = list_header;
		list_header = GetNext(data[list_header]);
		SetDataAt(elem, ind);

		return ind;
	}
}

template<typename TYPE>
inline int CustomContainer<TYPE>::RemoveById(int component_id)
{
	RemoveToListHead(data[component_id], list_header);
	list_header = component_id;
	return component_id;
}

template<typename TYPE>
inline void CustomContainer<TYPE>::SetDataAt(const TYPE& elem, int ind)
{
	SDL_memcpy(&data[ind], &elem, sizeof(TYPE));
}

template<typename TYPE>
inline void CustomContainer<TYPE>::AllocData(int new_capacity)
{
	if(new_capacity < 1)
		new_capacity = 1;
	data = (TYPE*)SDL_malloc(sizeof(TYPE) * new_capacity);
	if(nullptr == data)
		return;
	capacity = new_capacity;
}
template<typename TYPE>
inline void CustomContainer<TYPE>::ReleaseData()
{
	SDL_free(data);
	data = NULL;
	last_index = 0;
	capacity = 0;
	list_header = -1;
}

template<typename TYPE>
inline void CustomContainer<TYPE>::ReallocData(int new_capacity)
{
	int old_cap = capacity;
	TYPE* old_sprite = data;

	AllocData(new_capacity);

	// if allocation failed
	if(nullptr == data)
	{
		// restore previous
		capacity = old_cap;
		data = old_sprite;

		return;
	}

	// move data to new storage
	SDL_memcpy(data, old_sprite, sizeof(TYPE) * last_index);

	// release old
	SDL_free(old_sprite);
}
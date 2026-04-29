#pragma once
#include <base/includes.hpp>


// allocation of data1 and data2 are on the same buffer with data2 being offset by data1's size
// therefore, TYPE1 alignment*cap should align with TYPE2 cell size
template<typename TYPE1, typename TYPE2>
class CustomContainer2
{
public:
	// for Stupid RAII people - there is no resource allocation
	CustomContainer2() : data1(NULL), data2(NULL), last_index(0), capacity(0), increment(0), list_header(0) {}
	~CustomContainer2() {}

	// for actual use - C style manual resource management
	void Create(int initial_size, int inc = 100);
	void Clear()
	{
		last_index = 0;
		list_header = -1;
	}
	void Release();

	// return sprite/component index
	int AddElement(const TYPE1& prime, const TYPE2& second);

	// return hole index
	int RemoveById(int component_id);

	// inlines
	const TYPE1* GetPrimery() const { return data1; }
	const TYPE2* GetSecondary() const { return data2; }
	TYPE1* GetPrimeryRAW() { return data1; }
	TYPE2* GetSecondaryRAW() { return data2; }
	int Numdata() const { return last_index; }
	
	void SetDataAt(const TYPE1& elem1, const TYPE2& elem2, int ind)
	{
		SetPrimaryAt(elem1, ind);
		SetSecondaryAt(elem2, ind);
	}
	void SetPrimaryAt(const TYPE1& elem1, int ind);
	void SetSecondaryAt(const TYPE2& elem2, int ind);

	const TYPE1& GetPrimaryAt(int ind) const 
	{
		return data1[ind];
	}
	TYPE1& GetRawPrimaryAt(int ind)
	{
		return data1[ind];
	}
	const TYPE2& GetSecondaryAt(int ind) const 
	{
		return data2[ind];
	}
	TYPE2& GetRawSecondaryAt(int ind)
	{
		return data2[ind];
	}
private:
	void AllocData(int new_capacity);
	void ReleaseData();
	void ReallocData(int new_capacity);


	TYPE1* data1;
	TYPE2* data2;
	int last_index;
	int capacity;
	int increment;
	int list_header;
};


template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::Create(int initial_size, int inc)
{
	AllocData(initial_size);
	last_index = 0;
	increment = inc;
	list_header = -1;
}
template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::Release()
{
	ReleaseData();
}

template<typename TYPE1, typename TYPE2>
inline int CustomContainer2<TYPE1, TYPE2>::AddElement(const TYPE1& prime, const TYPE2& second)
{
	if(-1 == list_header)
	{
		if(capacity == last_index)
		{
			ReallocData(capacity + increment);
			if(capacity == last_index)
				return -1; // failed
		}

		SetDataAt(prime, second, last_index);

		return last_index++;
	}
	else
	{
		// refill hole
		int ind = list_header;
		list_header = GetNext(data1[list_header]);
		SetDataAt(prime, second, ind);

		return ind;
	}
}

template<typename TYPE1, typename TYPE2>
inline int CustomContainer2<TYPE1, TYPE2>::RemoveById(int component_id)
{
	RemoveToListHead(data1[component_id], list_header);
	ClearData(data2[list_header]);
	list_header = component_id;
	return component_id;
}

template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::SetPrimaryAt(const TYPE1& elem1, int ind)
{
	SDL_memcpy(&data1[ind], &elem1, sizeof(TYPE1));
}

template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::SetSecondaryAt(const TYPE2& elem2, int ind)
{
	SDL_memcpy(&data2[ind], &elem2, sizeof(TYPE2));
}

template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::AllocData(int new_capacity)
{
	if(new_capacity < 1)
		new_capacity = 1;
	
	char* raw_ptr = (char*)SDL_malloc((sizeof(TYPE1) + sizeof(TYPE2)) * new_capacity);
	if(nullptr == raw_ptr)
		return;

	data1 = (TYPE1*)raw_ptr;

	data2 = (TYPE2*)(raw_ptr + (new_capacity * sizeof(TYPE1)));

	capacity = new_capacity;
}
template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::ReleaseData()
{
	SDL_free(data1);
	data1 = nullptr;
	data2 = nullptr;
	
	last_index = 0;
	capacity = 0;
	list_header = -1;
}

template<typename TYPE1, typename TYPE2>
inline void CustomContainer2<TYPE1, TYPE2>::ReallocData(int new_capacity)
{
	int old_cap = capacity;
	TYPE1* old_prime = data1;
	TYPE2* old_second = data2;

	AllocData(new_capacity);

	// if allocation failed
	if(nullptr == data1)
	{
		// restore previous
		capacity = old_cap;
		data1 = old_prime;
		data2 = old_second;

		return;
	}

	// move data to new storage
	SDL_memcpy(data1, old_prime, sizeof(TYPE1) * last_index);
	SDL_memcpy(data2, old_second, sizeof(TYPE2) * last_index);

	// release old
	SDL_free(old_prime);
}
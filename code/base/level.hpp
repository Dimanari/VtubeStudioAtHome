#pragma once
#include "drawable.hpp"

class Level : public DimDrawable
{
public:
	virtual void load() = 0;
	virtual void unload() = 0;
	
	virtual void load_from_cache() {}
	virtual void unload_to_cache() {}

	virtual void render() = 0;
	virtual void update(bool& quit) = 0;
	virtual void input(const SDL_Event& c_event, bool& quit) = 0;
	virtual ~Level() {}

	virtual const char* LevelName() { return "Unnamed Level";}
	inline void QueueLevel(Level* allocated_level)
	{
		if (s_queue)
			delete s_queue;
		s_queue = allocated_level;
	}
	
	static Level* s_queue;
	static int s_cache_queue;
	static int s_level_place_cache;
	static int s_last_level_cache_index;
};

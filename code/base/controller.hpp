#pragma once
#include "level.hpp"

#define LEVEL_CACHE 10

class Controller : public DimDrawable
{
	bool quit;
	SDL_Window* gWindow;
	Level* current_level;
	
	Level* queue_level;

	Level* cache_levels[LEVEL_CACHE];
public:
	enum CONT_STATES_e
	{
		CONT_OK = 0, CONT_SDL_INIT, CONT_WINDOW_ERR, CONT_GL_CONTEXT_ERR, CONT_GLEW_INIT_ERR, CONT_MAX_ENUM
	};

	CONT_STATES_e Init(const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const char* title, bool VSync = true, bool Fullscreen = false);

	void render();
	void update();
	void load(Level* new_level);
	void reloadcache(int cache_index);
	void unload();

	void loop();

	void cleanup();
	void ToggleFullscreen();

private:
	void CacheCurrentLevel();
};

void ToggleFullscreen(SDL_Window* Window);
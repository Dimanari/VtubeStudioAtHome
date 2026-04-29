#pragma once
#include <open_gl/texture.hpp>
#include <open_gl_composite/simple_renderer.hpp>
#include <raw_input/SDL_GetHardwareKeys.hpp>
#include <sprite_manager/sprite_holder.hpp>
#include <base/level.hpp>

#define MAX_NUM_KEYS (NUM_BITFIELD*CHAR_BIT)

void QuitMainMenu(void* level);

struct KeyData{
	int sdlKey;
	int vKey;
	int gHandle;
};

class MainMenuLevel : public Level
{
public:
	void load();
	void unload();
	void render();
	void update(bool& quit);
	void input(const SDL_Event& c_event, bool& quit);

	void Mousemove(glm::ivec2 mouse_pos);

	glm::ivec2 TranslateMouseLocation(glm::vec2 relative);
	glm::ivec2 TranslateKeyLocation(glm::vec2 relative);
	glm::ivec2 TranslateKeySize(glm::vec2 relative);

	void ReadKeyConfig();
	void SaveKeyConfig();

	void Quit();
private:
	bool* m_quit;
	int m_mouse_prim;


	SimpleRenderer m_render_keys;
	SpriteManager m_vertex_keys;

	// drawing cursor
	SimpleRenderer m_render_cursor;
	Texture m_texture_cursor;
	SpriteManager m_vertex_cursor;


	// drawing cursor
	SimpleRenderer m_render_body;
	Texture m_texture_body;
	SpriteManager m_vertex_body;


	int m_num_keys, m_rows, m_cols;
	KeyData m_keys[MAX_NUM_KEYS];

	KeyState m_state;
};
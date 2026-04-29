#include <shader_features/internal_shaders.hpp>
#include <utility/my_file.hpp>

#include "level_test.hpp"
#include "configs.hpp"

#define GAME_FONT_PATH "./assets/game_font.ttf"
constexpr int set_keys = 4;

void QuitMainMenu(void* level)
{
	((MainMenuLevel*)level)->Quit();
}

void MainMenuLevel::Quit()
{
	if(m_quit)
		*m_quit = true;
}

void MainMenuLevel::load()
{

	if(-1 == SDL_CaptureMouse(SDL_TRUE))
		printf("global mouse tracking not supported\n");
	
	InitKeyState(&m_state, gWindow);

	if(IS_FULLSCREEN)
        SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
    // if alpha bending is required
    if (IS_BLEND)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

	SDL_ShowCursor(SHOW_DURSOR);


	m_vertex_keys.Create(100);
	m_render_keys.SetupRenderer(sizeof(PrimitiveVert2D), primitive_attribs, num_primitive_attribs, GL_DYNAMIC_DRAW, (const char*)m_vertex_keys.GetData(), m_vertex_keys.NumData());

	//"uniform vec2 SCREEN_SIZE;\n"
	//"uniform vec2 SCREEN_OFFSET;\n"
	m_render_keys.PremakeString(primitive_vert_shader, "Vertex Text", Shader::SHADE_VERT);
	m_render_keys.PremakeString(primitive_geom_shader, "Geometry Text", Shader::SHADE_GEOM);
	//"uniform float PIX_SIZE;\n"
	//"uniform float LINE_WIDTH;\n"
	//"uniform vec2 SCREEN_SIZE;\n"
	m_render_keys.PremakeString(primitive_frag_shader, "Fragment Text", Shader::SHADE_FRAG);
	m_render_keys.Link();

	m_render_keys.GetShade().Use();
	int Location = m_render_keys.GetShade().UniformLocation("SCREEN_SIZE");
	m_render_keys.GetShade().setFloat2(Location, {SCREEN_WIDTH, SCREEN_HEIGHT});

	Location = m_render_keys.GetShade().UniformLocation("SCREEN_OFFSET");
	m_render_keys.GetShade().setFloat2(Location, {0, 0});

	Location = m_render_keys.GetShade().UniformLocation("LINE_WIDTH");
	m_render_keys.GetShade().setFloat(Location, 1);

	Location = m_render_keys.GetShade().UniformLocation("PIX_SIZE");
	m_render_keys.GetShade().setFloat(Location, 0.01);

	SDL_Surface* surf = IMG_Load("assets/cursor.png");

	if(NULL == surf)
		exit(-1);
	m_texture_cursor.MakeFromSurface(surf);

	m_texture_cursor.BindToTex(2);
	Texture::ResetActiveTexture();

	SDL_FreeSurface(surf);
	surf = NULL;

	surf = IMG_Load("assets/template.png");

	if(NULL == surf)
		exit(-1);
	m_texture_body.MakeFromSurface(surf);

	m_texture_body.BindToTex(3);
	Texture::ResetActiveTexture();

	SDL_FreeSurface(surf);
	surf = NULL;

	m_vertex_cursor.Create(100);

	m_render_cursor.SetupRenderer(sizeof(PrimitiveVert2D), primitive_attribs, num_primitive_attribs, GL_DYNAMIC_DRAW, (const char*)m_vertex_cursor.GetData(), m_vertex_cursor.NumData());

//"uniform vec2 SCREEN_SIZE;\n"
//"uniform vec2 SCREEN_OFFSET;\n"
	m_render_cursor.PremakeString(simple_texture_vert_shader, "Vertex Text", Shader::SHADE_VERT);
//"uniform vec2 SPRITE_MAT;\n"
	m_render_cursor.PremakeString(simple_texture_geom_shader, "Geometry Text", Shader::SHADE_GEOM);
//"uniform sampler2D MY_TEX;\n"
//"uniform vec4 MUL_COLOR;\n"
	m_render_cursor.PremakeString(simple_texture_frag_shader, "Fragment Text", Shader::SHADE_FRAG);
	m_render_cursor.Link();

	m_render_cursor.GetShade().Use();
	Location = m_render_cursor.GetShade().UniformLocation("SCREEN_SIZE");
	m_render_cursor.GetShade().setFloat2(Location, {SCREEN_WIDTH, SCREEN_HEIGHT});

	Location = m_render_cursor.GetShade().UniformLocation("SCREEN_OFFSET");
	m_render_cursor.GetShade().setFloat2(Location, {0, 0});

	Location = m_render_cursor.GetShade().UniformLocation("SPRITE_MAT");
	m_render_cursor.GetShade().setFloat2(Location, {1, 1});

	Location = m_render_cursor.GetShade().UniformLocation("MY_TEX");
	m_render_cursor.GetShade().setInt(Location, 2);
	
	Location = m_render_cursor.GetShade().UniformLocation("MUL_COLOR");
	m_render_cursor.GetShade().setFloat4(Location, {1.0, 1.0, 1.0, 1.0});

	m_vertex_body.Create(100);

	m_render_body.SetupRenderer(sizeof(PrimitiveVert2D), primitive_attribs, num_primitive_attribs, GL_DYNAMIC_DRAW, (const char*)m_vertex_body.GetData(), m_vertex_body.NumData());

//"uniform vec2 SCREEN_SIZE;\n"
//"uniform vec2 SCREEN_OFFSET;\n"
	m_render_body.PremakeString(simple_texture_vert_shader, "Vertex Text", Shader::SHADE_VERT);
//"uniform vec2 SPRITE_MAT;\n"
	m_render_body.PremakeString(simple_texture_geom_shader, "Geometry Text", Shader::SHADE_GEOM);
//"uniform sampler2D MY_TEX;\n"
//"uniform vec4 MUL_COLOR;\n"
	m_render_body.PremakeString(simple_texture_frag_shader, "Fragment Text", Shader::SHADE_FRAG);
	m_render_body.Link();

	m_render_body.GetShade().Use();
	Location = m_render_body.GetShade().UniformLocation("SCREEN_SIZE");
	m_render_body.GetShade().setFloat2(Location, {SCREEN_WIDTH, SCREEN_HEIGHT});

	Location = m_render_body.GetShade().UniformLocation("SCREEN_OFFSET");
	m_render_body.GetShade().setFloat2(Location, {0, 0});

	Location = m_render_body.GetShade().UniformLocation("SPRITE_MAT");
	m_render_body.GetShade().setFloat2(Location, {1, 1});

	Location = m_render_body.GetShade().UniformLocation("MY_TEX");
	m_render_body.GetShade().setInt(Location, 3);
	
	Location = m_render_body.GetShade().UniformLocation("MUL_COLOR");
	m_render_body.GetShade().setFloat4(Location, {1.0, 1.0, 1.0, 1.0});


	PrimitiveVert2D vert;
	vert.pos[0] = 0;
	vert.pos[1] = 0;

	vert.size[0] = 20;
	vert.size[1] = 20;

	vert.color_RGBA = COLOR_WHITE;
	vert.shape = 0;

	m_mouse_prim = m_vertex_cursor.AddElement(vert);

	vert.pos[0] = 0;
	vert.pos[1] = 0;

	vert.size[0] = 200;
	vert.size[1] = 200;

	vert.color_RGBA = COLOR_WHITE;
	vert.shape = 0;
	m_vertex_body.AddElement(vert);
/*
	<UP> = 111;
	<LEFT> = 113;
	<DOWN> = 116;
	<RGHT> = 114;
*/
	ReadKeyConfig();
}
void MainMenuLevel::unload()
{
	m_render_cursor.Release();
	m_vertex_cursor.Release();
	m_texture_cursor.Release();

	m_render_body.Release();
	m_vertex_body.Release();
	m_texture_body.Release();

	m_render_keys.Release();
	m_vertex_keys.Release();
}
void MainMenuLevel::render()
{
	glClearColor(0.1, 0.15, 0.1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_render_body.Draw(GL_POINTS);
	m_render_keys.Draw(GL_POINTS);
	m_render_cursor.Draw(GL_POINTS);
}
void MainMenuLevel::update(bool& quit)
{
	UpdateKeyState(&m_state);
	for(int i = 0; i < m_num_keys; ++i)
	{
		if(0 == m_keys[i].vKey) continue;
		if(0 > m_keys[i].gHandle || m_vertex_keys.NumData() <= m_keys[i].gHandle)
		{
			printf("num keys %d\n", m_num_keys);
			printf("invalid key %d\n", i);
			continue;
		}
		
		if(GetKeyFromState(m_keys[i].vKey,&m_state))
		{
			m_vertex_keys.GetDataAtRaw(m_keys[i].gHandle).color_RGBA = COLOR_GREEN;
		}
		else
		{
			m_vertex_keys.GetDataAtRaw(m_keys[i].gHandle).color_RGBA = COLOR_BLUE;
		}
	}

	// GLOBAL MOUSE POSITION
	SDL_DisplayMode DM;
	// only support 1 display
	SDL_GetCurrentDisplayMode(0, &DM);
	glm::ivec2 display = {DM.w, DM.h};
	UNUSED(display);

	glm::ivec2 mouse_pos = GetMousePosFromState(&m_state);

	glm::vec2 relative = glm::vec2(mouse_pos);
	relative.x /= float(display.x);
	relative.y /= float(display.y);

	// doesn't work in XWayland for obvious reasons
	Mousemove(TranslateMouseLocation(relative));

	UNUSED(quit);
	if(!m_quit)
		m_quit = &quit;

	m_render_cursor.UpdateData((const char *)m_vertex_cursor.GetData(), m_vertex_cursor.NumData());
	m_render_body.UpdateData((const char *)m_vertex_body.GetData(), m_vertex_body.NumData());
	m_render_keys.UpdateData((const char *)m_vertex_keys.GetData(), m_vertex_keys.NumData());

}

void MainMenuLevel::input(const SDL_Event& c_event, bool& quit)
{
	// handle input
	switch(c_event.type)
	{
	case SDL_KEYDOWN:
		{
			if(c_event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
				SaveKeyConfig();
			}

			// dirty method identify hardware scancodes
			for(int i = 0; i < m_num_keys; ++i)
				if(m_keys[i].sdlKey == c_event.key.keysym.sym)
				{
					UpdateKeyState(&m_state);
					m_keys[i].vKey = FindCurrentKeyPress(&m_state);
					m_keys[i].sdlKey = SDL_SCANCODE_UNKNOWN;
				}
		}
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
		{
			// glm::ivec2 mouse_pos = { c_event.motion.x, c_event.motion.y };
			// Mousemove(mouse_pos);
		}
		break;
	}
}

void MainMenuLevel::Mousemove(glm::ivec2 mouse_pos)
{
	auto& data = m_vertex_cursor.GetDataAtRaw(m_mouse_prim);

	data.pos[0] = mouse_pos.x;
	data.pos[1] = mouse_pos.y;
}

glm::ivec2 MainMenuLevel::TranslateMouseLocation(glm::vec2 relative)
{
	glm::vec2 start_block = {100, 190};
	glm::vec2 size = {-50, -40};

	glm::vec2 coords = relative * size + start_block;

	
	return glm::ivec2(coords) - m_vertex_cursor.GetDataAt(m_mouse_prim).size / 2;
}

glm::ivec2 MainMenuLevel::TranslateKeyLocation(glm::vec2 relative)
{
	glm::vec2 start_block = {105, 145};
	glm::vec2 size = {95, 50};

	glm::vec2 coords = relative * size + start_block;

	return glm::ivec2(coords);
}

glm::ivec2 MainMenuLevel::TranslateKeySize(glm::vec2 relative)
{
	glm::vec2 size = {95, 50};
	return glm::ivec2{size * relative};
}

void MainMenuLevel::ReadKeyConfig()
{
	int vkeys[set_keys] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT }; //SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT};
	int hkeys[set_keys] = {0};

	int* actual_sdlkeys = vkeys;
	int* actual_physicalkeys = hkeys;

	m_rows = 2;
	m_cols = ceil(float(set_keys)/m_rows);
	bool wasMalloc = false;

	FILE* conf_file = _fopen("KeyConfig", "rb");
	int count = set_keys;
	if(NULL != conf_file)
	{
		fread(&count, sizeof(int), 1, conf_file);
		fread(&m_rows, sizeof(int), 1, conf_file);
		fread(&m_cols, sizeof(int), 1, conf_file);

		actual_sdlkeys = (int*)SDL_malloc(sizeof(int) * count);
		actual_physicalkeys = (int*)SDL_malloc(sizeof(int) * count);

		wasMalloc = true;

		fread(actual_sdlkeys, sizeof(int), count, conf_file);
		fread(actual_physicalkeys, sizeof(int), count, conf_file);
		fclose(conf_file);
	}
	int real_count = 0;
	for(int i=0;i<count;++i)
	{
		int row = i / m_cols;
		int col = i % m_cols;
		PrimitiveVert2D vert_elem;
		vert_elem.color_RGBA = COLOR_BLUE;
		vert_elem.pos = TranslateKeyLocation({ (1.f/m_cols) * col, (1.f/m_rows) * row});
		vert_elem.size = TranslateKeySize({ (1.f/m_cols), (1.f/m_rows)});
		vert_elem.shape = PRIMITIVE_RECT;

		int handle_for_key = m_vertex_keys.AddElement(vert_elem);

		if((actual_physicalkeys[i] == 0) && (actual_sdlkeys[i] == SDL_SCANCODE_UNKNOWN))
			continue;
		
		if(actual_physicalkeys[i] != 0)
			actual_sdlkeys[i] = SDL_SCANCODE_UNKNOWN;
		m_keys[real_count++] = {
		actual_sdlkeys[i], 
		actual_physicalkeys[i],
		handle_for_key};
	}

	if(wasMalloc)
	{
		SDL_free(actual_sdlkeys);
		SDL_free(actual_physicalkeys);
	}
	m_num_keys = real_count;
}

void MainMenuLevel::SaveKeyConfig()
{

	FILE* conf_file = _fopen("KeyConfig", "wb");
	if(NULL != conf_file)
	{
		fwrite(&m_num_keys, sizeof(int), 1, conf_file);
		fwrite(&m_rows, sizeof(int), 1, conf_file);
		fwrite(&m_cols, sizeof(int), 1, conf_file);

		for(int i = 0; i < m_num_keys; ++i)
		{
			fwrite(&m_keys[i].sdlKey, sizeof(int), 1, conf_file);
		}
		for(int i = 0; i < m_num_keys; ++i)
		{
			fwrite(&m_keys[i].vKey, sizeof(int), 1, conf_file);
		}
		fclose(conf_file);
	}
}
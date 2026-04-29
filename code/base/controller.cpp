#include "controller.hpp"

void ToggleFullscreen(SDL_Window* Window)
{
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
    SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
    //SDL_ShowCursor(IsFullscreen);
}
SDL_GLContext DimDrawable::gContext;
SDL_Window* DimDrawable::gWindow;
Level* Level::s_queue = nullptr;
int Level::s_cache_queue = -1;
int Level::s_level_place_cache = 0;
int Level::s_last_level_cache_index = -1;

Controller::CONT_STATES_e Controller::Init(const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const char* title, bool VSync, bool Fullscreen)
{
    current_level = NULL;
    queue_level = NULL;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
        return CONT_SDL_INIT;
    }

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return CONT_SDL_INIT;
    }
    //Initialize SDL_ttf
    // if (TTF_Init() == -1)
    // {
    //     printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    //     return CONT_SDL_INIT;
    // }

    /*
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return CONT_SDL_INIT;
    }*/

    // Use OpenGL 4.3 core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Turn on double buffering with a 24bit Z buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create a SDL window
    gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED_DISPLAY(1), SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (Fullscreen * SDL_WINDOW_FULLSCREEN));
    if (gWindow == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL window: %s\n", SDL_GetError());
        SDL_Quit();
        return CONT_WINDOW_ERR;
    }

    gContext = SDL_GL_CreateContext(gWindow);
    if (gContext == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return CONT_GL_CONTEXT_ERR;
    }

    if (VSync)
        SDL_GL_SetSwapInterval(1);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Allow experimental extensions
    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLEW: %s\n", glewGetErrorString(GlewError));
        SDL_GL_DeleteContext(gContext);
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return CONT_GLEW_INIT_ERR;
    }
    DimDrawable::gWindow = gWindow;
    return CONT_OK;
}

void Controller::render()
{
    if (current_level)
        current_level->render();
    SDL_GL_SwapWindow(gWindow);
}

void Controller::update()
{
    if (current_level)
        current_level->update(quit);
}

void Controller::load(Level* new_level)
{
    if (current_level)
    {
        CacheCurrentLevel();
        unload();
    }
    current_level = new_level;
    current_level->load();
}

void Controller::reloadcache(int cache_index)
{
    Level* find_cache_item = cache_levels[cache_index];
    if(NULL == find_cache_item)
        return;
    cache_levels[cache_index] = NULL;
    if (current_level)
    {
        CacheCurrentLevel();
        unload();
    }
    current_level = find_cache_item;
    current_level->load_from_cache();
}

void Controller::unload()
{
    if (current_level)
    {
        current_level->unload();
        delete current_level;
        current_level = NULL;
    }
}

void Controller::loop()
{
    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            current_level->input(e, quit);
        }
        update();
        render();

        if (Level::s_queue)
        {
            load(Level::s_queue);
            Level::s_queue = nullptr;
        }
        else if(-1 != Level::s_cache_queue)
        {
            reloadcache(Level::s_cache_queue);
            Level::s_cache_queue = -1;
        }
    }
}

void Controller::cleanup()
{
    for(int i=0;i<LEVEL_CACHE;++i)
    {
        if(cache_levels[i])
        {
            cache_levels[i]->unload();
            delete cache_levels[i];
            cache_levels[i] = NULL;
        }
    }
    if (current_level)
        unload();
    if (Level::s_queue)
    {
        delete Level::s_queue;
        Level::s_queue = nullptr;
    }
    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gContext = NULL;

    //Quit SDL subsystems
    //Mix_Quit();
    //TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Controller::ToggleFullscreen()
{
    ::ToggleFullscreen(gWindow);
}

void Controller::CacheCurrentLevel()
{
    if(Level::s_level_place_cache)
    {
        Level::s_level_place_cache = 0;
        int found_index = -1;

        for(int i=0;i<LEVEL_CACHE;++i)
        {
            if(NULL == cache_levels[i])
            {
                found_index = i;
                break;
            }
        }

        if(0 <= found_index)
        {
            cache_levels[found_index] = current_level;
            current_level->unload_to_cache();

            current_level = NULL;
            Level::s_last_level_cache_index = found_index;
        }
    }
}
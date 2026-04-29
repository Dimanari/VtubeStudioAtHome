#define _ENTRY_POINT
#include "includes.hpp"
#include "controller.hpp"
#include <concrete/configs.hpp>

Controller gControl;

int main(int argc, char** argv)
{
	(void)argc; // Unused
	(void)argv;

	if (Controller::CONT_OK != gControl.Init(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE))
	{
		return -1;
	}
#if defined _WIN32 and not defined _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
#ifdef NDEBUG
	SDL_ShowCursor(SDL_DISABLE);
#endif
	gControl.load(new INITIAL_LEVEL);
	gControl.loop();

	gControl.cleanup();
#if defined _WIN32 and not defined _DEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif
	printf("game closed\n");
	return 0;
}
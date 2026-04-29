#pragma once
#include <base/includes.hpp>
#ifndef _WIN32
#include <X11/Xlib.h>

// Add -lX11 to linker on linux

#define NUM_BITFIELD 32
struct KeyState
{
	Display* display;
	char bitfield[NUM_BITFIELD];
	glm::ivec2 mouse_pos;
};
Display* GetXDisplay(SDL_Window* window);
void GetKeyStateLinux(KeyState* state);

#else
#include <windows.h>
struct KeyState
{
	char* garbage1;
	char garbage2[32]; // for alignment
	glm::ivec2 mouse_pos;
	// might include stuff later if needed for OS features
};
#endif

// call at start
void InitKeyState(KeyState* keys, SDL_Window* window);
// call before check
void UpdateKeyState(KeyState* keys);
// actually get results
short GetKeyFromState(int vKey, KeyState* keystate);
glm::ivec2 GetMousePosFromState(KeyState* keystate);

// for mouse position use SDL_GetGlobalMouseState(x, y);
// for mouse button use the SDL_BUTTON(X) macro
#pragma once
#include <base/includes.hpp>
#define NUM_BITFIELD 32 // 256 bits bitfield
#ifndef _WIN32
#include <X11/Xlib.h>

// Add -lX11 to linker on linux

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
	unsigned char bitfield[NUM_BITFIELD * CHAR_BIT]; // for alignment
	glm::ivec2 mouse_pos;
	// might include stuff later if needed for OS features
};
#endif

// call at start
void InitKeyState(KeyState* keys, SDL_Window* window);
// call before check
void UpdateKeyState(KeyState* keys);
int FindCurrentKeyPress(KeyState* keys);
// actually get results
short GetKeyFromState(int vKey, KeyState* keystate);
glm::ivec2 GetMousePosFromState(KeyState* keystate);

// for mouse position use SDL_GetGlobalMouseState(x, y);
// for mouse button use the SDL_BUTTON(X) macro
#include "SDL_GetHardwareKeys.hpp"
#include <stdio.h>
#ifndef _WIN32
// Linux Exclusive
#include <SDL_syswm.h>

Display* GetXDisplay(SDL_Window* window)
{ 
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version); // Initialize info structure with current version

	if (SDL_TRUE == SDL_GetWindowWMInfo(window, &info))
	{
		if (info.subsystem == SDL_SYSWM_X11)
		{
			printf("Working on the LEGEND X11 - native support\n");
			// this means we can actually get global keys and mouse input - unlike wayland

			return XOpenDisplay(NULL);// info.info.x11.display
		}
		if (info.subsystem == SDL_SYSWM_WAYLAND)
		{
			printf("FUCKED BY WAYLAND - NOT SUPPORTED\n");
			return NULL;
		}
		if (info.subsystem == SDL_SYSWM_WINDOWS)
		{
			printf("Using Windows - WINAPI support\n");
			return NULL;
		}
	}
	
	return NULL;
}

void GetKeyStateLinux(KeyState* state)
{
	// bitfield[32] key value is in byte floor(N/8) and in bit (N & 7)
	XQueryKeymap(state->display, state->bitfield);
	// 14 * 8 + 5
	Window root = DefaultRootWindow(state->display);

	Window		rep_root, rep_child;
    int			rep_rootx, rep_rooty;
    unsigned int	rep_mask;

    if(false == XQueryPointer (state->display, root, &rep_root, &rep_child,
	    &rep_rootx, &rep_rooty, &state->mouse_pos.x, &state->mouse_pos.y, &rep_mask))
		printf("failed grabbing mouse\n");
}
#endif

void InitKeyState(KeyState* keys, SDL_Window* window)
{
	if(NULL == window)
		return;
	if(NULL == keys)
		return;
#ifndef _WIN32
	keys->display = GetXDisplay(window);
#else

#endif
}

void UpdateKeyState(KeyState* keys)
{
	if(NULL == keys)
		return;
#ifndef _WIN32
	GetKeyStateLinux(keys);
#else
	POINT mouse;
	GetCursorPos(&mouse);
	GetKeyboardState((PBYTE)keys->bitfield);

	// printf("pos: %d, %d \n", (int)mouse.x, (int)mouse.y);
	keys->mouse_pos.x = mouse.x;
	keys->mouse_pos.y = mouse.y;
#endif
}

int FindCurrentKeyPress(KeyState* keys)
{
	int found_byte = -1;
	// search key
#ifdef _WIN32
	for(int i=0;i<NUM_BITFIELD * CHAR_BIT;++i)
	{
		if(keys->bitfield[i])
		{
			found_byte = i;
			break;
		}
	}
	return found_byte;
#else
	int found_bit = -1;
	for(int i=0;i<NUM_BITFIELD;++i)
	{
		if(keys->bitfield[i])
		{
			found_byte = i;
			break;
		}
	}

	if(-1 == found_byte) return 0;

	for(int i=0;i<CHAR_BIT;++i)
	{
		if(keys->bitfield[found_byte] & (1 << i))
		{
			found_bit = i;
			break;
		}
	}
	if(-1 == found_bit) return 0;

	return found_byte * CHAR_BIT + found_bit;
#endif


}

short GetKeyFromState(int vKey, KeyState* keystate)
{
#ifdef _WIN32
	return keystate->bitfield[vKey] & 0x80;
#else
	int vbyte = vKey / 8;
	int vbit = vKey % 8;
	return keystate->bitfield[vbyte] & (1 << vbit);
#endif
}

glm::ivec2 GetMousePosFromState(KeyState* keystate)
{
#ifndef _WIN32
	return keystate->mouse_pos;
#else
	return keystate->mouse_pos;
#endif
}
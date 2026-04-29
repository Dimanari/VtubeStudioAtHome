#pragma once
#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <windows.h>
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <SDL.h>

#include <stdio.h>

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define COLORPACK(r,g,b,a) (((unsigned int)r) | (((unsigned int)g) << 8) | (((unsigned int)b) << 16) | (((unsigned int)a) << 24))

#define COLOR_WHITE COLORPACK(255, 255, 255, 255)
#define COLOR_BLACK COLORPACK(0, 0, 0, 255)
#define COLOR_RED COLORPACK(255, 0, 0, 255)
#define COLOR_GREEN COLORPACK(0, 255, 0, 255)
#define COLOR_BLUE COLORPACK(0, 0, 255, 255)
#define UNUSED(x) (void)(x)

inline bool MouseInRegion(glm::ivec2 mouse, glm::ivec2 pos, glm::ivec2 size)
{
	glm::ivec2 relative_mouse = mouse - pos;
	return relative_mouse.x >= 0 && relative_mouse.y >= 0 && relative_mouse.x < size.x && relative_mouse.y < size.y;
}


// ENUM String
#define STRINGIFY(a) #a

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1) __VA_OPT__(,)                                                   \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define ENUM_TO_STRING(ENUM_NAME, ...)                                                   \
    enum ENUM_NAME { __VA_ARGS__ };                                                      \
    const char* ENUM_NAME##_strings[] = { FOR_EACH(STRINGIFY, __VA_ARGS__) };

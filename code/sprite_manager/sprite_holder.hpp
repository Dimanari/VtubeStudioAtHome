#pragma once
#include <base/includes.hpp>
#include <shader_features/internal_shaders.hpp>
#include "custom_container.hpp"
#include "custom_container2.hpp"

struct SpriteComponent
{
	int hover_event;
	int click_event;
	int is_hover;
	int click_value;
	int alive;
	int info;
};

struct StringComponent
{
	int info;
};

// example functions for PrimitiveVert2D
inline int GetNext(const PrimitiveVert2D& sprite)
{
	return sprite.color_RGBA;
}

inline void RemoveToListHead(PrimitiveVert2D& sprite, int current_head)
{
	sprite.shape = -1; // empty shape
	sprite.color_RGBA = current_head; // point to the list's previous head, and now this will become the new head
}

inline void ClearData(SpriteComponent& comp)
{
	comp.hover_event = -1;
	comp.click_event = -1;
	comp.is_hover = 0;
	comp.click_value = 0;
	comp.alive = 0;
	comp.info = 0;
}

inline void ClearData(StringComponent& comp)
{
	comp.info = 0;
}

typedef CustomContainer<PrimitiveVert2D> SpriteManager;

typedef CustomContainer2<PrimitiveVert2D, SpriteComponent> SpriteManagerUI;
typedef CustomContainer2<PrimitiveVert2D, StringComponent> SpriteManagerString;
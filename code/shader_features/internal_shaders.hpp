#pragma once
#include <open_gl/vertex_array.hpp>
extern const char* primitive_vert_shader;
extern const char* primitive_geom_shader;
extern const char* primitive_frag_shader;
// extern const char* primitive_frag_shader_plus;

// extern const char* tilemap_vert_shader;
// extern const char* tilemap_c_vert_shader;

extern const char* simple_texture_vert_shader;
extern const char* simple_texture_geom_shader;
extern const char* simple_texture_frag_shader;
// extern const char* simple_texture_frag_shader_bg;
// extern const char* solid_color;

// extern const char* attributeless_vert_shader;
// extern const char* attributeless_vert_shader_unif_pos;
// extern const char* attributeless_frag_shader;
// extern const char* attributeless_frag_shader_grayscale;
// extern const char* attributeless_frag_shader_kernel;
// extern const char* attributeless_frag_shader_kernel_acc;


extern const VertexAttribute* primitive_attribs;
constexpr int num_primitive_attribs = 4;

struct PrimitiveVert2D
{
    glm::ivec2 pos;
    glm::ivec2 size;
    int shape;
    unsigned int color_RGBA;
};

enum PrimitiveShape_e
{
    PRIMITIVE_RECT, PRIMITIVE_ELIPSE, PRIMITIVE_LINE, PRIMITIVE_SKIP = -1
};
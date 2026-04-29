#include "internal_shaders.hpp"

const char* primitive_vert_shader = 
"#version 430 core\n"
"//precision highp::int;\n"
"//precision mediump::float;\n"
"layout (location = 0) in ivec2 aPos;\n"
"layout (location = 1) in ivec2 aSize;\n"
"layout (location = 2) in int shape;\n"
"layout (location = 3) in uint color;\n"
"out VS_OUT {\n"
"	vec2 size;\n"
"	int shape_type;\n"
"	ivec4 color;\n"
"} vs_out;\n"

"uniform vec2 SCREEN_SIZE;\n"
"uniform vec2 SCREEN_OFFSET;\n"

"void main()\n"
"{\n"
"	vec2 screen_delim = (SCREEN_SIZE / 2.0f);\n"
"	vec2 nPos = aPos + SCREEN_OFFSET;\n"
"	vec2 location = vec2(nPos.x, SCREEN_SIZE.y - nPos.y);\n"
"	vec2 aPosf = vec2(location.x, location.y);\n"
"	vec2 bPos = (aPosf / screen_delim) + vec2(-1.f);\n"
"	vs_out.size = vec2(aSize) / screen_delim;\n"
"	vs_out.shape_type = shape;\n"
"	vs_out.color = ivec4(color&0xff, (color >> 8)&0xff, (color >> 16)&0xff, (color >> 24));\n"
"	gl_Position = vec4(bPos, 0.0f, 1.0f);\n"
"}\n";
const char* primitive_geom_shader = 
"#version 430 core\n"
"#extension GL_EXT_geometry_shader : enable\n"
"#extension GL_OES_geometry_shader : enable\n"

"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 4) out;\n"

"in VS_OUT {\n"
"	vec2 size;\n"
"	int shape_type;\n"
"	ivec4 color;\n"
"} gs_in[];\n"

"out vec2 LocalCoords;\n"
"out vec2 RectSize;\n"
"out vec4 param_color;\n"
"out float state;\n"

"void main()\n"
"{\n"
"	vec4 p_color = vec4(gs_in[0].color) / 255.0;\n"
"	int my_shape_type = gs_in[0].shape_type;\n"
"	float my_state = float(my_shape_type);\n"
"	if(-1 == my_shape_type)\n"
"	{\n"
"		return;\n"
"	}\n"
"	else\n"
"	{\n"
"		vec2 size = gs_in[0].size;\n"
"		gl_Position = gl_in[0].gl_Position;\n"
"		LocalCoords = vec2(0.f, 0.f);\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"
"		gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].size.x, 0.f,0.f,0.f);\n"
"		LocalCoords = vec2(size.x, 0.f);\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"
"		gl_Position = gl_in[0].gl_Position + vec4(0.f, -gs_in[0].size.y,0.f,0.f);\n"
"		LocalCoords = vec2(0.f, size.y);\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"
"		gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].size.x, -gs_in[0].size.y,0.f,0.f);\n"
"		LocalCoords = size;\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"
"		EndPrimitive();\n"
"	}\n"
"}\n";

const char* primitive_frag_shader = 
"#version 430 core\n"
"//precision mediump::float;\n"

"in vec2 LocalCoords;\n"
"in vec2 RectSize;\n"
"in vec4 param_color;\n"
"in float state;\n"

"out vec4 FragColor;\n"

"uniform float PIX_SIZE;\n"
"uniform float LINE_WIDTH;\n"
"uniform vec2 SCREEN_SIZE;\n"

"void fillRect()\n"
"{\n"
"	FragColor = param_color;\n"
"}\n"

"void fillElipse()\n"
"{\n"
"	float radius = 0.5;\n"
"	vec2 dist = (LocalCoords / RectSize) - vec2(radius);\n"
"	float fdist = radius - length(dist)\n;"
"	if(fdist < 0.) discard;\n"
"	vec2 rect_in_pixels = radius * SCREEN_SIZE * RectSize;\n"
"	float pixel = PIX_SIZE / max(rect_in_pixels.x, rect_in_pixels.y);\n"
"	float alpha = smoothstep(0.0, pixel, fdist);\n"
"	FragColor = vec4(param_color.rgb, min(1., alpha));\n"
"}\n"

"void drawLine()\n"
"{\n"
"	vec2 rect_n = vec2(-RectSize.y, RectSize.x);\n"
"	vec2 rect_normal = rect_n / length(rect_n);\n"
"	float dist = dot(LocalCoords, rect_normal);\n"
"	float symmetry_times_screen_space_size = 2;"
"	vec2 rect_in_pixels = SCREEN_SIZE * symmetry_times_screen_space_size;\n"
"	float pixel = LINE_WIDTH / min(rect_in_pixels.x, rect_in_pixels.y);\n"
"	if(abs(dist) > pixel) discard;\n"
"	FragColor = param_color;\n"
"}\n"

"void main()\n"
"{\n"
"	if(state > 1.5f) drawLine();\n"
"	if(state > 0.5f) fillElipse();\n"
"	else			fillRect();\n"
"}\n"
;

const char* simple_texture_vert_shader = primitive_vert_shader;
const char* simple_texture_geom_shader =
"#version 430 core\n"
"#extension GL_EXT_geometry_shader : enable\n"
"#extension GL_OES_geometry_shader : enable\n"

"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 4) out;\n"

"in VS_OUT {\n"
"	vec2 size;\n"
"	int shape_type;\n"
"	ivec4 color;\n"
"} gs_in[];\n"

"out vec2 TexCoords;\n"
"out vec2 LocalCoords;\n"
"out vec2 RectSize;\n"
"out vec4 param_color;\n"
"out float state;\n"

"uniform vec2 SPRITE_MAT;\n"

"vec2 size_sprite()\n"
"{\n"
"	vec2 tex_pos = vec2(1.f / int(SPRITE_MAT.x), 1.f / int(SPRITE_MAT.y));\n"
"	return tex_pos;\n"
"}\n"

"vec2 from_sprite(int sprite_index)\n"
"{\n"
"	vec2 tex_pos = vec2(float(sprite_index % int(SPRITE_MAT.x)), float(sprite_index / int(SPRITE_MAT.x)));\n"
"	return tex_pos * size_sprite();\n"
"}\n"

"void main()\n"
"{\n"
"	vec4 p_color = vec4(gs_in[0].color) / 255.0;\n"
"	int my_sprite_ind = gs_in[0].shape_type;\n"
"	vec2 texture_start = from_sprite(my_sprite_ind);\n"
"	vec2 texture_end = texture_start + size_sprite();\n"
"	float my_state = float(my_sprite_ind);\n"
"	if(0.f > my_sprite_ind)\n"
"	{\n"
"		return;\n"
"	}\n"
"	else\n"
"	{\n"
"		vec2 size = gs_in[0].size;\n"
"		\n"
"		gl_Position = gl_in[0].gl_Position;\n"
"		TexCoords = texture_start;\n"
"		LocalCoords = vec2(0.f, 0.f);\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"

"		gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].size.x, 0.f,0.f,0.f);\n"
"		TexCoords = vec2(texture_end.x, texture_start.y);\n"
"		LocalCoords = vec2(size.x, 0.f);\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"

"		gl_Position = gl_in[0].gl_Position + vec4(0.f, -gs_in[0].size.y,0.f,0.f);\n"
"		TexCoords = vec2(texture_start.x, texture_end.y);\n"
"		LocalCoords = vec2(0.f, size.y);\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"

"		gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].size.x, -gs_in[0].size.y,0.f,0.f);\n"
"		TexCoords = texture_end;\n"
"		LocalCoords = size;\n"
"		RectSize = size;\n"
"		param_color = p_color;\n"
"		state = my_state;\n"
"		EmitVertex();\n"

"		EndPrimitive();\n"
"	}\n"
"}\n"
;
const char* simple_texture_frag_shader =
"#version 430 core\n"
"precision mediump float;\n"
"in vec2 TexCoords;\n"
"in vec2 LocalCoords;\n"
"in vec2 RectSize;\n"
"in vec4 param_color;\n"
"in float state;\n"

"out vec4 FragColor;\n"

"uniform sampler2D MY_TEX;\n"
"uniform vec4 MUL_COLOR;\n"

"vec4 DrawText(vec2 tex_cord, vec4 text_color)\n"
"{\n"
"	vec4 tex_color = text_color * texture(MY_TEX, tex_cord);\n"
"	return tex_color;\n"
"}\n"

"void main()\n"
"{\n"
"	vec4 TexColor = DrawText(TexCoords, MUL_COLOR) * param_color;\n"
"	if(TexColor.a < 0.01) discard;\n"
"	FragColor = TexColor;\n"
"}\n"
;

constexpr VertexAttribute attribs[] =
{
	{VAT_INT, 2, 0},
	{VAT_INT, 2, 0},
	{VAT_INT, 1, 0},
	{VAT_UINT, 1, 0}

};

const VertexAttribute* primitive_attribs = attribs;

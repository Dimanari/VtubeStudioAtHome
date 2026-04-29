#pragma once
#include "shader.hpp"

class Shader : public ShaderBase
{
public:
	Shader();
	enum ShaderType_e
	{
		SHADE_VERT, SHADE_GEOM, SHADE_FRAG, SHADE_MAX_SUPPORT
	};
	unsigned int shader_component[SHADE_MAX_SUPPORT];
	bool shader_component_link[SHADE_MAX_SUPPORT];
	// void Premake(const char* shader_path, ShaderType_e shader_type);
	void PremakeString(const char* shader_source, const char* name, ShaderType_e shader_type);
	void Link();
	void Release();
	~Shader();
};
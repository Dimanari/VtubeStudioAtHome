//#include <utility/read_text_file.hpp>
#include "shader_render.hpp"

Shader::Shader() : shader_component{ 0 }, shader_component_link{ false }
{
}

void Shader::PremakeString(const char* shader_source, const char* name, ShaderType_e shader_type)
{
	int success;
	char infoLog[512];
	const char* typestring;
	GLenum shader_type_gl;

	switch (shader_type)
	{
	case SHADE_VERT:
		typestring = "VERT";
		shader_type_gl = GL_VERTEX_SHADER;
		break;
	case SHADE_GEOM:
		typestring = "GEOM";
		shader_type_gl = GL_GEOMETRY_SHADER;
		break;
	case SHADE_FRAG:
		typestring = "FRAG";
		shader_type_gl = GL_FRAGMENT_SHADER;
		break;
	default:
		printf("ERROR::SHADER::UNKNOWN_TYPE %s\n", name);
		return;
	}

	unsigned int shader_temp = glCreateShader(shader_type_gl);
	glShaderSource(shader_temp, 1, &shader_source, NULL);

	glCompileShader(shader_temp);
	// print compile errors if any
	glGetShaderiv(shader_temp, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader_temp, 512, NULL, infoLog);
		printf("ERROR::SHADER::%s::COMPILATION_FAILED %s\n%s\n", typestring, name, infoLog);
		return;
	}
	shader_component_link[shader_type] = true;
	shader_component[shader_type] = shader_temp;
}

void Shader::Link()
{
	int success;
	char infoLog[512];
	program_id = glCreateProgram();
	for (int i = 0; i < SHADE_MAX_SUPPORT; ++i)
	{
		if (shader_component_link[i])
			glAttachShader(program_id, shader_component[i]);
	}
	glLinkProgram(program_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program_id, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}

	for (int i = 0; i < SHADE_MAX_SUPPORT; ++i)
	{
		if (shader_component_link[i])
		{
			shader_component_link[i] = false;
			glDeleteShader(shader_component[i]);
			shader_component[i] = 0;
		}
	}
}

void Shader::Release()
{
	for (int i = 0; i < SHADE_MAX_SUPPORT; ++i)
	{
		if (shader_component_link[i])
		{
			shader_component_link[i] = false;
			glDeleteShader(shader_component[i]);
			shader_component[i] = 0;
		}
	}
	if (program_id)
	{
		glDeleteProgram(program_id);
		program_id = 0;
	}
}

Shader::~Shader()
{
	Release();
}
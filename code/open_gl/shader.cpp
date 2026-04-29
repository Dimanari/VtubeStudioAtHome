// #include <utility/read_text_file.hpp>
#include "shader.hpp"

ShaderBase::ShaderBase() : program_id(0)
{
}

void ShaderBase::Use()
{
	glUseProgram(program_id);
}

void ShaderBase::Unuse()
{
	glUseProgram(0);
}

GLint ShaderBase::UniformLocation(const std::string& name) const
{
	return glGetUniformLocation(program_id, name.c_str());
}

void ShaderBase::setBool(GLint location, bool value)
{
	glUniform1i(location, (int)value);
}

void ShaderBase::setInt(GLint location, int value)
{
	glUniform1i(location, value);
}
void ShaderBase::setFloat(GLint location, float value)
{
	glUniform1f(location, value);
}
void ShaderBase::setFloat2(GLint location, float valx, float valy)
{
	glUniform2f(location, valx, valy);
}

void ShaderBase::setFloat2(GLint location, const glm::vec2 _vec2)
{
	setFloat2(location, _vec2.x, _vec2.y);
}
void ShaderBase::setFloat3(GLint location, float valx, float valy, float valz)
{
	glUniform3f(location, valx, valy, valz);
}

void ShaderBase::setFloat3(GLint location, const glm::vec3 _vec3)
{
	setFloat3(location, _vec3.x, _vec3.y, _vec3.z);
}

void ShaderBase::setFloat4(GLint location, float valx, float valy, float valz, float valw)
{
	glUniform4f(location, valx, valy, valz, valw);
}

void ShaderBase::setFloat4(GLint location, const glm::vec4 _vec4)
{
	setFloat4(location, _vec4.x, _vec4.y, _vec4.z, _vec4.w);
}

void ShaderBase::setMatrixF4(GLint location, const glm::mat4& matr, bool transpose)
{
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(matr));
}

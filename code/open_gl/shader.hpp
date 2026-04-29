#pragma once
#include <string>
#include <base/includes.hpp>

class ShaderBase
{
public:
	ShaderBase();
	unsigned int program_id;
	void Use();
	static void Unuse();
	// optimizing uniform access
	GLint UniformLocation(const std::string& name) const;
	static void setBool(GLint location, bool value);
	static void setInt(GLint location, int value);
	static void setFloat(GLint location, float value);
	static void setFloat2(GLint location, float valx, float valy);
	static void setFloat2(GLint location, const glm::vec2 _vec2);
	static void setFloat3(GLint location, float valx, float valy, float valz);
	static void setFloat3(GLint location, const glm::vec3 _vec3);
	static void setFloat4(GLint location, float valx, float valy, float valz, float valw);
	static void setFloat4(GLint location, const glm::vec4 _vec4);
	static void setMatrixF4(GLint location, const glm::mat4& matr, bool transpose = GL_FALSE);
};



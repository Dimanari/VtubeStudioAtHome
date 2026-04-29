#pragma once
#include <open_gl/vertex_array.hpp>
#include <open_gl/vertex_buffer.hpp>
#include <open_gl/shader_render.hpp>

class SimpleRenderer
{
public:
	void SetupRenderer(unsigned int stride, const VertexAttribute* const attribs, unsigned int num_attrib, GLenum usage, const char* data = nullptr, unsigned int elements = 0);
	void Draw(GLint draw_as);
	void UpdateData(const char* data, unsigned int elements);

	// void Premake(const char* shader_path, Shader::ShaderType_e shader_type);
	void PremakeString(const char* shader_source, const char* name, Shader::ShaderType_e shader_type);
	void Link();

	inline ShaderBase& GetShade() { return m_shade; }
	inline VertexBuffer& GetVBO() { return m_buffer; }
	inline VertexArray& GetVAO() { return m_array; }

	void Release();
private:
	Shader m_shade;
	VertexBuffer m_buffer;
	VertexArray m_array;
};
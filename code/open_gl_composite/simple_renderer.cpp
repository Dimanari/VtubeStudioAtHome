#include "simple_renderer.hpp"

void SimpleRenderer::SetupRenderer(unsigned int stride, const VertexAttribute* const attribs, unsigned int num_attrib, GLenum usage, const char* data, unsigned int elements)
{
	m_array.MakeVertexArray();
	m_array.Bind();
	m_buffer.SetupBuffer(data, elements, stride, usage);
	m_array.SetupBufferLayout(stride, attribs, num_attrib);
}

void SimpleRenderer::Release()
{
	m_array.Release();
	m_buffer.Release();
	m_shade.Release();
}

void SimpleRenderer::Draw(GLint draw_as)
{
	m_shade.Use();
	m_array.RenderData(draw_as, 0, m_buffer.Elements());
}

void SimpleRenderer::UpdateData(const char* data, unsigned int elements)
{
	m_buffer.UpdateData(data, elements);
}

void SimpleRenderer::PremakeString(const char* shader_source, const char* name, Shader::ShaderType_e shader_type)
{
	m_shade.PremakeString(shader_source, name, shader_type);
}
void SimpleRenderer::Link()
{
	m_shade.Link();
}
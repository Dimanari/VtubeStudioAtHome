#include "vertex_buffer.hpp"

void VertexBuffer::SetupBuffer(const char* data, unsigned int elements, unsigned int stride, GLenum usage)
{
	glGenBuffers(1, &VBO);
	_stride = stride;
	_usage = usage;
	_elements = elements;
	UpdateData(data, elements);
}

void VertexBuffer::UpdateData(const char* data, unsigned int elements)
{
	Bind();
	UpdateVBONoBind(data, elements);
}

void VertexBuffer::UpdateVBONoBind(const char* data, unsigned int elements)
{
	_elements = elements;
	glBufferData(GL_ARRAY_BUFFER, _stride * _elements, data, _usage);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Release()
{
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
}

VertexBuffer::~VertexBuffer()
{
	Release();
}
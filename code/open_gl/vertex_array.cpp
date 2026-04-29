#include "vertex_array.hpp"

void VertexArray::MakeVertexArray()
{
	glGenVertexArrays(1, &VAO);
}

void VertexArray::Bind()
{
	glBindVertexArray(VAO);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::Release()
{
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

VertexArray::~VertexArray()
{
	Release();
}

void VertexArray::SetupBufferLayout(unsigned int stride, const VertexAttribute* const attribs, unsigned int num_attrib)
{
	_stride = stride;
	size_t offset = 0;
	for (int i = 0; i < (int)num_attrib; ++i)
	{
		offset += AddAttrib(i, stride, offset, attribs[i]);
	}
}

void VertexArray::RenderData(GLint draw_as, int start, int amount)
{
	Bind();
	glDrawArrays(draw_as, start, amount);
}


unsigned int VertexArray::AddAttrib(unsigned int attribIndex, unsigned int stride, unsigned int offset, const VertexAttribute attrib)
{
	glEnableVertexAttribArray(attribIndex);
	unsigned int element_size, attribute_size;
	GLenum _gl_type;
	switch (attrib._type)
	{
	case VAT_BOOL:
		element_size = sizeof(char);
		_gl_type = GL_BOOL;
		break;
	case VAT_BYTE:
		element_size = sizeof(char);
		_gl_type = GL_BYTE;
		break;
	case VAT_SHORT:
		element_size = sizeof(short);
		_gl_type = GL_SHORT;
		break;
	case VAT_INT:
		element_size = sizeof(int);
		_gl_type = GL_INT;
		break;
	case VAT_UINT:
		element_size = sizeof(int);
		_gl_type = GL_UNSIGNED_INT;
		break;
	case VAT_DOUBLE:
		element_size = sizeof(double);
		_gl_type = GL_DOUBLE;
		break;
	case VAT_FLOAT:
	default:
		element_size = sizeof(float);
		_gl_type = GL_FLOAT;
		break;
	}
	attribute_size = element_size * attrib._elements;
	switch (attrib._type)
	{
	case VAT_DOUBLE:
	case VAT_FLOAT:
		glVertexAttribPointer(attribIndex, attrib._elements, _gl_type, GL_FALSE, stride, (void*)((size_t)offset));
		break;
	default:
		glVertexAttribIPointer(attribIndex, attrib._elements, _gl_type, stride, (void*)((size_t)offset));
	}

	return attribute_size + attrib._pad_after;
}
void VertexArray::InstanceAttrib(unsigned int attribIndex, unsigned int isInstanced)
{
	glVertexAttribDivisor(attribIndex, isInstanced);
}
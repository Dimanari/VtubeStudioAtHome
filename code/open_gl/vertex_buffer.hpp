#pragma once
#include <base/includes.hpp>

class VertexBuffer
{
public:
	void SetupBuffer(const char* data, unsigned int elements, unsigned int stride, GLenum usage);
	// normal use, binds the VBO then updates
	void UpdateData(const char* data, unsigned int elements);
	// when VBO is already bound, only update
	void UpdateVBONoBind(const char* data, unsigned int elements);
	void Bind();
	static void Unbind();
	void Release();
	unsigned int Elements() const { return _elements;}
	~VertexBuffer();
private:
	unsigned int VBO;
	unsigned int _stride;
	unsigned int _elements;
	GLenum _usage;
};
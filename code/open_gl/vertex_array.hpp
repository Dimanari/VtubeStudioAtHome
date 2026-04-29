#pragma once
#include <base/includes.hpp>

enum VertexAttribType_e
{
	VAT_BOOL, VAT_BYTE, VAT_SHORT, VAT_INT, VAT_UINT, VAT_FLOAT, VAT_DOUBLE, VAT_MAX_ENUM
};

struct VertexAttribute
{
	VertexAttribType_e _type;
	unsigned int _elements;
	unsigned int _pad_after;
};

class VertexArray
{
public:
	void MakeVertexArray();
	void SetupBufferLayout(unsigned int stride, const VertexAttribute* const attribs, unsigned int num_attrib);
	
	unsigned int AddAttrib(unsigned int attribIndex, unsigned int stride, unsigned int offset, const VertexAttribute attrib);
	void InstanceAttrib(unsigned int attribIndex, unsigned int isInstanced);
	void Bind();
	static void Unbind();

	void RenderData(GLint draw_as, int start, int amount);
	void Release();
	~VertexArray();
private:
	unsigned int VAO;
	unsigned int _stride;
};
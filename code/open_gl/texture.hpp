#pragma once
#include <base/includes.hpp>
// another optimization we can do is tracking currently bound asset and avoid asset bind to an already bound resource.
// this will use a static integer managed by our class that updates on Bind and Unbind.
class Texture
{
public:
	// makes an empty texture
	void MakeRAW(unsigned int width, unsigned int height, GLint format = GL_RGBA, void* pixels = NULL);
	void UpdatePix(void* pixels);

	void UpdatePixDims(unsigned int width, unsigned int height, void* pixels);
	// make texture from surface
	void MakeFromSurface(SDL_Surface* source);

	// add sprite image to texture from a surface
	void RenderSubSurface(SDL_Surface* source, unsigned int pos_x, unsigned int pos_y);
	void FrameTexture(int attach, int location);
	// bind the texture
	void Bind();
	void BindToTex(int tex);
	static void ResetActiveTexture();
	static void Unbind();
	
	// works on current active texture(GL_TEXTURE_2D)
	static void TexParam(GLuint field, GLint value);

	GLuint GetTex() const { return m_texture; }
	GLuint GetFormat() const { return m_format; }
	GLuint64 GetHandle() const { return m_handle; }
	glm::ivec2 GetSize() const { return m_dims; }
	
	void Release();
	~Texture();
	Texture();
private:
	// no real need to make this public
	GLuint64 m_handle;
	void GenTexture();
	GLuint m_texture;
	glm::ivec2 m_dims;
	GLint m_format;
};
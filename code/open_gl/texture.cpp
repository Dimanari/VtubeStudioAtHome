#include "texture.hpp"

void Texture::GenTexture()
{
	glGenTextures(1, &m_texture);
	m_handle = glGetTextureHandleARB(m_texture);
}

void Texture::MakeRAW(unsigned int width, unsigned int height, GLint format, void* pixels)
{
	if (0 == m_texture)
		GenTexture();
	Bind();
	// pixel storage packing
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// texture filtering - Nearest
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// create an empty texture
	m_dims = {width, height};
	m_format = format;
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_dims.x, m_dims.y, 0, m_format,
		GL_UNSIGNED_BYTE, pixels);
}

void Texture::UpdatePix(void* pixels)
{
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_dims.x, m_dims.y, 0, m_format,
		GL_UNSIGNED_BYTE, pixels);
}

void Texture::UpdatePixDims(unsigned int width, unsigned int height, void* pixels)
{
	m_dims = {width, height};
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_dims.x, m_dims.y, 0, m_format,
		GL_UNSIGNED_BYTE, pixels);
}
void Texture::MakeFromSurface(SDL_Surface* source)
{
	int format = GL_RGB;

	if (source->format->BytesPerPixel == 4) {
		format = GL_RGBA;
	}

	MakeRAW(source->w, source->h, format, source->pixels);
}

void Texture::RenderSubSurface(SDL_Surface* source, unsigned int pos_x, unsigned int pos_y)
{
	int format = GL_RGB;

	if (source->format->BytesPerPixel == 4) {
		format = GL_RGBA;
	}
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		pos_x, pos_y,
		source->w, source->h,
		format, GL_UNSIGNED_BYTE,
		source->pixels);
}

void Texture::FrameTexture(int attach, int location)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attach, GL_TEXTURE_2D, m_texture, location);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::BindToTex(int tex)
{
	glActiveTexture(GL_TEXTURE0 + tex);
	Bind();
}

void Texture::ResetActiveTexture()
{
	glActiveTexture(GL_TEXTURE0);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::TexParam(GLuint field, GLint value)
{
	glTexParameteri(GL_TEXTURE_2D, field, value);
}

void Texture::Release()
{
	if (0 != m_texture)
		glDeleteTextures(1, &m_texture);
	m_texture = 0;
}

Texture::~Texture()
{
	Release();
}

Texture::Texture() : m_texture(0), m_dims(0), m_format(0)
{
}

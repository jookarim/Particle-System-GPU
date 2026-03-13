#include "Texture.h"
#include <string>

void Texture::loadFromFile(std::string_view path)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        glDeleteTextures(1, &m_id);
        throw std::runtime_error(std::string("Failed to load texture: ") + std::string(path));
    }

    GLenum dataFormat = 0;
    GLenum internalFormat = 0;

    if (nrChannels == 1)
    {
        dataFormat = GL_RED;
        internalFormat = GL_R8;
    }
    else if (nrChannels == 3)
    {
        dataFormat = GL_RGB;
        internalFormat = GL_SRGB8;
    }
    else if (nrChannels == 4)
    {
        dataFormat = GL_RGBA;
        internalFormat = GL_SRGB8_ALPHA8;
    }

    int levels = std::floor(std::log2(std::max(width, height))) + 1;

    glTextureStorage2D(m_id, levels, internalFormat, width, height);

    glTextureSubImage2D(m_id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(m_id);

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
}


void Texture::destroy() noexcept
{
	if (m_id)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	if (m_handle)
	{
		glMakeTextureHandleNonResidentARB(m_handle);
		m_handle = 0;
	}
}

Texture::Texture(std::string_view path)
{
	loadFromFile(path);
}

Texture::~Texture() noexcept
{
	destroy();
}

void Texture::bind(unsigned int bindingIndex)
{
	if(!m_handle && m_id)
		glBindTextureUnit(bindingIndex, m_id);
}

Texture::Texture(Texture&& other) noexcept
{
	m_id = other.m_id;

	if (other.m_handle)
	{
		m_handle = other.m_handle;
	}

	other.m_id = 0;
	other.m_handle = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this == &other) return *this;

	destroy();

	m_id = other.m_id;

	if (other.m_handle)
	{
		m_handle = other.m_handle;
	}

	other.m_id = 0;
	other.m_handle = 0;

	return *this;
}

void Texture::makeResident()
{
	m_handle = glGetTextureHandleARB(m_id);
	glMakeTextureHandleResidentARB(m_handle);
}
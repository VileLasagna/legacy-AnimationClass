#include "Texture.h"
#include <GL/glu.h>
#include <SDL_Image.h>
#include "assert.h"

Texture::Texture()
{
    m_textureId = -1;
}

bool Texture::LoadTexture(const std::string& filename)
{
	glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	SDL_Surface* surf = IMG_Load(filename.c_str());
	assert(surf);
	unsigned char* data = (unsigned char*)surf->pixels;
	int width = surf->w;
	int height = surf->h;

    if (surf->format->BytesPerPixel == 3)
    {
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            data);

        gluBuild2DMipmaps(
            GL_TEXTURE_2D,  
            GL_RGB,
            width,
            height,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            data);
    }
    else if (surf->format->BytesPerPixel == 4)
    {
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data);

        gluBuild2DMipmaps(
            GL_TEXTURE_2D,
            GL_RGBA,
            width,
            height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data);
    }
    else
    {
        // Unexpected format
        assert(0);
    }

    SDL_FreeSurface(surf);

    return true;
}

Texture::~Texture()
{
    if (m_textureId > -1)
    {
    	glDeleteTextures(1, &m_textureId);
    }
}

void Texture::UseThisTexture()
{
	glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, m_textureId);
}


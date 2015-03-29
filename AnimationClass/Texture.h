#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include <string>

class Texture
{
public:
	Texture();
	~Texture();

    bool LoadTexture(const std::string& filename);
	void UseThisTexture();

private:
	GLuint m_textureId;
};

#endif
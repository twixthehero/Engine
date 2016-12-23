#include "TextureManager.h"
#include <FreeImage.h>
#include <iostream>
#include <glew\glew.h>
#include <Core\Texture.h>

TextureManager* TextureManager::_instance = nullptr;

TextureManager::TextureManager()
{
	_textures = std::map<std::string, Texture*>();

	_missingTexture = GetTexture("missing.png");
}


TextureManager::~TextureManager()
{
	for (std::map<std::string, Texture*>::iterator it = _textures.begin(); it != _textures.end(); it++)
		delete it->second;
}

void TextureManager::Init()
{
	FreeImage_Initialise(true);

	_instance = new TextureManager();
}

TextureManager* TextureManager::GetInstance()
{
	return _instance;
}

void TextureManager::Shutdown()
{
	if (_instance != nullptr)
	{
		delete _instance;
		_instance = nullptr;
	}

	FreeImage_DeInitialise();
}

Texture* TextureManager::GetTexture(std::string name)
{
	if (_textures.find(name) == _textures.end())
		if (!LoadTexture(name))
			return _missingTexture;

	return _textures[name];
}

//http://r3dux.org/2014/10/how-to-load-an-opengl-texture-using-the-freeimage-library-or-freeimageplus-technically/
bool TextureManager::LoadTexture(std::string name)
{
	std::string path = "textures\\" + name;
	const char* filename = path.c_str();

	std::cout << "loading texture: " << filename << std::endl;

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1)
	{
		std::cout << "Could not find image: " << filename << std::endl;
		return false;
	}

	if (format == FIF_UNKNOWN)
	{
		std::cout << "Couldn't determine file format - attemping by extension..." << std::endl;

		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format))
		{
			std::cout << "Detected image format that cannot be read!" << std::endl;
			return false;
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;

	if (bitsPerPixel == 32)
	{
		//std::cout << "skipping conversion" << std::endl;
		bitmap32 = bitmap;
	}
	else
	{
		//std::cout << "converting to 32bits" << std::endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);

	//std::cout << "width: " << imageWidth << std::endl;
	//std::cout << "height: " << imageHeight << std::endl;

	GLubyte* textureData = FreeImage_GetBits(bitmap32);

	GLuint tempTextureId;
	glGenTextures(1, &tempTextureId);
	glBindTexture(GL_TEXTURE_2D, tempTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minificationFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magnificationFilter);

	if (_minificationFilter == GL_LINEAR_MIPMAP_LINEAR ||
		_minificationFilter == GL_LINEAR_MIPMAP_NEAREST ||
		_magnificationFilter == GL_NEAREST_MIPMAP_LINEAR ||
		_magnificationFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		//std::cout << "generating mipmaps" << std::endl;
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	GLenum error = glGetError();

	if (error)
	{
		std::cout << "error loading texture: " << filename << std::endl;

		switch (error)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "Invalid value" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation" << std::endl;
			break;
		default:
			std::cout << "Unrecognized GLenum: " << error << std::endl;
			break;
		}

		std::cout << "see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details" << std::endl;
	}

	FreeImage_Unload(bitmap32);

	if (bitsPerPixel != 32)
		FreeImage_Unload(bitmap);

	std::cout << "loaded texture '" << name << "' to id " << tempTextureId << std::endl;
	_textures.insert(std::pair<std::string, Texture*>(name, new Texture(tempTextureId)));

	return true;
}
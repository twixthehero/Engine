#include "TextureManager.h"
#include <FreeImage.h>
#include <iostream>
#include <glew\glew.h>
#include <Core\Texture.h>
#include "Logger.h"

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

	Logger::WriteLine("loading texture: " + path);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1)
	{
		Logger::WriteLine("Could not find image: " + path);
		return false;
	}

	if (format == FIF_UNKNOWN)
	{
		Logger::WriteLine("Couldn't determine file format - attemping by extension...");

		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format))
		{
			Logger::WriteLine("Detected image format that cannot be read!");
			return false;
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;

	if (bitsPerPixel == 32)
	{
		//Logger::WriteLine("skipping conversion");
		bitmap32 = bitmap;
	}
	else
	{
		//Logger::WriteLine("converting to 32bits");
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);

	//Logger::WriteLine("width: " + std::to_string(imageWidth));
	//Logger::WriteLine("height: " + std::to_string(imageHeight));

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
		Logger::WriteLine("Error loading texture: " + path);

		switch (error)
		{
		case GL_INVALID_ENUM:
			Logger::WriteLine("Invalid enum");
			break;
		case GL_INVALID_VALUE:
			Logger::WriteLine("Invalid value");
			break;
		case GL_INVALID_OPERATION:
			Logger::WriteLine("Invalid operation");
			break;
		default:
			Logger::WriteLine("Unrecognized GLenum: " + std::to_string(error));
			break;
		}

		Logger::WriteLine("See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details");
	}

	FreeImage_Unload(bitmap32);

	if (bitsPerPixel != 32)
		FreeImage_Unload(bitmap);

	Logger::WriteLine("loaded texture '" + name + "' to id " + std::to_string(tempTextureId));
	_textures.insert(std::pair<std::string, Texture*>(name, new Texture(tempTextureId)));

	return true;
}
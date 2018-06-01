#include "TextureManager.h"
#include <FreeImage.h>
#include <iostream>
#include <GL/gl3w.h>
#include <Core/Texture.h>
#include "Logger.h"
#include "Utils.h"

namespace VoxEngine
{
	TextureManager* TextureManager::_instance = nullptr;

	TextureManager::TextureManager()
	{
		_textures = std::map<std::string, Texture*>();

		_missingTexture = GetTexture("missing.png");
		_missingTextureCube = GetTextureCube("missing.png");
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
#if defined _WIN32
		std::string path = "textures\\" + name;
#else
		std::string path = "textures/" + name;
#endif
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
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		}

		if (Utils::CheckGLError("Error loading texture: " + path))
			Logger::WriteLine("See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details");

		glBindTexture(GL_TEXTURE_2D, 0);

		FreeImage_Unload(bitmap32);

		if (bitsPerPixel != 32)
			FreeImage_Unload(bitmap);

		Logger::WriteLine("loaded texture '" + name + "' to id " + std::to_string(tempTextureId));
		_textures.insert(std::pair<std::string, Texture*>(name, new Texture(tempTextureId)));

		return true;
	}

	Texture* TextureManager::GetTextureCube(std::string name)
	{
		if (_textures.find(name) != _textures.end())
			return _textures[name];

		return nullptr;
	}

	bool TextureManager::LoadTextureCube(std::string name,
		std::string front, std::string back,
		std::string left, std::string right,
		std::string top, std::string bottom)
	{
		if (_textures.find(name) != _textures.end())
			return false;

		std::vector<std::string> names;
		names.push_back(left);
		names.push_back(right);
		names.push_back(top);
		names.push_back(bottom);
		names.push_back(front);
		names.push_back(back);

		GLuint tempTextureId;
		glGenTextures(1, &tempTextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tempTextureId);

		for (int i = 0; i < 6; i++)
		{
#if defined _WIN32
			std::string path = "textures\\" + name + "\\" + names[i];
#else
			std::string path = "textures/" + name + "/" + names[i];
#endif
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

			bool result = FreeImage_FlipVertical(bitmap32);

			if (!result)
			{
				Logger::WriteLine("Unable to flip cube texture vertically: " + names[i]);
			}

			int imageWidth = FreeImage_GetWidth(bitmap32);
			int imageHeight = FreeImage_GetHeight(bitmap32);

			//Logger::WriteLine("width: " + std::to_string(imageWidth));
			//Logger::WriteLine("height: " + std::to_string(imageHeight));

			GLubyte* textureData = FreeImage_GetBits(bitmap32);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureData);

			if (Utils::CheckGLError("Error loading texture: " + path))
				Logger::WriteLine("See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details");

			FreeImage_Unload(bitmap32);

			if (bitsPerPixel != 32)
				FreeImage_Unload(bitmap);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, _minificationFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, _magnificationFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if (_minificationFilter == GL_LINEAR_MIPMAP_LINEAR ||
			_minificationFilter == GL_LINEAR_MIPMAP_NEAREST ||
			_magnificationFilter == GL_NEAREST_MIPMAP_LINEAR ||
			_magnificationFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			//std::cout << "generating mipmaps" << std::endl;
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		Logger::WriteLine("loaded texture cube '" + name + "' to id " + std::to_string(tempTextureId));
		_textures.insert(std::pair<std::string, Texture*>(name, new Texture(tempTextureId)));

		return true;
	}
}

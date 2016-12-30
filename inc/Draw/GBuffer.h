#pragma once
#include <GL\gl3w.h>

namespace VoxEngine
{
	class GBuffer
	{
	public:
		enum GBufferTextureType
		{
			Position, Diffuse, Normal, TextureCoordinate, NumTextures
		};

		GBuffer();
		~GBuffer();

		bool Init(int width, int height);
		void BindForWriting();
		void BindForReading();
	private:
		GLuint _fbo;
		GLuint _textures[NumTextures];
		GLuint _depthTexture;
	};
}

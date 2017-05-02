#pragma once
#include <GL\gl3w.h>

namespace VoxEngine
{
	class GBuffer
	{
	public:
		enum GBufferTextureType
		{
			Position, Diffuse, Normal, Specular, NumTextures
		};

		GBuffer();
		~GBuffer();

		bool Init(int width, int height);

		void StartFrame();
		void BindForGeometryPass();
		void BindForStencilPass();
		void BindForLightPass();
		void BindForFinalPass();
	private:
		GLuint _fbo;
		GLuint _textures[NumTextures];
		GLuint _depthTexture;
		GLuint _finalTexture;
	};
}

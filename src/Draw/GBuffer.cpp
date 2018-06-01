#include "Draw/GBuffer.h"
#include "Logger.h"

namespace VoxEngine
{
	GBuffer::GBuffer()
	{
	}

	GBuffer::~GBuffer()
	{
		glDeleteFramebuffers(1, &_fbo);
	}

	bool GBuffer::Init(int width, int height)
	{
		//create the FBO
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

		//create the gbuffer textures
		glGenTextures(GBufferTextureType::NumTextures, _textures);
		glGenTextures(1, &_depthTexture);
		glGenTextures(1, &_finalTexture);

		for (int i = 0; i < GBufferTextureType::NumTextures; i++)
		{
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, _depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

		glBindTexture(GL_TEXTURE_2D, _finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, _finalTexture, 0);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::WriteLine("Framebuffer error: " + std::to_string(status));
			return false;
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	void GBuffer::StartFrame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::BindForGeometryPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(GBufferTextureType::NumTextures, drawBuffers);
	}

	void GBuffer::BindForStencilPass()
	{
		glDrawBuffer(GL_NONE);
	}

	void GBuffer::BindForLightPass()
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT4);

		for (int i = 0; i < GBufferTextureType::NumTextures; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _textures[GBufferTextureType::Position + i]);
		}
	}

	void GBuffer::BindForFinalPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
		glReadBuffer(GL_COLOR_ATTACHMENT4);
	}
}

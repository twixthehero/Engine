#include "Draw\GBuffer.h"
#include "Logger.h"

namespace VoxEngine
{
	GBuffer::GBuffer()
	{
	}

	GBuffer::~GBuffer()
	{
	}

	bool GBuffer::Init(int width, int height)
	{
		//create the FBO
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

		//create the gbuffer textures
		glGenTextures(GBufferTextureType::NumTextures, _textures);
		glGenTextures(1, &_depthTexture);

		for (int i = 0; i < GBufferTextureType::NumTextures; i++)
		{
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, _depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(GBufferTextureType::NumTextures, drawBuffers);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::WriteLine("Framebuffer error: " + std::to_string(status));
			return false;
		}

		//glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	void GBuffer::BindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	}

	void GBuffer::BindForReading()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		for (int i = 0; i < GBufferTextureType::NumTextures; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _textures[GBufferTextureType::Position + i]);
		}
	}
}

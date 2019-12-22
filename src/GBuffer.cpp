#include "GBuffer.h"
#include <GL/glew.h>
#include "Texture.h"

GBuffer::GBuffer()
	:bufferID(0) {

}

GBuffer::~GBuffer() {
}

bool GBuffer::Create(int width, int height) {
	// Create the framebuffer object
	glGenFramebuffers(1, &bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);

	// Add a depth buffer to this target
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// Create textures for each output in the G-buffer
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
		Texture* tex = new Texture();
		// We want three 32-bit float components for each texture
		tex->CreateForRendering(width, height, GL_RGB32F);
		textures.emplace_back(tex);
		// Attach this texture to a color output
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
			tex->GetTextureID(), 0);
	}

	// Create a vector of the color attachments
	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// Set the list of buffers to draw to
	glDrawBuffers(static_cast<GLsizei>(attachments.size()),
		attachments.data());

	// Make sure everything worked
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Destroy();
		return false;
	}

	return true;
}

void GBuffer::Destroy() {
	glDeleteFramebuffers(1, &bufferID);
	for (Texture* t : textures) {
		t->Unload();
		delete t;
	}
}

Texture* GBuffer::GetTexture(Type type) {
	if (textures.size() > 0) {
		return textures[type];
	}
	else {
		return nullptr;
	}
}

void GBuffer::SetTexturesActive() {
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
		textures[i]->SetActive(i);
	}
}
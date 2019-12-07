#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Actor* newOwner, int newDrawOrder)
	:Component(newOwner),
	texture(nullptr),
	drawOrder(newDrawOrder),
	texWidth(0),
	texHeight(0),
	visible(true)
{
	owner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent(){
	owner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader){
	if (texture){
		// Scale the quad by the width/height of texture
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(texWidth),
			static_cast<float>(texHeight), 1.0f);
		
		Matrix4 world = scaleMat * owner->GetWorldTransform();

		// Set world transform
		shader->SetMatrixUniform("uWorldTransform", world);
		// Set current texture
		texture->SetActive();
		// Draw quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* newTexture){
	texture = newTexture;
	// Set width/height
	texWidth = texture->GetWidth();
	texHeight = texture->GetHeight();
}

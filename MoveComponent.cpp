#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* newOwner, int updateOrder)
	:Component(newOwner, updateOrder),
	horizontalMove(0.0f),
	verticalMove(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime){
	if (!Math::NearZero(verticalMove)){
		Vector3 pos = owner->GetPosition() + Vector3(verticalMove, .0f, .0f);
		owner->SetPosition(pos);
	} else if (!Math::NearZero(horizontalMove)){
		Vector3 pos = owner->GetPosition() + Vector3(.0f, -horizontalMove, .0f);
		owner->SetPosition(pos);
	}
}

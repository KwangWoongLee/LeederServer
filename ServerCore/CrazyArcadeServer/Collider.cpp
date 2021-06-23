#include "stdafx.h"
#include "Collider.h"

Collider::Collider(GameObject* owner)
	: mOwner(owner)
{
}

void Collider::Update(float deltaTime)
{
}

bool Collider::IsIntersect(Collider& other, float* deltaX, float* deltaY)
{

	//simple collision test for spheres- are the radii summed less than the distance?
	Position targetLocation = other.GetPosition();
	float targetRadius = other.GetRadius();

	Position delta = { targetLocation.mX - GetPosition().mX , targetLocation.mY - GetPosition().mY };

	float distSq = std::pow(delta.mX, 2) + std::pow(delta.mY,2);

	float collisionDist = (GetRadius() + targetRadius);
	if (distSq < (collisionDist * collisionDist))
	{
		Position dirToTarget = delta;

		float length = std::sqrtf(distSq);
		if (length != 0)
		{
			dirToTarget.mX /= length;
			dirToTarget.mY /= length;
		}
		

		Position acceptableDeltaFromSourceToTarget;
		acceptableDeltaFromSourceToTarget.mX = dirToTarget.mX * collisionDist;
		acceptableDeltaFromSourceToTarget.mY = dirToTarget.mY * collisionDist;

		*deltaX = acceptableDeltaFromSourceToTarget.mX;
		*deltaY = acceptableDeltaFromSourceToTarget.mY;

		printf("%f, %f \n", *deltaX, *deltaY);

		return true;

	}

	return false;
}

#pragma once
#include "stdafx.h"

class Collider
{
public:
	Collider(GameObject* owner);

	const Position& GetPosition() const { return mOwner->GetPosition(); }

	void Update(float deltaTime);
	
	float& GetRadius() { return mRadius; }
	void SetRadius(float radius) { mRadius = radius; }

	bool IsIntersect(Collider& other, float* deltaX, float* deltaY);


private:
	GameObject* mOwner;
	float mRadius;
	



};


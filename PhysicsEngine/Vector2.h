#pragma once
#include "PhysicsHeader.h"

class Vector2
{
public:
	float x;
	float y;
	Vector2();
	Vector2(float xIn, float yIn);
	void normalize();
	float magnitude();
	Vector2 scalarMultiply(float scalar);
	void translate(float xOffset, float yOffset);
	void rotate(float angle);
	void rotateFromOrigin(float originX, float originY, float angle);
};


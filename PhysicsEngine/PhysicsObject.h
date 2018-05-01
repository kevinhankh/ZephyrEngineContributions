#pragma once
#include "PhysicsHeader.h"
#include "Vector2.h"

class PhysicsObject
{
public:
	std::string tag;
	std::string renderable;//sprite to render (should remove this from physics eventually
	Vector2 position;
	float width;
	float height;
	float rotation;
	Mast mast;
	Rudder rudder;
	float windScale;
	float rotationSpeed;
	float speed;
	float acceleration;
	Vector2 corners[4]; //UL, UR, BR, BL

	PhysicsObject();
	PhysicsObject(std::string tagIn, float xIn, float yIn, float wIn, float hIn, float rotIn, float scale, float rotSpeedIn, float speedIn, float accelerationIn, std::string _renderable);
	~PhysicsObject();
};


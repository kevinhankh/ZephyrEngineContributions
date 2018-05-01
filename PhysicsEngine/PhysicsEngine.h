#pragma once
#include "PhysicsHeader.h"
#include "PhysicsObject.h"
#include "Axis.h"

class PhysicsEngine
{
public:
	std::map<std::string, PhysicsObject> GameObjects;

	PhysicsEngine();
	~PhysicsEngine();

	void addObject(std::string ID, std::string tag, float x, float y, float width, float height, float rotation, float windscale, float rotationspeed, float speed, float acceleration, std::string _renderable);
	void removeObject(std::string ID);
	void absolute(float &number);
	Vector2 convertAngleToVector(float angle);
	float convertVectorToAngle(Vector2 vector);
	float checkAngle(float angle);
	void getCorners(PhysicsObject o, Vector2(&box)[4]);
	float dotProduct(Vector2 a, Vector2 b);
	bool checkCollision(PhysicsObject A, PhysicsObject B);
};

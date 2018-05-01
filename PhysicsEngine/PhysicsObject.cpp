#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(std::string tagIn, float xIn, float yIn, float wIn, float hIn, float rotIn, float scale, float rotSpeedIn, float speedIn, float accelerationIn, std::string _renderable)
{
	this->tag = tagIn;
	this->position.x = xIn;
	this->position.y = yIn;
	this->width = wIn;
	this->height = hIn;
	this->rotation = rotIn;
	this->mast = NONE;
	this->rudder = STRAIGHT;
	this->windScale = scale;
	this->rotationSpeed = rotSpeedIn;
	this->speed = speedIn;
	this->acceleration = accelerationIn;
	this->renderable = _renderable; //should eventually be removed
}

PhysicsObject::~PhysicsObject()
{
}

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{

}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::addObject(std::string ID, std::string tag, float xPosition, float yPosition, float width, float height, float rotation, float windscale, float rotationspeed, float speed, float acceleration, std::string _renderable)
{
	PhysicsObject NewObject(tag, xPosition, yPosition, width, height, rotation, windscale, rotationspeed, speed, acceleration, _renderable);
	GameObjects[ID] = NewObject;
}

void PhysicsEngine::removeObject(std::string ID)
{
	GameObjects.erase(ID);
}

void PhysicsEngine::absolute(float &number)
{
	number = sqrt(number * number);
}

Vector2 PhysicsEngine::convertAngleToVector(float angle)
{
	Vector2 result;
	result.x = 0;
	result.y = 1;
	result.rotate(checkAngle(angle));
	return result;
}

float PhysicsEngine::convertVectorToAngle(Vector2 vector)
{
	float result;
	if (vector.x >= 0 && vector.y >= 0)
		result = atan(abs(vector.x) / vector.y);
	else if (vector.x >= 0 && vector.y <= 0)
		result = 90 + atan(abs(vector.y) / abs(vector.x));
	else if (vector.x <= 0 && vector.y <= 0)
		result = 180 + atan(vector.x / abs(vector.y));
	else if (vector.x <= 0 && vector.y >= 0)
		result = 270 + atan(vector.y / vector.x);
	if (vector.x == 0 && vector.y == 0)
		result = NULL;

	return result;
}

//ensures angle is between 0 and 360
float PhysicsEngine::checkAngle(float angle)
{
	float result;
	result = angle; 
	if (result < 0 || result > 360)
	{
		while (result > 360)
		{
			result -= 360;
		}
		while (result < 0)
		{
			result += 360;
		}
	}
	return result;
}

//collision
void PhysicsEngine::getCorners(PhysicsObject o, Vector2(&box)[4]) {

	float sinAngle = sin(o.rotation * PI / 180);
	float cosAngle = cos(o.rotation * PI / 180);
	float halfWidth = o.width / 2;
	float halfHeight = o.height / 2;
	float result = 0;
	// [0] = Upper Left
	//box[0].x = o.position.x - halfWidth;
	__asm {
		fld o.position.x;
		fld halfWidth;
		fsub;
		fstp result;
	}
	box[0].x = result;
	//box[0].y = o.position.y + halfHeight;
	__asm {
		fld o.position.y;
		fld halfHeight;
		fadd;
		fstp result;
	}
	box[0].y = result;
	box[0].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));

	// [1] = Upper Right
	//box[1].x = o.position.x + halfWidth;
	__asm {
		fld o.position.x;
		fld halfWidth;
		fadd;
		fstp result;
	}
	box[1].x = result;
	//box[1].y = o.position.y + halfHeight;
	__asm {
		fld o.position.y;
		fld halfHeight;
		fadd;
		fstp result;
	}
	box[1].y = result;
	box[1].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));

	// [2] = Bottom Right
	//box[2].x = o.position.x + halfWidth;
	__asm {
		fld o.position.x;
		fld halfWidth;
		fadd;
		fstp result;
	}
	box[2].x = result;
	//box[2].y = o.position.y - halfHeight;
	__asm {
		fld o.position.y;
		fld halfHeight;
		fsub;
		fstp result;
	}
	box[2].y = result;
	box[2].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));

	// [3] = Bottom Left
	//box[3].x = o.position.x - halfWidth;
	__asm {
		fld o.position.x;
		fld halfWidth;
		fsub;
		fstp result;
	}
	box[3].x = result;
	//box[3].y = o.position.y - halfHeight;
	__asm {
		fld o.position.y;
		fld halfHeight;
		fsub;
		fstp result;
	}
	box[3].y = result;
	box[3].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));
}

float PhysicsEngine::dotProduct(Vector2 a, Vector2 b) {
	return (a.x * b.x) + (a.y * b.y);
}

bool PhysicsEngine::checkCollision(PhysicsObject A, PhysicsObject B) {
	// Step 1: Get the corners for each bouding box
	getCorners(A, A.corners);
	getCorners(B, B.corners);

	// Find X min and Y min
	//float minX, minY;
	//minX = A.corners[0].x; minY = A.corners[0].y;
	//for (int i = 0; i < 4; i++) {
	//	if (A.corners[i].x < minX)
	//		minX = A.corners[i].x;
	//	if (B.corners[i].x < minX)
	//		minX = B.corners[i].x;
	//	if (A.corners[i].y < minY)
	//		minY = A.corners[i].y;
	//	if (B.corners[i].y < minY)
	//		minY = B.corners[i].y;
	//}
	for (int i = 0; i < 4; i++) {
		A.corners[i].x += 1000;
		B.corners[i].x += 1000;
		A.corners[i].y += 1000;
		B.corners[i].y += 1000;
	}
	//// Shift positions to be positve
	//if (minX < 0) { // Shift X positions
	//	for (int i = 0; i < 4; i++) {
	//		A.corners[i].x -= minX - 1;
	//		B.corners[i].x -= minX - 1;
	//	}
	//}
	//if (minY < 0) { // Shift Y positions
	//	for (int i = 0; i < 4; i++) {
	//		A.corners[i].y -= minY - 1;
	//		B.corners[i].y -= minY - 1;
	//	}
	//}

	// Step 2: Get the 2 axis for each bounding box (4 axis total)
	Axis axis[4];
	axis[0].axis = convertAngleToVector(A.rotation);
	axis[1].axis = convertAngleToVector(A.rotation + 90);
	axis[2].axis = convertAngleToVector(B.rotation);
	axis[3].axis = convertAngleToVector(B.rotation + 90);

	// Step 3: Project all 4 corners of A and B onto each of the 4 axis
	float scalar;
	for (int i = 0; i < 4; i++) { // For each of the 4 axis
		for (int j = 0; j < 4; j++) { // Calculate A's corners projected onto the axis
			scalar = dotProduct(A.corners[j], axis[i].axis) / pow(axis[i].axis.magnitude(), 2);
			axis[i].Acorners[j] = axis[i].axis.scalarMultiply(scalar);
		}
		for (int k = 0; k < 4; k++) { // Calculate B's corners projected on the axis
			scalar = dotProduct(B.corners[k], axis[i].axis) / pow(axis[i].axis.magnitude(), 2);
			axis[i].Bcorners[k] = axis[i].axis.scalarMultiply(scalar);
		}

		// Step 4: Find the Amin, Amax, Bmin, Bmax for each of the 4 axis
		axis[i].Amin = axis[i].Acorners[0]; axis[i].Amax = axis[i].Acorners[0]; axis[i].Bmin = axis[i].Bcorners[0]; axis[i].Bmax = axis[i].Bcorners[0];
		for (int j = 0; j < 4; j++) {
			if (axis[i].Acorners[j].magnitude() < axis[i].Amin.magnitude()) // Find A min
				axis[i].Amin = axis[i].Acorners[j];
			if (axis[i].Acorners[j].magnitude() > axis[i].Amax.magnitude()) // Find A max
				axis[i].Amax = axis[i].Acorners[j];
			if (axis[i].Bcorners[j].magnitude() < axis[i].Bmin.magnitude()) // Find B min
				axis[i].Bmin = axis[i].Bcorners[j];
			if (axis[i].Bcorners[j].magnitude() > axis[i].Bmax.magnitude()) // Find B max
				axis[i].Bmax = axis[i].Bcorners[j];
		}

		// Step 5: Check if Amax < Bmin && Bmax < Amin

		if (axis[i].Amax.magnitude() < axis[i].Bmin.magnitude() || axis[i].Bmax.magnitude() < axis[i].Amin.magnitude())
			return false;

	} // End for each axis

	return true;
}
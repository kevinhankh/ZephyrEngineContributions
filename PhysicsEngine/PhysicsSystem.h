#pragma once
#include "PhysicsEngine.h"
#include "System.h"


#define PROJECTILE_FORCE 2
#define PROJECTILE_SPEED 100
#define SHIP_ACCELERATION 0.1

class PhysicsSystem : public System
{
	struct WindObject
	{
		float direction;
		float power;
	};
public:
	PhysicsEngine Physics;
	WindObject Wind;

	PhysicsSystem(MessageBus* mbus);
	~PhysicsSystem();

	void startSystemLoop();
	void handleMessage(Msg *msg);

	void setWind(float angle, float speed);
	void changeMast(std::string ID, int mast);
	void changeRudder(std::string ID, int rudder);
	void updateShip(PhysicsObject &ship);
	void updateProjectile(PhysicsObject &projectile);
	void collisionHandler(std::string object1);
	void Destroy(std::string IDin); //PhysicsObject &object);

	int timeFrame = 20;
	std::mutex mtx;

};
 
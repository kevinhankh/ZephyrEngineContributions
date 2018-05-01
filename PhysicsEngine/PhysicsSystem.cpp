#include "PhysicsSystem.h"

/*
HANK

There are issues with the messages not having enough information. Discussed this with Denis and he has a solution we can implement after alpha.
Have a look at handleMessage function. There are a lot of uncertainties with the messages, so I figured you should handle it for testing purposes.
Check the functions changeMast and changeRudder as well, and change it however you want for your tests.

You can use the StartPhysicsLoop for your tests. It just iterates through the PhysicsObject map, and updates ships and projectiles.
The tags for now seems to be the renderable string as discussed with Denis. So maybe name the image files Ship.png and Projectile.png, then change the loop to look for tag == "Ship.png" and stuff.

This class is basically the bridge between PhysicsEngine and the actual Game stuff, so we really should've worked on it together.
I have all the functions to do everything set up, so just use them with your messages for testing.
*/

PhysicsSystem::PhysicsSystem(MessageBus* mbus) : System(mbus)
{
	//create wind object(move to message handle?)
	//Physics.addObject("Wind", "Wind", 1,1,45, 0, 0, .8, 0, 0);//x,y,orientation,width,height, windscale, rotSPeed, inertia
	Wind.direction = 90;
	Wind.power = 0.5;
}

PhysicsSystem::~PhysicsSystem()
{
}

//Subject to change! This was used for testing.
void PhysicsSystem::startSystemLoop()
{
	clock_t thisTime = clock();

	int currentGameTime = 0;
	while (alive) {
		thisTime = clock();
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}
		currentGameTime = thisTime + timeFrame;

		handleMsgQ();

		////Display Thread ID for Debugging
		//std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
		//OutputDebugString("Physics Loop on thread: ");
		//OutputDebugString(s.c_str());
		//OutputDebugString("\n");

		//Subject to change! Will discuss how the loop will actually work.
		//Loop through Physics objects to update their movements and check collision.
		//mtx.lock();
		for (std::map<std::string, PhysicsObject>::iterator it = Physics.GameObjects.begin(); it != Physics.GameObjects.end(); ++it /* not hoisted */ /* no increment */)
		{
			collisionHandler(it->first);

			if (it->second.tag == "ShipObj")
			{
				updateShip(it->second);
				//checkCollision
			}
			else if (it->second.tag == "Cannonball")
			{
				updateProjectile(it->second);
			}
			else if (it->second.tag == "Remove")
			{
		/*		OutputDebugString("removing: ");
				OutputDebugString(it->first.c_str());
				OutputDebugString("\n");*/
				it = Physics.GameObjects.erase(it);
				break;
			}


			std::ostringstream oss;

			oss << it->first << "," //id
				<< ""<< "," //renderable
				<< it->second.position.x << "," //x
				<< it->second.position.y //y
				<< ",0," // z
				<< it->second.rotation << "," //orientation
				<< it->second.width << "," //width 
				<< it->second.height << ","//height
				<< "0,0"; //not sure if we need these

			Msg* mm = new Msg(UPDATE_OBJECT_POSITION, "");
			//mm->type = UPDATE_OBJECT_POSITION;
			mm->data = oss.str();
			msgBus->postMessage(mm, this);

			//OutputDebugString("physics posting");
			//OutputDebugString("\n");

			//OutputDebugString(oss.str().c_str());
			//OutputDebugString("\n");
			std::string s = std::to_string(Physics.GameObjects[it->first].mast);

			//OutputDebugString(s.c_str());
			//OutputDebugString("\n");

			//postmsg update to all TODO
			//e.g. oss << it->first << ',' << it->second.position.x << etcetc 
			
		}




		//mtx.unlock();

	}
}



//Subject to change! Temporary solution for testing and alpha build.
//Parses the msg string, then switch case msg type.
void PhysicsSystem::handleMessage(Msg *msg)
{
	System::handleMessage(msg);

	////Display Thread ID for Debugging
	//std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
	//OutputDebugString("Physics Handle Messsage on thread: ");
	//OutputDebugString(s.c_str());
	//OutputDebugString("\n");

	std::vector<std::string> data;
	std::string token;
	std::string messageData = msg->data;
	std::string splitter = ",";
	std::size_t pos = 0;
	std::string ID, tag, renderable;
	float x, y, rotation, width, height;

	// This entire section really irks me, from here

	// I have a split funciton remember?
	/*while ((pos = messageData.find(splitter)) != std::string::npos)
	{
	token = messageData.substr(0, pos);
	data.push_back(token);
	}*/

	data = split(msg->data, ',');



	// to here
	// because whats the point of sorting the datra into variables here? The data
	// in each message is different and cannot be expected to be of the same format
	// that's why it's passed in as a string, so that the case blocks can decide how to
	// read the data themself

	switch (msg->type)
	{
	case GO_ADDED:
		//Subject to change! Need to finalize message data system to identify the object type.
		//For now, only projectiles care about inertia, and you can hardcode windScale and rotationSpeed to 1 for testing purposes
		ID = data[0];
		renderable = data[1];
		tag = data[9];
		x = atof(data[2].c_str());
		y = atof(data[3].c_str());
		//skip z
		rotation = atof(data[5].c_str());
		width = atof(data[6].c_str());//data[6]?
		height = atof(data[7].c_str());//data[7]?

									   //check if physics enabled
		if (atof(data[8].c_str()) == 1) {
			if(tag == "ShipObj")
				Physics.addObject(ID, tag, x, y, width, height, rotation, 1, .5, 0, SHIP_ACCELERATION, renderable);
			if(tag == "Cannonball")
				Physics.addObject(ID, tag, x, y, width, height, rotation, 1, .5, PROJECTILE_SPEED, 0, renderable);
			if (tag == "GameObject")
				Physics.addObject(ID, tag, x, y, width, height, rotation, 1, .5, 0, 0, renderable);
			//OutputDebugString("GOADDED in physics ");
			//OutputDebugString("\n");
		}

		break;
	case GO_REMOVED: {
		
		//Use this if you are using the iterator in StartPhysicsLoop
		//mtx.lock();
	//	OutputDebugString("Go_Remove in Physics");
		//OutputDebugString("\n");
		//Destroy(Physics.GameObjects[ID]);
		//Destroy(ID);
		std::map<std::string, PhysicsObject>::iterator it;
		it = Physics.GameObjects.find(data[0]);
		if (it != Physics.GameObjects.end()) {
			it->second.tag = "Remove";		
		}
			
		break;
	}
	case LEVEL_LOADED:
		if (data[0] == "2") {
			Wind.direction = 90;
		}
		break;
	case PASS_WIND:
	{
		if (data[0] == "CW") {
			Wind.direction += (std::rand() % 10);
		} else {
			Wind.direction -= (std::rand() % 10);
		}

		Wind.direction = Physics.checkAngle(Wind.direction);

		// update obj sprite
		Msg* asdf = new Msg(UPDATE_OBJ_SPRITE, "");
		std::ostringstream oss;
		oss << "windmarker" << "," //id
			<< "" << "," //renderable
			<< "-175" << "," //x
			<< "175" //y
			<< ",2," // z
			<< Wind.direction << "," //orientation
			<< "20" << "," //width 
			<< "20" << ","//height
			<< "0,0"; //not sure if we need these

		Msg* mm = new Msg(UPDATE_OBJECT_POSITION, "");
		//mm->type = UPDATE_OBJECT_POSITION;
		mm->data = oss.str();
		msgBus->postMessage(mm, this);
	}
		break;
	case CHANGE_MAST:
		ID = data[0];
		changeMast(ID, atoi(data[1].c_str())); //  just cast the data 
		//Physics.GameObjects[ID].renderable = data[2];
		break;
	case CHANGE_RUDDER:
		ID = data[0];
		changeRudder(ID, atoi(data[1].c_str()));
		break;
		/*
		FOR SHOOTING
		Just do add object
		Physics.addObject(ID, tag, x, y, width, height, rotation, 1, 1, PROJECTILE_INERTIA);
		The orientation is the direction it will shoot at. PROJECTILE_FORCE is defined in PhysicsSystem.h, it's the speed of the projectile. -Used it for testing, can be changed.
		Upon creation the projectile will automatically move forward according to orientation. It gets destroyed after moving for a bit.
		Look at updateProjectile function.
		*/
	}
}

/*
void sendMessage
update position
collision
*/

void PhysicsSystem::setWind(float directionIn, float powerIn)
{
	//Physics.GameObjects["Wind"].rotation = Physics.checkAngle(angle);
	//Physics.GameObjects["Wind"].windScale = speed;
	Wind.direction = directionIn;
	Wind.power = powerIn;
}

void PhysicsSystem::changeMast(std::string ID, int mast)
{
	//Use this if you implement enum mast to the message. Change int mast to Mast mast in the parameters above.
	//Physics.GameObjects[ID].mast = mast;

	//Otherwise you can test using an int
	switch (mast)
	{
	case 0:
		Physics.GameObjects[ID].mast = NONE;
		break;
	case 1:
		Physics.GameObjects[ID].mast = HALFMAST;
		break;
	case 2:
		Physics.GameObjects[ID].mast = FULLMAST;
		break;
	}
}

void PhysicsSystem::changeRudder(std::string ID, int rudder)
{
	//Use this if you implement enum mast to the message. Change int rudder to Rudder rudder in the parameters above.
	//Physics.GameObjects[ID].rudder = rudder;

	//Otherwise you can test using an int
	switch (rudder)
	{
	case 0:
		Physics.GameObjects[ID].rudder = FULLPORT;
		break;
	case 1:
		Physics.GameObjects[ID].rudder = HALFPORT;
		break;
	case 2:
		Physics.GameObjects[ID].rudder = STRAIGHT;
		break;
	case 3:
		Physics.GameObjects[ID].rudder = HALFSTARBOARD;
		break;
	case 4:
		Physics.GameObjects[ID].rudder = FULLSTARBOARD;
		break;
	}
}

//Updates ship movement using its mast and rudder settings.
//The speed is determined by the wind power scaled by the mast setting: halfsail = 50%, fullsail = 100%
//Then it is further scaled by comparing the ship direction and the wind direction (percentage based).
//FUTURE REFERENCE: When creating different classes of ships, apply the object's windFactor to have different speeds.
void PhysicsSystem::updateShip(PhysicsObject &ship)
{
	Vector2 objectDirection;
	float windPercentage;
	float maxSpeed = ship.windScale * Wind.power;
	float movementScale = 0;
	if (ship.speed > 0) {
		switch (ship.rudder)
		{
		case STRAIGHT:
			break;
		case HALFPORT:
			ship.rotation -= ship.rotationSpeed / 2;
			break;
		case FULLPORT:
			ship.rotation -= ship.rotationSpeed;
			break;
		case HALFSTARBOARD:
			ship.rotation += ship.rotationSpeed / 2;
			break;
		case FULLSTARBOARD:
			ship.rotation += ship.rotationSpeed;
			break;
		}
	}
	ship.rotation = Physics.checkAngle(ship.rotation);

	windPercentage = Wind.direction - ship.rotation;
	Physics.absolute(windPercentage);
	windPercentage = 1 - (windPercentage / 180);
	Physics.absolute(windPercentage);

	if (windPercentage < 0.2)//temp limit the min movement to 20% speed
		windPercentage = 0.2;

	switch (ship.mast)
	{
	case NONE:
		if (ship.speed > 0)
			ship.speed -= ship.acceleration * timeFrame/1000;
		else
			ship.speed = 0;
		break;
	case HALFMAST:
		if (ship.speed < maxSpeed / 2)
			ship.speed += ship.acceleration * timeFrame/1000;
		else if (ship.speed > maxSpeed / 2)
			ship.speed -= ship.acceleration * timeFrame/1000;
		break;
	case FULLMAST:
		if (ship.speed < maxSpeed) 
			ship.speed += ship.acceleration * timeFrame/1000; //ship.acceleration * timeFrame/1000;
		else if (ship.speed >= maxSpeed)
			ship.speed = maxSpeed;
		break;
	}
	movementScale = ship.speed * windPercentage;
	
	objectDirection = Physics.convertAngleToVector(ship.rotation);
	ship.position.translate(objectDirection.x * movementScale, objectDirection.y * movementScale);
}

//Subject to change!
//Simple shooting for alpha. Projectile slows a bit overtime and gets destroyed.
void PhysicsSystem::updateProjectile(PhysicsObject &projectile)
{
	//TODO remove physics stopping object out of bounds
	if (projectile.speed > (PROJECTILE_SPEED - 80))
	{
		Vector2 objectDirection;
		float forceScale, force;

		objectDirection = Physics.convertAngleToVector(projectile.rotation);
		forceScale = projectile.speed / 100;
		force = PROJECTILE_FORCE * forceScale;

		projectile.position.translate(objectDirection.x * force, objectDirection.y * force);
		projectile.speed--;
	}
	else
	{
		//Destroy(projectile);
	}
}

void PhysicsSystem::collisionHandler(std::string object1)
{
	for (std::map<std::string, PhysicsObject>::iterator it = Physics.GameObjects.begin(); it != Physics.GameObjects.end(); ++it)
	{
		if (object1 != it->first)
		{
			if (Physics.checkCollision(Physics.GameObjects[object1], it->second))//collision detected
			{
				std::ostringstream oss;

				oss << object1 << "," //id of obj1
					<< it->first;//id of obj2
					

				Msg* mm = new Msg(GO_COLLISION, "");
				mm->data = oss.str();
				msgBus->postMessage(mm, this);
			}
		}
	}
}

//Subject to change! Depends on how we loop through the GameObjects.
//Might need to use this to safely remove objects, if we are using iterator to loop through the GameObjects map.
void PhysicsSystem::Destroy(std::string IDin)//PhysicsObject &object)
{
	std::map<std::string, PhysicsObject>::iterator it;
	it = Physics.GameObjects.find(IDin);
	if (it != Physics.GameObjects.end())
	it->second.tag = "Remove";
	//object.tag = "Remove";
}
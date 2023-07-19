#include "SlimeEntity.h"

using namespace cv;
using namespace std;

EntityPtr createSlime(std::string const& animationFolder)
{
	return make_shared<Entity>(CreateSlimeEnemy(animationFolder));
}

EntityStatePtr CreateSlimeEnemy(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr nonCollidingPhysicsPtr = 
		/*make_shared<NonCollidingPhysicsDecorator>*/(
			make_shared<FixedWidgetPhysics>());

	return make_shared<EntityState>(graphicsPtr, nonCollidingPhysicsPtr);
}
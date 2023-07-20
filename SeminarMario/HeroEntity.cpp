#include "HeroEntity.h"

enum HeroStates {
	HERO_IDLE,
	HERO_RUN_RIGHT,
	HERO_RUN_LEFT,
	HERO_DUCK,
	HERO_STAY_DUCK,
	HERO_STAND_AFTER_DUCK,
	HERO_JUMP,
	HERO_COLLISING
};
EntityStatePtr createHeroState(fs::path const& animationFolder, HeroStates state, Rect bounds = Rect())
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isNotCyclic =
		state == HeroStates::HERO_DUCK ||
		state == HeroStates::HERO_STAND_AFTER_DUCK ||
		state == HeroStates::HERO_JUMP;
	bool isCyclic = !isNotCyclic;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr physicsPtr = nullptr;
	Size frameSize = graphicsPtr->getCollisionMask().size();
	switch (state)
	{
	case HERO_IDLE:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_RUN_RIGHT:
		physicsPtr.reset(new BoundedPhysicsDecorator
		(make_shared<ConstVelocityPhysics>
			(ConstVelocityPhysics(Point(frameSize.width / 10, 0))), bounds));
		break;
	case HERO_RUN_LEFT:
		physicsPtr.reset(new BoundedPhysicsDecorator
		(make_shared<ConstVelocityPhysics>
			(ConstVelocityPhysics(Point(-frameSize.width / 10, 0))), bounds));
		break;
	case HERO_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAY_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAND_AFTER_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_JUMP:
		physicsPtr.reset(new BoundedPhysicsDecorator
		(make_shared<JumpPhysics>
			(JumpPhysics(20, 30, 4)), bounds));
		break;
	case HERO_COLLISING:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	default:
		throw std::exception("Unknown physics state!");
	}

	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}
EntityPtr createHero(std::string const& rootAnimationsFolder, cv::Rect bounds)
{
	fs::path root = rootAnimationsFolder;
	auto idleFolder = root / "idle";

	auto collising = createHeroState(root / "idle", HeroStates::HERO_COLLISING);
	auto idle = createHeroState(root / "idle", HeroStates::HERO_IDLE);
	auto runRight = createHeroState(root / "runRight", HeroStates::HERO_RUN_RIGHT, bounds);
	auto jump = createHeroState(root / "jump", HeroStates::HERO_JUMP, bounds);
	auto runLeft = createHeroState(root / "runLeft", HeroStates::HERO_RUN_LEFT, bounds);
	auto duck = createHeroState(root / "duckDown", HeroStates::HERO_DUCK);
	auto stayDuck = createHeroState(root / "duckStay", HeroStates::HERO_STAY_DUCK);
	auto standAfterDuck = createHeroState(root / "standAfterDuck", HeroStates::HERO_STAND_AFTER_DUCK);

	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, runRight);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, runLeft);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);

	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	runRight->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, collising);

	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS,EventCodes::ENTITY_PHYSICS_FINISHED }, idle);
	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, collising);

	duck->addState(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_GRAPHICS,EventCodes::ENTITY_FINISHED_ANIMATION }, stayDuck);
	stayDuck->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, standAfterDuck);

	standAfterDuck->addState(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_GRAPHICS,EventCodes::ENTITY_FINISHED_ANIMATION }, idle);

	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, idle);
	runLeft->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, collising);
	collising->addState(Event{ EventSenders::SENDER_TIMER,EventTypes::EVENT_TIMER,EventCodes::TIME_TICK }, idle);
	
	EntityPtr hero(new HeroEntity(idle));
	collising->Register(hero);
	idle->Register(hero);
	runRight->Register(hero);
	jump->Register(hero);
	runLeft->Register(hero);
	duck->Register(hero);
	stayDuck->Register(hero);
	standAfterDuck->Register(hero);

	return hero;
}

HeroEntity::HeroEntity(EntityStatePtr state):
	Entity(state)
{
}

void HeroEntity::onCollising()
{
	int heightOfScreem = cv::Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)).height;
	_state->getPhysics()->reset(Point(100,heightOfScreem-230));
	cout << "on collising of hero" << endl;
}

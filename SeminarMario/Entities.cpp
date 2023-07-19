#include "Entities.h"


EntityState::EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics)
	:_graphicsPtr(graphics), _physicsPtr(physics)
{
}

void EntityState::update()
{
	bool graphicsFinished = _graphicsPtr->update();
	bool physicsFinished = _physicsPtr->update(_graphicsPtr->getCollisionMask());

	if (physicsFinished)
		Notify(Event{EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED});
	if (graphicsFinished) {
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION });
	}
}

void EntityState::addState(Event const& e, std::shared_ptr<EntityState> toWhere)
{
	_adjecentStates[e] = toWhere;
}

std::shared_ptr<EntityState> EntityState::tryModifyState(Event const& e) const
{
	bool isEventMapped = _adjecentStates.count(e);
	if (isEventMapped)
		return _adjecentStates.at(e);
	
	return nullptr;
}

IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}

void EntityState::reset(cv::Point const& TL)
{
	_graphicsPtr->reset();
	_physicsPtr->reset(TL);
}

void EntityState::draw(cv::Mat & canvas)
{
	_graphicsPtr->draw(canvas, _physicsPtr->getTL());
}


////////////////////////////////////////////////////////

Entity::Entity(EntityStatePtr state)
	:_state(state)
{
}

void Entity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK)
	{
		_state->update();

	}
	
	auto newStateCandidate = _state->tryModifyState(e);
	
	if (newStateCandidate)
	{
		newStateCandidate->reset(_state->getPhysics()->getTL());
		_state = newStateCandidate;
	}
}

void Entity::reset(cv::Point const& TL)
{
	_state->reset(TL);
}

void Entity::draw(cv::Mat& canvas)
{
	_state->draw(canvas);
}


EntityPtr createLives(std::string const& liveImage, int countLives)
{
	Frame heart(liveImage);
	cv::resize(heart.image, heart.image,cv::Size(30,30));
	cv::resize(heart.mask, heart.mask, cv::Size(30, 30));
	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics(heart, countLives));

	IPhysicsComponentPtr physicsPtr =
		make_shared<FixedWidgetPhysics>();
	cv::Size size(cv::Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)));
	physicsPtr->reset(Point(size.width-50,30));

	return make_shared<Entity>(
		make_shared<EntityState>(graphicsPtr, physicsPtr));
}

EntityPtr createScore(float fontScale, int score, int fontFace)
{
	IGraphicsComponentPtr graphicsPtr(
		new ScoresGraphics(fontScale, score, fontFace));

	IPhysicsComponentPtr physicsPtr =
		make_shared<FixedWidgetPhysics>();

	physicsPtr->reset(Point(100,100));
	return make_shared<Entity>(
		make_shared<EntityState>(graphicsPtr, physicsPtr));
}

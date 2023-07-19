#pragma once
#include "Graphics.h"
#include "Physics.h"
#include "Observer.h"
#include "Animation.h"
#include "Config.h"
#include <memory>
#include <Windows.h>
#include <map>
#include <exception>
using namespace std;
using namespace cv;


class EntityState : public Subject
{
protected:
	IGraphicsComponentPtr _graphicsPtr;
	IPhysicsComponentPtr _physicsPtr;

	std::map<Event, std::shared_ptr<EntityState>> _adjecentStates;


public:
	EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics);

	virtual void update();
	
	void addState(Event const& e, std::shared_ptr<EntityState> toWhere);
	virtual std::shared_ptr<EntityState> tryModifyState(Event const& e) const;

	IPhysicsComponentPtr const& getPhysics() const;
	virtual void reset(cv::Point const& TL);

	void draw(cv::Mat & canvas);
};
typedef std::shared_ptr<EntityState> EntityStatePtr;


class Entity : public IObserver
{
protected:
	EntityStatePtr _state;

public:
	Entity(EntityStatePtr state);

	// Inherited via IObserver
	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL);

	void draw(cv::Mat& canvas);

	// Prototype Design Pattern
	//virtual std::shared_ptr<Entity> clone() = 0;
};

typedef std::shared_ptr<Entity> EntityPtr;

EntityPtr createLives(std::string const& liveImage, int countLives);
EntityPtr createScore(float fontScale, int score, int fontFace);
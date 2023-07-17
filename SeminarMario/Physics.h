#pragma once

#include <opencv2/opencv.hpp>

class IPhysicsComponent
{
public:
	// start over the physics from a given tl=topLeft.
	virtual void reset(cv::Point const & tl) = 0;
	
	// update physics to the next position, 
	// with current "collisionMask" = the "shape" of the current non-transparent-pixels.
	// it's called "collisionMask" since only if a non-transparent pixel is colliding it's considered a collision.
	// 
	// return value is true if the physics would like to be replaced 
	// (physics states that "it has fulfilled its duty and has no reason to be here anymore").
	virtual bool update(cv::Mat const& collisionMask) = 0;
	
	// return the current collisionMaks
	virtual cv::Mat const & getCollisionMask() const = 0;

	// check collision with other entitie's physics.
	virtual bool checkCollision(std::shared_ptr<IPhysicsComponent> const& other) const = 0;

	// return const ref to a current top left.
	// WARNING! you return a reference here, which is actually a POINTER!
	// therefore, you must return a reference to a member, not to a temporary variable.
	virtual cv::Point const& getTL() const = 0;
};

// using shorter, more convinient, name:
typedef std::shared_ptr<IPhysicsComponent> IPhysicsComponentPtr;

// physics of non-moving. implemented as an example.
class FixedWidgetPhysics : public IPhysicsComponent
{
private:
	cv::Point _topLeft;
	cv::Mat _mask;
public:
	FixedWidgetPhysics();
	// Inherited via IPhysicsComponent
	virtual void reset(cv::Point const& tl);
	virtual bool update(cv::Mat const& collisionMask) override;
	virtual cv::Mat const& getCollisionMask() const override;
	virtual bool checkCollision(IPhysicsComponentPtr const& other) const override;
	virtual cv::Point const& getTL() const override;
};

// physics of moving with const velocity.
// @2: Implement. HINT! right click on some function that has this green line beneath it, 
// choose "Quick actions and Refactoring" --> "Create Definition.. " 
class ConstVelocityPhysics : public IPhysicsComponent
{

public:
	// input: the velocity ( = speed) in terms of x and y axis.
	ConstVelocityPhysics(cv::Point const & velocity);
	// Inherited via IPhysicsComponent
	virtual void reset(cv::Point const& tl);
	virtual bool update(cv::Mat const& collisionMask);
	virtual cv::Mat const& getCollisionMask() const override;
	virtual bool checkCollision(IPhysicsComponentPtr const& other) const;
	virtual cv::Point const& getTL() const;
};

// physics of moving with const velocity in X direction, while moving "up and down" on the y axis.
// @2: Implement. HINT! (Same as above)
class JumpPhysics : public IPhysicsComponent
{
public:
	/// <param name="horizontalVelocity">positive or negative - x axis velocity</param>
	/// <param name="initialVerticalVelocity">positive number in pixels</param>
	/// <param name="gravity">positive number in pixels</param>
	JumpPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity);
	virtual void reset(cv::Point const& tl);
	virtual bool update(cv::Mat const& collisionMask);
	virtual cv::Mat const& getCollisionMask() const override;
	virtual bool checkCollision(IPhysicsComponentPtr const& other) const;
	virtual cv::Point const& getTL() const;
};
#include "LivesEntity.h"


LivesEntity::LivesEntity(EntityStatePtr state):
	Entity(state)
{
}

void LivesEntity::onCollising()
{
	static int numLives = 5;
	_state->getGraphics()->reset(--numLives);
	cout << "in collising of lives" << numLives;
}

EntityPtr createLives(std::string const& liveImage, int countLives)
{
	Frame heart(liveImage);
	cv::resize(heart.image, heart.image, cv::Size(30, 30));
	cv::resize(heart.mask, heart.mask, cv::Size(30, 30));
	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics(heart, countLives));

	IPhysicsComponentPtr physicsPtr =
		make_shared<FixedWidgetPhysics>();
	cv::Size size(cv::Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)));
	physicsPtr->reset(Point(size.width - 50, 30));

	return make_shared<LivesEntity>(
		make_shared<EntityState>(graphicsPtr, physicsPtr));
}


#include "ScoreEntity.h"

EntityPtr createScore(float fontScale, int score, int fontFace)
{
	IGraphicsComponentPtr graphicsPtr(
		new ScoresGraphics(fontScale, score, fontFace));

	IPhysicsComponentPtr physicsPtr =
		make_shared<FixedWidgetPhysics>();
	physicsPtr->reset(Point(100, 100));

	return make_shared<ScoreEntity>(
		make_shared<EntityState>(graphicsPtr, physicsPtr));
}

void ScoreEntity::onCollising()
{
	cout << "on collising of score";
}

ScoreEntity::ScoreEntity(EntityStatePtr state):
	Entity(state)
{
}

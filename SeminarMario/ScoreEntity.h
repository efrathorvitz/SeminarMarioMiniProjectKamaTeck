#pragma once
#include "Entities.h"
class ScoreEntity :
    public Entity
{
    virtual void onCollising() override;
public:
    ScoreEntity(EntityStatePtr state);
};
EntityPtr createScore(float fontScale, int score, int fontFace);


#pragma once
#include "Entities.h"
class LivesEntity :
    public Entity
{
    virtual void onCollising() override;
public:
    LivesEntity(EntityStatePtr state);
};
EntityPtr createLives(std::string const& liveImage, int countLives);



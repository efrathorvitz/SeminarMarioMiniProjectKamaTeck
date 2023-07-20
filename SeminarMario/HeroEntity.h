#pragma once
#include "Entities.h"
#include "opencv2/opencv.hpp"
#include "Config.h"
#include <memory>
#include <filesystem>
#include<Windows.h>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;
class HeroEntity :
    public Entity
{
    virtual void onCollising() override;
public:
    HeroEntity(EntityStatePtr state);
};
EntityPtr createHero(std::string const& rootAnimationsFolder, cv::Rect r = cv::Rect());

//typedef std::shared_ptr<HeroEntity> HeroEntityPtr;

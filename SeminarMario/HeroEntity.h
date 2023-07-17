#pragma once
#include "opencv2/opencv.hpp"
#include "Entities.h"

EntityPtr createHero(std::string const& rootAnimationsFolder,cv::Rect r=cv::Rect());

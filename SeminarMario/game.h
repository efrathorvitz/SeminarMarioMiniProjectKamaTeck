#pragma once
#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "Timer.h"
#include "HeroEntity.h"
#include <Windows.h>
#include "SlimeEntity.h"

using namespace cv;

class game :public Subject
{
public:
	void startGame();
};


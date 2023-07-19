#include "game.h"
void game::startGame()
{
	IPhysicsComponentPtr _phvgj;
	Mat background = imread(R"(..\Animations\background.png)", IMREAD_UNCHANGED);
	cv::resize(background, background, cv::Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)));

	Rect bounds = Rect(0, 0, background.size().width, background.size().height);
	EntityPtr hero = createHero(R"(..\Animations\Hero)", bounds);
	hero->reset(Point(background.size().width / 2, background.size().height * 2 / 3));


	EntityPtr slime = createSlime(R"(..\Animations\SlimeOrange)");
	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));
	EntityPtr lives = createLives("..\\Animations\\heart.png", 5);
	EntityPtr score = createScore(3, 100, FONT_HERSHEY_COMPLEX_SMALL);


	Timer timer(/*freq. ms=*/100);


	timer.Register(slime);
	timer.Register(hero);




	bool isToExit = false;
	while (false == isToExit)
	{
		Mat canvas = background.clone();

		timer.tick();
		slime->draw(canvas);
		hero->draw(canvas);
		lives->draw(canvas);
		score->draw(canvas);
		imshow("test", canvas);
	}
}

#include "Animation.h"
#include <opencv2/opencv.hpp>
void duck();
void jumpRight();
cv::Mat background = cv::imread(R"(../Animations/background.png)", cv::IMREAD_UNCHANGED);

cv::Point topLeft(background.size().width / 3, background.size().height * 2 / 4);
int key = ' ';
cv::Point walkRight()
{
	
	Animation animation(R"(../Animations/Hero/runRight)");
	
	
	for (int i = 0; i < animation.numFrames(); i++)
	{
		cv::Mat canvas = background.clone();
		canvas.create(100, 200, CV_8UC3);
		Frame const& frame = animation.getFrame(i);
		topLeft.x += (background.size().width )/ 10;
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
	    key = cv::waitKey(100);
	}
	return topLeft;
}
void stand();

int main_p()
{
	bool isToExit = false;
	while (false == isToExit)
	{
		switch (key){
		case 'd':
			walkRight();
			break;
		case 's':
			duck();
			break;
		case 'w':
			jumpRight();
			break;
		case 'a':
			stand();
			break;
		default:
			stand();
			break;
		}
		
		    key = cv::waitKey(100);
			
			if (key == 27) // if you pressed ESC key
			{
				isToExit = true;
			}
		
	}

	return 0;
}
// 1.
// draws an animation of our hero that ducks in place, and stays down till you 
// press the 'w' key. (if (key == 'w'))
void duck() {
	Animation duck_animation(R"(../Animations/Hero/duckDown)");
	for (int i = 0; i < duck_animation.numFrames(); i++)
	{
		cv::Mat canvas = background.clone();
		Frame const& frame =duck_animation.getFrame(i);
		topLeft.x += (1 / 10) * (background.size().width);
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
	}
	
	Animation sty_animation(R"(../Animations/Hero/duckStay)");
	
	while (key != 'w')
	{
		cv::Mat canvas = background.clone();
		drawFrame(sty_animation.getFrame(0), canvas, topLeft); 
		cv::imshow("test", canvas);
	    key = cv::waitKey(100);
	}
}

// 2.
// draws an animation of our hero that walks to the right, and ACTUALLLY MOVES RIGHT
// with a constant speed, of (1/10)*(image width) per 100 milliseconds (=0.1 second).
void stand() {
	Animation animation(R"(../Animations/Hero/idle)");
	for (int i = 0; i < animation.numFrames(); i++)
	{
		cv::Mat canvas = background.clone();
		Frame const& frame = animation.getFrame(i);
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
		int key = cv::waitKey(100);
	}
}

// 3.
// draw our hero jumps up and right, and stays standing after jump finishes.
void jumpRight() {
	Animation animation(R"(../Animations/Hero/idle)");

		cv::Mat canvas = background.clone();
		Frame const& frame = animation.getFrame(0);
		topLeft.x += 40;
		topLeft.y += 40;
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
	    key = cv::waitKey(100);

	    canvas = background.clone();
		topLeft.y -= 30;
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
		key = cv::waitKey(100);
		stand();
}

// 4.
// write a main() that:
// when user presses 'd' - hero walks right
// when user presses 's' - hero ducks
// when user presses 'w' - hero jumps
// when user presses 'a' - hero stops (if it was walking right before)


// 5.
// in walkRight - when hero reaches the canvas boundery - don't let it go outside of image.
// force the hero x coordinate to be such that we'll see her.
// hint: you can get hero image cv::Mat with frame.image

// 6.
// do the same with jumpRight
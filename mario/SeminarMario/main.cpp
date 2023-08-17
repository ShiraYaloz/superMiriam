#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "SlimeEntity.h"
#include "HeroEntity.h"
#include "Timer.h"
#include "HeartsEntity.h"
#include "scoreEntity.h"
#include "EntitiesPool.h"
#include "Config.h"
#include "communicateEntities.h"
#include <memory>
#include "AppleEntity.h"

#define N 5
using namespace cv;


// before you start: open SeminarMario project properties, go to 
// Debugging -> Environment
// set the following value:
// PATH=$(PATH);../OpenCV_451/bin
// hit Ctrl + F5 and see a walking lady. that's our hero!
// press Esc to exit.
// read carefully the comments below - it contains explanations and instructions.
// and do the exercises.
int main()
{
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	cv::resize(background, background, cv::Size(1500, 700));

	int standOnFloor = background.size().height * 4 / 5;

	//Point randomPlaces[N] =
	//{ Point(0,standOnFloor),
	//	Point(100,standOnFloor),
	//	Point(200,standOnFloor),
	//	Point(300,standOnFloor),
	//	Point(background.size().width * 2 / 3, standOnFloor) };

	//EntityPtr hero = createHero(R"(../Animations/Hero)", background);
	//hero->reset(Point(background.size().width / 2, background.size().height * 2 / 3));


	//EntityPtr slime = createSlime(R"(../Animations/SlimeOrange)");
	//slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));


	//EntityPtr hearts = createHearts(R"(../Animations/Hearts/heart.png)", 3);
	//hearts->reset(Point(background.size().width - 200, 0), 3);


	//EntityPtr score = createScore(2, 1);
	//score->reset(Point(10, 35));

	//Timer timer(/*freq. ms=*/100);
	//timer.Register(slime);
	//timer.Register(hero);

	/*hero->registerToState1(hearts);*/

	/*EntitiesPool pool;
	pool.insert(slime);
	slime = pool.  getNext();*/

	EntityPtr hero = createHero(R"(../Animations/Hero)", background);
	EntityPtr slime = createSlime(R"(../Animations/SlimeOrange)");
	EntityPtr slime1 = createSlime(R"(../Animations/SlimeOrange)");
	EntityPtr hearts = createHearts(R"(../Animations/Hearts/heart.png)", 3);
	EntityPtr score = createScore(2, 1);
	EntityPtr apple = createApple(R"(../Animations/apple/apple.png)");
	shared_ptr<communicateEntities> _allEntities(new communicateEntities);
	//Point p(background.size().width / 2, background.size().height * 2 / 3);
	_allEntities->insert(SignalEntitiesInGame::HERO, true,true ,hero,
	Point(background.size().width / 2, background.size().height * 2 / 3));
	_allEntities->insert(SignalEntitiesInGame::APPLE, true,false, apple, Point(80,-25));
	_allEntities->insert(SignalEntitiesInGame::HEARTS, true,true ,hearts, Point(10, 35),4);
	_allEntities->insert(SignalEntitiesInGame::SCORE, true,true, score, Point(background.size().width - 200, 30) , 400);
	_allEntities->insert(ManyEntitiesInGame::SLIMES, false,true, slime, Point(100, 600));
	_allEntities->insert(ManyEntitiesInGame::SLIMES, false, true, slime1, Point(background.size().width * 2 / 3, 600));

	Timer timer(100);
	timer.Register(slime);
	timer.Register(slime1);
	timer.Register(hero);
	timer.Register(_allEntities);
	timer.Register(apple);
	hearts->entityRegister(_allEntities);
	Mat canvas =  background.clone();
	while (true)
	{
		
		 canvas = background.clone();
		_allEntities->setCanvas(canvas);
		//_allEntities->gameOver();
		if (_allEntities->isGameOver()) {
			_allEntities->gameOver();
		}

		timer.tick();
		_allEntities->draw();
		imshow("test", canvas);

		

	}
	/*canvas = background.clone();

	
	imshow("test", canvas);*/

	return 0;
}



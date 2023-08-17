#pragma once
#include "Entities.h"
#include "EntitiesPool.h"
#include "Config.h"
using namespace std;
//class whoAgaintWho {
//public:
//	EntityPtr signal;
//	std::shared_ptr<EntitiesPool> many;
//	bool operator==(const whoAgaintWho& obj2) {
//		return 
//	}
//};
using namespace cv;
class communicateEntities:public Subject , public IObserver
{
private:
	bool _game_over = false;
	map<SignalEntitiesInGame,EntityPtr> _signalEntities;
	map<ManyEntitiesInGame, std::shared_ptr<EntitiesPool>> _allManyEntities;
	map<array<int, 2>, Event> _optionalCollisions;
	int standOnFloor = 600;
	cv::Point randomPlaces[5] =
	{ Point(0,standOnFloor),
		Point(100,standOnFloor),
		Point(250,standOnFloor),
		Point(800,standOnFloor),
		Point(500, standOnFloor) };
	Mat _canvas;
	//map< ManyEntitiesInGame, vector<SignalEntitiesInGame>> _registersTo;
public:
	communicateEntities();
	void setCanvas(Mat canvas);
	virtual void onNotify(Event const& e) override;
	//void reset();
	void insert(int type, bool isSignal, bool setAsActive, EntityPtr whom, cv::Point tl, int code = 0);
	void update();
	void checkCollision();
	void eventsResult(Event const& , EntityPtr , ManyEntitiesInGame  , SignalEntitiesInGame );
	void setManyEntityToBeNotActive(EntityPtr, ManyEntitiesInGame);
	void setSignalEntityToNonActive(SignalEntitiesInGame);
	void setManyEntityToBeActive(EntityPtr, ManyEntitiesInGame);
	void setSignalEntityToActive(SignalEntitiesInGame);
	void gameOver();
	void draw();
	bool isGameOver();
};

 
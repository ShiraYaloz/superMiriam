#include "communicateEntities.h"
#include <opencv2/opencv.hpp>
#include <memory>
using namespace std;
using namespace cv;
communicateEntities::communicateEntities()
{
	
	for (size_t i = 0; i < Lengthes::NUM_OF_MANY_ENTITIES; i++)
	{
		std::shared_ptr<EntitiesPool> p(new EntitiesPool);
		_allManyEntities[ManyEntitiesInGame(i)] = p;
	}

	// fill connections
	_optionalCollisions[{ { SignalEntitiesInGame::HERO, ManyEntitiesInGame::SLIMES }}] =
		Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY };
	_optionalCollisions[{{ SignalEntitiesInGame::APPLE, ManyEntitiesInGame::SLIMES }}]=
		Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_LIFE };
	//_registersTo[ManyEntitiesInGame::SLIMES].push_back(SignalEntitiesInGame::HERO);

}

void communicateEntities::setCanvas(Mat canvas)
{
	_canvas = canvas;
}


//void gameOver(cv::Mat canvas) {
//
//	string text = "dcfvfv: " ;
//	Scalar color(0, 0, 255);
//	int thickness = 2;
//	int lineType = FILLED;
//	// finally:
//	putText(canvas, text, Point(300,300), 1, 2, color, thickness, lineType);
//	imshow("test", canvas);
//	//string text = "GAME OVER";
//	//Scalar color(0, 0, 255);
//	//int thickness = 4;
//	//int lineType = FILLED;
//	//// finally:
//	//putText(canvas, text, Point(400, 30), 3, 4, color, thickness, lineType);
//	//imshow("test", canvas);
//}

// tאולי שזה יירש מאנטיטי ישות שמנהלת את הקטע של ההאזות
void communicateEntities::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_ENTITY_STATE &&
		e.type == EventTypes::EVENT_GRAPHICS &&
		e.code == EventCodes::ENTITY_FINISHED_ANIMATION)
	{
		//gameOver();
		_game_over = true;
	}

	else if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK)
	{
		checkCollision();
	}
	else if (e.sender == EventSenders::SENDER_KEYBOARD
		&&
		e.type == EventTypes::EVENT_KEY_PRESSED
		&&
		e.code == EventCodes::SPACE)
	{
		setSignalEntityToActive(APPLE);
		_signalEntities[APPLE]->reset(_signalEntities[HERO]->getState()->getPhysics()->getTL() + Point(80, -25));
		
	}
	
}



void communicateEntities::insert(int type, bool isSignal, bool setAsActive, EntityPtr whom, cv::Point tl, int code)
{
	whom->reset(tl, code);
	if (isSignal) {
		_signalEntities[SignalEntitiesInGame(type)] = whom;
		if (!setAsActive)
			_signalEntities[SignalEntitiesInGame(type)]->SetNonActive();
	}
	else 
	{
		_allManyEntities[ManyEntitiesInGame(type)]->insert(whom, setAsActive);
		/*if (_registersTo.count(ManyEntitiesInGame(type)))
		{
			for (auto item: _registersTo[ManyEntitiesInGame(type)])
			{
				item
			}

		}*/
	}

	
	this->Register(whom);
}


void communicateEntities::checkCollision()
{
	for (auto item : _optionalCollisions) {
		//if(_allEntities[item[0]]->getNumOfActive() == 1)si
		SignalEntitiesInGame signal = SignalEntitiesInGame(item.first[0]);
		ManyEntitiesInGame many = ManyEntitiesInGame(item.first[1]);
		if (_signalEntities[signal]->getActive()) {
			auto c = _allManyEntities[many];
			int check = _allManyEntities[many]->getNumOfActive();
			for (size_t i = 0; i < _allManyEntities[many]->getNumOfActive(); i++)
			{
				EntityPtr result = _allManyEntities[many]->checkCollisionWithAllPool(_signalEntities[signal]);
				if (result != nullptr)
				{
					//_signalEntities[signal]->entityNotify(item.second);
					Notify(item.second);
					eventsResult(item.second, result, many, signal);
				}
			}
		}
	}
}

//EntityStatePtr createNonCollState(EntityStatePtr stat) {
//	IGraphicsComponentPtr graphicsPtr(
//		stat->getGraphics());
//	IPhysicsComponentPtr notColphysicsPtr = nullptr;
//	notColphysicsPtr.reset(new NotCollisionPhysicsDecorator(stat->getPhysics()));
//	return make_shared<EntityState>(graphicsPtr, notColphysicsPtr);
//}
void communicateEntities::eventsResult(Event const& e , EntityPtr result , ManyEntitiesInGame typeMany ,
	 SignalEntitiesInGame typeSignal)
{
	if (e.code == EventCodes::COLLISION_WITH_ENEMY)
		//נשעה דקורטור לסליים שבפונ הריסט יעביר למקום אחר 
	{
		srand(time(NULL));

		result->reset(randomPlaces[rand() % 5]);
	}
	else if (e.code == EventCodes::COLLISION_WITH_FOOD_LIFE) {
		setManyEntityToBeNotActive(result, typeMany);
		setSignalEntityToNonActive(typeSignal);
	}
		/*
		יש כמה דרכים לטפל במקרה שבו רוצים שאובייקט כלשהוא לא יתגש:
		1. ליצור דקורטור שמחזיר מסיכה ריקה ושאין התנגשות
		2. לעשות בכל ישות משתנה שאומר האם הוא מתנגש או לא 
		*/

	//else if(e.code == EventCodes::COLLISION_WITH_ENEMY)

}
void communicateEntities::setManyEntityToBeNotActive(EntityPtr ePtr, ManyEntitiesInGame type)
{
	_allManyEntities[type]->returnEntity(ePtr);
}

void communicateEntities::setSignalEntityToNonActive(SignalEntitiesInGame type)
{
	_signalEntities[type]->SetNonActive();
}

void communicateEntities::setManyEntityToBeActive(EntityPtr e, ManyEntitiesInGame type)
{
	_allManyEntities[type]->setActive(e);
}

void communicateEntities::setSignalEntityToActive(SignalEntitiesInGame type)
{
	_signalEntities[type]->setToActive();
}

void communicateEntities::gameOver()
{

	string text = "GAME OVER ";
	Scalar color(0, 0, 255);
	int thickness = 2;
	int lineType = FILLED;
	// finally:
	putText(_canvas, text, Point(300, 300), 2, 6, color, thickness, lineType);
	imshow("test", _canvas);

}

void communicateEntities::draw()
{
	for (auto item : _signalEntities) {
		if (item.second->getActive())
			item.second->draw(_canvas);
	}
	for (auto item : _allManyEntities) {
		item.second->draw(_canvas);
	}
	
}

bool communicateEntities::isGameOver()
{
	return _game_over;
}


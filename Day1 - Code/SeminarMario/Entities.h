//#pragma once
//#include "Graphics.h"
//#include "Physics.h"
//#include "Observer.h"
//#include "EntitiesPool.h"
//#include "Config.h"
//#include <map>
////#include "EntitiesPool.h"
//class EntityState : public Subject
//{
//protected:
//	IGraphicsComponentPtr _graphicsPtr;
//	IPhysicsComponentPtr _physicsPtr;
//
//	std::map<Event, std::shared_ptr<EntityState>> _adjecentStates;
//
//
//public:
//	EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics);
//	//EntitiesPoolPtr pool = nullptr
//	virtual void update();
//	
//	void addState(Event const& e, std::shared_ptr<EntityState> toWhere);
//	virtual std::shared_ptr<EntityState> tryModifyState(Event const& e) const;
//
//	IPhysicsComponentPtr const& getPhysics() const;
//	virtual void reset(cv::Point const& TL , int code = 0);
//
//	void draw(cv::Mat & canvas);
//};
//typedef std::shared_ptr<EntityState> EntityStatePtr;
//
//
//class Entity : public IObserver
//{
//protected:
//	EntityStatePtr _state;
//
//public:
//	Entity(EntityStatePtr state);
//
//	// Inherited via IObserver
//	virtual void onNotify(Event const& e) override;
//	//virtual void register()
//	virtual void reset(cv::Point const& TL , int code = 0);
//	/*virtual bool checkPixelLevelCollision(std::shared_ptr<Entity> other);*/
//	//void entityNotify(Event const& e);
//	/*void registerToState1(std::shared_ptr<Entity> e);*/
//	void draw(cv::Mat& canvas);
//
//};
//typedef std::shared_ptr<Entity> EntityPtr;


//
//class comunicateBetEtities :public Subject {
///*
// לעשות את כל בישויות במשחק כאן
// יהיה מערך של בריכות 
// כולם מסוג מצביעים לישויות
// יוכלו להוסיף לפי סוג ישות
// יהיה קשרים באתחול מי מאיים על מי לדוגמא מפלצות על הגיבורה תפוחים על מפלצות מי המאויים ומי המאיים
// תהיה פונ שתבדוק כל פעם בין כל הישויות הפעילות את הבעיות
// ותודיע
// כולם יקשיבו לאובייקט הכולל
//
// reset: מאתחל את כל האובייקטים הפעילים
// שיגביל אותן  לעשות אולי עוד משתנה בבריכות 
//*/
//
//private:
//	
//	/*std::map<EntitiesInGame, EntitiesPoolPtr> _allEntities;
//	std::map<EntitiesInGame[2] , Event> _optionalCollisions;*/
//public:
//	//comunicateBetEtities();
//	////void reset();
//	//void insert(EntitiesInGame type , EntityPtr whom , cv::Point tl, int code = 0);
//	//void update();
//	//void checkCollision();
//};


/**/
#pragma once
#include "Graphics.h"
#include "Physics.h"
#include "Observer.h"
#include <map>
#include "Config.h"
//#include "EntitiesPool.h"

class EntityState : public Subject
{
protected:
	IGraphicsComponentPtr _graphicsPtr;
	IPhysicsComponentPtr _physicsPtr;

	std::map<Event, std::shared_ptr<EntityState>> _adjecentStates;


public:
	EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics);

	virtual void update();

	void addState(Event const& e, std::shared_ptr<EntityState> toWhere);
	virtual std::shared_ptr<EntityState> tryModifyState(Event const& e) const;

	IPhysicsComponentPtr const& getPhysics() const;
	IGraphicsComponentPtr const& getGraphics() const;
	//void changeStateToNotColl();
	virtual void reset(cv::Point const& TL,int code =0);

	void draw(cv::Mat& canvas);
};
typedef std::shared_ptr<EntityState> EntityStatePtr;


class Entity : public IObserver
{
protected:
	EntityStatePtr _state;
	bool _isActive;
public:
	Entity(EntityStatePtr state);

	// Inherited via IObserver
	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL , int code = 0);
	bool getActive();
	void setToActive();
	void SetNonActive();
	virtual bool CheckCollision(std::shared_ptr<Entity> other);
	EntityStatePtr getState();
	void entityNotify(Event const& e);
	void entityRegister(IObserverPtr);
	void draw(cv::Mat& canvas);
	EntityStatePtr createNonCollState();
	virtual bool checkPixelLevelCollision(std::shared_ptr<Entity> other);
	/*void setStateAsNotColl();
	void setStateToColl();*/
	// Prototype Design Pattern
	//virtual std::shared_ptr<Entity> clone() = 0;
};
typedef std::shared_ptr<Entity> EntityPtr;


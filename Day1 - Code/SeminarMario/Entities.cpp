/*#include "Entities.h"
#include "Graphics.h"
#include "Physics.h"
#include "Animation.h"
#include "Config.h"


#include <memory>
using namespace std;
using namespace cv;

EntityState::EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics)
	:_graphicsPtr(graphics), _physicsPtr(physics)
{
}

void EntityState::update()
{
	bool graphicsFinished = _graphicsPtr->update();
	bool physicsFinished = _physicsPtr->update(_graphicsPtr->getCollisionMask());

	if (physicsFinished)
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED });
	if (graphicsFinished) {
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION });
	}
	//if there is collision

}

void EntityState::addState(Event const& e, std::shared_ptr<EntityState> toWhere)
{
	_adjecentStates[e] = toWhere;
}

std::shared_ptr<EntityState> EntityState::tryModifyState(Event const& e) const
{
	bool isEventMapped = _adjecentStates.count(e);
	if (isEventMapped)
		return _adjecentStates.at(e);

	return nullptr;
}

IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}

void EntityState::reset(cv::Point const& TL , int code)
{
	_graphicsPtr->reset(code);
	_physicsPtr->reset(TL);
}

void EntityState::draw(cv::Mat & canvas)
{
	_graphicsPtr->draw(canvas, _physicsPtr->getTL());
}


////////////////////////////////////////////////////////

Entity::Entity(EntityStatePtr state)
	:_state(state)
{
}

void Entity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK)
	{
		_state->update();
		//this->_state->getPhysics()->checkCollision(other->_state->getPhysics());

	}

	auto newStateCandidate = _state->tryModifyState(e);

	if (newStateCandidate)
	{
		newStateCandidate->reset(_state->getPhysics()->getTL());
		_state = newStateCandidate;
	}
}

void Entity::reset(cv::Point const& TL , int code)
{
	_state->reset(TL , code);
}

bool Entity::checkPixelLevelCollision(std::shared_ptr<Entity> other)
{
	return this->_state->getPhysics()->checkCollision(other->_state->getPhysics());
}

//void Entity::entityNotify(Event const& e)
//{
//	_state->Notify(e);
//}

//void Entity::registerToState1(std::shared_ptr<Entity> e)
//{
//	*_state.Register(e);
//}


void Entity::draw(cv::Mat& canvas)
{
	_state->draw(canvas);
}
/// <summary>
///
/// </summary>
comunicateBetEtities::comunicateBetEtities()
{
	for (size_t i = 0; i < Lengthes::NUM_OF_ENTITIES; i++)
	{
		EntitiesPoolPtr p(new EntitiesPool);
		_allEntities[EntitiesInGame(i)] = p;
	}

	// fill connections
	_optionalCollisions.insert({ EntitiesInGame::HERO, EntitiesInGame::SLIMES },
		Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY });
	_optionalCollisions.insert({ EntitiesInGame::SLIMES, EntitiesInGame:: APPLE},
		Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_LIFE });

}

void comunicateBetEtities::insert(EntitiesInGame type, EntityPtr whom,cv::Point tl, int code)
{
	whom->reset(tl, code);
	_allEntities[type]->insert(whom);
}
// hapen in tick
void comunicateBetEtities::update()
{

}



*/
#include "Entities.h"
#include "Graphics.h"
#include "Physics.h"
#include "Animation.h"
#include "Config.h"
#include "EntitiesPool.h"
#include <memory>
using namespace std;
using namespace cv;

#pragma region implement of EntityState
EntityState::EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics)
	:_graphicsPtr(graphics), _physicsPtr(physics) {}
void EntityState::update()
{
	bool graphicsFinished = _graphicsPtr->update();
	bool physicsFinished = _physicsPtr->update(_graphicsPtr->getCollisionMask());
	//if(_physicsPtr->checkCollision())
	if (physicsFinished)
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED });
	if (graphicsFinished) {
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION });
	}
}
//EntityStatePtr createNonCollState(EntityStatePtr stat) {
//	IGraphicsComponentPtr graphicsPtr(
//		stat->getGraphics());
//	IPhysicsComponentPtr notColphysicsPtr = nullptr;
//	notColphysicsPtr.reset(new NotCollisionPhysicsDecorator(stat->getPhysics()));
//	return make_shared<EntityState>(graphicsPtr, notColphysicsPtr);
//}
//EntityStatePtr returnToCollState(EntityStatePtr stat) {
//	IGraphicsComponentPtr graphicsPtr(
//		stat->getGraphics());
//	IPhysicsComponentPtr ColphysicsPtr = NotCollisionPhysicsDecorator(stat->getPhysics()).getBase();
//	return make_shared<EntityState>(graphicsPtr, ColphysicsPtr);
//}
void EntityState::addState(Event const& e, std::shared_ptr<EntityState> toWhere)
{
	_adjecentStates[e] = toWhere;
}
std::shared_ptr<EntityState> EntityState::tryModifyState(Event const& e) const
{
	bool isEventMapped = _adjecentStates.count(e);
	if (isEventMapped)
		return _adjecentStates.at(e);
	return nullptr;
}
IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}
IGraphicsComponentPtr const& EntityState::getGraphics() const
{
	return _graphicsPtr;
}


void EntityState::reset(cv::Point const& TL, int code)
{
	if(TL != Point(-1,-1))
		_physicsPtr->reset(TL);
	_graphicsPtr->reset(code);
	
}
void EntityState::draw(cv::Mat& canvas)
{
	_graphicsPtr->draw(canvas, _physicsPtr->getTL());
}
#pragma endregion
////////////////////////////////////////////////////////
#pragma region implement of class Entity
Entity::Entity(EntityStatePtr state)
	:_state(state), _isActive(true) {}
void Entity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK)
	{
		
		_state->update();
	}
	else if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&& e.type == EventTypes::EVENT_PHYSICS
		&& e.code == EventCodes::COLLISION_WITH_ENEMY)
	{
		// ישויות שרק אם קרתה התנגשות מתעדכנות
		_state->update();
	}
	else if (e.sender == EventSenders::SENDER_ENTITY_STATE &&
		e.type == EventTypes::EVENT_PHYSICS &&
		e.code == EventCodes::COLLISION_WITH_FOOD_LIFE) {
			_state->reset(Point(-1,-1),-1);

	}
	auto newStateCandidate = _state->tryModifyState(e);
	if (newStateCandidate)
	{
		newStateCandidate->reset(_state->getPhysics()->getTL());
		_state = newStateCandidate;
	}
}
void Entity::reset(cv::Point const& TL, int code)
{
	_state->reset(TL, code);
}
bool Entity::getActive()
{
	return _isActive;
}
void Entity::setToActive()
{
	_isActive = true;
}
void Entity::SetNonActive()
{
	_isActive = false;
}
EntityStatePtr Entity::getState()
{
	return _state;
}
void Entity::entityNotify(Event const& e)
{
	_state->Notify(e);
}
void Entity::entityRegister(IObserverPtr o)
{
	_state->Register(o);
}
void Entity::draw(cv::Mat& canvas)
{
	_state->draw(canvas);
}
EntityStatePtr Entity::createNonCollState()
{
	IGraphicsComponentPtr graphicsPtr(
				_state->getGraphics());
			IPhysicsComponentPtr notColphysicsPtr = nullptr;
			notColphysicsPtr.reset(new NotCollisionPhysicsDecorator(_state->getPhysics()));
	return make_shared<EntityState>(graphicsPtr, notColphysicsPtr);
}
bool Entity::checkPixelLevelCollision(std::shared_ptr<Entity> other)
{
	return this->_state->getPhysics()->checkCollision(other->_state->getPhysics());
}
//void Entity::setStateAsNotColl()
//{
//	_state = createNonCollState(_state);
//	//_isColl = false;
//}
//void Entity::setStateToColl()
//{
//	returnToCollState(_state);
//	//_isColl = false;
//}
bool Entity::CheckCollision(std::shared_ptr<Entity> other)
{
	return _state->getPhysics()->checkCollision(other->_state->getPhysics());
}
#pragma endregion
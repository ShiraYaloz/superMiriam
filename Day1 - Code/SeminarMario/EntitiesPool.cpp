//#include "EntitiesPool.h"
//#include <stdexcept>
//
//EntitiesPool::EntitiesPool(int limit)
//{
//	_pool.clear();
//	_isInUse.clear();
//	if (limit != -1)
//		_limitNumber = limit;
//}
//
//void EntitiesPool::insert(EntityPtr whom)
//{
//	if (numOfActive + 1 < _limitNumber) {
//		_pool.push_back(whom);
//		_isInUse.push_back(false);
//	}
//}
//
//EntityPtr EntitiesPool::getNext()
//{
//	size_t i = 0;
//	for (; i < _pool.size() && _isInUse[i]; i++);
//	if (i == _pool.size())
//		return nullptr;
//	_isInUse[i] = true;
//	numOfActive++;
//	return _pool[i];
//	
//
//}
//
//void EntitiesPool::returnEntity(EntityPtr toFree)
//{
//	size_t i = 0;
//	for (; i < _pool.size() && _pool[i] != toFree; i++);
//	_isInUse[i] = false;
//	numOfActive--;
//}
//
//EntityPtr EntitiesPool::checkCollisionWithAllPool(EntityPtr colEntity)
//{
//	if (numOfActive > 0) {
//	for (size_t i = 0; i < _pool.size(); i++) {
//		if (_isInUse[i]) {
//			bool b = _pool[i]->checkPixelLevelCollision(colEntity);
//			if (b)
//				return _pool[i];
//		}
//	}
//	}
//	return nullptr;
//}
//
//

#include "EntitiesPool.h"
#include <stdexcept>
using namespace cv;

EntitiesPool::EntitiesPool(int limit)
{
	_pool.clear();
	_isInUse.clear();
	if (limit != -1)
		_limitNumber = limit;
}
void EntitiesPool::insert(EntityPtr whom , bool setToActive)
{
	_pool.push_back(whom);
	if (setToActive)
	{
		_isInUse.push_back(true);
		numOfActive++;
	}
	else
		_isInUse.push_back(false);
}
EntityPtr EntitiesPool::getNext()
{
	
	for (size_t i = 0; i < _isInUse.size(); ++i)
	{
		if (!_isInUse[i])
		{
			_isInUse[i] = true;
			numOfActive++;
			return _pool[i];
		}
	}
	throw std::runtime_error("All entities are in use.");
}
void EntitiesPool::draw(Mat canvas)
{
	for (size_t i = 0; i < _pool.size(); ++i) {
		if (_isInUse[i])
			_pool[i]->draw(canvas);
	}
}
int EntitiesPool::getNumOfActive()
{
	return numOfActive;
}
void EntitiesPool::setActive(EntityPtr toCatch)
{
	size_t i = 0;
	for (; i < _pool.size() ; i++) {
		if (_pool[i] == toCatch) {
			_isInUse[i] = true;
			numOfActive++;
			return;
		}
	}
	
}
void EntitiesPool::returnEntity(EntityPtr toFree)
{
	
	for (size_t i = 0; i < _pool.size() ; i++) {
		if (_pool[i] == toFree)
		{
		_isInUse[i] = false;
		numOfActive--;
		return;
		}

	}

}

EntityPtr EntitiesPool::checkCollisionWithAllPool(EntityPtr colEntity)
{
	if (numOfActive > 0) {
	for (size_t i = 0; i < _pool.size(); i++) {
		if (_isInUse[i]) {
			bool b = _pool[i]->checkPixelLevelCollision(colEntity);
			if (b)
				return _pool[i];
		}
	}
	}
	return nullptr;
}



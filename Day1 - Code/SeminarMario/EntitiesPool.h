#pragma once
#include <vector>
#include "Entities.h"
using namespace cv;
using namespace std;

class EntitiesPool
{
		private:
			int numOfActive = 0;
			int _limitNumber;
			std::vector<std::shared_ptr<Entity>> _pool;
			std::vector<bool> _isInUse;
		public:
			EntitiesPool(int limit = -1);
		
			void insert(EntityPtr , bool);
		
			// throws exception if all entities are in use.
			EntityPtr getNext();
			void draw(cv::Mat canvas);
			int getNumOfActive();
			void setActive(EntityPtr toCatch);
			void returnEntity(EntityPtr toFree);
			EntityPtr checkCollisionWithAllPool(EntityPtr colEntity);

};
//typedef std::shared_ptr<EntitiesPool> EntitiesPoolPtr
//typedef std::shared_ptr<EntitiesPool> EntitiesPoolPtr;

#include "HeartsEntity.h"
//#include "HeartsEntity.h"
//#include "Config.h"
//using namespace cv;
//using namespace std;
//
//
//enum heartsActions{
//	INCREASE,
//	DECREASE,
//	STATIC
//};
//
//EntityStatePtr createHeartsMinuses(std::string const& , int , heartsActions );
//
//EntityPtr createHearts(std::string const& PictureFolder ,int numOfHearts)
//{
//	auto increaseLive = createHeartsMinuses(PictureFolder, numOfHearts, heartsActions::INCREASE);
//	auto decreaseLive = createHeartsMinuses(PictureFolder, numOfHearts, heartsActions::DECREASE);
//	auto staticLives = createHeartsMinuses(PictureFolder, numOfHearts, heartsActions::STATIC);
//	increaseLive->addState(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY },decreaseLive);
//	decreaseLive->addState(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, decreaseLive);
//	staticLives->addState(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, decreaseLive);
//	EntityPtr hearts(new Entity(staticLives));
//
//	increaseLive->Register(hearts);
//	decreaseLive->Register(hearts);
//	staticLives->Register(hearts);
//
//	return hearts;
//	
//}
//
//
//EntityStatePtr createHeartsMinuses(std::string const& PictureFolder ,int lastNum , heartsActions state)
//{
//	
//	IGraphicsComponentPtr graphicsPtr = nullptr;
//	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
//
//	switch (state)
//	{
//	case INCREASE:
//		graphicsPtr.reset(new LivesGraphics(PictureFolder, lastNum + 1));
//		break;
//	case DECREASE:
//		graphicsPtr.reset(new LivesGraphics(PictureFolder, lastNum - 1));
//		break;
//	case STATIC:
//		graphicsPtr.reset(new LivesGraphics(PictureFolder, lastNum));
//		break;
//	default:
//		throw std::exception("Unknown physics state!");
//	}
//	
//	/*IPhysicsComponentPtr pDecorator = nullptr;
//	pDecorator.reset(new BoundedPhysicsDecorator(physicsPtr, canvasRect));*/
//	return make_shared<EntityState>(graphicsPtr, physicsPtr);
//}
using namespace std;
EntityPtr createHearts(std::string const& PictureFolder, int numOfHearts)
{

		// הסליים מחכה ובאירוע של פגישה של הגיבורה איתו הוא משנה את נקודת העמידה שלו


		IGraphicsComponentPtr graphicsPtr(
			new LivesGraphics(PictureFolder, numOfHearts));

		IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
		EntityPtr hearts(new Entity(make_shared<EntityState>(graphicsPtr, physicsPtr)));
		return hearts;
}

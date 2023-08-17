#include "SlimeEntity.h"

using namespace cv;
using namespace std;


EntityPtr createSlime(std::string const& animationFolder)
{
// הסליים מחכה ובאירוע של פגישה של הגיבורה איתו הוא משנה את נקודת העמידה שלו
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr heatPhysicsPtr = make_shared<FixedWidgetPhysics>();
	IPhysicsComponentPtr NonheatPhysicsPtr = make_shared<NotCollisionPhysicsDecorator>(make_shared<FixedWidgetPhysics>());
	EntityStatePtr heatSlime(make_shared<EntityState>(graphicsPtr, heatPhysicsPtr));
	EntityStatePtr NonheatSlime(make_shared<EntityState>(graphicsPtr, NonheatPhysicsPtr));
	heatSlime->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY },
		NonheatSlime);
	NonheatSlime->addState(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::HALF_SECOND_FROM_COLL },
		heatSlime);
	//IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	EntityPtr slime(new Entity(heatSlime));
	return slime;
}

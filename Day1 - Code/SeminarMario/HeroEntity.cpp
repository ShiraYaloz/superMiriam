#include "HeroEntity.h"
#include "opencv2/opencv.hpp"
#include "Config.h"

#include <memory>
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

enum HeroStates {
	HERO_IDLE,
	HERO_RUN_RIGHT,
	HERO_RUN_LEFT,
	HERO_DUCK, 
	HERO_STAY_DUCK, 
	HERO_STAND_AFTER_DUCK, 
	HERO_JUMP,
	THROW_APPLE
};

EntityStatePtr createHeroState(
	fs::path const& animationFolder,
	HeroStates state,
	cv::Rect canvasRect)
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isNotCyclic =
		state == HeroStates::HERO_DUCK ||
		state == HeroStates::HERO_STAND_AFTER_DUCK ||
		state == HeroStates::HERO_JUMP;
	bool isCyclic = !isNotCyclic;

	IGraphicsComponentPtr graphicsPtr(
	new SingleAnimationGraphics(animation, isCyclic));
	IPhysicsComponentPtr physicsPtr = nullptr;
	Size frameSize = graphicsPtr->getCollisionMask().size();
	switch (state)
	{
	case HERO_IDLE:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_RUN_RIGHT:
		physicsPtr.reset(new ConstVelocityPhysics(Point(frameSize.width / 10, 0)));
		break;
	case HERO_RUN_LEFT:
		physicsPtr.reset(new ConstVelocityPhysics(Point(-(frameSize.width / 10), 0)));
		break;
	case HERO_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAY_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAND_AFTER_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_JUMP:
		physicsPtr.reset(new JumpPhysics(10,30,3));
		break;
	case THROW_APPLE:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	default:
		throw std::exception("Unknown physics state!");
	}
	IPhysicsComponentPtr pDecorator = nullptr;
	pDecorator.reset(new BoundedPhysicsDecorator(physicsPtr, canvasRect));
	return make_shared<EntityState>(graphicsPtr, pDecorator);
}
//EntityStatePtr createNonCollState(EntityStatePtr stat) {
//	IGraphicsComponentPtr graphicsPtr(
//		stat->getGraphics());
//	IPhysicsComponentPtr notColphysicsPtr = nullptr;
//	notColphysicsPtr.reset(new NotCollisionPhysicsDecorator(stat->getPhysics()));
//	return make_shared<EntityState>(graphicsPtr, notColphysicsPtr);
//}
EntityPtr createHero(std::string const & rootAnimationsFolder , cv::Mat& canvas)
{
	cv::Rect canvasRect(0,0,canvas.size().width, canvas.size().height);
	fs::path root = rootAnimationsFolder;
	auto idleFolder = root / "idle";

	auto idle = createHeroState(root / "idle", HeroStates::HERO_IDLE , canvasRect);
	auto runRight = createHeroState(root / "runRight", HeroStates::HERO_RUN_RIGHT , canvasRect);
	auto jump = createHeroState(root / "jump", HeroStates::HERO_JUMP , canvasRect);
	auto runLeft = createHeroState(root / "runLeft", HeroStates::HERO_RUN_LEFT , canvasRect);
	auto duckDown = createHeroState(root / "duckDown", HeroStates::HERO_DUCK , canvasRect);
	auto duckStay = createHeroState(root / "duckStay", HeroStates::HERO_STAY_DUCK , canvasRect);
	auto standAfterDuck = createHeroState(root / "standAfterDuck", HeroStates::HERO_STAND_AFTER_DUCK , canvasRect);
	auto throwApple = createHeroState(root / "idle", HeroStates::THROW_APPLE, canvasRect);
	IPhysicsComponentPtr nonCollPhysicd = nullptr;
	//////////----------- fill ---------------------/////////////
	//-----idle
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, runRight);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, runLeft);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duckDown);
	//idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::SPACE }, duckDown);
	/*idle->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY },
		createNonCollState(idle));*/

	
	//-----runRight
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, runRight);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duckDown);
	//runRight->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, createNonCollState(runRight));

	//createNonCollState(runRight)->addState()
	//-----runLeft
	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, idle);
	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, runLeft);
	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duckDown);
	//-----jump
	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED }, idle);
	//jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, collisionWithSlime);
	//-----duckDown
	duckDown->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, duckStay);
	
	duckStay->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, standAfterDuck);
	//-----standAfterDuck
	standAfterDuck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, idle);
	EntityPtr hero(new Entity(idle));

	idle->Register(hero);
	runRight->Register(hero);
	jump->Register(hero);
	runLeft->Register(hero);
	duckDown->Register(hero);
	duckStay->Register(hero);
	standAfterDuck->Register(hero);


	return hero;
}
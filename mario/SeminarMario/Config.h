#pragma once

enum EventSenders { SENDER_ENTITY_STATE, SENDER_KEYBOARD, SENDER_TIMER };
enum EventTypes { EVENT_KEY_PRESSED, EVENT_PHYSICS, EVENT_GRAPHICS, EVENT_TIMER };
enum EventCodes {
	ENTITY_PHYSICS_FINISHED,
	ENTITY_FINISHED_ANIMATION,
	TIME_TICK,
	HALF_SECOND_FROM_COLL,
	KEY_ESC = 27,
	KEY_LEFT = 'a',
	KEY_RIGHT = 'd',
	KEY_DOWN = 's',
	KEY_UP = 'w',
	SPACE = 'q',
	//hero
	COLLISION_WITH_ENEMY,
	//add life
	COLLISION_WITH_FOOD_LIFE,
	LIFE_ENDED,
	COLLISION_WITH_FOOD_SCORE,
};
enum SignalEntitiesInGame
{
	HERO,
	HEARTS,
	SCORE,
	APPLE
};
enum ManyEntitiesInGame
{
	SLIMES,
	//APPLE
};
enum Lengthes {
	NUM_OF_SIGNAL_ENTITIES = 4 , 
	NUM_OF_MANY_ENTITIES = 1
};
//enum col1 {
//	SLIMES_T = 0
//	//APPLE = 3
//};
//enum col2 {
//	SLIMES
//};
//NUM_OF_ENTITIES , 4


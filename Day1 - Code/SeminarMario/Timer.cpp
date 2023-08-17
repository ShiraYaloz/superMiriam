#include "Timer.h"
#include "opencv2/opencv.hpp"
#include "Config.h"
#include "EntitiesPool.h"

Timer::Timer(size_t frequency_ms)
	:_frequency_ms(frequency_ms)
{
}

void Timer::tick()
{
	ticsCycles++;
	int key = cv::waitKey(_frequency_ms);

	Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::TIME_TICK });
	if (key != -1)
		Notify(Event{EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED , key });
	if (ticsCycles == 20)
	{
		Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::HALF_SECOND_FROM_COLL });
		ticsCycles = 0;
	}
}

//void Timer::eachHalfSecond()
//{
//	cv::(1000);
//	Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::HALF_SECOND_FROM_COLL });
//}

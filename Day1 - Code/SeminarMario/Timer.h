#pragma once
#include "Observer.h"

class Timer : public Subject
{
private:
	size_t _frequency_ms;
	int ticsCycles = 0;
public:
	Timer(size_t frequency_ms);

	void tick();
};


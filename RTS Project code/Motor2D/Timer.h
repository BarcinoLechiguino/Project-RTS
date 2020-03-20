#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at;
};

#endif //__j1TIMER_H__
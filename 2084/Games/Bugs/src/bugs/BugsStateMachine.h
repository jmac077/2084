#pragma once

#include "mg\gsm\state\GameStateMachine.h"

class BugsStateMachine : public GameStateMachine
{
public:
	BugsStateMachine() {}
	~BugsStateMachine() {}
	void update();
};
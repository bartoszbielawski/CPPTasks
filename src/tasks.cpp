/*
 * tasks.cpp
 *
 *  Created on: 12.2016
 *      Author: Bartosz Bielawski
 *     License: MIT
 */

#include "task.hpp"

using namespace std;
using namespace Tasks;

void Tasks::scheduleSingle(Task* t)
{
	if (t == nullptr)
		return;

	if (t->getState() == State::READY)
		t->run();
}

void Tasks::updateSleepSingle(Task* t)
{
	if (!t)
		return;

	switch (t->getState())
	{
		case State::WAITING:
			if (t->checkCondition())
			{
				t->resume();
				break;
			}
			//no break
		case State::SLEEPING:
			if (!t->decreaseTimer())
				t->resume();
			break;

		default:
			break;
	}
}

#pragma once
#include "types.h"
#include <functional>

typedef std::function<void()> eventproc;

class Event {
public:
	Event(eventproc f, time_type t, int p, long long index) {
		func = f;
		time = t;
		priority = p;
		tiebreaker = index;
	}
	time_type time;
	long long tiebreaker;
	int priority;
	void execute() {
		func();
	}
	void setFunc(eventproc f) {
		func = f;
	}
private:
	eventproc func;
};
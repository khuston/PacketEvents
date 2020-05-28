#pragma once
#include "event.h"
#include <queue>
#include <vector>
#include <memory>

struct is_event_later {
	constexpr bool operator()(Event & L, Event & R) const {
		if (L.time == R.time) {
			if (L.priority == R.priority) {
				return L.tiebreaker > R.tiebreaker;
			}
			return L.priority < R.priority;
		}
		return L.time > R.time;
	}
};

class EventQueue {
public:
	EventQueue();
	~EventQueue();
	void add_eventproc(eventproc event_handler, time_type event_time, int priority);
	void do_next();
	void reset_time();
	time_type get_time();
	bool empty();
private:
	std::priority_queue<Event, std::vector<Event>, is_event_later> * event_queue;
	void (*func) ();
	time_type time;
	long long event_index;
};
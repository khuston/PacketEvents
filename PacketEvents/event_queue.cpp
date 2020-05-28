#include "event.h"
#include "event_queue.h"

EventQueue::EventQueue()
{
	event_queue = new std::priority_queue<Event, std::vector<Event>, is_event_later>();
	event_index = 0;

	reset_time();
}

EventQueue::~EventQueue()
{
	delete event_queue;
}

void EventQueue::add_eventproc(eventproc event_handler, time_type event_time, int priority)
{
	std::shared_ptr<Event> event(new Event(event_handler, event_time, priority, event_index));
	event_queue->push(*event);
	event_index++;
}

void EventQueue::do_next()
{
	Event event = event_queue->top();
	event_queue->pop();

	time = event.time;

	event.execute();
}

void EventQueue::reset_time()
{
	time = 0;
}

time_type EventQueue::get_time()
{
	return time;
}

bool EventQueue::empty()
{
	return event_queue->empty();
}

#pragma once

#include "types.h"
#include "event_queue.h"
#include <memory>
#include <list>

struct Request {
	Request(time_type duration);
	time_type arrival_time;
	time_type process_duration;
	time_type process_start_time;
};

class ProcessorSimulation {
public:
	ProcessorSimulation();
	int request_capacity;
	void run();
	void schedule_request_arrival(time_type arrival_time, std::shared_ptr<Request>);
private:
	void on_receive_request(std::shared_ptr<Request> request);
	void on_add_request_to_queue(std::shared_ptr<Request> request);
	void on_complete_request(std::shared_ptr<Request> request);
	time_type get_time();
	void schedule_next_request_completion();
	std::list<std::shared_ptr<Request>> request_queue;
	std::shared_ptr<Request> active_request;
	std::unique_ptr<EventQueue> events;
};
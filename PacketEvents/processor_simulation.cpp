#include "processor_simulation.h"

ProcessorSimulation::ProcessorSimulation()
{
	std::list<std::shared_ptr<Request>> request_queue();
	std::shared_ptr<Request> active_request(nullptr);
	events = std::make_unique<EventQueue>();
	request_capacity = 0;
}

void ProcessorSimulation::run()
{
	events->reset_time();

	while (!events->empty()) {
		events->do_next();
	}
}

void ProcessorSimulation::schedule_request_arrival(time_type arrival_time, std::shared_ptr<Request> request)
{
	eventproc request_arrives = [this, request]() {
		on_receive_request(request);
	};
	events->add_eventproc(request_arrives, arrival_time, 1);
}

void ProcessorSimulation::on_receive_request(std::shared_ptr<Request> request)
{
	if (request_queue.size() < request_capacity) {
		// Schedule processor check of queue. The instructions state that requests arriving at the same instant of
		// time should all be queued *before* any is taken from the queue for processing.
		// This would seemingly mean that two packets of duration 0 arriving at the same time would require two slots
		// in the buffer in order for both to be processed, but this is not the case.
		eventproc request_is_ready = [this, request]() {
			on_add_request_to_queue(request);
		};

		if ((request->process_duration == 0) && (active_request == nullptr)) {
			request->process_start_time = get_time();
		}
		else {
			request_queue.push_back(request);
			events->add_eventproc(request_is_ready, get_time(), 2);
		}
	}
	// else request is dropped
}

void ProcessorSimulation::on_add_request_to_queue(std::shared_ptr<Request> request)
{
	if (!active_request) {
		schedule_next_request_completion();
	}
}

void ProcessorSimulation::on_complete_request(std::shared_ptr<Request> request)
{
	request_queue.pop_front();
	active_request = nullptr;
	schedule_next_request_completion();
}

time_type ProcessorSimulation::get_time()
{
	return events->get_time();
}

void ProcessorSimulation::schedule_next_request_completion()
{
	if (!request_queue.empty()) {
		active_request = request_queue.front();
		active_request->process_start_time = get_time();

		eventproc request_completes = [this]() {
			on_complete_request(this->active_request);
		};
		events->add_eventproc(request_completes, get_time() + active_request->process_duration, 3);
	}
}

Request::Request(time_type duration)
{
	process_duration = duration;
	process_start_time = -1;
	arrival_time = -1;
}

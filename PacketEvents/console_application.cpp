#include <iostream>
#include "processor_simulation.h"

typedef std::vector<std::shared_ptr<Request>> Requests;

std::unique_ptr<Requests> read_requests() {
    std::unique_ptr<Requests> requests = std::make_unique<Requests>();
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        time_type arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        std::shared_ptr<Request> request(new Request(process_time));
        request->arrival_time = arrival_time;
        requests->push_back(request);
    }
    return requests;
}


void print_responses(Requests requests) {
    for (auto request : requests)
        std::cout << (request->process_start_time) << std::endl;
}


int main()
{
    int size;
    std::cin >> size;
    std::unique_ptr<Requests> requests = read_requests();

    ProcessorSimulation sim;

    sim.request_capacity = size;
    for (auto request : *requests) {
        sim.schedule_request_arrival(request->arrival_time, request);
    }

    sim.run();

    print_responses(*requests);
}
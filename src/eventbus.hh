#pragma once
#include "headers.hh"

class EventBus {
private:
	std::map<std::string, std::set<EBus_Fn*>> callbacks;
	//std::queue<Event*> pending;

public:

	// Int types for error codes
	void send(Event* event); // Called by any object
	//void dispatch; // Called per cycle
	int subscribe(std::string type, EBus_Fn* callback);
	int unsubscribe(std::string type, EBus_Fn* callback);
};

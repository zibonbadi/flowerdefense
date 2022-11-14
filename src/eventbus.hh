#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "headers.hh"

using EBus_Fn = std::function<void(Event*)>;

class EventBus{
	private:
		std::map<std::string, std::set<EBus_Fn*>> callbacks;
		//std::queue<Event*> pending;

	public:
		EventBus();
		~EventBus();
		
		// Int types for error codes
		void send(Event* event); // Called by any object
		//void dispatch(); // Called per cycle
		int subscribe(std::string type, EBus_Fn *callback);
		int unsubscribe(std::string type, EBus_Fn *callback);
};
//} g_eventbus;

#endif


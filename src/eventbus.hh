#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "headers.hh"

using EBus_Fn = std::function<void(Event*)>;



class Eventbus{
	private:
		std::map<std::string, std::set<EBus_Fn*>> callbacks;
		std::queue<Event*> pending;

	public:
		Eventbus();
		~Eventbus();
		
		// Int types for error codes
		void send(Event* event); // Called by any object
		//void dispatch(); // Called per cycle
		int subscribe(std::string type, EBus_Fn *callback);
		int unsubscribe(std::string type, EBus_Fn *callback);
};

//Eventbus g_eventbus;

#endif




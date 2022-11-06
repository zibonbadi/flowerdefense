#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "headers.hh"

class Eventbus{
	private:
		std::map<std::string, std::vector<void*>> callbacks;

	public:
		Eventbus();
		~Eventbus();
		
		// Int types for error codes
		void send(std::string event);
		int subscribe();
		int unsubscribe();
};
#endif




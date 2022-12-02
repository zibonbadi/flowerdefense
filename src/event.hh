#pragma once
#include "headers.hh"

class Event{
	private:
		std::map<std::string, std::string> attributes;

	public:
		Event(std::string type);
		~Event();
		
		// Int types for error codes
		std::string get(std::string key);
		std::string find(std::string value); // Find key by value
		void set(std::string key, std::string value);
};





#include "event.hh"

Event::Event(std::string type){
	this->set("type", type);
};
Event::~Event(){
};

std::string Event::get(std::string key){
	return this->attributes[key];
};
std::string Event::find(std::string value){
	// Find key by value
	auto rVal = this->attributes.find(value);
	if(rVal != this->attributes.end()){
		return rVal->first;
	}
	return "";
};
void Event::set(std::string key, std::string value){
	this->attributes[key] = value;
};


#include "eventbus.hh"

EventBus::EventBus(){
};

EventBus::~EventBus(){
};

void EventBus::send(Event *event){
	//this->pending.push(event);
	
	// Find event type
	auto e_type = event->get("type");
	if( e_type == ""){
		throw std::runtime_error("Invalid Event type");
	}

	auto type_i = this->callbacks.find(e_type);
	if(type_i != this->callbacks.end()){
		std::clog << "EventBus.send(): Propagating event \"" << e_type << "\"." << std::endl;
		for(auto & cb : this->callbacks[e_type]){
			//std::clog << cb << std::endl;
			(*cb)(event);
		}
	}else{
		throw std::runtime_error("Event propagation failed");
	}
};

/*
void EventBus::dispatch(){
	//for(auto & : this->pending);
}
*/

/*
void operator<<(EventBus& channel, const Event& event){ channel.send(event); };
*/

int EventBus::subscribe(std::string type, EBus_Fn* callback){
	// Create missing vector
	auto type_i = this->callbacks.find(type);
	if(type_i == this->callbacks.end()){ this->callbacks[type] = std::set<EBus_Fn*>(); }
	
	// Reload for assurance & Insert callback
	type_i = this->callbacks.find(type);
	if(type_i != this->callbacks.end() && this->callbacks[type].find(callback) == this->callbacks[type].end() ){
		this->callbacks[type].insert(callback);
		std::clog << "EventBus.subscribe(\"" << type << "\", " << callback << "): Successfully subscribed." << std::endl;
		return 0;
	}
	return -1;
};

int EventBus::unsubscribe(std::string type, EBus_Fn* callback){
	/*
	auto vec_it = std::find(this->callbacks[type].begin(), this->callbacks[type].end(), callback);
	if(vec_it != this->callbacks[type].end()){ vec_it.erase(); }
	if(this->callbacks[type]->size() > 0){ delete this->callbacks[type]; }
	*/

	// Filter for event type
	auto type_i = this->callbacks.find(type);
	if(type_i != this->callbacks.end()){
		// Filter for event type
		auto callback_i = this->callbacks[type].find(callback);
		// Remove callback
		if(callback_i != this->callbacks[type].end()){
			//this->callbacks[type].push_back(callback);
			
			// Remove all callbacks that match input
			// (should be one; TODO: make unique in container)
			this->callbacks[type].erase(
				callback_i
				/*
				//std::remove_if(
				std::remove(
					this->callbacks[type].begin(),
					this->callbacks[type].end(),
					//[callback](EBus_Fn* f){ return f == callback; }
					callback
				)
					*/
				);
		}
		// Remove vector if empty
		if(callback_i == this->callbacks[type].end()){
			this->callbacks.erase(type);
		}
		std::clog << "EventBus.unsubscribe(\"" << type << "\", " << callback << "): Successfully unsubscribed." << std::endl;
		return 0;
	}
	std::cerr << "EventBus.unsubscribe(\"" << type << "\", " << callback << "): Failed." << std::endl;
	return -1;
};


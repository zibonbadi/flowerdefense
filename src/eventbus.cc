#include "eventbus.hh"

EventBus g_eventbus;

void EventBus::send(Event *event){
	//this->pending.push(event);
	
	// Find event type
	auto e_type = event->get("type");
	if( e_type == ""){
		throw std::runtime_error("Invalid Event type");
	}

	ENGINE_DEBUG_MSG("EventBus.send(): Propagating event \"" << e_type << "\".");
	auto type_i = this->callbacks.find(e_type);
	if(type_i != this->callbacks.end()){
		if(this->callbacks[e_type].empty()){
			throw std::runtime_error("No subscribers found");
		}
		for(auto & cb : this->callbacks[e_type]){
			//ENGINE_DEBUG_MSG(cb << std::endl;
			(*cb)(event);
		}
	}else{
		throw std::runtime_error("Event type not found");
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
		ENGINE_DEBUG_MSG("EventBus.subscribe(\"" << type << "\", " << callback << "): Successfully subscribed.");
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
		ENGINE_DEBUG_MSG("EventBus.unsubscribe(\"" << type << "\", " << callback << "): Successfully unsubscribed.");
		return 0;
	}
	std::cerr << "EventBus.unsubscribe(\"" << type << "\", " << callback << "): Failed." << std::endl;
	return -1;
};


#include "eventbus.hh"

Eventbus::Eventbus(){
};

Eventbus::~Eventbus(){
};

void Eventbus::send(Event *event){
	//this->pending.push(event);
	
	// Find event type
	auto e_type = event->get("type");
	if( e_type == ""){
		throw std::runtime_error("Invalid Event type");
	}

	auto type_i = this->callbacks.find(e_type);
	if(type_i != this->callbacks.end()){
		for(auto & callback : this->callbacks[e_type]){
			std::clog << callback << std::endl;
			//callback(event);
		}
	}else{
		throw std::runtime_error("Event propagation failed");
	}
};

/*
void Eventbus::dispatch(){
	//for(auto & : this->pending);
}
*/

/*
void operator<<(Eventbus& channel, const Event& event){ channel.send(event); };
*/

int Eventbus::subscribe(std::string type, EBus_Fn* callback){
	// Create missing vector
	auto type_i = this->callbacks.find(type);
	if(type_i == this->callbacks.end()){ this->callbacks[type] = std::set<EBus_Fn*>(); }
	// Insert callback
	if(type_i != this->callbacks.end()){
		this->callbacks[type].insert(callback);
		return 0;
	}
	return -1;
};

#if 0
int Eventbus::unsubscribe(std::string type, std::function<int(const Event&)> callback){
	/*
	auto vec_it = std::find(this->callbacks[type].begin(), this->callbacks[type].end(), callback);
	if(vec_it != this->callbacks[type].end()){ vec_it.erase(); }
	if(this->callbacks[type]->size() > 0){ delete this->callbacks[type]; }
	*/

	auto type_i = this->callbacks.find(type);
	if(type_i != this->callbacks.end()){
		auto callback_i = this->callbacks[type].find(callback);
		// Remove callback
		if(callback_i != this->callbacks[type].end()){
			//this->callbacks[type].push_back(callback);
			
			// Remove all callbacks that match input
			// (should be one; TODO: make unique in container)
			this->callbacks[type].erase(
				std::remove(
					this->callbacks[type].begin(),
					this->callbacks[type].end(),
					callback
				));
		}
		// Remove vector if empty
		if(callback_i == this->callbacks[type].end()){
			this->callbacks.erase(type);
		}
		return 0;
	}
	return -1;
};
#endif


#include "keymapper.hh"

KeyMapper::KeyMapper(EventBus* target){
};

KeyMapper::~KeyMapper(){
};


int KeyMapper::bind(SDL_Keycode key, Event event){
	try{
		this->key_events.at(key) = event;
		std::clog << "KeyMapper.bind(" << key << "): Bound key " << key << " to \"" << event.get("type") << "\"." << std::endl;
		return 0;
	}catch(std::exception &e){
		std::cerr << "KeyMapper.bind(" << key << ") exception: " << e.what() << std::endl;
		return -1;
	}
};

int KeyMapper::unbind(SDL_Keycode key){
	try{
		this->key_events.erase(key);
		std::clog << "KeyMapper.unbind(" << key << "): Unbound key " << key << std::endl;
		return 0;
	}catch (std::exception &e){
		std::cerr << "KeyMapper.unbind(" << key << ") exception: " << e.what() << std::endl;
		return -1;
	}
};

int KeyMapper::probe(SDL_KeyboardEvent sdlevent){
	try{
		std::cerr << "KeyMapper.probe(" << sdlevent.keysym.sym << "): Called." << std::endl;
		this->bus->send(&this->key_events.at(sdlevent.keysym.sym));
		return 0;
	}catch (std::exception &e){
		std::cerr << "KeyMapper.probe() exception: " << e.what() << std::endl;
		return -1;
	}
};

#include "keymapper.hh"

KeyMapper::KeyMapper(Eventbus* target){
};

KeyMapper::~KeyMapper(){
};


int KeyMapper::bind(SDL_Keycode key, Event event){
	try{
		std::cerr << "KeyMapper.bind(" << key << "): Bound key " << key << " to " << event.get("type") << std::endl;
		return 0;
	}catch(std::exception &e){
		std::cerr << "KeyMapper.bind(" << key << ") exception: " << e.what() << std::endl;
		return -1;
	}
};

int KeyMapper::unbind(SDL_Keycode key){
	try{
		std::cerr << "KeyMapper.probe(" << key << "): Unbound key " << key << std::endl;
		return 0;
	}catch (std::exception &e){
		std::cerr << "KeyMapper.probe(" << key << ") exception: " << e.what() << std::endl;
		return -1;
	}
};

int KeyMapper::probe(SDL_KeyboardEvent sdlevent){
	try{
		std::cerr << "KeyMapper.probe(): Called." << std::endl;
		return 0;
	}catch (std::exception &e){
		std::cerr << "KeyMapper.probe() exception: " << e.what() << std::endl;
		return -1;
	}
};

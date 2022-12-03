#include "plane.hh"

Plane::Plane(){
};

Plane::Plane(Z_PlaneMeta init){
	this->transform_rect = init;
};

Plane::~Plane(){
};


void Plane::attach(Tilemap *src){
	this->tilemaps.push_back(src);
};
void Plane::attach(Sprite *src){
	//this->sprites.insert(std::pair<>, src);
	this->sprites.push_back(src);
};

void Plane::detach(Tilemap* src) {
	this->tilemaps.erase(std::remove(this->tilemaps.begin(), this->tilemaps.end(), src), this->tilemaps.end());
};
void Plane::detach(Sprite* src) {
	this->sprites.erase(std::remove(this->sprites.begin(), this->sprites.end(), src), this->sprites.end());
};

void Plane::set_visible(bool value){
	this->visible = value;
};

bool Plane::is_visible(){
	return this->visible;
}

void Plane::toggle_visible(){
	this->visible = !this->visible;
};

void Plane::transform(Z_PlaneMeta transform){
	//Z_PlaneMeta new_tf = this->transform;
	this->transform_rect.x += transform.x;
	this->transform_rect.y += transform.y;
	this->transform_rect.w += transform.w;
	this->transform_rect.h += transform.h;
	this->transform_rect.pivot_x += transform.pivot_x;
	this->transform_rect.pivot_y += transform.pivot_y;
	this->transform_rect.deg += transform.deg;
	this->transform_rect.u += transform.u;
	this->transform_rect.v += transform.v;
	this->transform_rect.uw += transform.uw;
	this->transform_rect.vw += transform.uw;
	//this->setTransform(new_tf);
};

void Plane::set_transform(Z_PlaneMeta new_tf){
	this->transform_rect = new_tf;
};

Z_PlaneMeta Plane::get_transform(){
	return this->transform_rect;
}

void Plane::render(SDL_Renderer *renderer){

	if(!visible){ return; }
	// Set a few crops (for fancy stuff later)
	//this->transform_rect;
	Z_PlaneMeta tmp = {
		.x = this->transform_rect.x,
			.y = this->transform_rect.y,
			// Only tranlate x/y. Don't bother with affine stuff
			.w = 0.0f,
			.h = 0.0f,
			.pivot_x = 0.0f,
			.pivot_y = 0.0f,
			.deg = 0.0f,
			.u = 0.0f,
			.v = 0.0f,
			.uw = 0.0f,
			.vw = 0.0f,
	};
	
	// Render planes in registration order
	//ENGINE_DEBUG_MSG("Plane.render(): Calling Tilemaps...");
	//for(auto & t : this->tilemaps){ t->render(renderer, tmp); }
	for(auto i = 0; i < this->tilemaps.size(); i++){
	//for(auto & t : this->tilemaps){
		try{
			//ENGINE_DEBUG_MSG("Plane.render(): Calling Tilemap " << i);
			// Plane-based Frustum culling
			/*
			auto pos = i.first;
			auto pos_x = pos.first;
			auto pos_y = pos.second;
			auto val = i.second;

			if(
				pos.x > screen.w ||
				pos.y > screen.h ||
				pos.x+pos.w < 0 ||
				pos.y+pos.h < 0
			){ continue; }
			*/

			//t->render(renderer, tmp);
			this->tilemaps.at(i)->render(renderer, tmp);
		}catch(std::exception &e){
			std::cerr << "Plane.render() exception: " << e.what() << std::endl;
		}
	}
	//for(auto & t : this->tilemaps){ std::get<1>(t)->render(renderer, this->transform_rect); }
	//ENGINE_DEBUG_MSG("Plane.render(): Calling Sprites...");
	//for(auto & s : this->sprites){ s->render(renderer, tmp); }
	for(auto & s : this->sprites){
		try{
			s->render(renderer, tmp);
		}catch(std::exception &e){
			std::cerr << "Plane.render() exception: " << e.what() << std::endl;
		}
	}
	//for(auto & s : this->sprites){ std::get<1>(s)->render(renderer, this->transform_rect); }
	//ENGINE_DEBUG_MSG("Plane.render(): Done.");

};

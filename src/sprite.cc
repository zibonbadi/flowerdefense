#include "plane.hh"

Sprite::Sprite(){
	Z_PlaneMeta insert = {
		.x = 0.0f,
			.y = 0.0f,
			.w = 0.0f,
			.h = 0.0f,
			.pivot_x = 0.0f,
			.pivot_y = 0.0f,
			.deg = 0.0f,
			.u = 0.0f,
			.v = 0.0f,
			.uw = 0.0f,
			.vw = 0.0f
	};
	this->transform_rect = insert;
};

Sprite::Sprite(std::string first_anim_name, Animation* first_anim, Z_PlaneMeta init){
	this->add_animation(first_anim_name, first_anim);
	this->transform_rect = init;
	this->switch_to_anim(first_anim_name);
};

Sprite::Sprite(Z_PlaneMeta init){
	this->transform_rect = init;
};

Sprite::~Sprite(){
};

int Sprite::set_color(Z_RGBA color){
	this->color = color;
	std::clog << "Sprite.set_color(): Updated Color: RGBA("
		<< this->color.r
		<< ", " << this->color.g
		<< ", " << this->color.b
		<< ", " << this->color.b
		<< ", " << this->color.a
		<< ")." << std::endl;
	return 0;
};

int Sprite::add_animation(std::string name, Animation* animation){
	if(animation == nullptr){
		std::cerr << "Sprite.add_animation(): Invalid Animation (nullptr)" << std::endl;
		return -1;
	}
	this->anime[name] = animation;
	return 0;
};

void Sprite::transform(Z_PlaneMeta add_tf){
	this->transform_rect.x += add_tf.x;
	this->transform_rect.y += add_tf.y;
	this->transform_rect.w += add_tf.w;
	this->transform_rect.h += add_tf.h;
	this->transform_rect.pivot_x += add_tf.pivot_x;
	this->transform_rect.pivot_y += add_tf.pivot_y;
	this->transform_rect.deg += add_tf.deg;
	this->transform_rect.u += add_tf.u;
	this->transform_rect.v += add_tf.v;
	this->transform_rect.uw += add_tf.uw;
	this->transform_rect.vw += add_tf.uw;
};

void Sprite::setTransform(Z_PlaneMeta new_tf){
	this->transform_rect = new_tf;
};

Z_PlaneMeta Sprite::get_transform(){
	return this->transform_rect;
}

int Sprite::switch_to_anim(std::string animation_name){
	if(this->anime.find(animation_name) != this->anime.end()){
		this->current_anime = animation_name;
		this->anime[animation_name]->restart();
		//this->anime[animation_name]->sync();
	}
	return 0;
};

void Sprite::render(SDL_Renderer *renderer){
	Z_PlaneMeta zero = {
		.x = 0.0f,
		.y = 0.0f,
		.w = 0.0f,
		.h = 0.0f,
		.pivot_x = 0.0f,
		.pivot_y = 0.0f,
		.deg = 0.0f,
		.u = 0.0f,
		.v = 0.0f,
		.uw = 0.0f,
		.vw = 0.0f
	};
	this->render(renderer, zero);
};

void Sprite::render(SDL_Renderer* renderer, Z_PlaneMeta transform){
	Z_PlaneMeta plane_meta = {
		.x = this->transform_rect.x + transform.x,
			.y = this->transform_rect.y + transform.y,
			.w = (transform.w >= 1.0f)?transform.w:this->transform_rect.w,
			.h = (transform.h >= 1.0f)?transform.h:this->transform_rect.h,
			.pivot_x = this->transform_rect.pivot_x + transform.pivot_x,
			.pivot_y = this->transform_rect.pivot_y + transform.pivot_y,
			.deg = this->transform_rect.deg + transform.deg,
			/*
			.u = this->transform_rect.u + transform.u,
			.v = this->transform_rect.v + transform.v,
			.uw = this->transform_rect.uw + transform.uw,
			.vw = this->transform_rect.vw + transform.vw
			*/
			.u = 0.0f,
			.v = 0.0f,
			.uw = 0.0f,
			.vw = 0.0f
	};

	/*
	std::clog << "Sprite.render(): Calculative UVs " <<
		"[" << this->transform_rect.u << ',' << this->transform_rect.v << ':' << this->transform_rect.uw << 'x' << this->transform_rect.vw << ']' << " => " <<
		"(" << transform.u << ',' << transform.v << ':' << transform.u << 'x' << transform.vw << ')' << 
	std::endl;
	*/

	SDL_Rect pos, crop;
	SDL_Point pivot;

	pivot.x = plane_meta.pivot_x;
	pivot.y = plane_meta.pivot_y;

	pos.x = plane_meta.x;
	pos.y = plane_meta.y;
	pos.w = plane_meta.w;
	pos.h = plane_meta.h;
	crop.x = plane_meta.u;
	crop.y = plane_meta.v;
	crop.w = plane_meta.uw;
	crop.h = plane_meta.vw;

	/*
	std::clog << "Sprite.render(): Current frame " <<
		"[" << crop.x << ',' << crop.y << ':' << crop.w << 'x' << crop.h << ']' << " => " <<
		"(" << pos.x << ',' << pos.y << ':' << pos.w << 'x' << pos.h << ')' << 
	std::endl;
*/

	// Frustum culling
	/*
	if(
		pos.x > screen.w ||
		pos.y > screen.h ||
		pos.x+pos.w < 0 ||
		pos.y+pos.h < 0
	){ return; }
	*/

	try{
		/*
		if(
			this->anime.size() < 1 ||
			this->anime.find(this->current_anime) == this->anime.end() ||
			this->anime.at(this->current_anime) == nullptr
		){
		*/
			////std::cerr << "Sprite animation error @ \"" << this->current_anime << "\":" << this->anime.size() << std::endl;
			//SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, 255);
			//SDL_RenderFillRect(renderer, &pos);
		//}else{
			//std::clog << "Rendering animation \"" << this->current_anime << "\":" << this->anime.size() << "\"..." << std::endl;
			if(this->debug_sprite){
				SDL_Rect dest = {
					.x = (int)plane_meta.x,
					.y = (int)plane_meta.y,
					.w = (int)plane_meta.w,
					.h = (int)plane_meta.h
				};
				SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x7F, 0x5F);
				SDL_RenderDrawRect(renderer, &dest);
			}
			if(this->debug_collide && this->box_collide != nullptr){
				SDL_Rect dest = {
					.x = (int)plane_meta.x+(int)this->box_collide->x,
					.y = (int)plane_meta.y+(int)this->box_collide->y,
					.w = (int)this->box_collide->w,
					.h = (int)this->box_collide->h
				};
				SDL_SetRenderDrawColor(renderer, 0x7F,0x00,0x00, 0x5F);
				SDL_RenderDrawRect(renderer, &dest);
			};
			this->anime.at(this->current_anime)->render(renderer, plane_meta);
		//}
	}catch(std::exception &e){
		//std::cerr << "Sprite.render() exception: " << e.what() << std::endl;
		SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
		SDL_RenderFillRect(renderer, &pos);
	}
};


void Sprite::setCollider(Z_PlaneMeta* box){
	// Coordinates are in sprite space
	// xywh rect for bounds check
	this->box_collide = box;
};

Z_PlaneMeta* Sprite::getCollider(){
	return this->box_collide;
};

bool Sprite::collision(Sprite* opponent){
	// Boolean collision
	// Collisions are in plane space (cross-plane collisions ignored for now)
	auto opponent_collide = opponent->getCollider();
	auto opponent_transform = opponent->get_transform();
	return (
		opponent_collide != nullptr &&
		this->transform_rect.x+this->box_collide->x <= (opponent_transform.x+opponent_collide->x+opponent_collide->w) &&
		opponent_transform.x+opponent_collide->x <= (this->transform_rect.x+this->box_collide->x+this->box_collide->w) &&
		this->transform_rect.y+this->box_collide->y <= (opponent_transform.y+opponent_collide->y+opponent_collide->h) &&
		opponent_transform.y+opponent_collide->y <= (this->transform_rect.y+this->box_collide->y+this->box_collide->h) 
	);
};

Z_PlaneMeta Sprite::getCollisionArea(Sprite* opponent){
	// Collide rect (intersection)
	// Collisions are in plane space (cross-plane collisions ignored for now)
	auto opponent_collide = opponent->getCollider();
	auto opponent_transform = opponent->get_transform();
	if(opponent_collide != nullptr){
		Z_PlaneMeta intersect = {
			// Reformed 
			.x = (opponent_transform.x+opponent_collide->x+opponent_collide->w) - this->transform_rect.x+this->box_collide->x,
			.y = (this->transform_rect.x+this->box_collide->x+this->box_collide->w) - opponent_transform.x+opponent_collide->x,
			.w = (opponent_transform.y+opponent_collide->y+opponent_collide->h) - this->transform_rect.y+this->box_collide->y,
			.h = (this->transform_rect.y+this->box_collide->y+this->box_collide->h) - opponent_transform.y+opponent_collide->y
		};

		// Adjust for sequence
		intersect.x *= (intersect.w<0)?-1:1;
		intersect.w *= (intersect.w<0)?-1:1;
		intersect.y *= (intersect.h<0)?-1:1;
		intersect.h *= (intersect.h<0)?-1:1;

		return intersect;
	};

	// Dummy rect. w/h=0 should make it invalid
	return Z_PlaneMeta {
		.x = 0,
		.y = 0,
		.w = 0,
		.h = 0,
	};
};

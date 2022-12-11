#include "tilemap.hh"

Tilemap::Tilemap(){
	Z_PlaneMeta tmp = {
		.x = 0.0f,
			.y = 0.0f,
			// Only tranlate x/y. Don't bother with affine stuff
			.w = (float)this->grit_w,
			.h = (float)this->grit_h,
			.pivot_x = 0.0f,
			.pivot_y = 0.0f,
			.deg = 0.0f,
			.u = 0.0f,
			.v = 0.0f,
			.uw = 0.0f,
			.vw = 0.0f,
	};
	this->grit_w = 32;
	this->grit_h = 32;
	this->set_transform(tmp);
}

/*
Tilemap::Tilemap(SDL_Texture* base, int grit_w, int grit_h){
	this->base = base;
	this->grit_w = grit_w;
	this->grit_h = grit_h;
	//this->tiles = nullptr;
	this->print_map();
};
*/

Tilemap::Tilemap(int grit_w, int grit_h){
	this->grit_w = grit_w;
	this->grit_h = grit_h;
	Z_PlaneMeta tmp = {
		.x = 0.0f,
			.y = 0.0f,
			// Only tranlate x/y. Don't bother with affine stuff
			.w = (float)this->grit_w,
			.h = (float)this->grit_h,
			.pivot_x = 0.0f,
			.pivot_y = 0.0f,
			.deg = 0.0f,
			.u = 0.0f,
			.v = 0.0f,
			.uw = 0.0f,
			.vw = 0.0f,
	};
	this->set_transform(tmp);
	//this->tiles = nullptr;
	this->print_map();
};

Tilemap::~Tilemap(){
};

int Tilemap::set_grit(int grit_w, int grit_h){
	if(grit_w > 0){ this->grit_w = grit_w; }
	if(grit_h > 0){ this->grit_h = grit_h; }
	return 0;
};

void Tilemap::transform(Z_PlaneMeta transform){
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
};

Z_PlaneMeta Tilemap::get_transform(){
	return this->transform_rect;
};

void Tilemap::set_transform(Z_PlaneMeta new_tf){
	this->transform_rect = new_tf;
};

/*
//int Tilemap::new_crop(int x, int y, int w, int h){
char Tilemap::new_crop(char id, Z_PlaneMeta crop){
	try{
		SDL_Rect tmp {
			.x = (int) crop.u,
			.y = (int) crop.v,
			.w = (int) crop.uw,
			.h = (int) crop.vw
		};
		this->crops[id] = tmp;
		ENGINE_DEBUG_MSG("Tilemap.new_crop(): Added crop [" << id << "]:("
			<< crop.x << ':' << crop.y << ';' << crop.w << 'x' << crop.h << ')' << std::endl;
		return id;
	}catch(std::exception &e){
		std::cerr << "Tilemap.new_crop() error: " << e.what() << std::endl;
	}
	return '\0';
};
*/

char Tilemap::add_tile(char id, Sprite* tile){
	try{
		this->tiles[id] = tile;
		ENGINE_DEBUG_MSG("Tilemap.add_tile(): Added Tile [" << id << "]");
		return id;
	}catch(std::exception &e){
		std::cerr << "Tilemap.new_crop() error: " << e.what() << std::endl;
	}
	return '\0';
};

void Tilemap::print_map(){
	ENGINE_DEBUG_MSG("Tilemap: Current map = "
		<< "[");
	//if(this->tiles != nullptr){
		for(auto & i : this->map){
			auto pos = std::get<0>(i);
			auto pos_x = std::get<0>(pos);
			auto pos_y = std::get<1>(pos);
			auto val = std::get<1>(i);
			ENGINE_DEBUG_MSG("(" << pos_x << "," << pos_y << "|" << val << "), ");
		};
	//};
		ENGINE_DEBUG_MSG("]");
}

int Tilemap::import_map(std::map<std::pair<unsigned int,unsigned int>, char> data){
	// Kosher check
	for(auto & i : data){
		if(this->tiles.find(std::get<1>(i)) == this->tiles.end()){
		return -1;
		}
		/*
		else{
			this->tiles[std::pair<int,int> (x,y)] = tile;
		}
		*/
	}
	this->map = data;
	//print_map();
	return 0;
}

void Tilemap::write(std::pair<unsigned int,unsigned int> top_left, std::string gameOverText){
	auto top_left_x = top_left.first;
	auto top_left_y = top_left.second;
	unsigned int pos_x = 0;
	unsigned int pos_y = 0;

	for(auto & i : gameOverText){
		//auto pos = i.first;
		//auto val = i.second;
		switch(i){
		case '\r':{
			pos_x = 0;
			break;
		}
		case '\n':{
			pos_y++;
			break;
		}
		case ' ':{
			pos_x++;
			break;
		}
		default:{
			//ENGINE_DEBUG_MSG('(' << (top_left_x+pos_x) << ',' << (top_left_y+pos_y) << "):" << i << ' ' ;
			this->map[std::pair<unsigned int, unsigned int>(top_left_y+pos_y, top_left_x+pos_x)] = i;
			pos_x++;
			break;
		}
		}
	}
		//ENGINE_DEBUG_MSG(std::endl;
		//ENGINE_DEBUG_MSG("Rendered tilemap text: (" << top_left_x << ',' << top_left_y << ") " << text << std::endl;
	//this->print_map();

};

std::map<std::pair<unsigned int,unsigned int>, char> Tilemap::get_map(){
	return this->map;
};

char Tilemap::get_spot(int x, int y){
	try{
		return this->map.at(std::pair<int,int> (y,x));
	}catch (...){
		return ' ';
	}
};

std::pair<int,int> Tilemap::get_coordinate_from_offset(float x, float y){
	return std::pair(
			(int)y / this->grit_h,
			(int)x / this->grit_w
			);
};

int Tilemap::fill(int map_x, int map_y, int w, int h, char tile){
	try{
		//if(this->tiles != nullptr && this->map.find(tile) != this->map.end()){
		if(this->tiles.find(tile) != this->tiles.end() || tile == ' '){
			//ENGINE_DEBUG_MSG("Tilefind is valid");
			// Is this even valid? If no -> skip
			for(int y = map_y; y < map_y+h; y++){
				for(int x = map_x; x < map_x+w; x++){
					//ENGINE_DEBUG_MSG('(' << x << ',' << y << ") => '" << tile << '\'');
					if(tile == ' '){
						// Remove tile
						//ENGINE_DEBUG_MSG('"Tilemap.fill(): Clearing tile (" << x << ',' << y << ").");
						this->map.erase(std::pair(y,x));
					}else{
						this->map[std::pair<int,int> (y,x)] = tile;
					}
				}
			}
		}

	}catch(std::exception &e){
		std::cerr << "Tilemap.fill() error: " << e.what() << std::endl;
	}
	print_map();
	return 0;
};

int Tilemap::clear_map(){
	this->map.clear();
	ENGINE_DEBUG_MSG("Tilemap: Map cleared" << std::endl);
	return 0;
};

int Tilemap::clear_tileset(){
	this->tiles.clear();
	ENGINE_DEBUG_MSG("Tilemap: Tileset cleared" << std::endl);
	return 0;
};

void Tilemap::render(SDL_Renderer* renderer){
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

void Tilemap::render(SDL_Renderer* renderer, Z_PlaneMeta transform){
	//ENGINE_DEBUG_MSG("Tilemap.render(): " << this->map << std::endl;

/*
	ENGINE_DEBUG_MSG("Tilemap.render("
	<< "grit_w = " << (int) this->grit_w << "; " 
	<< "grit_h = " << (int) this->grit_h
	<< "): [" << std::endl;
	*/

	//if(this->tiles != nullptr){
	if(!this->visible){ return; }

		Z_PlaneMeta zp_tmp = {
			.x = 0.0f,
				.y = 0.0f,
				// Only tranlate x/y. Don't bother with affine stuff
				.w = (float)this->grit_w,
				.h = (float)this->grit_h,
				.pivot_x = 0.0f,
				.pivot_y = 0.0f,
				.deg = 0.0f,
				.u = 0.0f,
				.v = 0.0f,
				.uw = 0.0f,
				.vw = 0.0f,
		};
		for(auto & i : this->map){
			try{
				auto pos = i.first;
				auto pos_x = pos.second;
				auto pos_y = pos.first;
				auto val = i.second;

			//.x = transform.x+this->transform_rect.x,
				zp_tmp.x = (this->grit_w*pos_x)+transform.x+this->transform_rect.x;
				zp_tmp.y = (this->grit_h*pos_y)+transform.y+this->transform_rect.y;

				SDL_Rect sdlr_tmp {
						.x = (int)zp_tmp.x,
						.y = (int)zp_tmp.y,
					//.x = pos_x*this->grit_w+zp_tmp.x,
						//.y = pos_y*this->grit_h+zp_tmp.y,
						.w = this->grit_w,
						.h = this->grit_h
				};
	
				/*
				ENGINE_DEBUG_MSG("Plotting (" << pos_x << ',' << pos_y <<
					'|' << sdlr_tmp.x << ',' << sdlr_tmp.y <<
					':' << sdlr_tmp.w << 'x' << sdlr_tmp.h <<
					'|' << val << ")" << std::endl;
					*/
				if( this->tiles.find(val) == this->tiles.end() ){
					//ENGINE_DEBUG_MSG("Plotting debug texture " << val << std::endl;
					SDL_SetRenderDrawColor(renderer, 0xff,0,0, 255);
					SDL_RenderDrawRect(renderer, &sdlr_tmp);
				}else{
					//ENGINE_DEBUG_MSG("Plotting real texture " << val << std::endl;
					Sprite* to_render = this->tiles[val];
					//zp_tmp.x = (this->grit_w*pos_x+zp_tmp.x) - to_render->get_transform().x;
					//zp_tmp.y = (this->grit_h*pos_y+zp_tmp.y) - to_render->get_transform().y;
					zp_tmp.x = zp_tmp.x - to_render->get_transform().x;
					zp_tmp.y = zp_tmp.y - to_render->get_transform().y;
					to_render->render(renderer, zp_tmp);
				}
				/*
				   SDL_RenderCopy(renderer, this->base,
				   &this->map[val], &tmp
				   );
				   */
			}catch(std::exception &e){
				std::cerr << "Tilemap.render() exception: " << e.what() << std::endl;
			}
		};
		//};
};


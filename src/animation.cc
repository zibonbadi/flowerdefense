#include "animation.hh"

Animation::Animation(SDL_Texture* base, unsigned int tickrate){
	this->base = base;
	this->set_rate(tickrate);
	this->restart();
	ENGINE_DEBUG_MSG("Animation created: " << this);
};

Animation::~Animation(){
};


int Animation::advance(unsigned int now){
	// Only update once per engine step
	if(this->last_update >= now){
		ENGINE_DEBUG_MSG("Animation.advance() Skipped: " <<  now-this->last_update);
		return 0;
	}

	if( this->frames.size() < 1 ||
		this->xsheet.size() < 1){
		this->step_n = 0;
		this->xsheet_i = 0;
		return -1;
	}

	this->time_acc += now - this->last_update;
	//ENGINE_DEBUG_MSG("Animation.advance() Tick time beforehand: " <<  this->time_acc << '/' << this->ticktime);
	while(this->time_acc >= ticktime){ 
		// Halting frame?
		/*
		ENGINE_DEBUG_MSG("Animation.advance() timer data: [" << xsheet_i << ':' << this->xsheet.size()-1
			<< " - " << step_n << ':' << this->xsheet[this->xsheet_i].second-1 << ']');
			*/
		if(this->xsheet[this->xsheet_i].second > 0){
			this->step_n++;
			// Advance xsheet index when full
			if(this->step_n >= this->xsheet[this->xsheet_i].second){
				//this->step_n %= std::get<1>(this->xsheet[this->xsheet_i]);
				this->step_n = 0;
				this->xsheet_i++;
			}
			// Loop xsheet
			if(this->xsheet_i >= this->xsheet.size()){ 
				xsheet_i = 0; 
				//this->restart();
			}
			//this->xsheet_i %= this->xsheet.size();
		}
		this->time_acc -= this->ticktime;
	}
	//ENGINE_DEBUG_MSG("Animation.advance() Tick time after: " <<  this->time_acc << '/' << this->ticktime);
	this->last_update = now;
	return this->time_acc;
};
int Animation::sync(unsigned int now){
	this->last_update = now;
	this->time_acc = 0;
	return 0;
};
int Animation::restart(){
	this->step_n = 0;
	this->xsheet_i = 0;
	//this->sync();
	return 0;
};


int Animation::add_frame(int u, int v, int uw, int vw){
	this->add_frame(Z_PlaneMeta{.u = u, . v = v, .uw = uw, .vw = vw, });
};
int Animation::add_frame(Z_PlaneMeta crop){
	try{
		auto insert = crop;
		/*
		SDL_Rect insert = {
			.x = (int)crop.u,
			.y = (int)crop.v,
			.w = (int)crop.uw,
			.h = (int)crop.vw
		};
		*/
		this->frames.push_back(insert);
		ENGINE_DEBUG_MSG("Added frame to animation " << this << ": [" << this->frames.size()-1 << "](" <<
			insert.x << ',' << insert.y << ':' << insert.y << 'x' << insert.h << ')');
		return this->frames.size()-1;
	}catch(std::exception &e){
		std::cerr << "Animation.add_frame() error: " << e.what() << std::endl;
		return -1;
	}
};

int Animation::add_xsheet_phase(unsigned int frame, unsigned int duration){
	this->xsheet.push_back(std::pair<int,int>(frame,duration));
	ENGINE_DEBUG_MSG("Added xsheet phase to animation " << this << ": [" << this->xsheet.size() - 1 << "]: " << frame << '.' << duration);
	return this->xsheet.size() - 1;
};

int Animation::clear_frames() {
	this->frames.clear();
	ENGINE_DEBUG_MSG("Cleared frame set of animation " << this);
	return 0;
};
int Animation::clear_xsheet() {
	this->xsheet.clear();
	ENGINE_DEBUG_MSG("Cleared xsheet of animation " << this);
	return 0;
};
int Animation::clear_all(){
	this->clear_frames();
	this->clear_xsheet();
	this->base = nullptr;
	return 0;
};

int Animation::set_rate(unsigned int tps){
	if(tps > 0){
		this->ticktime = 1000.0f/tps;
		return this->ticktime;
	}
	return -1;
};
void Animation::render(SDL_Renderer* renderer){
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

void Animation::render(SDL_Renderer* renderer, Z_PlaneMeta transform){

	//try{
		//ENGINE_DEBUG_MSG("Animation.render(): " << this->frames.size() << ':' << this->xsheet.size());
		if(this->frames.size() < 1){ throw std::runtime_error("Frame set missing!"); }
		if(this->xsheet.size() < 1){ throw std::runtime_error("X-Sheet missing!"); }
		/*
		ENGINE_DEBUG_MSG("Animation.render(): [" << xsheet_i << ':' << this->xsheet.size()-1
			<< " - " << step_n << ':' << this->xsheet[this->xsheet_i].second-1 << ']');
		*/

		auto currentframe = this->frames[this->xsheet[this->xsheet_i].first];

		SDL_Rect pos {
				.x = (int)transform.x,
				.y = (int)transform.y,
				.w = (int)transform.w,
				.h = (int)transform.h,
		}, crop {
				.x = (int)currentframe.u+(int)transform.u,
				.y = (int)currentframe.v+(int)transform.v,
				.w = (int)currentframe.uw+(int)transform.uw,
				.h = (int)currentframe.vw+(int)transform.vw,
		};
		SDL_Point pivot = {
			.x = (int)transform.pivot_x+(currentframe.pivot_x),
			.y = (int)transform.pivot_y+(currentframe.pivot_y)
		};

		/*
		ENGINE_DEBUG_MSG("Animation.render(): Current frame " <<
			"[" << crop.x << ',' << crop.y << ':' << crop.w << 'x' << crop.h << ']' << " => " <<
			"(" << pos.x << ',' << pos.y << ':' << pos.w << 'x' << pos.h << ')');
			*/


		if(
				SDL_RenderCopyEx(renderer, this->base,
					&crop, &pos,
					transform.deg+currentframe.deg, &pivot,
					SDL_FLIP_NONE 
					) != 0){
			throw std::runtime_error("Render error");
		};
		/*
	}catch(std::exception &e){
		SDL_Rect pos {
				.x = (int)transform.x,
				.y = (int)transform.y,
				.w = (int)transform.w,
				.h = (int)transform.h,
		};
		std::cerr << "Animation.render() exception: " << e.what() << std::endl;
		SDL_SetRenderDrawColor(renderer, 0xff,0,0xff, 255);
		SDL_RenderFillRect(renderer, &pos);
	}
		*/
}


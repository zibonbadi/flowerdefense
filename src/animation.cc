#include "animation.hh"

Animation::Animation(SDL_Texture* base, unsigned int tickrate){
	this->base = base;
	this->set_rate(tickrate);
	this->restart();
};

Animation::~Animation(){
};


int Animation::advance(unsigned int now){
	// Only update once per engine step
	if(this->last_update >= now){
		std::clog << "Animation.advance() Skipped: " <<  now-this->last_update << std::endl;
		return 0;
	}

	if( this->frames.size() < 1 ||
		this->xsheet.size() < 1){
		this->step_n = 0;
		this->xsheet_i = 0;
		return -1;
	}

	this->time_acc += now - this->last_update;
	//std::clog << "Animation.advance() Tick time beforehand: " <<  this->time_acc << '/' << this->ticktime << std::endl;
	while(this->time_acc >= ticktime){ 
		// Halting frame?
		/*
		std::clog << "Animation.advance() timer data: [" << xsheet_i << ':' << this->xsheet.size()-1
			<< " - " << step_n << ':' << this->xsheet[this->xsheet_i].second-1 << ']' << std::endl;
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
	//std::clog << "Animation.advance() Tick time after: " <<  this->time_acc << '/' << this->ticktime << std::endl;
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
	SDL_Rect insert = {
		.x = u,
		.y = v,
		.w = uw,
		.h = vw
	};
	this->frames.push_back(insert);
	std::clog << "Added frame to animation " << this << ": [" << this->frames.size()-1 << "](" <<
	insert.x << ',' << insert.y << ':' << insert.w << 'x' << insert.h << ')' << std::endl;
	return this->frames.size()-1;
};
int Animation::add_frame(Z_PlaneMeta crop){
	SDL_Rect insert = {
		.x = (int)crop.u,
		.y = (int)crop.v,
		.w = (int)crop.uw,
		.h = (int)crop.vw
	};
	this->frames.push_back(insert);
	std::clog << "Added frame to animation " << this << ": [" << this->frames.size()-1 << "](" <<
	insert.x << ',' << insert.y << ':' << insert.y << 'x' << insert.h << ')' << std::endl;
	return this->frames.size()-1;
};

int Animation::add_xsheet_phase(unsigned int frame, unsigned int duration){
	this->xsheet.push_back(std::pair<int,int>(frame,duration));
	std::clog << "Added xsheet phase to animation " << this << ": [" << this->xsheet.size()-1 << "]: " << frame << '.' << duration << std::endl;
	return this->xsheet.size()-1;
};

int Animation::clear_frames(){
	this->frames.clear();
	std::clog << "Cleared frame set of animation " << this << std::endl;
	return 0;
};
int Animation::clear_xsheet(){
	this->xsheet.clear();
	std::clog << "Cleared xsheet of animation " << this << std::endl;
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

	try{
		//std::clog << "Animation.render(): " << this->frames.size() << ':' << this->xsheet.size() << std::endl;
		if(this->frames.size() < 1){ throw std::runtime_error("Frame set missing!"); }
		if(this->xsheet.size() < 1){ throw std::runtime_error("X-Sheet missing!"); }
		/*
		std::clog << "Animation.render(): [" << xsheet_i << ':' << this->xsheet.size()-1
			<< " - " << step_n << ':' << this->xsheet[this->xsheet_i].second-1 << ']' << std::endl;
		*/

		SDL_Rect currentframe = this->frames[this->xsheet[this->xsheet_i].first];

		SDL_Rect pos {
				.x = (int)transform.x,
				.y = (int)transform.y,
				.w = (int)transform.w,
				.h = (int)transform.h,
		}, crop {
				.x = (int)currentframe.x+(int)transform.u,
				.y = (int)currentframe.y+(int)transform.v,
				.w = (int)currentframe.w+(int)transform.uw,
				.h = (int)currentframe.h+(int)transform.vw,
		};
		SDL_Point pivot = {
			.x = (int)transform.pivot_x,
			.y = (int)transform.pivot_y
		};

		/*
		std::clog << "Animation.render(): Current frame " <<
			"[" << crop.x << ',' << crop.y << ':' << crop.w << 'x' << crop.h << ']' << " => " <<
			"(" << pos.x << ',' << pos.y << ':' << pos.w << 'x' << pos.h << ')' << 
			std::endl;
			*/


		if(
				SDL_RenderCopyEx(renderer, this->base,
					&crop, &pos,
					transform.deg, &pivot,
					SDL_FLIP_NONE 
					) != 0){
			//throw std::runtime_error("Plane blit error");
		};
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
}


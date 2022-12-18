#include "game.hh"

// Actual stuff

Game::Game(){
	_init(SCREEN_W, SCREEN_H);
}

Game::Game(Z_RGBA background){
	this->background = background;
};

Game::Game(uint16_t w, uint16_t h, Z_RGBA background){
	this->background = background;
	_init(w, h);
};

Game::Game(uint16_t w, uint16_t h){
	_init(w, h);
};

Game::~Game(){
	delete this->a_specs;
	SDL_CloseAudioDevice(this->a_master);
};


void Game::_init(uint16_t w, uint16_t h){
	this->w = w;
	this->h = h;
	
	uint8_t rflags, wflags, imgflags, mixflags;

	rflags =
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC
		;
	wflags =
		SDL_WINDOW_SHOWN |
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_ALLOW_HIGHDPI
		;
	imgflags =
		IMG_INIT_PNG
		;
	mixflags =
		MIX_INIT_FLAC |
		MIX_INIT_MP3 |
		MIX_INIT_OGG
		;

	ENGINE_DEBUG_MSG("Initializing canvas (SDL)..." << SDL_GetError());

	// Init main SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
		ENGINE_DEBUG_MSG("SDL_Init() failed! Why? " << SDL_GetError());
		throw std::runtime_error("Runtime error in Game constructor");
	}

	/* Audio */
	/*
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
		ENGINE_DEBUG_MSG("SDL2_Mixer failed! Why? " << SDL_GetError());
		throw std::runtime_error("Runtime error in Game constructor");
	}
	*/

	if(Mix_Init(mixflags) < 0){
		ENGINE_DEBUG_MSG("SDL2_Mixer failed! Why? " << SDL_GetError());
		throw std::runtime_error("Unable to initialize SDL2_Mixer");
	}

	SDL_AudioSpec aspec_want = {
		.freq = 48000,
		.format = AUDIO_F32,
		.channels = 2,
		.samples = 4096,
		//.callback = void*
	};
	SDL_AudioSpec* a_spec_tmp = new SDL_AudioSpec;


	this->a_master = SDL_OpenAudioDevice(NULL, 0, &aspec_want, a_spec_tmp, 0);
	if (this->a_master <= 0) {
		std::cerr << "SDL_OpenAudioDevice() failed. Why? " << SDL_GetError() << std::endl;
	}
	else {
		this->a_specs = a_spec_tmp;
		this->a_buf_interleaved = std::vector<float>(this->a_specs->samples * 2);
		SDL_PauseAudioDevice(this->a_master, 0);
	}
	ENGINE_DEBUG_MSG("Audio (SDL_Audio) Initialized.");

	this->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->w, this->h, wflags);

	/* Window & GPU */
	if (!this->window) {
		ENGINE_DEBUG_MSG("SDL_CreateWindow() failed! Why? " << SDL_GetError());
		throw std::runtime_error("Runtime error in Game constructor");
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	this->renderer = SDL_CreateRenderer(this->window, -1, rflags);

	if (!this->renderer) {
		ENGINE_DEBUG_MSG("SDL_CreateRenderer() failed! Why? " << SDL_GetError());
		throw std::runtime_error("Runtime error in Game constructor");
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");

	SDL_RenderSetViewport(this->renderer, nullptr);
	SDL_RenderSetLogicalSize(this->renderer, this->w, this->h);

	ENGINE_DEBUG_MSG("GPU (SDL) Initialized.");

	this->framebuf = SDL_CreateTexture(
		this->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		this->w, this->h
	);
	if (!this->framebuf) {
		ENGINE_DEBUG_MSG("SDL_CreateTexture() failed! Why? " << SDL_GetError());
		throw std::runtime_error("Runtime error in Game constructor");
	}

	// Init SDL_Image
	if (IMG_Init(imgflags) < 0) {
		printf("IMG_Init() failed! Why? %s\n", SDL_GetError());
		throw std::runtime_error("Runtime error in Game constructor");
	}

	ENGINE_DEBUG_MSG("Game (SDL) Initialized.");

}

SDL_Renderer* Game::getRenderer() {
	return this->renderer;
}
SDL_Window* Game::getWindow() {
	return this->window;
}
SDL_Texture* Game::getFramebuffer() {
	return this->framebuf;
}

int Game::load_mod(std::string path, int32_t subsong, int32_t repeats) {
	try {
		//std::vector<float> audio_left(480);
		//std::vector<float> audio_right(480);
		//if(this->mod_file != nullptr){ this->mod_file = nullptr; }
		std::ifstream modfile(path, std::ios::binary);
		if (!modfile.good()) { throw std::runtime_error("Unable to open file"); }
		//this->mod_file = &modfile;
		// Avoid memory leakage
		if (this->mod_buf != nullptr) { delete this->mod_buf; }
		this->mod_buf = new openmpt::module(modfile);
		//openmpt::module mod(modfile);
		this->mod_buf->select_subsong(subsong);
		this->mod_buf->set_repeat_count(repeats);
		//this->mod_buf = &mod;
		auto mod_title = this->mod_buf->get_metadata("title");
		auto mod_artist = this->mod_buf->get_metadata("artist");

		ENGINE_DEBUG_MSG("Game.load_mod(\"" << path << "\") now playing: " <<
			((mod_artist == "") ? "[UNKNOWN]" : mod_artist) << " - " <<
			((mod_title == "") ? "[UNKNOWN]" : mod_title) <<
			'[' << (int)subsong << "]x" << (int)repeats);
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << "Game.load_mod(" << path << ") exception: " << e.what() << std::endl;
		return -1;
	}
}

int Game::load_mod(openmpt::module* mod, int32_t subsong, int32_t repeats) {
	try {
		// Segfault check
		if (mod == nullptr) { throw std::runtime_error("Invalid tracker module (nullptr)"); }
		// Avoid memory leakage
		this->mod_buf = mod;
		this->mod_buf->select_subsong(subsong);
		this->mod_buf->set_repeat_count(repeats);
		//this->mod_buf = &mod;
		auto mod_title = this->mod_buf->get_metadata("title");
		auto mod_artist = this->mod_buf->get_metadata("artist");

		ENGINE_DEBUG_MSG("Game.load_mod(<openmpt::module>:" << mod << ") now playing: " <<
			((mod_artist == "") ? "[UNKNOWN]" : mod_artist) << " - " <<
			((mod_title == "") ? "[UNKNOWN]" : mod_title) <<
			'[' << (int)subsong << "]x" << (int)repeats);
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << "Game.load_mod(<openmpt::module>:" << mod << ") exception: " << e.what() << std::endl;
		return -1;
	}
}

void Game::attach(Plane* src) {
	this->planes.push_back(src);
}

void Game::set_background(Z_RGBA background) {
	this->background = background;
};

void Game::render() {
	try {
		if (this->mod_buf != nullptr && this->a_master > 0) {
			/* Audio render */
			size_t audio_count = this->mod_buf->read_interleaved_stereo((int32_t)48000, (size_t)this->a_specs->samples, this->a_buf_interleaved.data());
			// Attempting to correct for audio source. FAILED
			//size_t audio_count = this->mod_buf->read_interleaved_stereo((int32_t) this->a_specs->freq, (size_t) this->a_specs->samples, this->a_buf_interleaved.data());
			if (audio_count != 0) {
				// Stereo 32-Bit samples -> 2 channels * 4 Bytes of audio
				//int audio_status = SDL_QueueAudio(this->a_master, this->a_buf_interleaved.data(), audio_count * 8);
			}
		}

		/* Video render */
		//SDL_SetRenderDrawColor(this->renderer, 0,0,0, 255);
		SDL_SetRenderDrawColor(this->renderer, this->background.r, this->background.g, this->background.b, 255);
		SDL_RenderClear(this->renderer);

		/*
		// 4 byte RGBA video buffer
		unsigned char buf[this->h*this->w*4];
		for(int i = 0; i < this->h*this->w; i++){
		buf[i*4] = SDL_ALPHA_OPAQUE; // Alpha
		buf[i*4+1] = rand()%256; // B
		buf[i*4+2] = rand()%256; // G
		buf[i*4+3] = rand()%256; // R
		}
		SDL_UpdateTexture(this->videomagic.framebuf, NULL, buf, this->w*4);
		SDL_RenderCopy(this->videomagic.renderer,this->videomagic.framebuf, NULL, NULL);
		*/

		//ENGINE_DEBUG_MSG("Game.render(): Calling planes..." << std::endl;
		// Render planes in registration order
		for (auto& i : this->planes) {
			i->render(this->renderer);
		}

		//ENGINE_DEBUG_MSG("Game.render(): Calling SDL_RenderPresent..." << std::endl;
		// Pass to graphics card
		SDL_RenderPresent(this->renderer);
		//ENGINE_DEBUG_MSG("Game.render(): Done." << std::endl;
	}catch(std::exception &e){
		std::cerr << "Game.render() exception: " << e.what() << std::endl;
	}
};


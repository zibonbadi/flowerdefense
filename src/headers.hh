#ifndef HEADERS_H
#define HEADERS_H

/* SDL setup */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
/* libopenmpt for gloptastic tracker goodness */
#include <libopenmpt/libopenmpt.hpp>
/* stdlib */
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <exception>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <array>
#include <memory>
#include <functional>

/* utils */
#include "defs.h"
#include "structs.hh"

/* Forward declarations */
class Animation;
class Event;
class EventBus;
class Game;
class KeyMapper;
class Plane;
class ResourceManager;
class Sprite;
class Tilemap;
class Hud;

/* custom classes */
#include "animation.hh"
#include "event.hh"
#include "eventbus.hh"
#include "game.hh"
#include "keymapper.hh"
#include "plane.hh"
#include "resourcemanager.hh"
#include "sprite.hh"
#include "tilemap.hh"
#include "hud.hh"

// Global Engine Objects
extern EventBus g_eventbus;
//extern KeyMapper g_keymapper(&g_eventbus);

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

#endif


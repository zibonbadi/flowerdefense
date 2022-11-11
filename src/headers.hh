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
class Game;
class Plane;
class Sprite;
class ResourceManager;
class Eventbus;
class Event;
class Tile;
class Tilemap;
class Tileset;

/* custom classes */
#include "resourcemanager.hh"
#include "game.hh"
#include "plane.hh"
#include "sprite.hh"
#include "tilemap.hh"
#include "animation.hh"
#include "event.hh"
#include "eventbus.hh"
#endif


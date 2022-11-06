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
#include <array>
#include <memory>

/* utils */
#include "defs.h"
#include "structs.hh"

/* Forward declarations */
class Animation;
class Canvas;
class Plane;
class Sprite;
class Texturepool;
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
#endif


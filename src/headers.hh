#pragma once

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

using EBus_Fn = std::function<void(Event*)>;

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

/* custom game classes Forward declarations */
class BFS;
class Player;
class Enemy;
class Enemypool;
class BFS;

/* custom game classes */
#include "bfs.hh"
#include "player.hh"
#include "enemy.hh"
#include "enemypool.hh"
#include "hud.hh"

// Global Engine Objects
extern EventBus g_eventbus;
extern ResourceManager g_rc;
extern KeyMapper g_keymapper;

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

#define TILE_HEIGHT 32
#define TILE_WIDTH 32

#define BFS_TILE_HEIGHT 16
#define BFS_TILE_WIDTH 16

extern bool g_isPrintEngineDEBUG;

#define DEBUG_MSG(str) if(true) std::cout << "Flowerdefense: " << str << std::endl;
#define ENGINE_DEBUG_MSG(str) if(g_isPrintEngineDEBUG) std::cout << "ENGINE: " << str << std::endl;

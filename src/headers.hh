#ifndef HEADERS_H
#define HEADERS_H

/* MyDbgNew.h
/* Defines global operator new to allocate from
/* client blocks*/
#ifdef _DEBUG
#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//allocations to be of _CLIENT_BLOCK type
#else
#define MYDEBUG_NEW
#endif // _DEBUG
/* MyApp.cpp
/* Compile options needed: /Zi /D_DEBUG /MLd
/* or use a
/* Default Workspace for a Console Application to
/* build a Debug version*/
#include "crtdbg.h"

#ifdef _DEBUG
#define new MYDEBUG_NEW
#endif
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

// Global Engine Objects
extern EventBus g_eventbus;
//extern KeyMapper g_keymapper(&g_eventbus);

#endif


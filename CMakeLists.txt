cmake_minimum_required(VERSION 3.24.2)
project(flowerdefense)
set(CMAKE_CXX_STANDARD 20)

set(EXEC flowerdefense)
include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

#add_executable(${EXEC} ${SOURCES})
add_executable(${EXEC} 
src/animation.cc
src/canvas.cc
src/eventbus.cc
src/main.cc
src/plane.cc
src/resourcepool.cc
src/sprite.cc
src/texturepool.cc
src/tilemap.cc
)

include(FindPkgConfig)

if (MSVC)
else()
	pkg_search_module(SDL2      REQUIRED sdl2>=2.0.1)
	#pkg_search_module(SDL2TTF   REQUIRED SDL2_ttf>=2.0.0)
	#pkg_search_module(SDL2NET   REQUIRED SDL2_net>=2.0.0)
	#pkg_search_module(SDL2GFX   REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct
	pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
	pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	pkg_search_module(LIBOPENMPT REQUIRED libopenmpt>=0.6)
endif()

include_directories(
	${SDL2_INCLUDE_DIRS}
	#${SDL2TTF_INCLUDE_DIRS}
	#${SDL2NET_INCLUDE_DIRS}
	#${SDL2GFX_INCLUDE_DIRS}
	${SDL2MIXER_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
	${LIBOPENMPT_INCLUDE_DIRS}
)

target_link_libraries(
	${PROJECT_NAME}
	${SDL2_LIBRARIES}
	#${SDL2TTF_LIBRARIES}
	#${SDL2NET_LIBRARIES}
	#${SDL2GFX_LIBRARIES}
	${SDL2MIXER_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
	openmpt
)



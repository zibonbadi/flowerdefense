cmake_minimum_required(VERSION 3.24.2)
project(flowerdefense)
set(CMAKE_CXX_STANDARD 20)

set(EXEC flowerdefense)
include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCES src/*.cc)

add_executable(${EXEC} ${SOURCES})
#add_executable(${EXEC} 
#src/animation.cc
#src/canvas.cc
#src/eventbus.cc
#src/main.cc
#src/plane.cc
#src/resourcepool.cc
#src/sprite.cc
#src/texturepool.cc
#src/tilemap.cc
#)

include(FindPkgConfig)

if (MSVC)
	find_package(SDL2 CONFIG REQUIRED)
	find_package(SDL2_ttf CONFIG REQUIRED)
	find_package(SDL2_net CONFIG REQUIRED)
	find_package(sdl2-gfx CONFIG REQUIRED)
	#find_package(sdl2_mixer CONFIG REQUIRED)
	find_package(SDL2_mixer CONFIG REQUIRED)
	find_package(sdl2-image CONFIG REQUIRED)
	find_package(libopenmpt CONFIG REQUIRED)
	# Set the path to SDL2_mixer.lib
	#set(SDL2_MIXER_LIBRARY "C:/Dev/vcpkg/packages/sdl2-mixer_x64-windows/lib/SDL2_mixer.lib")
elseif(APPLE)
	find_package(SDL2 CONFIG REQUIRED)
	#pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
	#pkg_search_module(SDL2NET REQUIRED SDL2_net>=2.0.0)
	#pkg_search_module(SDL2GFX REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct
	pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
	pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	pkg_search_module(LIBOPENMPT REQUIRED libopenmpt>=0.5.12)

else()
	pkg_search_module(SDL2 REQUIRED sdl2>=2.0.1)
	#pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
	#pkg_search_module(SDL2NET REQUIRED SDL2_net>=2.0.0)
	#pkg_search_module(SDL2GFX REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct
	pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
	pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	pkg_search_module(LIBOPENMPT REQUIRED libopenmpt>=0.5.12)
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
)

if (MSVC)
	target_link_libraries(
		${PROJECT_NAME}
		SDL2::SDL2_image
		libopenmpt::libopenmpt
		$<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
	)
else()
	target_link_libraries(
		${PROJECT_NAME}
		openmpt
	)
endif()

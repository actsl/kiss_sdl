cmake_minimum_required(VERSION 3.19)
project(kiss_sdl C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message(STATUS "SDL2 Header files is in ${SDL2_INCLUDE_DIRS}")

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
message(STATUS "SDL2TTF Header files is in ${SDL2_TTF_INCLUDE_DIR}")

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
message(STATUS "SDL2IMAGE Header files is in ${SDL2_IMAGE_INCLUDE_DIRS}")

file(GLOB TTF_FILES ${PROJECT_SOURCE_DIR}/*.ttf)
file(COPY ${TTF_FILES} DESTINATION ${CMAKE_BINARY_DIR})
file(GLOB PNG_FILES ${PROJECT_SOURCE_DIR}/*.png)
file(COPY ${PNG_FILES} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(.)

set(KISS_SDL_HEADERS kiss_sdl.h)

set(KISS_SDL_SRC kiss_draw.c
        kiss_general.c
        kiss_posix.c
        kiss_widgets.c)

add_executable(example1 kiss_example1.c ${KISS_SDL_HEADERS} ${KISS_SDL_SRC})
add_executable(example2 kiss_example2.c ${KISS_SDL_HEADERS} ${KISS_SDL_SRC})

target_link_libraries(example1 ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(example2 ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

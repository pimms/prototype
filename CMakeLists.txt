cmake_minimum_required(VERSION 2.8)

file(GLOB game_SRC
	"src/*.h"
	"src/*.cpp"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(game ${game_SRC})
target_link_libraries(game sfml-system)
target_link_libraries(game sfml-window)
target_link_libraries(game sfml-graphics)

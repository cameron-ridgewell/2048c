cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")
add_executable (game game.cxx board board.cxx box box.cxx)
target_link_libraries (game boost_program_options -lncurses)

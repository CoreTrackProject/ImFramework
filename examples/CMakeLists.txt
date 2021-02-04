cmake_minimum_required(VERSION 3.5)

project(hello_world)


set(EX_SRC
	hello_world.cpp
)

add_executable(hello_world ${EX_SRC})
target_include_directories(hello_world
    PUBLIC
    .
)
target_link_libraries(hello_world PUBLIC ImFramework)

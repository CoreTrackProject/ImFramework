cmake_minimum_required(VERSION 3.5)

project(ImFramework)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

set(IMFRAMEWORK_SRC
	ImFramework.h
	ImFramework.cpp

	ImThread.h
	ImThread.cpp
)


add_library(ImFramework ${IMFRAMEWORK_SRC})
target_include_directories(ImFramework 
    PUBLIC
    .
)
target_link_libraries(ImFramework PUBLIC glfw GLEW::GLEW)




#project(hello_world)

#set(SRC 
#	examples/hello_world.cpp
#)

#add_executable(hello_world ${SRC})

#target_link_libraries(hello_world PRIVATE ImFramework glfw GLEW::GLEW)

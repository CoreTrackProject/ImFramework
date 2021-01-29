cmake_minimum_required(VERSION 3.5)

project(ImFramework)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

set(IMGUI_VERSION "imgui")
set(IMGUI_SRC
    ${IMGUI_VERSION}/imgui.h
    ${IMGUI_VERSION}/imgui.cpp

    ${IMGUI_VERSION}/imgui_internal.h
    ${IMGUI_VERSION}/imconfig.h

    ${IMGUI_VERSION}/imgui_demo.cpp
    ${IMGUI_VERSION}/imgui_draw.cpp

    ${IMGUI_VERSION}/imgui_widgets.cpp
    ${IMGUI_VERSION}/imgui_tables.cpp

    ${IMGUI_VERSION}/imstb_rectpack.h 
    ${IMGUI_VERSION}/imstb_textedit.h
    ${IMGUI_VERSION}/imstb_truetype.h

    ${IMGUI_VERSION}/backends/imgui_impl_glfw.h
    ${IMGUI_VERSION}/backends/imgui_impl_glfw.cpp

    ${IMGUI_VERSION}/backends/imgui_impl_opengl3.h
    ${IMGUI_VERSION}/backends/imgui_impl_opengl3.cpp
)


set(IMFRAMEWORK_SRC
	ImFramework.h
	ImFramework.cpp
	
	ImThread.h
	ImThread.cpp
)


add_library(ImFramework ${IMGUI_SRC} ${IMFRAMEWORK_SRC})
target_include_directories(ImFramework 
    PUBLIC
    .
    ./imgui
)
target_link_libraries(ImFramework PUBLIC glfw GLEW::GLEW)




#project(hello_world)

#set(SRC 
#	examples/hello_world.cpp
#)

#add_executable(hello_world ${SRC})

#target_link_libraries(hello_world PRIVATE ImFramework glfw GLEW::GLEW)

project(external C CXX)

set(BUILD_TESTING_BCKP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Force disable of tests for external dependencies" FORCE)
set(CUSTOM_COVERAGE_EXCLUDE ${CUSTOM_COVERAGE_EXCLUDE} "external" PARENT_SCOPE) # Replaced in CTestCustom.cmake.in
set(BUILD_TESTING ${BUILD_TESTING_BCKP} CACHE BOOL "Build tests (default variable for CTest)" FORCE) #Set it back to its past value

#===============#
# ImGui library #
#===============#

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

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_library(ImGui ${IMGUI_SRC})
target_link_libraries(ImGui PRIVATE glfw GLEW::GLEW)
target_include_directories(ImGui PRIVATE imgui)



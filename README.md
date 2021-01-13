# ImFramework

ImGui package, a collection of ImGui libraries including window handling

Goal:
+ Window handling (glfw)
+ Backend vulkan/opengl
+ ImGui docking branch
+ Plugin handling


pseudo code code:


+ design pattern for threading which is convinient to use together with imgui

maybe: #include <ImThread.h> ?

ThreadToken token = ImFramework::DefineThread("Name", async_function_ptr, report_function_ptr, cancellation_token);

if(ImGui::Button("Start thread")) {

  ImFramework::StartThread(token)
  
}


if(ImFramework::ThreadHasData(token)) {

}

if(ImFramework::ThreadFinished(token)) {

}







Additional libraries:
+ Sequencer
+ Node editor
+ Plot lib








#include <ImFramework.h>

int main() {

ImFramework::Init(ImFramework::Window::GLFW, ImFramework::Backend::Vulkan);

while(ImFramework::Begin()) {
  ImGui::Begin("Hello world");
  
  ImGui::Button("Click me");
  
  ImGui::End();
  ImFramework::End();
}

return 0;
};

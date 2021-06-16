#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>


bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
	}
	
	ImGui::End();
}

void PhysicsInit() {
}

void PhysicsUpdate(float dt) {
}

void PhysicsCleanup() {
}
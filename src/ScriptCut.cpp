#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#include <iostream>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(480, 720, "ScriptCut", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(1.0f, 0.55f, 0.60f, 1.00f);
	int width, height = 0;
	int titleBarHeight = 84;

	enum MenuCollapseState
	{
		NoneOpen,
		CommandOpen,
		SequencesOpen,
		WorkspacesOpen
	};
	int openState = MenuCollapseState::CommandOpen;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glfwGetWindowSize(window, &width, &height);
		ImVec2 size(width, height);
		{
			ImGui::SetNextWindowPos(ImVec2());
			ImGui::SetNextWindowSize(size);
			ImGui::Begin("##Main", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);

			int numOpenMenus = 0;
			ImGui::SetNextItemOpen(openState == MenuCollapseState::CommandOpen);
			if (ImGui::CollapsingHeader("Commands", ImGuiTreeNodeFlags_SpanFullWidth))
			{
				openState = MenuCollapseState::CommandOpen;
				numOpenMenus++;
				ImGui::BeginChild("InsideCommands", ImVec2(0, height - titleBarHeight), true, ImGuiWindowFlags_AlwaysAutoResize);

				for (int i = 0; i < 5; i++)
				{
					{
						ImGui::BeginGroup();
						static char name[64] = "C:/my/cool/script.bat";
						ImGui::LabelText("##Command_label", "My description");
						ImGui::Indent();
						ImGui::InputText("##Input_label", name, 64);
						ImGui::SameLine();
						if (ImGui::Button("...")) {}
						if (width > 350) {
							ImGui::SameLine(width - 60);
						}
						else {
							ImGui::SameLine();
						}
						if (ImGui::Button("Run")) {}
						if (ImGui::CollapsingHeader("Arguments"))
						{
							ImGui::BeginChild("InsideArguments", ImVec2(), true);
							ImGui::Text("Place for arguments passed to the script");
							ImGui::EndChild();
						}
						ImGui::Unindent();
						ImGui::Separator();
						ImGui::Spacing();
						ImGui::EndGroup();
					}
				}
				ImGui::EndChild();
			}

			ImGui::SetNextItemOpen(openState == MenuCollapseState::SequencesOpen);
			if (ImGui::CollapsingHeader("Sequences", ImGuiTreeNodeFlags_SpanFullWidth))
			{
				openState = MenuCollapseState::SequencesOpen;
				numOpenMenus++;
				ImGui::BeginChild("InsideSequences", ImVec2(0, height - titleBarHeight), true, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::Text("Sequences here!");
				ImGui::EndChild();
			}

			ImGui::SetNextItemOpen(openState == MenuCollapseState::WorkspacesOpen);
			if (ImGui::CollapsingHeader("Workspaces", ImGuiTreeNodeFlags_SpanFullWidth))
			{
				openState = MenuCollapseState::WorkspacesOpen;
				numOpenMenus++;
				ImGui::BeginChild("InsideWorkspaces", ImVec2(0, height - titleBarHeight), true, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::Text("Workspaces here!");
				ImGui::EndChild();
			}

			if (numOpenMenus <= 0) openState = MenuCollapseState::NoneOpen;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

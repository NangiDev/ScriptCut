#include "ScriptCut.h"

int ScriptCut::Render(State *state)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int titleBarHeight = 84;
	{
		ImGui::SetNextWindowPos(ImVec2());
		ImGui::SetNextWindowSize(state->GetWindowSize());
		ImGui::Begin("##Main", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);

		int numOpenMenus = 0;
		ImGui::SetNextItemOpen(state->MenuOpenState == MenuCollapseState::CommandOpen);
		if (ImGui::CollapsingHeader("Commands", ImGuiTreeNodeFlags_SpanFullWidth))
		{
			state->MenuOpenState = MenuCollapseState::CommandOpen;
			numOpenMenus++;
			ImGui::BeginChild("InsideCommands", ImVec2(0, state->WindowHeight - titleBarHeight), true, ImGuiWindowFlags_AlwaysAutoResize);

			for (int i = 0; i < 5; i++)
			{
				{
					ImGui::BeginGroup();
					static char name[64] = "C:/my/cool/script.bat";
					ImGui::LabelText("##Command_label", "My description");
					ImGui::Indent();
					ImGui::InputText("##Input_label", name, 64);
					ImGui::SameLine();
					if (ImGui::Button("..."))
					{
					}
					if (state->WindowWidth > 350)
					{
						ImGui::SameLine(state->WindowWidth - 60);
					}
					else
					{
						ImGui::SameLine();
					}
					if (ImGui::Button("Run"))
					{
					}
					std::string title = "Arguments";
					std::string index = std::to_string(i);
					if (ImGui::CollapsingHeader((title + index).c_str()))
					{
						ImGui::Text("Place for arguments passed to the script");
					}
					ImGui::Unindent();
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::EndGroup();
				}
			}
			ImGui::EndChild();
		}

		ImGui::SetNextItemOpen(state->MenuOpenState == MenuCollapseState::SequencesOpen);
		if (ImGui::CollapsingHeader("Sequences", ImGuiTreeNodeFlags_SpanFullWidth))
		{
			state->MenuOpenState = MenuCollapseState::SequencesOpen;
			numOpenMenus++;
			ImGui::BeginChild("InsideSequences", ImVec2(0, state->WindowHeight - titleBarHeight), true, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Sequences here!");
			ImGui::EndChild();
		}

		ImGui::SetNextItemOpen(state->MenuOpenState == MenuCollapseState::WorkspacesOpen);
		if (ImGui::CollapsingHeader("Workspaces", ImGuiTreeNodeFlags_SpanFullWidth))
		{
			state->MenuOpenState = MenuCollapseState::WorkspacesOpen;
			numOpenMenus++;
			ImGui::BeginChild("InsideWorkspaces", ImVec2(0, state->WindowHeight - titleBarHeight), true, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Workspaces here!");
			ImGui::EndChild();
		}

		if (numOpenMenus <= 0)
			state->MenuOpenState = MenuCollapseState::NoneOpen;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(ScriptCut::GetWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(state->ClearColor.x * state->ClearColor.w, state->ClearColor.y * state->ClearColor.w, state->ClearColor.z * state->ClearColor.w, state->ClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(ScriptCut::GetWindow());

	return 0;
}

int ScriptCut::Update(State *state)
{
	glfwPollEvents();
	glfwGetWindowSize(ScriptCut::GetWindow(), &state->WindowWidth, &state->WindowHeight);
	return 0;
}

int main(int, char **)
{
	// Init GLFW window
	if (!ScriptCut::Init())
		return 1;

	// Implement State/Update/Render (SUR)
	int openState = MenuCollapseState::CommandOpen;

	State state{};
	state.ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (ScriptCut::IsRunning())
	{
		ScriptCut::Update(&state);
		ScriptCut::Render(&state);
	}

	ScriptCut::Clean();
	return 0;
}
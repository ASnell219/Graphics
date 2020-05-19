#include "ui.h"

UI::UI(Engine* engine) : System(engine)
{
	//
}

bool UI::Initialize()
{
	Renderer* renderer = m_engine->Get<Renderer>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(renderer->m_window, renderer->m_context);
	const char* glsl_version = "#version 430";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Style
	ImGui::StyleColorsDark();

	return true;
}

void UI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void UI::Update()
{
	ImGui_ImplSDL2_ProcessEvent(&m_engine->GetEvent());
}

void UI::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Start()
{
	Renderer* renderer = m_engine->Get<Renderer>();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(renderer->m_window);
	ImGui::NewFrame();
}

void UI::End()
{

}

bool UI::VectorOfStringGetter(void* data, int n, const char** out_text)
{
	const std::vector<std::string>* v = (std::vector<std::string>*)data;
	*out_text = v->at(n).c_str();
	return true;
}



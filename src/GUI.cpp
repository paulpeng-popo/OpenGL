#include "../myheaders/GUI.h"

GUI::GUI()
{
}

GUI::~GUI()
{
}

void GUI::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void GUI::startFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");
}

void GUI::render()
{
    ImGui::SetWindowPos(ImVec2(10, 10));
    ImGui::SetWindowSize(ImVec2(400, 250));
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void GUI::createSlider(std::string name, float *value, float min, float max)
{
    ImGui::SliderFloat(name.c_str(), value, min, max);
}

void GUI::createCheckbox(std::string name, bool *value)
{
    ImGui::Checkbox(name.c_str(), value);
}

bool VectorOfStringGetter(void *data, int n, const char **out_text)
{
    const std::vector<std::string> *v = (std::vector<std::string> *)data;
    std::string str = (*v)[n];
    str = str.substr(str.find_last_of("/\\") + 1);
    *out_text = str.c_str();
    return true;
}

void GUI::createCombo(std::string name, std::vector<std::string> items, int *value)
{
    ImGui::Separator();
    ImGui::Combo(name.c_str(), value, VectorOfStringGetter, (void *)&items, items.size());
}

void GUI::createExplorer(std::string name, std::string path, std::string filter, std::string *value)
{
    if (ImGui::Button("Add Texture"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(name.c_str(), "Choose File", filter.c_str(), path.c_str());
    }

    if (ImGuiFileDialog::Instance()->Display(name.c_str()))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            *value = ImGuiFileDialog::Instance()->GetFilePathName();
        }
        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::SameLine();
    const char *c = (*value == "") ? "No file selected" : (*value).substr((*value).find_last_of("/\\") + 1).c_str();
    ImGui::Text(c);
}

void GUI::endFrame()
{
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::cleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

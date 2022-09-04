#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/ImGuiFileDialog.h"

class GUI
{
public:
    GUI();
    ~GUI();
    void init(GLFWwindow *window);
    void startFrame();
    void render();
    void createSlider(std::string name, float *value, float min, float max);
    void createCheckbox(std::string name, bool *value);
    void createCombo(std::string name, std::vector<std::string> items, int *value);
    void createExplorer(std::string name, std::string path, std::string filter, std::string *value);
    void endFrame();
    void cleanUp();
};

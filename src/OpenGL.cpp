/*
	File: OpenGL.cpp
	Implementation of OpenGL.h
	Author: Paul peng
	Date: 2022.8.28
*/

#include "../myheaders/OpenGL.h"
#include "../myheaders/GLShader.h"
#include "../myheaders/MeshLoader.h"
#include "../myheaders/Error_Debug.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/ImGuiFileDialog.h"

std::string vs_path = "shaders/vertex.glsl";
std::string fs_path = "shaders/fragment.glsl";
std::string obj_path = "objects/3dmeshes/";

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

bool wireframe = false;
bool zoomMode = false;
bool grayMode = false;
bool mosaicMode = false;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model = glm::mat4(1.0f);

GLShader shader;
GLFWwindow *window;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int screen_width;
int screen_height;

// find out the path of all objecs in the directory
std::vector<std::string> get_obj_paths(std::string directory)
{
	std::vector<std::string> paths;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(directory.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				if (strstr(ent->d_name, ".obj") != NULL)
					paths.push_back(directory + ent->d_name);
			}
		}
		closedir(dir);
	}
	else
		ERROR("Could not open directory: %s", directory.c_str());

	return paths;
}

// Constructors
OpenGL::OpenGL()
{
	width = 800;
	height = 600;
	title = "test";

	DEBUG("OpenGL constructor");
	DEBUG("Width: %d, Height: %d", this->width, this->height);
	DEBUG("Window name: %s", this->title.c_str());
}

OpenGL::OpenGL(int w, int h, std::string str)
{
	width = w;
	height = h;
	title = str;

	DEBUG("OpenGL constructor");
	DEBUG("Width: %d, Height: %d", this->width, this->height);
	DEBUG("Window name: %s", this->title.c_str());
}

// Destructor
OpenGL::~OpenGL() {}

int OpenGL::UseGLFW()
{
	if (!glfwInit())
	{
		ERROR("Failed to initialize GLFW");
		return -1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		ERROR("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 50, 50);

	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetCursorPos(window, width / 2, height / 2);
	screen_width = width;
	screen_height = height;

	return 0;
}

int OpenGL::UseGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ERROR("Failed to initialize GLAD");
		return -1;
	}

	return 0;
}

int OpenGL::UseIMGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return 0;
}

void OpenGL::InitDefault()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	shader = GLShader(vs_path, fs_path);

	model = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// glEnable(GL_CULL_FACE);
}

void OpenGL::DumpInfo()
{
	int Attributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &Attributes);
	DEBUG("GL_MAX_VERTEX_ATTRIBS: %d", Attributes);
	DEBUG("GL_VERSION: %s", glGetString(GL_VERSION));
	DEBUG("GL_VENDOR: %s", glGetString(GL_VENDOR));
	DEBUG("GL_RENDERER: %s", glGetString(GL_RENDERER));
	DEBUG("GL_SHADING_LANGUAGE_VERSION: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

bool VectorOfStringGetter(void *data, int n, const char **out_text)
{
	const std::vector<std::string> *v = (std::vector<std::string> *)data;
	std::string str = (*v)[n];
	str = str.substr(str.find_last_of("/\\") + 1);
	*out_text = str.c_str();
	return true;
}

void OpenGL::RenderLoop()
{
	std::vector<std::string> paths = get_obj_paths(obj_path);

	int size = paths.size();
	int selectedMesh = 3;
	int selected = selectedMesh;
	MeshLoader mesh(paths[selectedMesh]);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// error checker
		glCheckError();

		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// polygon mode
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// shader program
		shader.use();

		// view matrix and projection matrix
		projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 1000.0f);
		view = camera.GetViewMatrix();

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		// light
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);

		glm::vec3 lightAmbient = lightColor * glm::vec3(0.2f);
		glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
		glm::vec3 lightSpecular = lightColor * glm::vec3(1.0f);

		shader.setVec3("light.direction", lightDirection);
		shader.setVec3("light.ambient", lightAmbient);
		shader.setVec3("light.diffuse", lightDiffuse);
		shader.setVec3("light.specular", lightSpecular);

		shader.setVec3("cameraPosition", camera.Position);

		// material
		glm::vec3 materialAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 materialDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		float materialShininess = 32.0f;

		shader.setVec3("material.ambient", materialAmbient);
		shader.setVec3("material.diffuse", materialDiffuse);
		shader.setVec3("material.specular", materialSpecular);
		shader.setFloat("material.shininess", materialShininess);

		// switch
		shader.setBool("grayScale", grayMode);
		shader.setBool("mosaic", mosaicMode);

		// Mesh painting
		if (selectedMesh != selected)
		{
			mesh.loadMesh(paths[selectedMesh]);
			selected = selectedMesh;
			camera.Reset();
			model = glm::mat4(1.0f);
		}

		mesh.paintMesh();

		// IMGUI
		ImGui::Begin("Settings");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(400, 300));
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Wireframe", &wireframe);
		ImGui::Checkbox("Zoom Mode", &zoomMode);
		ImGui::Checkbox("Gray Scale", &grayMode);
		ImGui::Checkbox("Mosaic", &mosaicMode);
		ImGui::ListBox("Mesh", &selectedMesh, VectorOfStringGetter, &paths, size);

		if (ImGui::Button("Add Texture"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", ".");
		}

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
				std::cout << "File Path: " << filePath << std::endl;
				std::cout << "File Path Name: " << filePathName << std::endl;
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// refresh
		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll IO events (keys pressed/released, mouse moved etc.)
	}

	// shut down IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	shader.del();
	glfwDestroyWindow(window);
}

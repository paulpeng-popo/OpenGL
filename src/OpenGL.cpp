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

std::string vs_path = "shaders/vertex.glsl";
std::string fs_path = "shaders/fragment.glsl";
std::string obj_path = "objects/3dmeshes/";
std::string texture_path = "textures/";

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

bool wireframe = false;
bool zoomMode = false;
bool grayMode = false;

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
	gui.init(window);
	return 0;
}

void OpenGL::InitDefault()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	shader = GLShader(vs_path, fs_path);

	model = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
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

void OpenGL::shaderPass()
{
	shader.use();

	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 1000.0f);
	view = camera.GetViewMatrix();

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);

	// light
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection1(camera.Front.x, camera.Front.y, camera.Front.z);
	glm::vec3 lightDirection2(camera.Right.x, camera.Right.y, camera.Right.z);
	glm::vec3 lightDirection3(camera.Up.x, camera.Up.y, camera.Up.z);

	glm::vec3 lightAmbient = lightColor * glm::vec3(0.2f);
	glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
	glm::vec3 lightSpecular = lightColor * glm::vec3(1.0f);

	shader.setVec3("light1.direction", lightDirection1);
	shader.setVec3("light2.direction", lightDirection2);
	shader.setVec3("light3.direction", lightDirection3);

	shader.setVec3("light1.ambient", lightAmbient);
	shader.setVec3("light1.diffuse", lightDiffuse);
	shader.setVec3("light1.specular", lightSpecular);

	shader.setVec3("light2.ambient", lightAmbient);
	shader.setVec3("light2.diffuse", lightDiffuse);
	shader.setVec3("light2.specular", lightSpecular);

	shader.setVec3("light3.ambient", lightAmbient);
	shader.setVec3("light3.diffuse", lightDiffuse);
	shader.setVec3("light3.specular", lightSpecular);

	// camera position
	shader.setVec3("cameraPosition", camera.Position);

	// material
	glm::vec3 materialAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 materialDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
	float materialShininess = 32.0f;

	shader.setVec3("material.ambient", materialAmbient);
	shader.setVec3("material.diffuse", materialDiffuse);
	shader.setVec3("material.specular", materialSpecular);
	shader.setFloat("material.shininess", materialShininess);

	// gray scale switch
	shader.setBool("grayScale", grayMode);
}

void OpenGL::RenderLoop()
{
	std::vector<std::string> paths = get_obj_paths(obj_path);
	std::string chosen_texture = "";

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
		gui.startFrame();
		gui.render();
		gui.createCheckbox("Wireframe", &wireframe);
		gui.createCheckbox("Gray Mode", &grayMode);
		gui.createCheckbox("Zoom Mode", &zoomMode);
		gui.createCombo("Mesh", paths, &selectedMesh);
		gui.createExplorer("Texture", texture_path, ".*", &chosen_texture);
		gui.endFrame();

		// refresh
		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll IO events (keys pressed/released, mouse moved etc.)
	}

	gui.cleanUp();
	shader.del();
	glfwDestroyWindow(window);
}

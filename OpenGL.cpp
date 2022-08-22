/*
	File: OpenGL.cpp
	Implementation of OpenGL.h
	Author: Paul peng
	Date: 2022.8.20
*/

#include "myheaders/OpenGL.h"
#include "myheaders/GLShader.h"
#include "myheaders/ModelLoader.h"
#include "myheaders/Actions.h"

std::string vs_path = ".\\src\\shaders\\vertex.glsl";
std::string fs_path = ".\\src\\shaders\\fragment.glsl";
std::string obj_path = ".\\src\\objects\\";

GLShader shader;
GLFWwindow *window;

// Camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
bool firstMouse = true;
float lastX;
float lastY;

vector<glm::vec3> translation(BODY_PARTS, glm::vec3(0.0f));
vector<glm::vec3> rotation(BODY_PARTS, glm::vec3(0.0f));
vector<glm::vec3> scalar(BODY_PARTS, glm::vec3(1.0f));

glm::mat4 projection;
glm::mat4 view;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool lightswitch = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightAmbient = lightColor * glm::vec3(0.2f);
glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
glm::vec3 lightSpecular = lightColor * glm::vec3(1.0f);

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
	{
		std::cout << "[ERROR] Cannot open directory: " << directory << std::endl;
	}
	return paths;
}

// Constructors
OpenGL::OpenGL() : width(800), height(600), window_name("test")
{
	fprintf(stdout, "[INFO] Using default settings ...\n");
	fprintf(stdout, "[INFO] Window size: width = 800, height = 600\n");
	fprintf(stdout, "[INFO] Window title: <test>\n");
}

OpenGL::OpenGL(int w, int h, std::string title) : width(w), height(h), window_name(title)
{
	fprintf(stdout, "[INFO] Window size: width = %d, height = %d\n", w, h);
	fprintf(stdout, "[INFO] Window title: <%s>\n", title.c_str());
}

// Destructor
OpenGL::~OpenGL()
{
	fprintf(stdout, "[INFO] End of using OpenGL tools.\n");
}

/*
	Function: Initialize GLFW, set window context and create window
	Params:		None
	Return:
		-1: 	Something failed
		0:  	Glfw library used successfully
*/
int OpenGL::UseGLFW()
{
	if (!glfwInit())
	{
		fprintf(stderr, "[ERROR] Failed to initialize GLFW.\n");
		getchar();
		return -1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, window_name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "[ERROR] Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE); // key will be held down in a range of time
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPos(window, width / 2, height / 2);
	lastX = (float)width / 2.0f;
	lastY = (float)height / 2.0f;

	return 0;
}

/*
	Function: Initialize GLAD
	Params:		None
	Return:
		-1:		Something failed
		0:		Glad functions settled successfully
*/
int OpenGL::UseGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "[ERROR] Failed to initialize GLAD.\n");
		return -1;
	}

	return 0;
}

/*
	Function: Set initial value for all stuff
	Params:		None
	Return:		None
*/
void OpenGL::InitDefault()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // gray-green
	shader = GLShader(vs_path, fs_path);

	std::cout << "[INFO] Shader program: " << shader.getProgram() << std::endl;

	initial(translation, rotation, scalar);
	stand(translation, rotation, scalar);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// glEnable(GL_CULL_FACE);
}

/*
	Function: Print out versions of used hardware and software
	Params:		None
	Return:		None
*/
void OpenGL::DumpInfo()
{
	int Attributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &Attributes);
	fprintf(stdout, "[INFO] OpenGL vender: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "[INFO] OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "[INFO] OpenGL version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "[INFO] GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(stdout, "[INFO] GLSL maximum vertex attributes supported: %d\n", Attributes);
}

/*
	Function: A while loop for intermediately updating window things
	Params:		None
	Return:		None
*/
void OpenGL::RenderLoop()
{
	std::vector<ModelLoader> objects;
	std::vector<std::string> paths = get_obj_paths(obj_path);

	for (int i = 0; i < paths.size(); i++)
	{
		objects.push_back(ModelLoader(paths[i]));
	}

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glCheckError();

		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// shader program
		shader.use();

		// view matrix and projection matrix
		projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		shader.setMat4("camera.projection", projection);
		shader.setMat4("camera.view", view);
		shader.setVec3("camera.position", camera.Position);

		// light
		shader.setBool("lightswitch", lightswitch);
		shader.setVec3("light.position", lightPos);
		shader.setVec3("light.ambient", lightAmbient);
		shader.setVec3("light.diffuse", lightDiffuse);
		shader.setVec3("light.specular", lightSpecular);

		// render with model matrix changing each frame
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i].Draw(shader);

			shader.setVec3("size", scalar[i]);
			shader.setVec3("rotation", rotation[i]);
			shader.setVec3("translation", translation[i]);
		}

		// refresh
		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll IO events (keys pressed/released, mouse moved etc.)
	}
	shader.deleteProgram();
	glfwDestroyWindow(window);
}

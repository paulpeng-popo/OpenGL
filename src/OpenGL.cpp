/*
	File: OpenGL.cpp
	Implementation of OpenGL.h
	Author: Paul peng
	Date: 2022.8.20
*/

#include "../myheaders/OpenGL.h"
#include "../myheaders/GLShader.h"
#include "../myheaders/ModelLoader.h"
#include "../myheaders/Actions.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/ImGuiFileDialog.h"

std::string vs_path = "shaders/vertex.glsl";
std::string fs_path = "shaders/fragment.glsl";
std::string obj_path = "objects/robot/";

GLShader shader;
GLFWwindow *window;
int screen_width;
int screen_height;

// Camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
bool firstMouse = true;
bool cursor_lock = true;
float lastX;
float lastY;

bool wireframe = false;
bool walking = false;
glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f); // gray-green

vector<glm::vec3> translation(BODY_PARTS, glm::vec3(0.0f));
vector<glm::vec3> rotation(BODY_PARTS, glm::vec3(0.0f));
vector<glm::vec3> scalar(BODY_PARTS, glm::vec3(1.0f));

glm::mat4 projection;
glm::mat4 view;
vector<glm::mat4> model(BODY_PARTS, glm::mat4(1.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool lightswitch = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

bool onlymaterial = false;
bool textureswitch = true;

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
	glfwSetWindowPos(window, 50, 50);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE); // key will be held down in a range of time
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPos(window, width / 2, height / 2);
	lastX = (float)width / 2.0f;
	lastY = (float)height / 2.0f;

	screen_width = width;
	screen_height = height;

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
	Function: Initialize IMGUI
	Params:		None
	Return:
		-1:		Something failed
		0:		IMGUI settings settled successfully
*/
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

/*
	Function: Set initial value for all stuff
	Params:		None
	Return:		None
*/
void OpenGL::InitDefault()
{
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
	shader = GLShader(vs_path, fs_path);

	std::cout << "[INFO] Shader program: " << shader.getProgram() << std::endl;

	model = initial(translation, rotation, scalar);
	model = stand(translation, rotation, scalar);

	startTime = 0.0f;
	actionFreq = 0.01f;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
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

	OpenMesh::TriMesh_ArrayKernelT<> mesh;
	OpenMesh::IO::read_mesh(mesh, paths[0]);

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
		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
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
		projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		shader.setMat4("camera.projection", projection);
		shader.setMat4("camera.view", view);
		shader.setVec3("camera.position", camera.Position);

		// light
		glm::vec3 lightAmbient = lightColor * glm::vec3(0.2f);
		glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
		glm::vec3 lightSpecular = lightColor * glm::vec3(1.0f);

		shader.setBool("lightswitch", lightswitch);
		shader.setVec3("light.position", lightPos);
		shader.setVec3("light.ambient", lightAmbient);
		shader.setVec3("light.diffuse", lightDiffuse);
		shader.setVec3("light.specular", lightSpecular);

		shader.setBool("textureswitch", textureswitch);
		shader.setBool("onlymaterial", onlymaterial);

		// action
		if (walking)
			model = walk(translation, rotation, scalar);
		else
			model = stand(translation, rotation, scalar);

		// render with model matrix changing each frame
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i].Draw(shader);

			// model matrix
			shader.setMat4("model", model[i]);
		}

		// IMGUI
		ImGui::Begin("Settings");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(400, 500));
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("IsWalk", &walking);
		ImGui::SliderFloat("Walk Speed", &actionFreq, 0.0f, 1.0f);
		ImGui::Checkbox("Wireframe", &wireframe);
		ImGui::Checkbox("Light Switch", &lightswitch);
		ImGui::ColorEdit3("Light Color", (float *)&lightColor);
		ImGui::InputFloat3("Light Position", (float *)&lightPos);
		ImGui::ColorEdit4("Background Color", (float *)&bgColor);
		ImGui::Checkbox("Texture Switch", &textureswitch);
		ImGui::Checkbox("Only Material", &onlymaterial);

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

	shader.deleteProgram();
	glfwDestroyWindow(window);
}

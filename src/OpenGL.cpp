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
std::string screen_vs_path = "shaders/screen_vertex.glsl";
std::string screen_fs_path = "shaders/screen_fragment.glsl";
std::string obj_path = "objects/robot/";

GLShader shader;
GLFWwindow *window;
int screen_width;
int screen_height;

GLShader screen_shader;
GLuint framebuffer;
GLuint textureColorbuffer;
GLuint renderbuffer;

GLubyte *colorbuffer;
int datasize = 0;
const int PBO_COUNT = 2;
GLuint pboIds[PBO_COUNT];

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
bool firstMouse = true;
bool cursor_lock = true;
float lastX;
float lastY;

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
bool mosaic = false;

GLuint quadVAO, quadVBO;

float quadVertices[] = {
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f};

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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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

	datasize = width * height * sizeof(GLubyte) * 4;

	colorbuffer = new GLubyte[datasize];
	memset(colorbuffer, 255, datasize);

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
	screen_shader = GLShader(screen_vs_path, screen_fs_path);

	std::cout << "[INFO] Shader program: " << shader.getProgram() << std::endl;

	model = initial(translation, rotation, scalar);
	model = stand(translation, rotation, scalar);

	startTime = 0.0f;
	actionFreq = 0.01f;

	glGenBuffers(PBO_COUNT, pboIds);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
	glBufferData(GL_PIXEL_PACK_BUFFER, datasize, 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
	glBufferData(GL_PIXEL_PACK_BUFFER, datasize, 0, GL_STREAM_READ);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &textureColorbuffer);
	glGenRenderbuffers(1, &renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[ERROR] Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	int blur_pixel = 0;
	int index = 0;
	int nextIndex = 0;

	glm::vec4 box = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

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

		// error checker
		// glCheckError();

		// input
		processInput(window);

		// render
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// shader program
		shader.use();

		// view matrix and projection matrix
		float ratio = (float)screen_width / (float)screen_height;
		projection = glm::perspective(glm::radians(camera.Zoom), ratio, 0.1f, 100.0f);
		// projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
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

		// mosaic effect
		if (mosaic)
		{
			index = (index + 1) % 2;
			nextIndex = (index + 1) % 2;
			blur_pixel = 0;

			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
			glReadPixels(0, 0, screen_width, screen_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			// initalize the bounding box
			box[0] = screen_width - 1;
			box[1] = screen_height - 1;
			box[2] = 0.0f;
			box[3] = 0.0f;

			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
			colorbuffer = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

			// find the bounding box of the object if its color is not equal to the background color
			for (int i = 0; i < screen_width; i++)
			{
				for (int j = 0; j < screen_height; j++)
				{
					if ((int)colorbuffer[(j * screen_width + i) * 4] != (int)(bgColor.x * 255) ||
						(int)colorbuffer[(j * screen_width + i) * 4 + 1] != (int)(bgColor.y * 255) ||
						(int)colorbuffer[(j * screen_width + i) * 4 + 2] != (int)(bgColor.z * 255) ||
						(int)colorbuffer[(j * screen_width + i) * 4 + 3] != (int)(bgColor.w * 255))
					{
						blur_pixel++;

						if (i < box[0])
							box[0] = i;
						if (i > box[2])
							box[2] = i;
						if (j < box[1])
							box[1] = j;
						if (j > box[3])
							box[3] = j;
					}
				}
			}

			glUnmapBuffer(GL_PIXEL_PACK_BUFFER); // release pointer to the mapped buffer
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

			box[0] = box[0] / screen_width;
			box[1] = box[1] / screen_height;
			box[2] = box[2] / screen_width;
			box[3] = box[3] / screen_height;

			// std::cout << box[0] << " " << box[1] << " " << box[2] << " " << box[3] << std::endl;
		}
		else
		{
			blur_pixel = 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screen_shader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		screen_shader.setBool("is_blur", mosaic);
		screen_shader.setVec4("box", box);

		// IMGUI
		ImGui::Begin("Settings");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Resolution: %d x %d", screen_width, screen_height);
		ImGui::Text("Total pixels: %d", screen_width * screen_height);
		ImGui::Checkbox("IsWalk", &walking);
		ImGui::SliderFloat("Walk Speed", &actionFreq, 0.0f, 1.0f);
		// ImGui::Checkbox("Light Switch", &lightswitch);
		// ImGui::ColorEdit3("Light Color", (float *)&lightColor);
		// ImGui::InputFloat3("Light Position", (float *)&lightPos);
		ImGui::ColorEdit4("Background Color", (float *)&bgColor);
		// ImGui::Checkbox("Texture Switch", &textureswitch);
		// ImGui::Checkbox("Only Material", &onlymaterial);
		ImGui::Checkbox("Mosiac", &mosaic);
		ImGui::Text("Mosaic Pixels: %d", blur_pixel);

		// if (ImGui::Button("Add Texture"))
		// {
		// 	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", ".");
		// }

		// // display
		// if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		// {
		// 	// action if OK
		// 	if (ImGuiFileDialog::Instance()->IsOk())
		// 	{
		// 		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
		// 		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
		// 		// action
		// 		std::cout << "File Path: " << filePath << std::endl;
		// 		std::cout << "File Path Name: " << filePathName << std::endl;
		// 	}

		// 	// close
		// 	ImGuiFileDialog::Instance()->Close();
		// }

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// refresh
		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll IO events (keys pressed/released, mouse moved etc.)
	}

	// shut down IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	shader.deleteProgram();
	screen_shader.deleteProgram();
	glfwDestroyWindow(window);
}

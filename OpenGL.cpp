/*
	File: OpenGL.cpp
	Implementation of OpenGL.h
	Author: Paul peng
	Date: 2022.8.14
*/

#include "myheaders/OpenGL.h"
#include "myheaders/GLShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "myheaders/STBImage.h"

std::string vs_path = ".\\src\\shaders\\vertex.glsl";
std::string fs_path = ".\\src\\shaders\\fragment.glsl";

std::string japan_texture_path = ".\\src\\textures\\japan.png";
std::string chess_texture_path = ".\\src\\textures\\chess.png";

GLuint program;
GLFWwindow *window;

GLfloat vertices[] = {
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

GLuint indices[] = { // start from 0
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
};

int width, height, nrChannels;
unsigned char *data;

GLuint VAO, VBO, EBO;

GLuint texture1;
GLuint texture1Loc;

GLuint texture2;
GLuint texture2Loc;

// Constructors
OpenGL::OpenGL() :
	width(800), height(600), window_name("test")
{
	fprintf(stdout, "[INFO] Using default settings ...\n");
	fprintf(stdout, "[INFO] Window size: width = 800, height = 600\n");
	fprintf(stdout, "[INFO] Window title: <test>\n");
}

OpenGL::OpenGL(int w, int h, std::string title) :
	width(w), height(h), window_name(title)
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
	*****Callback functions*****
	Function: Set viewport dynamically fit window size
	Params:
		window: GLFWwindow identifier
		width:  Width after resizing
		height: Height after resizing
	Return:		None
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (width == 0) width = 1;
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);
}

/*
	*****Callback functions*****
	Function: Manipulate keyboard input in glfw window
	Params:
		window: GLFWwindow identifier
	Return:		None
*/
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/*
	Function: Initialize GLFW, set window context and create window
	Params:		None
	Return:
		-1 -> something failed
		0 -> glfw library used successfully
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

	#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

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

	return 0;
}

/*
	Function: Initialize GLAD
	Params:		None
	Return:
		-1 -> something failed
		0 -> glad functions settled successfully
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
	program = LoadShader(vs_path.c_str(), fs_path.c_str());
	std::cout << "[INFO] Shader program: " << program << std::endl;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(6*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	std::cout << "[NOTE] Texture size should be power of 2" << std::endl;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(japan_texture_path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		fprintf(stderr, "[ERROR] Failed to load texture.\n");
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// stbi_set_flip_vertically_on_load(true);
	data = stbi_load(chess_texture_path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		fprintf(stderr, "[ERROR] Failed to load texture.\n");
	}
	stbi_image_free(data);

	// glCheckError();

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	GLfloat timeValue;

	while (!glfwWindowShouldClose(window))
   	{
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// get time value
		timeValue = glfwGetTime();
		timeValue = glm::clamp(cos(timeValue), 0.0f, 1.0f);

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(program, "tex1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(program, "tex2"), 1);

		glUniform1f(glGetUniformLocation(program, "alpha"), timeValue);

		// render
		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// refresh
        glfwSwapBuffers(window); // swap buffers
        glfwPollEvents(); // poll IO events (keys pressed/released, mouse moved etc.)
    }

	// release memory
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);

	glfwDestroyWindow(window);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "[Error] %s\n", description);
}

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
		std::cerr << "[ERROR] " << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

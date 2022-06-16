#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../include/Core/Debug/Assertion.h"
#include "../include/Application.h"
#include "../include/LowRenderer/Light.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;"
"layout(location = 1) in vec3 aColor;"
"layout(location = 2) in vec2 aTexCoord;"

"out vec3 ourColor;"
"out vec2 TexCoord;"

"void main()"
"{"
"	gl_Position = vec4(aPos, 1.0);"
"	ourColor = aColor;"
"	TexCoord = aTexCoord;"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"

"in vec3 ourColor;"
"in vec2 TexCoord;"
"uniform sampler2D texture1;"
"uniform sampler2D texture2;"
"void main()"
"{"
"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
"}\0";

int main()
{
	//ASSERT(false);
	Camera* cam = new Camera(90, ((float)4 / 3), 0.1f, 1000);
	Application app(cam);
	ResourceManager* resManager = app.GetResourceManager();

	//-----------------------Model-----------------------
	/*Model* model = resManager->Create<Model>("Assets/Meshes/happy-buddha.obj");
	Model* model2 = resManager->Create<Model>("Assets/Meshes/tank.obj");
	Model* model3 = resManager->Create<Model>("Assets/Meshes/bunny.obj");
	Model* model4 = resManager->Create<Model>("Assets/Meshes/diablo3_pose/diablo3_pose.obj");
	Model* model5 = resManager->Create<Model>("Assets/Meshes/objects/backpack/backpack.obj");
	Model* model6 = resManager->Create<Model>("Assets/Meshes/objects/cyborg/cyborg.obj");
	Model* model7 = resManager->Create<Model>("Assets/Meshes/objects/planet/planet.obj");
	Model* model8 = resManager->Create<Model>("Assets/Meshes/Handgun_obj.obj");
	Model* model9 = resManager->Create<Model>("Assets/Meshes/Bike.obj");

	//-----------------------Shader-----------------------
	Shader* shader = resManager->Create<Shader>("Shader1", "Source/shaders/LightVertexShader.glsl", "Source/shaders/LightFragShader.glsl");
	Shader* shader2 = resManager->Create<Shader>("Shader2", "Source/shaders/LightVertexShader.glsl", "Source/shaders/LightFragShader.glsl");
	Shader* shader3 = resManager->Create<Shader>("Shader3", "Source/shaders/LightVertexShader.glsl", "Source/shaders/LightFragShader.glsl");
	Shader* shader4 = resManager->Create<Shader>("Shader4", "Source/shaders/LightVertexShader.glsl", "Source/shaders/LightFragShader.glsl");
	Shader* shader5 = resManager->Create<Shader>("Shader5", "Source/shaders/LightVertexShader.glsl", "Source/shaders/LightFragShader.glsl");
	Shader* shader6 = resManager->Create<Shader>("Shader6", "Source/shaders/LightVertexShader.glsl", "Source/shaders/LightFragShader.glsl");

	//-----------------------Texture-----------------------
	Texture* tex1 = resManager->Create<Texture>("texture1", "Assets/Textures/sample.png", false);
	Texture* tex2 = resManager->Create<Texture>("texture2", "Assets/Textures/container.jpg", true);
	Texture* tex3 = resManager->Create<Texture>("texture3", "Assets/Meshes/diablo3_pose/diablo3_pose_diffuse.tga", true);
	Texture* tex4 = resManager->Create<Texture>("texture4", "Assets/Meshes/objects/backpack/diffuse.jpg", true);
	Texture* tex6 = resManager->Create<Texture>("texture6", "Assets/Meshes/objects/cyborg/cyborg_diffuse.png", true);
	Texture* tex5 = resManager->Create<Texture>("texture5", "Assets/Meshes/objects/planet/mars.png", true);

	//-----------------------Mesh-----------------------
	Mesh* mesh1 = resManager->Create<Mesh>("mesh1", model, shader, tex1, lm::mat4::createTransformMatrix({ 0, -135, 0 }, { 0, -5, -3 }, { 1, 1, 1 }));
	Mesh* mesh2 = resManager->Create<Mesh>("mesh3", model3, shader, tex1, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { -5, 0, -3 }, { 1, 1, 1 }));
	Mesh* mesh3 = resManager->Create<Mesh>("mesh2", model2, shader2, tex2, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { 5, 0, -3 }, { 1, 1, 1 }));
	resManager->Create<Mesh>("mesh4", model4, shader3, tex3, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { -10, 0, -3 }, { 1, 1, 1 }));
	resManager->Create<Mesh>("mesh5", model5, shader4, tex4, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { 10, 0, -3 }, { 1, 1, 1 }));
	resManager->Create<Mesh>("mesh6", model6, shader6, tex6, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { 15, 0, -3 }, { 1, 1, 1 }));
	resManager->Create<Mesh>("mesh7", model7, shader5, tex5, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { -15, 0, -3 }, { 1, 1, 1 }));
	resManager->Create<Mesh>("mesh8", model8, shader5, tex5, lm::mat4::createTransformMatrix({ 0, 0, 0 }, { 20, 0, -3 }, { 1, 1, 1 }));
	resManager->Create<Mesh>("mesh9", model9, shader5, tex5, lm::mat4::createTransformMatrix({ 0, 90, 0 }, { -20, 0, -3 }, { 0.01, 0.01, 0.01 }));

	//-----------------------Light-----------------------
	app.getDirLights().push_back(resManager->Create<DirectionalLight>("directionalLight1", lm::vec3(0.3f, 0.3f, 0.3f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.0f, 0.0f, -1.0f)));
	app.getPointLights().push_back(resManager->Create<PointLight>("PointLight1", lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.7f, 0.2f, 2.0f), 1.0f, 0.09f, 0.032f));
	app.getSpotLights().push_back(resManager->Create<SpotLight>("SpotLight1", lm::vec3(0.3f, 0.3f, 0.3f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), cam->GetPosition(), cam->GetFoward(), std::cos(lm::vec3::degreesToRadians(12.5f)), std::cos(lm::vec3::degreesToRadians(15.0f)), 1.0f, 0.09f, 0.032f));
	
	//-----------------------SceneGraph-----------------------
	Graph* graph1 = resManager->Create<Graph>("graph1", mesh1);
	app.getSceneGraph().push_back(graph1);
	Graph* graph2 = resManager->Create<Graph>("graph2", mesh2, graph1, app.GetWindow());
	Graph* graph2 = resManager->Create<Graph>("graph2", mesh3);
	graph2->AddChild(*graph1, app.GetWindow());*/

	/*
	// generate the texture data
	// ------------------------------------
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Assets/Textures/sample.png", &width, &height, &nrChannels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(data);

	data = stbi_load("Assets/Textures/sample2.png", &width, &height, &nrChannels, 0);

	GLuint texture2;

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(data);


	// create a sampler and parameterize it
	// ------------------------------------
	GLuint sampler = 0;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);


	glBindTextureUnit(0, texture);
	glBindSampler(0, sampler);

	glBindTextureUnit(1, texture2);
	glBindSampler(1, sampler);*/


	// render loop
	// -----------

	app.Gameloop();


	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	/*glDeleteTextures(1, &texture);

	glDeleteSamplers(1, &sampler);*/

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	app.TerminateImGui();
	glfwDestroyWindow(app.GetWindow());
	glfwTerminate();
	
	return 0;
}/*

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "../include/Core/Debug/Assertion.h"
#include "../include/Ressource/Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}





// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;"
"layout(location = 1) in vec3 aColor;"
"layout(location = 2) in vec2 aTexCoord;"

"out vec3 ourColor;"
"out vec2 TexCoord;"

"void main()"
"{"
"	gl_Position = vec4(aPos, 1.0);"
"	ourColor = aColor;"
"	TexCoord = aTexCoord;"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"

"in vec3 ourColor;"
"in vec2 TexCoord;"
"uniform sampler2D texture1;"
"uniform sampler2D texture2;"
"void main()"
"{"
"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
"}\0";

int main()
{
	//ASSERT(false);
	ResourceManager resManager;
	resManager.Create<Model>("Assets/Meshes/f.obj");

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;

	// glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// generate the texture data
	// ------------------------------------
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("Assets/Textures/sample.png", &width, &height, &nrChannels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(data);

	data = stbi_load("Assets/Textures/sample2.png", &width, &height, &nrChannels, 0);

	GLuint texture2;

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(data);


	// create a sampler and parameterize it
	// ------------------------------------
	GLuint sampler = 0;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);


	glBindTextureUnit(0, texture);
	glBindSampler(0, sampler);

	glBindTextureUnit(1, texture2);
	glBindSampler(1, sampler);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		glfwPollEvents();
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);



		// draw our first triangle
		glUseProgram(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // set it manually
		glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1); // set it manually

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);

	}



	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &texture);

	glDeleteSamplers(1, &sampler);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

*/


#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include<string>

#include"SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

GLFWwindow* window;
const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;



GLfloat lastXPos = WIDTH / 2.0;
GLfloat lastYPos = HEIGHT / 2.0;
GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


bool keys[1024];
bool mouse = true;
void KeyInput(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseInput(GLFWwindow *window, double xPos, double yPos);
void ScrollInput(GLFWwindow *window, double xOffset, double yOffset);

double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;

void MoveCamera();

int InitOpenGL();
//void InitBuffers(std::vector<GLfloat> v, GLuint vertexArrayObj);
//void SetupAttributes();

GLuint program;
Shader shader;


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	InitOpenGL();

	Shader shader(program,"./res/shaders/modelLoading.vert", "./res/shaders/modelLoading.frag");
	Model hugh("./res/models/Hugh Neutron/hugh.obj");
	Model carl("./res/models/Carl Wheezer/carl.obj");
	Model sheen("./res/models/Sheen/sheen.obj");




	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		
		// Set frame time
		
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		MoveCamera();

		// Clear the colorbuffer
		glClearColor(1.0f,1.0f,1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 0.2f, 0.2f));	
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		carl.Draw(shader);

		
		model = glm::translate(model, glm::vec3(4.0f, -2.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		sheen.Draw(shader);

		model = glm::translate(model, glm::vec3(-5.0f, -2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		hugh.Draw(shader);


		// Swap the buffers
		glfwSwapBuffers(window);
	
	}

	
	//End the program
	glfwTerminate();
	return EXIT_SUCCESS;

}

int InitOpenGL()
{
	//Init GFLW
	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}
	//GFLW options and settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create a window
	window = glfwCreateWindow(WIDTH, HEIGHT, "BensCoolGame", nullptr, nullptr);

	if (nullptr == window) //Checking if a window was created
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);


	//Set callbacks for peripharals inputs
	glfwSetKeyCallback(window, KeyInput);
	glfwSetCursorPosCallback(window, MouseInput);

	//Setting the cursor to be hidden and locked
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Init GLEW and checking for init success
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	//Setting the dimentions of the viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Opengl setting(s)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
			//glDepthFunc(GL_LEQUAL);
	return 1;
}



void MoveCamera()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProccessKeyboardInput(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProccessKeyboardInput(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProccessKeyboardInput(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProccessKeyboardInput(RIGHT, deltaTime);
	}

	
}

void KeyInput(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
			cout << key;
			
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	
}


void MouseInput(GLFWwindow *window, double xPos, double yPos)
{
	if (mouse)
	{
		lastX = xPos;
		lastY = yPos;
		mouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProccessMouseInput(xOffset, yOffset);
}







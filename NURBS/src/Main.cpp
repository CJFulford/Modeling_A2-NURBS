#include "Header.h"
#include "ShaderBuilder.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

double  mouse_old_x, 
        mouse_old_y;

float   aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        uInc = 0.01f;
int order = 2, 
    splineSize = 0, 
    pointToMove = -1, 
    window_width = WINDOW_WIDTH,
    window_height = WINDOW_HEIGHT;
bool movePoint = false;

float   rotate_x = 0.0,
        rotate_y = 0.0,
        zoom = defaultZoom;

std::vector<glm::vec2> controls;

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };

glm::vec3   up = defaultUp,
            cam = defaultCam,
            center = defaultCenter;

GLuint  splineVertexArray = -1,
        controlsVertexArray = -1,
        splineProgram = -1, 
        controlsProgram = -1;

void errorCallback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_size_callback(GLFWwindow* window, int width, int height);
void printOpenGLVersion();

// takes in the order of the spline and returns the number of points in the resulting curve
int generateSplineBuffers(int order)
{
    GLuint vertexBuffer = 0;

    glGenVertexArrays(1, &splineVertexArray);
    glBindVertexArray(splineVertexArray);

    std::vector<glm::vec2> spline;

    bSpline(controls, spline, order, uInc);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spline[0]) * spline.size(), &spline[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return spline.size();
}

void generateControlsBuffer()
{
    GLuint vertexBuffer = 0;

    glGenVertexArrays(1, &controlsVertexArray);
    glBindVertexArray(controlsVertexArray);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(controls[0]) * controls.size(), &controls[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void generateShaders()
{
    splineProgram = generateProgram("shaders/general.vert", "shaders/general.frag");
    controlsProgram = generateProgram("shaders/general.vert", "shaders/general.frag");
}

void renderSpline(int numOfVertices)
{
    glBindVertexArray(splineVertexArray);
    glUseProgram(splineProgram);

    glDrawArrays(GL_LINE_STRIP, 0, numOfVertices);

    glBindVertexArray(0);
}

void renderControls()
{
    glBindVertexArray(controlsVertexArray);
    glUseProgram(controlsProgram);

    glPointSize(15);
    glDrawArrays(GL_POINTS, 0, controls.size());

    glBindVertexArray(0);
}

void pointMove(GLFWwindow *window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float x = xpos * 2.f / window_width - 1;
    float y = -(ypos * 2.f / window_height - 1);

    controls[pointToMove] = glm::vec2(x, y);

    if (controls.size() >= 1)
        generateControlsBuffer();

    if (controls.size() >= 2)
        splineSize = generateSplineBuffers(order);
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_SAMPLES, 32);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NURBS Curve", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback          (window, key_callback); 
    glfwSetMouseButtonCallback  (window, mouse_button_callback);
    glfwSetScrollCallback       (window, scroll_callback);
    glfwSetWindowSizeCallback   (window, window_size_callback);
	glfwMakeContextCurrent      (window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	printOpenGLVersion();

    generateShaders();

    glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

        if (controls.size() > 1)
            renderSpline(splineSize);
        if (controls.size() > 0)
            renderControls();

        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();

        if (movePoint) pointMove(window);
	}

	// Shutdow the program
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case(GLFW_KEY_N):
			std::cout << "Recompiling Shaders... ";
            generateShaders();
            std::cout << "Done" << std::endl;
			break;
        case(GLFW_KEY_W):
            if (order + 1 <= (int)controls.size())
            {
                order++;
                splineSize = generateSplineBuffers(order);
            }
            std::cout << "order = " << order << std::endl;
            break;
        case(GLFW_KEY_S):
            if (order - 1 > 1)  // dont allow an order of 1, rather a degree 0 curve
            {
                order--;
                splineSize = generateSplineBuffers(order);
            }
            std::cout << "order = " << order << std::endl;
            break;
		default:
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float x = xpos * 2.f / window_width - 1;
    float y = -(ypos * 2.f / window_height - 1);

    float pointSize = 0.04f;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        int i = 0;
        for (i; i < controls.size(); i++)
        {
            if (glm::distance(glm::vec2(x, y), controls[i]) <= pointSize)
            {
                movePoint = true;
                pointToMove = i;
                break;
            }
        }
        if (i == controls.size())
            controls.push_back(glm::vec2(x, y));
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        for (int i = 0; i < controls.size(); i++)
        {
            if (glm::distance(glm::vec2(x, y), controls[i]) <= pointSize)
            {
                controls.erase(controls.begin() + i);
                break;
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        movePoint = false;

    if (controls.size() >= 1)
        generateControlsBuffer();

    if(controls.size() >= 2)
        splineSize = generateSplineBuffers(order);
}

void incU(bool inc)
{
    float   uIncStep    = 0.005f, 
            uIncLimit   = 0.0001f, 
            uInc2       = 0.f;
    if (!inc)
        uInc2 = min(1.f - uIncLimit, uInc + uIncStep);
    else
        uInc2 = max(uIncLimit, uInc - uIncStep);
    std::cout << "u Increment = " << uInc << std::endl;
    if (uInc2 != uInc)
    {
        uInc = uInc2;
        splineSize = generateSplineBuffers(order);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Scroll Down
    if (yoffset < 0)
        if (glfwGetKey(window, GLFW_KEY_I))
            incU(true);
        else
            zoom += 0.1f;


    // scroll Up
    else if (yoffset > 0)
        if (glfwGetKey(window, GLFW_KEY_I))
            incU(false);
        else
            zoom -= 0.1f;
}

void printOpenGLVersion()
{
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("OpenGL on %s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
	printf("GLSL version supported %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("GL version major, minor: %i.%i\n", major, minor);
}

void errorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR " << error << ": " << description << std::endl;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    aspectRatio = (float)width / (float)height;
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}
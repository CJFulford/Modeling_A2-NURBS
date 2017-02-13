#include "Header.h"
#include "ShaderBuilder.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

double  mouse_old_x, 
        mouse_old_y;

float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
#ifdef ThreeD
    float   rotate_x = 0.0,
            rotate_y = 0.0,
            zoom = defaultZoom;
#endif // ThreeD



const GLfloat clearColor[] = { 0.f, 0.f, 0.f };

glm::vec3   up = defaultUp,
            cam = defaultCam,
            center = defaultCenter;




GLuint vertexArray = -1, program = -1;
std::vector<glm::vec3> vertices;


void errorCallback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_size_callback(GLFWwindow* window, int width, int height);
void motion(GLFWwindow* window, double x, double y);
void printOpenGLVersion();

void generateBuffer()
{
    GLuint vertexBuffer = 0;

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

#ifdef ThreeD
    std::vector<glm::vec3> verts;
    verts.push_back(glm::vec3(-0.5f, -0.1f, -0.5f));
    verts.push_back(glm::vec3(-0.5f, -0.1f, 0.5f));
    verts.push_back(glm::vec3(0.5f, -0.1f, -0.5f));
    verts.push_back(glm::vec3(0.5f, -0.1f, 0.5f));
#else
    std::vector<glm::vec2> verts;
    verts.push_back(glm::vec2(-0.5f, -0.5f));
    verts.push_back(glm::vec2(-0.5f, 0.5f));
    verts.push_back(glm::vec2(0.5f, -0.5f));
    verts.push_back(glm::vec2(0.5f, 0.5f));
#endif

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
#ifdef ThreeD
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
#else
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
#endif
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void generateShaders()
{
#ifdef ThreeD
    program = generateProgram("shaders/general3D.vert", "shaders/general3D.frag");
#else
    program = generateProgram("shaders/general2D.vert", "shaders/general2D.frag");
#endif
}

void passBasicUniforms(GLuint program)
{
#ifdef ThreeD
    glm::mat4   modelview = glm::lookAt(cam * zoom, center, up),
        projection = glm::perspective(45.0f, aspectRatio, 0.01f, 100.0f);

    glm::mat4   rotationX = rotate(identity, rotate_x  * PI / 180.0f, glm::vec3(1.f, 0.f, 0.f)),
        rotationY = rotate(rotationX, rotate_y  * PI / 180.0f, glm::vec3(0.f, 1.f, 0.f));

    modelview *= rotationY;

    glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, value_ptr(modelview));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, value_ptr(projection));
#endif // ThreeD
}

void render(GLuint program)
{
    glBindVertexArray(vertexArray);
    glUseProgram(program);

    passBasicUniforms(program);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	glfwSetErrorCallback(errorCallback);

	//glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_SAMPLES, 32);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rollercoaster", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, motion);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	printOpenGLVersion();

    generateShaders();

    generateBuffer();

    glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

        render(program);

        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
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
        case(GLFW_KEY_M):
            std::cout << "Recalculating Curve... ";
            generateBuffer();
            std::cout << "Done" << std::endl;
            break;
		default:
			break;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
#ifdef ThreeD
    if (yoffset < 0)
        zoom += 0.1f;
    else if (yoffset > 0)
        zoom -= 0.1f;
#endif // ThreeD
}

void motion(GLFWwindow* window, double x, double y)
{
#ifdef ThreeD
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
    {
        rotate_x += (float)((y - mouse_old_y) * 0.5f);
        rotate_y += (float)((x - mouse_old_x) * 0.5f);
    }
    mouse_old_x = x;
    mouse_old_y = y;
#endif // ThreeD
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
    glViewport(0, 0, width, height);
}
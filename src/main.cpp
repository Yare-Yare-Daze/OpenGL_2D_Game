#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLfloat vertexes[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

const char* vertexShaderSource =
	"#version 460\n"
	"layout(location = 0) in vec3 vertex_position;"
	"layout(location = 1) in vec3 vertex_color;"
	"out vec3 color;"
	"void main() {"
	"   color = vertex_color;"
    "   gl_Position = vec4(vertex_position, 1.0);"
	"}";

const char* fragmentShaderSource =
	"#version 460\n"
	"in vec3 color;"
	"out vec4 frag_color;"
	"void main() {"
	"   frag_color = vec4(color, 1.0);"
	"}";

int g_windowsSizeX = 640;
int g_windowsSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	g_windowsSizeX = width;
	g_windowsSizeY = height;
	glViewport(0, 0, g_windowsSizeX, g_windowsSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
    	std::cout << "glfwInit failed" << std::endl;
    	return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowsSizeX, g_windowsSizeY, "OpenGL_2D_Game", nullptr, nullptr);
    if (!pWindow)
    {
    	std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);
	
	if(!gladLoadGL()){
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(1, 1, 0, 1);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint points_vertex_buffer_object= 0;
	glGenBuffers(1, &points_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, points_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

	GLuint colors_vertex_buffer_object= 0;
	glGenBuffers(1, &colors_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint vertex_array_object= 0;
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	
	glEnableVertexAttribArray(0); // enable layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, points_vertex_buffer_object);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1); // enable layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, colors_vertex_buffer_object);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

    	glUseProgram(shaderProgram);
    	glBindVertexArray(vertex_array_object);
    	glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
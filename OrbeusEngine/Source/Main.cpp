#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


std::string readFile(const std::string& filePath)
{
	std::string result;

	std::ifstream file(filePath.c_str());
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			result += line + '\n';
		}

		file.close();
	}

	return result;
}

unsigned int createShader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	std::string vertexSource = readFile(vertexFilePath);
	std::string fragmentSource = readFile(fragmentFilePath);

	std::cout << "Vertex Source" << std::endl;
	std::cout << vertexSource << std::endl;
	std::cout << "Fragment Source" << std::endl;
	std::cout << fragmentSource << std::endl;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsSrc = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vsSrc, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: VERTEX COMPILATION FAILED" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsSrc = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fsSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: FRAGMENT COMPILATION FAILED" << infoLog << std::endl;
	}

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER LINKING FAILED" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 600, "Orbeus Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f, // left  
		 0.5f, -0.5f, // right 
		 0.0f,  0.5f, // top   
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int shaderProgram = createShader("Resources/Basic-vs.shader", "Resources/Basic-fs.shader");

	glUseProgram(shaderProgram);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"

#include "Transform.h"

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

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertices[] = {
		-0.5f, -0.5f, // left  
		 0.5f, -0.5f, // right 
		 0.0f,  0.5f, // top   
	};

	Shader shader("Resources/Basic-vs.shader", "Resources/Basic-fs.shader");

	VertexBuffer* vertexBuffer = new VertexBuffer(vertices, sizeof(vertices), 3);
	VertexBufferLayout layout;
	layout.Push<float>(2);

	VertexArray vertexArray;
	vertexArray.AddBuffer(vertexBuffer, layout);

	vertexArray.bind();

	Vector3 color(0.0f, 1.0f, 0.0f);

	Transform transform;
	transform.position += Vector3(0.5f, 0.0f, 0.0f);
	transform.rotation = Vector3(0.0f, 0.0f, 180.0f);
	transform.scale = Vector3(0.5f, 0.5f, 1.0f);

	Matrix4 view;
	view.translate(Vector3(0.0f, 0.0f, -3.0f));

	shader.bind();

	shader.SetMatrix("view", view);
	shader.SetMatrix("model", transform.getMatrix());

	shader.SetFloat3("color", color);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		vertexArray.draw(shader);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	delete vertexBuffer;

	glfwTerminate();
	return 0;
}

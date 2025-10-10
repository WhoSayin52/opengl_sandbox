#include "input.hpp"
#include "render.hpp"
#include "shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

GLFWwindow* init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {

	GLFWwindow* window{ init() };

	Shader shader{ "../shaders/shader.vs", "../shaders/shader.fs" };

	init_renderer(&shader);

	while (glfwWindowShouldClose(window) == 0) {
		// input handler
		input(window);

		// rendering commands
		render(window, &shader);
	}

	glfwTerminate();

	return 0;
}

GLFWwindow* init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		exit(-1);
	}

	glViewport(0, 0, 800, 600);

	return window;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}



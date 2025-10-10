#include "input.hpp"

#include <GLFW/glfw3.h>

void input(GLFWwindow* window) {
	// check and call events
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

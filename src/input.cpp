#include "input.hpp"

#include "camera.hpp"

#include <GLFW/glfw3.h>

void input(GLFWwindow* window) {
	// check and call events
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.pos -= cam.speed * cam.z_axis;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.pos += cam.speed * cam.z_axis;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.pos -= cam.speed * cam.x_axis;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.pos += cam.speed * cam.x_axis;
	}
}

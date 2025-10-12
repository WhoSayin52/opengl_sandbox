#include "input.hpp"

#include "camera.hpp"
#include "time.hpp"

#include <GLFW/glfw3.h>

static glm::vec2 mouse_center_pos{ 400.0f, 300.0f };
static float pitch{};
static float yaw{ 90.0f };

void mouse_callback(GLFWwindow* window, double x, double y);
void scroll_callback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double x_offset, double y_offset);

void init_input(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPos(window, mouse_center_pos.x, mouse_center_pos.y);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void input(GLFWwindow* window) {
	// check and call events
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.pos -= cam.direction * cam.speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.pos += cam.direction * cam.speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.pos += glm::normalize(glm::cross(cam.direction, glm::vec3{ 0.0f, 1.0f, 0.0f })) * cam.speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.pos -= glm::normalize(glm::cross(cam.direction, glm::vec3{ 0.0f, 1.0f, 0.0f })) * cam.speed * delta;
	}
}

void mouse_callback(GLFWwindow* window, double x, double y) {
	glm::vec2 mouse_pos_offset{ (float)x - mouse_center_pos.x, mouse_center_pos.y - (float)y };
	const float sensitivity{ 5.0f };
	mouse_pos_offset *= sensitivity * delta;
	yaw += mouse_pos_offset.x;
	pitch -= mouse_pos_offset.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cam.direction.x = (float)cos(glm::radians(yaw)) * (float)cos(glm::radians(pitch));
	cam.direction.y = (float)sin(glm::radians(pitch));
	cam.direction.z = (float)sin(glm::radians(yaw)) * (float)cos(glm::radians(pitch));
	cam.direction = glm::normalize(cam.direction);

	glfwSetCursorPos(window, mouse_center_pos.x, mouse_center_pos.y);
}

void scroll_callback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double x_offset, double y_offset) {
	cam.fov -= (float)y_offset;
	if (cam.fov < 1.0f)
		cam.fov = 1.0f;
	if (cam.fov > 45.0f)
		cam.fov = 45.0f;
}

#include "input.hpp"

#include "camera.hpp"
#include "core.hpp"

#include <GLFW/glfw3.h>

void init_input() {

	int mouse_x{ game_state.window_width / 2 };
	int mouse_y{ game_state.window_height / 2 };

	input_state = InputState{
		.mouse_position = Vector2{mouse_x, mouse_y}
	};
}

void input() {
	// checks and calls events
	glfwPollEvents();

	GLFWwindow* window = game_state.window;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	camera_process_movement(window);
}

/*
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
*/

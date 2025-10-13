#include "camera.hpp"

#include <GLFW/glfw3.h>

constexpr f64 CAMERA_SPEED{ 2.5 };

void init_camera() {

	GLFWwindow* window{ game_state.window };
	Vector2 mouse_position{ input_state.mouse_position };

	camera = Camera{
	.position = Vector3{0.0, 0.0, 3.0},
	.direction = Vector3{0.0, 0.0, 1.0},
	.zoom = 45.0,
	};

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, mouse_position.x, mouse_position.y);
}

void camera_process_movement(GLFWwindow* window) {

	f64 delta_time{ game_state.delta_time };
	Vector3 direction{ 0.0 };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		direction.z -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		direction.z += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		direction.x -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		direction.x += 1;
	}

	if (glm::length(direction) != 0) {
		direction = glm::normalize(direction);
	}

	camera.position += direction * CAMERA_SPEED * delta_time;

	
}

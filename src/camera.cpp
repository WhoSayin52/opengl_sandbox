#include "camera.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr f64 CAMERA_SPEED{ 2.5 };
constexpr f64 ROTATION_SPEED{ 10.0 };
constexpr f64 ZOOM_SPEED{ 500.0 };

void init_camera() {

	GLFWwindow* window{ game_state.window };
	Vector2 window_center{ input_state.mouse.lock_position };

	camera = Camera{
	.position = Vector3{0.0, 0.0, 3.0},
	.right = Vector3{ 1.0,  0.0,  0.0 },
	.up = Vector3{ 0.0,  1.0,  0.0 },
	.front = Vector3{ 0.0,  0.0, -1.0 },
	.pitch = 0.0,
	.yaw = -90.0,
	.zoom = 45.0,
	};

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, window_center.x, window_center.y);
}

void camera_process_movement(GLFWwindow* window) {

	f64 delta_time{ game_state.delta_time };

	// keyboard movement
	Vector3 direction{ 0.0 };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		direction += camera.front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		direction -= camera.front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		direction -= camera.right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		direction += camera.right;
	}

	if (glm::length(direction) != 0) {
		direction = glm::normalize(direction);
	}

	camera.position += direction * (CAMERA_SPEED * delta_time);

	// mouse rotation
	Vector2 mouse_offset{ input_state.mouse.current_position - input_state.mouse.lock_position };

	camera.pitch -= mouse_offset.y * ROTATION_SPEED * delta_time;
	camera.yaw += mouse_offset.x * ROTATION_SPEED * delta_time;

	if (camera.pitch > 89.0) {
		camera.pitch = 89.0;
	}
	else if (camera.pitch < -89.0) {
		camera.pitch = -89.0;
	}

	camera.front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.front.y = sin(glm::radians(camera.pitch));
	camera.front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

	camera.front = glm::normalize(camera.front);

	camera.right = glm::cross(camera.front, Direction3::UP);
	camera.up = glm::cross(camera.right, camera.front);

	// zoom
	camera.zoom -= input_state.mouse.scroll_offset.y * (ZOOM_SPEED * delta_time);

	if (camera.zoom < 1.0) {
		camera.zoom = 1.0;
	}
	else if (camera.zoom > 45.0) {
		camera.zoom = 45.0;
	}
}

Matrix4f camera_get_view_matrix() {
	return glm::lookAt(
		camera.position,
		camera.position + camera.front,
		Direction3::UP
	);
}

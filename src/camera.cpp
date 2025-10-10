#include "camera.hpp"

Camera camera{ init_camera() };

Camera init_camera() {
	Camera camera;

	camera.pos = glm::vec3{ 0.0f, 0.0f, 3.0f };

	glm::vec3 camera_target{ 0.0f, 0.0f, 0.0f };

	camera.back = glm::normalize(camera.pos - camera_target);

	glm::vec3 world_up_vector{ 0.0f, 1.0f, 0.0f };

	camera.right = glm::normalize(glm::cross(world_up_vector, camera.back));

	camera.up = glm::normalize(glm::cross(camera.back, camera.right));

	return camera;
}

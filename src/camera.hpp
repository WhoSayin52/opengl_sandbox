#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

struct Camera {
	glm::vec3 pos;
	glm::vec3 x_axis;
	glm::vec3 y_axis;
	glm::vec3 z_axis;
	const float speed;
};

inline Camera cam{
	glm::vec3{0.0f, 0.0f, 3.0f},
	glm::vec3{1.0f, 0.0f, 0.0f},
	glm::vec3{0.0f, 1.0f, 0.0f},
	glm::vec3{0.0f, 0.0f, 1.0f},
	0.05f
};

#endif // CAMERA_HPP

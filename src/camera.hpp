#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

struct Camera {
	glm::vec3 pos;
	glm::vec3 direction;
	float fov;
	const float speed;
};

inline Camera cam{
	glm::vec3{0.0f, 0.0f, 3.0f},
	glm::vec3{0.0f, 0.0f, 1.0f},
	45.0f,
	2.5f
};

#endif // CAMERA_HPP

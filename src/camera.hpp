#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

struct Camera {
	glm::vec3 pos;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 back;
};

extern Camera camera;

#endif // CAMERA_HPP

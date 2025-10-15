#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "core.hpp"

void init_camera();

void camera_process_movement(GLFWwindow* window);

Matrix4f camera_get_view_matrix();

#endif // CAMERA_HPP

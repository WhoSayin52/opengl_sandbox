#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "core.hpp"

struct LightComponent {
	Vector3f ambient;
	Vector3f diffuse;
	Vector3f specular;
};

struct DirectionalLight {
	Vector3f direction;

	LightComponent component;
};

struct PointLight {
	Vector3f position;

	LightComponent component;
};

#endif // LIGHT_HPP

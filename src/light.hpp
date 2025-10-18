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
	f32 constant;
	f32 linear;
	f32 quadratic;
};

struct Spotlight {
	Vector3f position;
	Vector3f direction;

	LightComponent component;

	f32 half_angle;
};

#endif // LIGHT_HPP

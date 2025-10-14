#ifndef CORE_HPP
#define CORE_HPP

#include <glm/glm.hpp>

#include <cstdint>

using int32 = int32_t;
using s32 = int32_t;
using s64 = int64_t;

using uint = unsigned int;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using ubyte = unsigned char;
using byte = char;

using uchar = unsigned char;

using Vector4 = glm::dvec4;
using Vector3 = glm::dvec3;
using Vector2 = glm::dvec2;

namespace Direction3 {
	constexpr Vector3 UP{ 0.0, 1.0, 0.0 };
	constexpr Vector3 DOWN{ 0.0, -1.0, 0.0 };
	constexpr Vector3 FRONT{ 0.0, 0.0, -1.0 };
	constexpr Vector3 BACK{ 0.0, 0.0, 1.0 };
	constexpr Vector3 RIGHT{ 1.0, 0.0, 0.0 };
	constexpr Vector3 LEFT{ -1.0, 0.0, 0.0 };
}

using Matrix4 = glm::dmat4;
using Matrix3 = glm::dmat3;
using Matrix2 = glm::dmat2;

struct GLFWwindow;
struct GameState {
	GLFWwindow* window;
	s32 window_width;
	s32 window_height;
	f64 delta_time;
};

inline GameState game_state{};

struct Camera {
	Vector3 position;
	Vector3 right;
	Vector3 up;
	Vector3 front;
	f64 pitch;
	f64 yaw;
	f64 zoom;
};

inline Camera camera{};

struct MouseData {
	Vector2 lock_position;
	Vector2 current_position;
	Vector2 scroll_offset;
};

struct InputState {
	MouseData mouse;
};

inline InputState input_state{};

#endif // CORE_HPP

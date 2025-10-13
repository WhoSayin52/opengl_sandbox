#include "game.hpp"

#include "camera.hpp"
#include "core.hpp"
#include "input.hpp"
#include "render.hpp"
#include "shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

static void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static Shader* shader = nullptr;

void init_game(s32 window_width, s32 window_height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGl Sandbox", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "FAILURE: unable to create GLFW window\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	int result{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
	if (result == 0) {
		std::cerr << "FAILURE: unable to initialize glad\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	game_state = GameState{
	.window = window,
	.window_width = window_width,
	.window_height = window_height,
	.delta_time = 0.0
	};

	init_input();
	shader = new Shader{ "../shaders/shader.vs", "../shaders/shader.fs" };
	init_renderer(shader);
	init_camera();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void game_loop() {

	f64 last_frame_duration{};

	while (glfwWindowShouldClose(game_state.window) == 0) {

		f64 curren_frame_duration{ glfwGetTime() };
		game_state.delta_time = curren_frame_duration - last_frame_duration;
		last_frame_duration = curren_frame_duration;

		input();
		render(game_state.window, shader);
	}
}

void free_game() {
	glfwDestroyWindow(game_state.window);
	game_state.window = nullptr;

	glfwTerminate();
}


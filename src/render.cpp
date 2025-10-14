#include "render.hpp"

#include "camera.hpp"
#include "core.hpp"
#include "shapes.hpp"
#include "shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

static u32 light_vao;
static u32 cube_vao;

static u32 shader_program{};

void init_renderer() {

	glEnable(GL_DEPTH_TEST);

	shader_program = shader_create_program("../shaders/shader.vs", "../shaders/shader.fs");

	glUseProgram(shader_program);

	glUniform3f(glGetUniformLocation(shader_program, "light_color"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader_program, "object_color"), 1.0f, 0.5f, 0.31f);

	init_cube(&light_vao);
	init_cube(&cube_vao);
}

void render() {
	GLFWwindow* window = game_state.window;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the preferred VAO for object we want to draw
	glBindVertexArray(cube_vao);

	// model
	glm::mat4 model{ 1.0f };

	int model_location{ glGetUniformLocation(shader_program, "model") };
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	// view
	glm::mat4 view;
	view = glm::lookAt(
		camera.position,
		camera.position + camera.front,
		Direction3::UP
	);

	int view_location{ glGetUniformLocation(shader_program, "view") };
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	// projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.zoom), 800.0 / 600.0, 0.1, 100.0);

	int projection_location{ glGetUniformLocation(shader_program, "projection") };
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	// Set polygon rasterization mode of how OpenGL should draw its primatives (default: GL_FRONT_AND_BACK, GL_FILL)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw function 
	// glDrawArrays function that draws primitives using the currently active shader, 
	// the previously defined vertex attribute configuration and with the VBO's vertex data (indirectly bound via the VAO). 
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// swap the buffers
	glfwSwapBuffers(window);
}

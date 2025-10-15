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

static u32 light_shaders;
static u32 object_shaders;

Vector3f light_position(1.2f, 1.0f, 2.0f);

void init_renderer() {

	glEnable(GL_DEPTH_TEST);

	// light shaders
	light_shaders = shader_create_program("../shaders/object.vert", "../shaders/light.frag");

	glUseProgram(light_shaders);

	// object shaders
	object_shaders = shader_create_program("../shaders/object.vert", "../shaders/object.frag");

	glUseProgram(object_shaders);

	glUniform3f(glGetUniformLocation(object_shaders, "light_color"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(object_shaders, "object_color"), 1.0f, 0.5f, 0.31f);

	init_cube(&light_vao);
	init_cube(&cube_vao);
}

void render() {
	GLFWwindow* window = game_state.window;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// view
	Matrix4f view;
	view = camera_get_view_matrix();

	// projection
	Matrix4f projection;
	projection = glm::perspective(glm::radians(camera.zoom), 800.0 / 600.0, 0.1, 100.0);

	// drawing cube
	Matrix4f model{ 1.0f };

	glUseProgram(object_shaders);

	int model_location{ glGetUniformLocation(object_shaders, "model") };
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	int view_location{ glGetUniformLocation(object_shaders, "view") };
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	int projection_location{ glGetUniformLocation(object_shaders, "projection") };
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// drawing light 
	model = Matrix4f{ 1.0f };
	model = glm::translate(model, light_position);
	model = glm::scale(model, Vector3f{ 0.2f });

	glUseProgram(light_shaders);

	model_location = glGetUniformLocation(light_shaders, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

	view_location = glGetUniformLocation(light_shaders, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	projection_location = glGetUniformLocation(light_shaders, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(light_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// swap the buffers
	glfwSwapBuffers(window);
}


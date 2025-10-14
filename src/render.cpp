#include "render.hpp"

#include "camera.hpp"
#include "core.hpp"
#include "shapes.hpp"
#include "shader.hpp"
#include "textures.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

static uint texture1;
static uint texture2;

static uint triangle_vao;
static uint rectangle_vao;
static uint cube_vao;

static u32 shader_program{};

void init_renderer() {

	glEnable(GL_DEPTH_TEST);

	shader_program = shader_create_program("../shaders/shader.vs", "../shaders/shader.fs");

	texture1 = generate_texture("../assets/img/container.jpg", 0, GL_RGB);
	texture2 = generate_texture("../assets/img/awesomeface.png", 1, GL_RGBA);

	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader_program, "texture2"), 1);

	triangle_vao = init_triangle();
	rectangle_vao = init_rectangle();
	cube_vao = init_cube();
}

static glm::vec3 cube_positions[10] = {
 glm::vec3(0.0f,  0.0f,  0.0f),
 glm::vec3(2.0f,  5.0f, -15.0f),
 glm::vec3(-1.5f, -2.2f, -2.5f),
 glm::vec3(-3.8f, -2.0f, -12.3f),
 glm::vec3(2.4f, -0.4f, -3.5f),
 glm::vec3(-1.7f,  3.0f, -7.5f),
 glm::vec3(1.3f, -2.0f, -2.5f),
 glm::vec3(1.5f,  2.0f, -2.5f),
 glm::vec3(1.5f,  0.2f, -1.5f),
 glm::vec3(-1.3f,  1.0f, -1.5f)
};

void render() {
	GLFWwindow* window = game_state.window;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Binding texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Bind the preferred VAO for object we want to draw
	//glBindVertexArray(triangle_vao);
	//glBindVertexArray(rectangle_vao);
	glBindVertexArray(cube_vao);

	glm::mat4 view;
	view = glm::lookAt(
		camera.position,
		camera.position + camera.front,
		Direction3::UP
	);

	int view_location{ glGetUniformLocation(shader_program, "view") };
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.zoom), 800.0 / 600.0, 0.1, 100.0);

	int projection_location{ glGetUniformLocation(shader_program, "projection") };
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	// Set polygon rasterization mode of how OpenGL should draw its primatives (default: GL_FRONT_AND_BACK, GL_FILL)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw function 
	// glDrawArrays function that draws primitives using the currently active shader, 
	// the previously defined vertex attribute configuration and with the VBO's vertex data (indirectly bound via the VAO). 
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	u32 count = 3;
	for (u32 i{ 0 }; i < 10; ++i) {
		glm::mat4 model{ 1.0f };
		model = glm::translate(model, cube_positions[i]);
		float angle = 20.0f * (float)i;
		if (count == 3) {
			count = 0;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		}
		else {
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		}

		++count;

		int model_location{ glGetUniformLocation(shader_program, "model") };
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Changing the triangle
	/*
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
	trans = glm::scale(trans, glm::vec3((float)glm::sin(glfwGetTime()), (float)glm::sin(glfwGetTime()), 1.0f));

	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

	glBindVertexArray(triangle_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	*/

	// swap the buffers
	glfwSwapBuffers(window);
}

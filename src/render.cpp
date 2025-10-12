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

/*
float vertices[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
};

static uint vbo;
static uint vao;
*/


static uint texture1;
static uint texture2;

static uint triangle_vao;
static uint rectangle_vao;
static uint cube_vao;



void init_renderer(Shader* shader) {

	/*
	// Creating Vertex Array Object and binding it to store vertex attribute configuration and which VBO to use
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Creating Vertex Buffer Object and binding data to it
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specifying how OpenGL should interpret the vertex data (float vertices[]) and enabling vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VBO since its not registered in VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO for later use or to bind and configure another VAO
	glBindVertexArray(0);
	*/

	glEnable(GL_DEPTH_TEST);

	texture1 = generate_texture("../assets/img/container.jpg", 0, GL_RGB);
	texture2 = generate_texture("../assets/img/awesomeface.png", 1, GL_RGBA);

	shader->use();
	shader->set_int("texture1", 0);
	shader->set_int("texture2", 1);

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

void render(GLFWwindow* window, Shader* shader) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Tell OpenGL to use(make active) this shader program
	shader->use();

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
	view = glm::lookAt(cam.pos, cam.pos - cam.direction, glm::vec3{ 0.0f, 1.0f, 0.0f });

	int view_location{ glGetUniformLocation(shader->id, "view") };
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(cam.fov), 800.0f / 600.0f, 0.1f, 100.0f);

	int projection_location{ glGetUniformLocation(shader->id, "projection") };
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	// Set polygon rasterization mode of how OpenGL should draw its primatives (default: GL_FRONT_AND_BACK, GL_FILL)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw function 
	// glDrawArrays function that draws primitives using the currently active shader, 
	// the previously defined vertex attribute configuration and with the VBO's vertex data (indirectly bound via the VAO). 
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	uint count = 3;
	for (uint i{ 0 }; i < 10; ++i) {
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

		int model_location{ glGetUniformLocation(shader->id, "model") };
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

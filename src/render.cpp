#include "render.hpp"

#include "camera.hpp"
#include "core.hpp"
#include "light.hpp"
#include "shapes.hpp"
#include "shader.hpp"
#include "textures.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

static u32 light_vao;
static u32 cube_vao;

static u32 light_shaders;
static u32 object_shaders;

static u32 wooden_container_texture;
static u32 wooden_container_specular;

static DirectionalLight directional_light{
	.direction = Vector3f{-0.2f, -1.0f, -0.3f},

	.component = LightComponent{
		Vector3f{0.05f, 0.05f, 0.05f},
		Vector3f{0.1f, 0.1f, 0.1f},
		Vector3f{1.0f, 1.0f, 1.0f}
	}
};

static PointLight point_light{
	.position = Vector3f{1.2f, 1.0f, 2.0f},

	.component = LightComponent{
		Vector3f{0.05f, 0.05f, 0.05f},
		Vector3f{0.5f, 0.5f, 0.5f},
		Vector3f{1.0f, 1.0f, 1.0f}
	},
	.constant = 1.0f,
	.linear = 0.32f,
	.quadratic = 0.064f
};
// used for multiple point lights. just making it work, current system not scalable
glm::vec3 point_light_positions[4] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

static Spotlight flashlight{ // a flashlight is a spot light that beams from the viewers camera
	.position = Vector3f{0.0f},
	.direction = Vector3f{0.0f},

	.component = LightComponent{
		Vector3f{0.05f, 0.05f, 0.05f},
		Vector3f{0.5f, 0.5f, 0.5f},
		Vector3f{1.0f, 1.0f, 1.0f}
	},

	.inner_cone_angle = 6.0f,
	.outer_cone_angle = 12.0f,
};

void init_renderer() {

	// generating texture
	wooden_container_texture = generate_texture("../assets/img/container2.png", GL_TEXTURE0, GL_RGBA);
	wooden_container_specular = generate_texture("../assets/img/container2_specular.png", GL_TEXTURE1, GL_RGBA);

	glEnable(GL_DEPTH_TEST);

	// -- light shaders --
	light_shaders = shader_create_program("../shaders/object.vert", "../shaders/light.frag");

	// -- object shaders --
	object_shaders = shader_create_program("../shaders/object.vert", "../shaders/object.frag");

	glUseProgram(object_shaders);

	// Directional Light struct
	glUniform3fv(glGetUniformLocation(object_shaders, "directional_light.direction"), 1, glm::value_ptr(directional_light.direction));
	glUniform3fv(glGetUniformLocation(object_shaders, "directional_light.ambient"), 1, glm::value_ptr(directional_light.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "directional_light.diffuse"), 1, glm::value_ptr(directional_light.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "directional_light.specular"), 1, glm::value_ptr(directional_light.component.specular));

	// Point Light struct single(original)
	/*
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].position"), 1, glm::value_ptr(point_light.position));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].ambient"), 1, glm::value_ptr(point_light.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].diffuse"), 1, glm::value_ptr(point_light.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].specular"), 1, glm::value_ptr(point_light.component.specular));
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[0].constant"), point_light.constant);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[0].linear"), point_light.linear);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[0].quadratic"), point_light.quadratic);
	*/


	// Point Light struct 1
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].position"), 1, glm::value_ptr(point_light_positions[0]));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].ambient"), 1, glm::value_ptr(point_light.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].diffuse"), 1, glm::value_ptr(point_light.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[0].specular"), 1, glm::value_ptr(point_light.component.specular));
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[0].constant"), point_light.constant);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[0].linear"), point_light.linear);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[0].quadratic"), point_light.quadratic);

	// Point Light struct 2
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[1].position"), 1, glm::value_ptr(point_light_positions[1]));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[1].ambient"), 1, glm::value_ptr(point_light.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[1].diffuse"), 1, glm::value_ptr(point_light.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[1].specular"), 1, glm::value_ptr(point_light.component.specular));
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[1].constant"), point_light.constant);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[1].linear"), point_light.linear);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[1].quadratic"), point_light.quadratic);

	// Point Light struct 3
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[2].position"), 1, glm::value_ptr(point_light_positions[2]));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[2].ambient"), 1, glm::value_ptr(point_light.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[2].diffuse"), 1, glm::value_ptr(point_light.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[2].specular"), 1, glm::value_ptr(point_light.component.specular));
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[2].constant"), point_light.constant);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[2].linear"), point_light.linear);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[2].quadratic"), point_light.quadratic);

	// Point Light struct 4
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[3].position"), 1, glm::value_ptr(point_light_positions[3]));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[3].ambient"), 1, glm::value_ptr(point_light.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[3].diffuse"), 1, glm::value_ptr(point_light.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "point_lights[3].specular"), 1, glm::value_ptr(point_light.component.specular));
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[3].constant"), point_light.constant);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[3].linear"), point_light.linear);
	glUniform1f(glGetUniformLocation(object_shaders, "point_lights[3].quadratic"), point_light.quadratic);

	// Flash Light struct
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.position"), 1, glm::value_ptr(flashlight.position));
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.direction"), 1, glm::value_ptr(flashlight.direction));
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.ambient"), 1, glm::value_ptr(flashlight.component.ambient));
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.diffuse"), 1, glm::value_ptr(flashlight.component.diffuse));
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.specular"), 1, glm::value_ptr(flashlight.component.specular));
	glUniform1f(glGetUniformLocation(object_shaders, "flashlight.inner_cone_angle"), glm::cos(glm::radians(flashlight.inner_cone_angle)));
	glUniform1f(glGetUniformLocation(object_shaders, "flashlight.outer_cone_angle"), glm::cos(glm::radians(flashlight.outer_cone_angle)));

	// Material struct
	glUniform1i(glGetUniformLocation(object_shaders, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(object_shaders, "material.specular"), 1);
	glUniform1f(glGetUniformLocation(object_shaders, "material.shininess"), 32.0f);

	init_cube(&light_vao);
	init_cube(&cube_vao);
}

glm::vec3 cube_positions[10] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

void render() {
	GLFWwindow* window = game_state.window;

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// model
	Matrix4f model{ 1.0f };

	// view
	Matrix4f view;
	view = camera_get_view_matrix();

	// projection
	Matrix4f projection;
	projection = glm::perspective(glm::radians(camera.zoom), 800.0 / 600.0, 0.1, 100.0);

	// drawing cube
	glUseProgram(object_shaders);
	glBindVertexArray(cube_vao);

	// activating texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wooden_container_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, wooden_container_specular);

	// passing camera positions
	glUniform3fv(glGetUniformLocation(object_shaders, "camera_position"), 1, glm::value_ptr(Vector3f(camera.position)));
	// passing flashlight data
	flashlight.position = camera.position;
	flashlight.direction = camera.front;
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.position"), 1, glm::value_ptr(flashlight.position));
	glUniform3fv(glGetUniformLocation(object_shaders, "flashlight.direction"), 1, glm::value_ptr(flashlight.direction));

	// passing view matrix
	int view_location{ glGetUniformLocation(object_shaders, "view") };
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	// passing projection matrix
	int projection_location{ glGetUniformLocation(object_shaders, "projection") };
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	// passing model matrix 
	int model_location{ glGetUniformLocation(object_shaders, "model") };
	s32 count{ 0 };
	for (s32 i = 0; i < 10; i++) {

		model = Matrix4f(1.0f);
		model = glm::translate(model, cube_positions[i]);
		f32 angle = 20.0f * (float)i;

		if (count == 2) {
			count = 0;
			model = glm::rotate(model, glm::radians(angle * (float)glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
		}
		else {
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			++count;
		}

		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// drawing light 
	glUseProgram(light_shaders);
	glBindVertexArray(light_vao);

	// temp
	//light.position = Vector3f((float)cos(glfwGetTime()) * 5, 1.0f, (float)sin(glfwGetTime()) * 5);
	//glUniform3fv(glGetUniformLocation(light_shaders, "light.position"), 1, glm::value_ptr(light.position));
	// temp

	view_location = glGetUniformLocation(light_shaders, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	projection_location = glGetUniformLocation(light_shaders, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	for (s32 i = 0; i < 4; i++) {
		model = Matrix4f{ 1.0f };
		model = glm::translate(model, point_light_positions[i]);
		model = glm::scale(model, Vector3f{ 0.1f });

		model_location = glGetUniformLocation(light_shaders, "model");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// swap the buffers
	glfwSwapBuffers(window);
}


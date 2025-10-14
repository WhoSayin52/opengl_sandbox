#include "shader.hpp"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

u32 shader_create_program(const char* vertex_path, const char* fragment_path) {
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream vertex_file;
	std::ifstream fragment_file;

	vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);
		std::stringstream vertex_stream, fragment_stream;
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		vertex_file.close();
		fragment_file.close();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
	}

	const char* vertex_shader_code{ vertex_code.c_str() };
	const char* fragment_shader_code{ fragment_code.c_str() };

	uint vertex, fragment;
	int success;
	char info_log[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << '\n';
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << '\n';
	}

	u32 id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << '\n';
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return id;
}

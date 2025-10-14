#ifndef SHADER_HPP
#define SHADER_HPP

#include "core.hpp"

u32 shader_create_program(const char* vertex_path, const char* fragment_path);






/*
#include <string>

class Shader
{
public:
	// the program ID
	unsigned int id;

	// constructor reads and builds the shader
	Shader(const char* vertex_path, const char* fragment_path);
	// use/activate the shader
	void use();
	// utility uniform functions
	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;

private:
	void check_compile_errors(unsigned int shader, std::string type);
};
*/
#endif // SHADER_HPP


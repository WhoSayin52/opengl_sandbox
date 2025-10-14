#include "textures.hpp"

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <iostream>

u32 generate_texture(const char* path, u32 text_num, u32 rgb) {

	u32 texture;
	glGenTextures(1, &texture);
	glActiveTexture(text_num);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channel_count;
	stbi_set_flip_vertically_on_load(true);
	ubyte* data{
		stbi_load(
			path,
			&width,
			&height,
			&channel_count,
			0
		)
	};

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgb, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture\n";
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

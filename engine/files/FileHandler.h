#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <stb_image.h>
#include <string>

class FileHandler {
  public:
	FileHandler() {}
	~FileHandler() {}

	static unsigned char *loadImage(const char *fileName, int *width, int *height, int *nrChannels,
	                                GLenum *format, bool flipped = false) {

		if (!fileName) {
			std::cout << "Error: fileName is NULL" << std::endl;
			return nullptr;
		}

		std::stringstream ss;
		ss << "../assets/textures/" << fileName;
		std::string temp = ss.str();
		const char *filePath = temp.c_str();

		stbi_set_flip_vertically_on_load(!flipped);

		unsigned char *data = stbi_load(filePath, width, height, nrChannels, 0);

		if (data) {
			*format = (*nrChannels == 4) ? GL_RGBA : GL_RGB;
		} else {
			std::cout << "Error: Failed to load to texture:" << filePath << std::endl;
			std::cout << "Reason: " << stbi_failure_reason() << std::endl;
		}
		return data;
	}

	static unsigned char *convertRGBtoRGBA(unsigned char *rgbData, int width, int height) {
		unsigned char *rgbaData = new unsigned char[width * height * 4];
		for (int i = 0; i < width * height; ++i) {
			rgbaData[i * 4] = rgbData[i * 3];
			rgbaData[i * 4 + 1] = rgbData[i * 3 + 1];
			rgbaData[i * 4 + 2] = rgbData[i * 3 + 2];
			rgbaData[i * 4 + 3] = 255; // Full alpha
		}
		return rgbaData;
	}

	static void freeImage(unsigned char *data) { stbi_image_free(data); }
};
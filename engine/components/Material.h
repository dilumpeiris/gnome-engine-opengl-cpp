#pragma once
#include "ecs/ECS.h"
#include <iostream>
#include <sstream>
#include <memory.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Texture struct later needs to be flushed out.
struct Texture {
	unsigned int ID;
	const char *name;
	float transparency;
};

// =====================================================================================================
// Material Component
// =====================================================================================================
class Material : public Component {

  public:
	unsigned int textureArrayID; // Single texture array for all frames
	std::vector<Texture> texture_items;
	int currentTexLocation = 0;
	bool textureArrayInitialized = false;
	int arrayWidth = 0, arrayHeight = 0;

  public:
	Material() : textureArrayID(0) {}

	void addTexture(const char *filePath, bool repeat_x = false, bool repeat_y = false,
	                bool flipped = false) {
		Texture text;
		text.name = "texture00";

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data = loadImage(filePath, &width, &height, &nrChannels, &format, flipped);

		if (!data) {
			std::cout << "Error: Failed to load texture: " << filePath << std::endl;
			return;
		}

		// Initialize texture array on first texture
		if (!textureArrayInitialized) {
			initializeTextureArray(width, height, 16); // Support up to 16 layers
			textureArrayInitialized = true;
			arrayWidth = width;
			arrayHeight = height;
		}

		// Check if dimensions match
		if (width != arrayWidth || height != arrayHeight) {
			std::cout << "Error: Texture dimensions don't match array dimensions" << std::endl;
			stbi_image_free(data);
			return;
		}

		// Convert data to consistent format (RGBA)
		unsigned char *finalData = data;
		if (format == GL_RGB) {
			finalData = convertRGBtoRGBA(data, width, height);
			stbi_image_free(data);
			format = GL_RGBA;
		}

		// Add texture to array layer
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, currentTexLocation, // x, y, layer
		                width, height, 1, format, GL_UNSIGNED_BYTE, finalData);

		text.ID = textureArrayID; // All textures use the same array
		texture_items.emplace_back(text);
		currentTexLocation++;

		if (finalData != data) {
			delete[] finalData;
		} else {
			stbi_image_free(data);
		}
	}

	void initializeTextureArray(int width, int height, int maxLayers) {
		glGenTextures(1, &textureArrayID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);

		// Set texture parameters for ARRAY target (FIXED)
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Allocate storage for texture array
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, maxLayers, 0, GL_RGBA,
		             GL_UNSIGNED_BYTE, nullptr);
	}

	unsigned char *convertRGBtoRGBA(unsigned char *rgbData, int width, int height) {
		unsigned char *rgbaData = new unsigned char[width * height * 4];
		for (int i = 0; i < width * height; ++i) {
			rgbaData[i * 4] = rgbData[i * 3];
			rgbaData[i * 4 + 1] = rgbData[i * 3 + 1];
			rgbaData[i * 4 + 2] = rgbData[i * 3 + 2];
			rgbaData[i * 4 + 3] = 255; // Full alpha
		}
		return rgbaData;
	}
	// Utility functions may have to be written as seperate static class functions for better usage.
	unsigned char *loadImage(const char *fileName, int *width, int *height, int *nrChannels,
	                         GLenum *format, bool flipped = false) {

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

	void setShaderTextures(unsigned int shaderID) {
		glUniform1i(glGetUniformLocation(shaderID, "spriteArray"), 0);
	}

	void bind() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
	}
};

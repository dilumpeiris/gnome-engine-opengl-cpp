#pragma once

#include "renderer/GPUTexture.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Texture {
	unsigned int ID;
	const char *name;
	float transparency;
};

class OpenGLTexture : public GPUTexture {
  private:
	// std::unordered_map<std::size_t, std::vector<Texture>> texture_items;
	std::vector<Texture> texture_items;
	unsigned int textureArrayID;
	int currentTexLocation = 0;
	bool textureArrayInitialized = false;
	int arrayWidth = 0, arrayHeight = 0;

  public:
	OpenGLTexture() {}
	~OpenGLTexture() {}

	void init() override {}
	void initializeTextureArray(std::size_t entityID, int width, int height, int maxLayers) {

		glGenTextures(1, &textureArrayID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);

		std::cout << "Texture array initialized: " << arrayWidth << "x" << arrayHeight << std::endl;

		// Set texture parameters for ARRAY target (FIXED)
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::cout << "Texture parameters set" << std::endl;

		// Allocate storage for texture array
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, maxLayers, 0, GL_RGBA,
		             GL_UNSIGNED_BYTE, nullptr);

		std::cout << "Texture storage allocated" << std::endl;
	}

	void addTexture(std::size_t entityID, const char *filePath) override {

		bool repeat_x = false, repeat_y = false, flipped = false;

		Texture text;
		text.name = "texture00";

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data = loadImage(filePath, &width, &height, &nrChannels, &format, flipped);

		if (!data) {
			std::cout << "Error: Failed to load texture: " << filePath << std::endl;
			return;
		}

		std::cout << "Texture loaded successfully: " << filePath << std::endl;

		// Initialize texture array on first texture
		if (!textureArrayInitialized) {
			initializeTextureArray(entityID, width, height, 16); // Support up to 16 layers
			textureArrayInitialized = true;
			arrayWidth = width;
			arrayHeight = height;
		}

		std::cout << "Texture array initialized: " << arrayWidth << "x" << arrayHeight << std::endl;

		// Check if dimensions match
		if (width != arrayWidth || height != arrayHeight) {
			std::cout << "Error: Texture dimensions don't match array dimensions" << std::endl;
			stbi_image_free(data);
			return;
		}

		std::cout << "Texture dimensions match array dimensions" << std::endl;

		// Convert data to consistent format (RGBA)
		unsigned char *finalData = data;
		if (format == GL_RGB) {
			finalData = convertRGBtoRGBA(data, width, height);
			stbi_image_free(data);
			format = GL_RGBA;
		}

		std::cout << "Texture converted to RGBA" << std::endl;

		// Add texture to array layer
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, currentTexLocation, // x, y, layer
		                width, height, 1, format, GL_UNSIGNED_BYTE, finalData);

		std::cout << "Texture added to array layer" << std::endl;

		text.ID = textureArrayID; // All textures use the same array
		texture_items.emplace_back(text);
		currentTexLocation++;

		std::cout << "Texture added to array layer" << std::endl;

		if (finalData != data) {
			delete[] finalData;
		} else {
			stbi_image_free(data);
		}

		std::cout << "Texture added to array layer" << std::endl;
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

	void setShaderTextures(std::size_t entityID, unsigned int shaderID) override {
		glUniform1i(glGetUniformLocation(shaderID, "spriteArray"), 0);
	}

	void use(std::size_t entityID) override {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
	}
};
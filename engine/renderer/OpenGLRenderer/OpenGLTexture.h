#pragma once

#include "renderer/GPUTexture.h"
#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

// This should not be here!! Or maybe it should be.
#include "files/FileHandler.h"

// =====================================================================================================
// OpenGLTexture Class
// =====================================================================================================

class OpenGLTexture : public GPUTexture {
  private:
	std::unordered_map<std::size_t, unsigned int> textures;
	std::unordered_map<std::size_t, int> currentTextureLocations;

	bool textureArrayInitialized = false;
	int arrayWidth = 0, arrayHeight = 0;

  public:
	OpenGLTexture() {}
	~OpenGLTexture() {}

	void init() override {}

	void initializeTextureArray(std::size_t entityID, int width, int height, int maxLayers) {

		glGenTextures(1, &textures[entityID]);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textures[entityID]);

		currentTextureLocations[entityID] = 0;

		// Set texture parameters for ARRAY target (FIXED)
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Allocate storage for texture array
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, maxLayers, 0, GL_RGBA,
		             GL_UNSIGNED_BYTE, nullptr);
	}

	void addTexture(std::size_t entityID, const char *filePath) override {

		bool repeat_x = false, repeat_y = false, flipped = false;

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data =
		    FileHandler::loadImage(filePath, &width, &height, &nrChannels, &format, flipped);

		if (!data) {
			std::cout << "Error: Failed to load texture: " << filePath << std::endl;
			return;
		}

		// Move this logic to init().
		// Initialize texture array on first texture.
		if (!textureArrayInitialized) {
			initializeTextureArray(entityID, width, height, 16); // Support up to 16 layers
			textureArrayInitialized = true;
			arrayWidth = width;
			arrayHeight = height;
		}

		// Check if dimensions match
		if (width != arrayWidth || height != arrayHeight) {
			std::cout << "Error: Texture dimensions don't match array dimensions" << std::endl;
			FileHandler::freeImage(data);
			return;
		}

		// Convert data to consistent format (RGBA)
		unsigned char *finalData = data;
		if (format == GL_RGB) {
			finalData = FileHandler::convertRGBtoRGBA(data, width, height);
			FileHandler::freeImage(data);
			format = GL_RGBA;
		}

		// Add texture to array layer
		glBindTexture(GL_TEXTURE_2D_ARRAY, textures[entityID]);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,
		                currentTextureLocations[entityID], // x, y, layer
		                width, height, 1, format, GL_UNSIGNED_BYTE, finalData);

		currentTextureLocations[entityID]++;

		if (finalData != data) {
			delete[] finalData;
		} else {
			FileHandler::freeImage(data);
		}
	}

	void use(std::size_t entityID) override { glActiveTexture(GL_TEXTURE0); }
};
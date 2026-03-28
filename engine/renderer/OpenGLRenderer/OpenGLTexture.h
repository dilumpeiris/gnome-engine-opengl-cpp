#pragma once

#include "renderer/GPUTexture.h"
#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

// This should not be here!! Or maybe it should be.
#include "files/FileHandler.h"

enum class TextureType { Standard2D, Array2D /* Animation */, CubeMap };

struct TextureImage {
	unsigned int textureID;
	TextureType textureType;
	const char *filePath;
	const char *name;
	int width, height, currentLayer = 0;
	float transparency;
};

// =====================================================================================================
// OpenGLTexture Class
// =====================================================================================================

class OpenGLTexture : public GPUTexture {
  private:
	std::unordered_map<std::size_t, std::vector<TextureImage>> textureImages;
	std::unordered_map<std::size_t, int> currentTextureLocations;

	// Maybe this will help in runtime texture binding.
	std::unordered_map<std::size_t, std::unordered_map<const char *, unsigned int>> textures;

  public:
	OpenGLTexture() {}
	~OpenGLTexture() {}

	void init(std::size_t entityID) override { currentTextureLocations[entityID] = 0; }

	void initializeTexture3D(std::size_t entityID, const char *name, const char *filePath,
	                         int width, int height, int maxLayers) {

		TextureImage textureImage;
		textureImage.textureType = TextureType::Array2D;
		textureImage.width = width;
		textureImage.height = height;
		textureImage.name = name;
		textureImage.filePath = filePath;

		textureImage.currentLayer = 0;

		glGenTextures(1, &textureImage.textureID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureImage.textureID);

		// Set texture parameters for ARRAY target (FIXED)
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Allocate storage for texture array
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, maxLayers, 0, GL_RGBA,
		             GL_UNSIGNED_BYTE, nullptr);

		textureImages[entityID].push_back(textureImage);
	}

	void initializeTexture2D(std::size_t entityID, const char *name, const char *filePath,
	                         int width, int height) {

		TextureImage textureImage;
		textureImage.textureType = TextureType::Standard2D;
		textureImage.width = width;
		textureImage.height = height;
		textureImage.currentLayer = 0;
		textureImage.name = name;
		textureImage.filePath = filePath;

		glGenTextures(1, &textureImage.textureID);
		glBindTexture(GL_TEXTURE_2D, textureImage.textureID);

		// Set texture parameters for 2D target
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Allocate storage for 2D texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		             nullptr);

		textureImages[entityID].push_back(textureImage);
	}

	void addTexture(std::size_t entityID, const char *name, const char *filePath) override {

		bool repeat_x = false, repeat_y = false, flipped = false;

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data =
		    FileHandler::loadImage(filePath, &width, &height, &nrChannels, &format, flipped);

		if (!data) {
			std::cout << "Error: Failed to load texture: " << filePath << std::endl;
			return;
		}

		initializeTexture2D(entityID, name, filePath, width, height);

		// Convert data to consistent format (RGBA)
		unsigned char *finalData = data;
		if (format == GL_RGB) {
			finalData = FileHandler::convertRGBtoRGBA(data, width, height);
			FileHandler::freeImage(data);
			format = GL_RGBA;
		}

		// Add texture to 2D layer
		glBindTexture(GL_TEXTURE_2D, textureImages[entityID].back().textureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, finalData);

		if (finalData != data) {
			delete[] finalData;
		} else {
			FileHandler::freeImage(data);
		}
	}

	void addTexture3D(std::size_t entityID, const char *name, const char *filePath,
	                  int maxLayers = 16) override {
		bool repeat_x = false, repeat_y = false, flipped = false;

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data =
		    FileHandler::loadImage(filePath, &width, &height, &nrChannels, &format, flipped);

		if (!data) {
			std::cout << "Error: Failed to load texture: " << filePath << std::endl;
			return;
		}

		// Find if an animation with this name already exists
		TextureImage *targetAnimation = nullptr;
		if (textureImages.count(entityID)) {
			for (auto &img : textureImages[entityID]) {
				if (img.textureType == TextureType::Array2D && std::string(img.name) == name) {
					targetAnimation = &img;
					break;
				}
			}
		}

		// If not found, initialize a new texture array
		if (!targetAnimation) {
			initializeTexture3D(entityID, name, filePath, width, height, maxLayers);
			targetAnimation = &textureImages[entityID].back();
		}

		// Verify dimensions
		if (width != targetAnimation->width || height != targetAnimation->height) {
			std::cout << "Error: Texture dimensions don't match array dimensions for animation: "
			          << name << std::endl;
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
		glBindTexture(GL_TEXTURE_2D_ARRAY, targetAnimation->textureID);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,
		                targetAnimation->currentLayer, // layer index
		                width, height, 1, format, GL_UNSIGNED_BYTE, finalData);

		targetAnimation->currentLayer++;

		if (finalData != data) {
			delete[] finalData;
		} else {
			FileHandler::freeImage(data);
		}
	}

	int getTextureLocation(std::size_t entityID, const char *name) override {
		int location = 0;
		for (auto &img : textureImages[entityID]) {
			if (std::string(img.name) == name) {
				return location;
			}
			location++;
		}
		return -1;
	}

	// Refactor thiss to use 0-8 2d texture slots and 9-16 3d texture slots
	void bind(std::size_t entityID) override {
		int location = 0;
		for (auto &img : textureImages[entityID]) {
			glActiveTexture(GL_TEXTURE0 + location);
			if (img.textureType == TextureType::Array2D) {
				glBindTexture(GL_TEXTURE_2D_ARRAY, img.textureID);
			} else {
				glBindTexture(GL_TEXTURE_2D, img.textureID);
			}
			location++;
		}
	}

	void use(std::size_t entityID, const char *name) override {
		for (auto &img : textureImages[entityID]) {
			if (std::string(img.name) == name) {
				if (img.textureType == TextureType::Array2D) {
					glActiveTexture(GL_TEXTURE0 + 1);
					glBindTexture(GL_TEXTURE_2D_ARRAY, img.textureID);
				} else if (img.textureType == TextureType::Standard2D) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, img.textureID);
				}
			}
		}
	}

	const char *getTextureName(std::size_t entityID, int textureLocation) override {
		// if (textureImages.count(entityID)) {
		// 	for (auto &img : textureImages[entityID]) {
		// 		// if (img.textureID == textureLocation) {
		// 		// 	return img.name;
		// 		// }
		// 	}
		// }
		return "walking-left";
	}

	void printTextureNames(std::size_t entityID) override {
		int location = 0;
		for (auto &img : textureImages[entityID]) {
			std::cout << "Texture Name: " << img.name << " at location: " << location
			          << "layer count: " << img.currentLayer << std::endl;
			location++;
		}
	}
};
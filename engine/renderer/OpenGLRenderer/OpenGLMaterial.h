#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include "../GPUMaterial.h"
#include "assets/MaterialAsset.h"
#include "assets/AnimationAsset.h"
#include <cstddef>

#include "files/FileHandler.h"
#include "ShaderLibrary.h"

enum TextureType { Standard2D, Array2D };

struct GPUTexture {
	GLuint textureID;
	TextureType textureType;
	const char *filePath;
	const char *name;
	int width, height, currentLayer = 0;
	float transparency;
};

struct GPUMesh {
	GLuint VAO, VBO, EBO;
	int indexCount;
	int vertexCount;
	GLenum renderMode;
};

struct GPUMat {
	std::vector<GPUTexture> textures;
	// blend, depth state
	bool depthWrite;
	GLenum blendSrc, blendDst;
};

struct GPUAnim {
	GLuint id;
	std::string name = "";

	int height, width, depth; // depth = number of frames/layers
	float frameDuration;
	bool loop;
	float timer = 0.0f;
	int currentFrameIndex = 0;
};

struct RenderEntry {
	GPUMesh mesh;
	GPUMat material;
	glm::mat4 transform; // updated each frame
};

// =====================================================================================================
// OpenGLMaterial Class
// =====================================================================================================

class OpenGLMaterial : public GPUMaterial {
  private:
	std::unordered_map<std::size_t, RenderEntry> renderEntries;
	std::unordered_map<std::size_t, std::vector<std::string>> entityShaders;
	std::unordered_map<std::size_t, std::unordered_map<std::string, GPUAnim>> entityAnimations;

  public:
	void init(std::size_t entityID, MaterialAsset material, MeshAsset mesh) override {

		GPUMesh gpuMesh = createMesh(mesh);

		GPUMat gpuMaterial;

		renderEntries[entityID] = {gpuMesh, gpuMaterial, glm::mat4(1.0f)};
	}

	void addShader(std::size_t entityID, ShaderPass shader) override {
		ShaderLibrary::get().addShader(shader.name, shader.vertexSrc, shader.fragmentSrc);
		entityShaders[entityID].push_back(shader.name);
	}

	void update(std::size_t entityID) override {}

	void draw(std::size_t entityID) override {
		for (auto &name : entityShaders[entityID]) {
			GPUShaderPass &shader = ShaderLibrary::get().shaders[name];
			glUseProgram(shader.ID);
			glBindVertexArray(renderEntries[entityID].mesh.VAO);
			glDrawElements(renderEntries[entityID].mesh.renderMode,
			               renderEntries[entityID].mesh.indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	GPUMesh createMesh(MeshAsset mesh) {
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indexCount * sizeof(int), mesh.indices.data(),
		             GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(float), mesh.vertices.data(),
		             GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		                      (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Material coordinates
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		                      (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		GPUMesh gpuMesh;
		gpuMesh.VAO = VAO;
		gpuMesh.VBO = VBO;
		gpuMesh.EBO = EBO;
		gpuMesh.indexCount = mesh.indexCount;
		gpuMesh.vertexCount = mesh.vertexCount;
		gpuMesh.renderMode = mesh.renderMode;

		return gpuMesh;
	}

	GPUTexture createTexture(TextureAsset texture) {

		bool repeat_x = false, repeat_y = false, flipped = false;

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data = FileHandler::loadImage(texture.filePath, &width, &height, &nrChannels,
		                                             &format, flipped);

		if (!data) {
			std::cout << "Error: Failed to load texture: " << texture.filePath << std::endl;
			return GPUTexture{};
		}

		GPUTexture textureImage;
		textureImage.width = texture.width;
		textureImage.height = texture.height;
		textureImage.currentLayer = 0;
		textureImage.name = texture.name;
		textureImage.filePath = texture.filePath;
		textureImage.textureType = TextureType::Standard2D;

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

		return textureImage;
	}

	void createTexture3D(GPUAnim &anim) {
		glBindTexture(GL_TEXTURE_3D, anim.id);

		// Set texture parameters for 3D target
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // depth axis
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Allocate storage for 3D texture
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
		             anim.width,  // width
		             anim.height, // height
		             anim.depth,  // depth = number of frames/layers
		             0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	void addTexture3D(GPUAnim &anim, TextureAsset texture) {

		bool repeat_x = false, repeat_y = false, flipped = false;

		int width, height, nrChannels;
		GLenum format;
		unsigned char *data = FileHandler::loadImage(texture.filePath, &width, &height, &nrChannels,
		                                             &format, flipped);
		if (!data) {
			std::cout << "Error: Failed to load texture: " << texture.filePath << std::endl;
			return;
		}

		glBindTexture(GL_TEXTURE_3D, anim.id); // bind existing texture, don't generate new

		// upload into specific layer
		glTexSubImage3D(GL_TEXTURE_3D,
		                0,                      // mip level
		                0, 0,                   // x, y offset
		                anim.currentFrameIndex, // z offset = layer index ← correct place
		                width, height,
		                1, // one layer at a time
		                GL_RGBA, GL_UNSIGNED_BYTE, data);

		anim.currentFrameIndex++;

		FileHandler::freeImage(data);
	}

	void addAnimation(std::size_t entityID, std::string name, AnimationAsset asset) override {

		if (entityAnimations[entityID][name].name != name) {
			GPUAnim anim;
			anim.name = name;
			anim.width = asset.width;
			anim.height = asset.height;
			anim.depth = asset.depth;
			anim.frameDuration = asset.frameDuration;
			anim.loop = asset.loop;

			createTexture3D(anim);

			entityAnimations[entityID][name] = anim;
		}

		GPUAnim &anim = entityAnimations[entityID][name];

		for (auto &frame : asset.frames) {
			addTexture3D(anim, frame);
		}
	}
};

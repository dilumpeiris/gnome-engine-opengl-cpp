#pragma once
namespace GRectangleMesh {
float vertices[] = {
    // positions // colors // texture coords
    1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, // top right

    1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, // bottom right

    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, // bottom left

    -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    1.0f // top left
};

int indices[] = {0, 1, 3, 1, 2, 3};

const int verticesCount = sizeof(vertices) / sizeof(vertices[0]);
const int indicesCount = sizeof(indices) / sizeof(indices[0]);
} // namespace GRectangleMesh
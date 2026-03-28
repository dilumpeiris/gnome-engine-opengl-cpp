#pragma once
namespace GLineMesh {
float vertices[] = {
    // positions // colors // texture coords (unused but needed for stride of 8 floats)
    -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};

int indices[] = {0, 1};

const int verticesCount = sizeof(vertices) / sizeof(vertices[0]);
const int indicesCount = sizeof(indices) / sizeof(indices[0]);
} // namespace GLineMesh

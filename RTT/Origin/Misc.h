#pragma once

#include <iostream>
#include <functional>
#include "gl/glew.h"


GLuint CreateGLBufferObject(GLenum target, GLsizeiptr size, GLenum usage, const void* data = nullptr);
GLuint CreateGLVertexArrayObject(std::function<void()> Setting);
GLuint CreateFrameBufferObject(GLuint& colorBufferObject, GLuint& depthBufferObject, int width, int height);
char* LoadFileContent(const char* path);
GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath);
GLuint CreateTextureFromFile(const char* imagePath);

void CheckGLError(const char* file, unsigned int line);
#define GL_CALL(X) do {X; CheckGLError(__FILE__, __LINE__);}while (0)
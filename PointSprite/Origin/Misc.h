#pragma once

#include <iostream>
#include "gl/glew.h"


GLuint CreateGLBufferObject(GLenum target, GLsizeiptr size, GLenum usage, const void* data = nullptr);
char* LoadFileContent(const char* path);
GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath);
GLuint CreateTextureFromFile(const char* imagePath);

void CheckGLError(const char* file, unsigned int line);
#define GL_CALL(X) do {X; CheckGLError(__FILE__, __LINE__);}while (0)
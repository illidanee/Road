#pragma once

#include <iostream>
#include "gl/glew.h"


GLuint CreateGLBufferObject(GLenum target, GLsizeiptr size, GLenum usage, const void* data = nullptr);
char* LoadFileContent(const char* path);
GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath);
GLuint CreateTextureFromFile(const char* imagePath);
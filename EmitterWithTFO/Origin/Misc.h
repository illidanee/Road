#pragma once

#include <iostream>
#include <functional>
#include "gl/glew.h"

float MyFrand1();
float MyFrand2();
GLuint CreateGLBufferObject(GLenum target, GLsizeiptr size, GLenum usage, const void* data = nullptr);
GLuint CreateGLVertexArrayObject(std::function<void()> Setting);
GLuint CreateFrameBufferObject(GLuint& colorBufferObject, GLuint& depthBufferObject, int width, int height, GLuint* colorBufferObject2 = nullptr);
GLuint Create3DTexture(int w, int h, int d);
char* LoadFileContent(const char* path);
GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath, const char* gsShaderPath = nullptr);
GLuint CreateComputeProgram(const char* computeShaderPath);
char* DecodeBmp(char* content, int& width, int& height);
GLuint CreateTextureFromFile(const char* imagePath);

void CheckGLError(const char* file, unsigned int line);
#define GL_CALL(X) do {X; CheckGLError(__FILE__, __LINE__);}while (0)


//TFO

GLuint CreateTFO(GLuint TFOBuffer);
GLuint CreateTFOProgram(const char* vsShaderPath, GLsizei attribNum, const GLchar* const* attribs, GLenum model);
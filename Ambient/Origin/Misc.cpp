#include "Misc.h"

GLuint CreateGLBufferObject(GLenum target, GLsizeiptr size, GLenum usage, const void* data)
{
	GLuint obj;
	glGenBuffers(1, &obj);
	glBindBuffer(target, obj);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
	return obj;
}

char* LoadFileContent(const char* path)
{
	FILE* pf = fopen(path, "rb");
	if (pf == 0)
		return 0;

	fseek(pf, 0, SEEK_END);
	long len = ftell(pf);
	rewind(pf);
	char* buffer = new char[len + 1];
	fread(buffer, 1, len, pf);
	buffer[len] = 0;
	fclose(pf);

	return buffer;
}

GLuint CreateShader(GLenum shaderType, const char* shaderPath)
{
	char* shaderSource = LoadFileContent(shaderPath);
	if (shaderSource == 0)
	{
		std::cout << "Load File Error! " << shaderPath << std::endl;
		return 0;
	}

	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		std::cout << "Create Shader Error! " << shaderPath << std::endl;
		delete[] shaderSource;
		return 0;
	}

	glShaderSource(shader, 1, &shaderSource, 0);
	glCompileShader(shader);
	GLint glRes = true;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &glRes);
	if (glRes == false)
	{
		char compileInfo[1024];
		GLsizei length;
		glGetShaderInfoLog(shader, 1024, &length, compileInfo);
		std::cout << "Compile Shader Error! " << shaderPath << std::endl;
		std::cout << compileInfo << std::endl;
		delete[] shaderSource;
		glDeleteShader(shader);
		return 0;
	}
	
	delete[] shaderSource;
	return shader;
}

GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath)
{
	GLuint vsShader = CreateShader(GL_VERTEX_SHADER, vsShaderPath);
	GLuint fsShader = CreateShader(GL_FRAGMENT_SHADER, fsShaderPath);
	GLuint pragram = glCreateProgram();
	glAttachShader(pragram, vsShader);
	glAttachShader(pragram, fsShader);
	glLinkProgram(pragram);
	GLint glRes = true;
	glGetProgramiv(pragram, GL_LINK_STATUS, &glRes);
	if (glRes == false)
	{
		char linkInfo[1024];
		GLsizei length;
		glGetProgramInfoLog(pragram, 1024, &length, linkInfo);
		std::cout << "Link Program Error! " << std::endl;
		std::cout << linkInfo << std::endl;
		glDeleteProgram(pragram);
		return 0;
	}
	glDetachShader(pragram, vsShader);
	glDetachShader(pragram, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	return pragram;
}

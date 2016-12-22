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

GLuint CreateGLVertexArrayObject(std::function<void()> Setting)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	Setting();
	glBindVertexArray(0);
	return VAO;
}

GLuint CreateFrameBufferObject(GLuint& colorBufferObject, GLuint& depthBufferObject, int width, int height, GLuint* colorBufferObject2)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &colorBufferObject);
	glBindTexture(GL_TEXTURE_2D, colorBufferObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferObject, 0);

	if (colorBufferObject2 != nullptr)
	{
		glGenTextures(1, colorBufferObject2);
		glBindTexture(GL_TEXTURE_2D, *colorBufferObject2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *colorBufferObject2, 0);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, buffers);
	}

	glGenTextures(1, &depthBufferObject);
	glBindTexture(GL_TEXTURE_2D, depthBufferObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferObject, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Frame Buffer Compile Error! \n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fbo;
}

char* LoadFileContent(const char* path)
{
	FILE* pf;
	fopen_s(&pf, path, "rb");
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

static char* DecodeBmp(char* content, int& width, int& height)
{
	int contentOffset = *((int*)(content + 10));
	width = *((int*)(content + 18));
	height = *((int*)(content + 22));

	return content + contentOffset;
}

const unsigned long FORMAT_DXT1 = 0x31545844l;//DXT1 -> 1 T X D
static char* DecodeDXT1(char* content, int& width, int& height, int&pixelSize)
{
	height = *(unsigned long*)(content + sizeof(unsigned long) * 3);
	width = *(unsigned long*)(content + sizeof(unsigned long) * 4);
	pixelSize = *(unsigned long*)(content + sizeof(unsigned long) * 5);
	unsigned long compressFormat;
	compressFormat = *(unsigned long*)(content + sizeof(unsigned long) * 21);
	switch (compressFormat)
	{
	case FORMAT_DXT1:
		break;
	}
	char*pixelData = new char[pixelSize];
	memcpy(pixelData, content + sizeof(unsigned long) * 32, pixelSize);

	return pixelData;
}

GLuint CreateTextureFromFile(const char* imagePath)
{
	char* fileContent = LoadFileContent(imagePath);
	if (fileContent == 0)
	{
		return 0;
	}

	char* content = nullptr;
	int width = 0;
	int height = 0;
	int pixelSize = 0;
	GLenum srcFormat = GL_RGB;

	if (*((unsigned short*)fileContent) == 0x4D42)
	{
		content = DecodeBmp(fileContent, width, height);
	}
	else if ((memcmp(fileContent, "DDS ", 4) == 0))
	{
		content = DecodeDXT1(fileContent, width, height, pixelSize);
		srcFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}
	else
	{
		printf("Can not parser %s \n", imagePath);
		delete[] fileContent;
		return 0;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (srcFormat == GL_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, content);
	}
	else if (srcFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, srcFormat, width, height, 0, pixelSize, content);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] fileContent;
	return texture;
}

void CheckGLError(const char* file, unsigned int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			printf("OpenGL Call Error: GL_INVALID_ENUM in file %s on %d line \n", file, line);
			break;
		case GL_INVALID_VALUE:
			printf("OpenGL Call Error: GL_INVALID_VALUE in file %s on %d line \n", file, line);
			break;
		case GL_INVALID_OPERATION:
			printf("OpenGL Call Error: GL_INVALID_OPERATION in file %s on %d line \n", file, line);
			break;
		case GL_STACK_OVERFLOW:
			printf("OpenGL Call Error: GL_STACK_OVERFLOW in file %s on %d line \n", file, line);
			break;
		case GL_STACK_UNDERFLOW:
			printf("OpenGL Call Error: GL_STACK_UNDERFLOW in file %s on %d line \n", file, line);
			break;
		case GL_OUT_OF_MEMORY:
			printf("OpenGL Call Error: GL_OUT_OF_MEMORY in file %s on %d line \n", file, line);
			break;
		default:
			printf("OpenGL Call Error: Others 0x%x in file %s on %d line \n", error, file, line);
			break;
		}
	}
}
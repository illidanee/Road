#include <stdio.h>
#include <iostream>
#include <windows.h>

//#include <gl/GL.h>
//#pragma comment(lib, "opengl32.lib")

#include <GL/glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Misc.h"
#include "Model.h"

#include "Timer.h"
#include "Frustum.h"

float identity[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f)) * glm::rotate(glm::mat4(1.0f), -3.14f/2, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) * glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
glm::mat4 projection = glm::perspective(45.0f, 1280 / 800.0f, 0.1f, 1000.0f);
//glm::mat4 uiMatrix = glm::ortho(-640.0f, 640.0f, -400.0f, 400.0f);
glm::mat4 normalMatrix = glm::inverseTranspose(model);

LRESULT CALLBACK WinPro(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//重定向输出日志
	//freopen("main.log", "w", stdout);

	//初始化参数
	int width = 1280;
	int height = 800;


	//注册窗口类
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinPro;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"OpenGL";
	wc.hIconSm = 0;
	DWORD res = RegisterClassEx(&wc);
	if (res == 0)
	{
		DWORD err = GetLastError();
	}

	//计算窗口大小和位置
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	RECT rect
	{
		(screenW - width) / 2,
		(screenH - height) / 2,
		rect.left + width,
		rect.top + height
	};

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);


	//创建窗口并显示
	HWND hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"Demo", WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	if (hWnd == 0)
	{
		DWORD err = GetLastError();
	}

	//创建OpenGL环境
	HDC dc = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	int pixelformat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelformat, &pfd);
	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	//初始化Glew
	glewInit();

	//编译链接GPU Program
	GLuint program = CreateGPUProgram("../res/shader/MixLight.vs", "../res/shader/MixLight.fs");

	//计算参数位置
	GLuint posLocation, texcoordLocation, normalLocation, MLocation, VLocation, PLocation, NMLocation, Texture2DLocation;
	posLocation = glGetAttribLocation(program, "pos");
	texcoordLocation = glGetAttribLocation(program, "texcoord");
	normalLocation = glGetAttribLocation(program, "normal");

	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	NMLocation = glGetUniformLocation(program, "NM");
	Texture2DLocation = glGetUniformLocation(program, "U_MainTexture");
	
	//Subroutine
	GLuint surfaceColorLocation;
	surfaceColorLocation = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "U_SurfaceColor");

	GLuint ambientLocation = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "Ambient");
	GLuint diffuseLocation = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "Diffuse");
	GLuint specularLocation = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "Specular");

	//加载Obj文件
	Vertex* pVertex;
	unsigned int vertexSize;
	unsigned int* pIndex;
	unsigned int indexSize;

	LoadObjModel("../res/model/niutou.obj", &pVertex, vertexSize, &pIndex, indexSize);

	//创建缓冲区对象
	GLuint vbo = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexSize, GL_STATIC_DRAW, pVertex);
	GLuint ibo = CreateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexSize, GL_STATIC_DRAW, pIndex);
	GLuint texture = CreateTextureFromFile("../res/image/niutou.bmp");

	//VAO
	GLuint vao = CreateGLVertexArrayObject([&]()->void
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 5));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});

	//FBO
	GLuint colorBuffer, depthBuffer;
	GLuint fbo = CreateFrameBufferObject(colorBuffer, depthBuffer, width, height);


	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//定义绘制Lambert表达式
	auto DrawSomething = [&]()->void
	{
		//调用GPU Program
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(NMLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(Texture2DLocation, 0);

		glBindVertexArray(vao);

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &diffuseLocation);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);
		glUseProgram(0);
	};

	//显示和更新窗口
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//消息循环
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawSomething();

		glFlush();
		SwapBuffers(dc);
	}

	//glDeleteProgram(program);

	//fclose(stdout);
	return 0;
}

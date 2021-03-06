#include <stdio.h>
#include <iostream>
#include <windows.h>

//#include <gl/GL.h>
//#pragma comment(lib, "opengl32.lib")

#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Misc.h"
#include "Model.h"

#include "Timer.h"
#include "Frustum.h"

struct FloatBundle
{
	float v[4];
};

//初始化参数
int width = 1280;
int height = 800;

float identity[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

HGLRC CreateMSAARC(HDC hdc)
{
	HGLRC rc;

	GLint attribs [] = 
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_RED_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 16,
		NULL, NULL
	};

	int format[256] = {0};
	UINT formatSize = 0;

	wglChoosePixelFormatARB(hdc, attribs, nullptr, 256, format, &formatSize);
	if (formatSize > 0)
	{
		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(hdc, format[0], sizeof(pfd), &pfd);
		SetPixelFormat(hdc, format[0], &pfd);

		int contexAttribs [] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			NULL
		};
		rc = wglCreateContextAttribsARB(hdc, nullptr, contexAttribs);
	}
	
	return rc;
}

LRESULT CALLBACK WinPro(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
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

	//开启MSAA,多重采样抗锯齿
	if (wglChoosePixelFormatARB)
	{
		wglMakeCurrent(dc, nullptr);
		wglDeleteContext(rc);
		rc = nullptr;
		ReleaseDC(hWnd, dc);
		dc = nullptr;
		DestroyWindow(hWnd);
		hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"Demo", WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
		dc = GetDC(hWnd);
		rc = CreateMSAARC(dc);
		wglMakeCurrent(dc, rc);
	}

	bool bEmitter = false;

// Emitter Program ----------------------------------------------------------------------

	// 设置 Emitter 位置
	FloatBundle vertex[1];
	vertex[0].v[0] = 0.0f;
	vertex[0].v[1] = -2.0f;
	vertex[0].v[2] = 0.0f;
	vertex[0].v[3] = 1.0f;

	// 创建 Emitter In Buffer
	GLuint emitterTFOInBuffer = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(FloatBundle) * 1, GL_STATIC_DRAW, vertex);

	// 创建 Emitter Out Buffer 和 Emitter TFO
	GLuint emitterTFOOutBuffer = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(FloatBundle) * 1, GL_STATIC_DRAW, nullptr);
	GLuint emitterTFO = CreateTFO(emitterTFOOutBuffer);

	// 创建 Emitter Program
	const char* emitterTFOAttribs[] = { "gl_Position" };
	GLuint emitterTFOProgram = CreateTFOProgram("../res/shader/Emitter.vs", 1, emitterTFOAttribs, GL_INTERLEAVED_ATTRIBS);

	// 获取 Emitter Program 参数位置
	GLuint emitterTFOProgramPosLocation = glGetAttribLocation(emitterTFOProgram, "pos");
	GLuint mLocation = glGetUniformLocation(emitterTFOProgram, "M");

	// 实现 Emitter Program 调用过程
	auto CallTFOProcess = [&]()->void
	{
		glEnable(GL_RASTERIZER_DISCARD);
		glUseProgram(emitterTFOProgram);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, emitterTFO);
		glBeginTransformFeedback(GL_POINTS);

		glBindBuffer(GL_ARRAY_BUFFER, emitterTFOInBuffer);
		glEnableVertexAttribArray(emitterTFOProgramPosLocation);
		glVertexAttribPointer(emitterTFOProgramPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_POINTS, 0, 1);

		glEndTransformFeedback();
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

		glUseProgram(0);
		glDisable(GL_RASTERIZER_DISCARD);

		bEmitter = true;
	};
	
	// 调用 Emitter Program
	CallTFOProcess();

	// 查看 Emitter Out Buffer 中的内容
	glBindBuffer(GL_ARRAY_BUFFER, emitterTFOOutBuffer);
	FloatBundle* pDataForPrint = (FloatBundle*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	printf("%f,%f,%f,%f\n", pDataForPrint[0].v[0], pDataForPrint[0].v[1], pDataForPrint[0].v[2], pDataForPrint[0].v[3]);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// Update Program ----------------------------------------------------------------------

	// 创建 UpdateTFO Buffer 和 UpdateTFO
	GLuint updateTFOBuffer[2];
	GLuint updateTFO[2];
	for (int i = 0; i < 2; ++i)
	{
		updateTFOBuffer[i] = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(FloatBundle) * 10240, GL_STATIC_DRAW, nullptr);
		updateTFO[i] = CreateTFO(updateTFOBuffer[i]);
	}
	// 创建 UpdateTFO Program
	const char* updateTFOAttribs[] = { "gl_Position" };
	GLuint updateTFOProgram = CreateTFOProgram("../res/shader/TFO.vs", 1, updateTFOAttribs, GL_INTERLEAVED_ATTRIBS);

	// 获取 UpdateTFO Program 参数位置
	GLuint updateTFOProgramPosLocation = glGetAttribLocation(updateTFOProgram, "pos");

	// 实现 UpdateTFO Program 调用过程 
	int currentTFOIndex = 0;
	int currentTFOForDrawIndex = 0;
	auto UpdateTFOProcess = [&]()->void
	{
		glEnable(GL_RASTERIZER_DISCARD);
		glUseProgram(updateTFOProgram);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, updateTFO[currentTFOIndex]);
		glBeginTransformFeedback(GL_POINTS);

		if (bEmitter)
		{
			bEmitter = false;

			glBindBuffer(GL_ARRAY_BUFFER, emitterTFOOutBuffer);
			glEnableVertexAttribArray(updateTFOProgramPosLocation);
			glVertexAttribPointer(updateTFOProgramPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawTransformFeedback(GL_POINTS, emitterTFO);
		}

		glBindBuffer(GL_ARRAY_BUFFER, updateTFOBuffer[currentTFOForDrawIndex]);
		glEnableVertexAttribArray(updateTFOProgramPosLocation);
		glVertexAttribPointer(updateTFOProgramPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawTransformFeedback(GL_POINTS, updateTFO[currentTFOForDrawIndex]);

		glEndTransformFeedback();
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

		glUseProgram(0);
		glDisable(GL_RASTERIZER_DISCARD);

		currentTFOForDrawIndex = currentTFOIndex;

		//查看 UpdateTFO Buffer 输出内容
		glBindBuffer(GL_ARRAY_BUFFER, updateTFOBuffer[currentTFOForDrawIndex]);
		FloatBundle* pDataForDraw = (FloatBundle*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		printf("%f,%f,%f,%f\n", pDataForDraw[0].v[0], pDataForDraw[0].v[1], pDataForDraw[0].v[2], pDataForDraw[0].v[3]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		currentTFOIndex = (currentTFOIndex + 1) % 2;
	};

// Draw Program ----------------------------------------------------------------------

	//编译链接GPU Program
	GLuint program = CreateGPUProgram("../res/shader/SimplePV.vs", "../res/shader/SimplePV.fs");

	//计算参数位置
	GLuint worldPosLocation;
	worldPosLocation = glGetAttribLocation(program, "pos");

	GLuint  PLocation, VLocation;
	PLocation = glGetUniformLocation(program, "P");
	VLocation = glGetUniformLocation(program, "V");

	//纹理
	GLuint textureLocation = glGetUniformLocation(program, "U_MainTexture");

	GLuint texture = CreateAlphaTexture(256, 256);
	
	//定义绘制Lambert表达式
	auto DrawProcess = [&]()->void
	{
		//调用GPU Program
		glUseProgram(program);

		glBindBuffer(GL_ARRAY_BUFFER, updateTFOBuffer[currentTFOForDrawIndex]);
		glEnableVertexAttribArray(worldPosLocation);
		glVertexAttribPointer(worldPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1d(textureLocation, 0);
		
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);

		glDrawArrays(GL_POINTS, 0, 1);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	};

	//初始状态
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);

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

		glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		UpdateTFOProcess();
		DrawProcess();
		  
		glFlush();
		SwapBuffers(dc);
	}

	glDeleteProgram(program);
	glDeleteProgram(emitterTFOProgram);
	return 0;
}

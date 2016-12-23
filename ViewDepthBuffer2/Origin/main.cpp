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

//��ʼ������
int width = 1280;
int height = 800;

float identity[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f)) * glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
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
	//ע�ᴰ����
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

	//���㴰�ڴ�С��λ��
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
	//�������ڲ���ʾ
	HWND hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"Demo", WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	if (hWnd == 0)
	{
		DWORD err = GetLastError();
	}

	//����OpenGL����
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

	//��ʼ��Glew
	glewInit();


/************************************************************************/
/*	Obj                                                                 */

	//��������GPU Program
	GLuint program = CreateGPUProgram("../res/shader/Simple.vs", "../res/shader/Simple.fs");

	//�������λ��
	GLuint posLocation, MLocation, VLocation, PLocation;
	posLocation = glGetAttribLocation(program, "pos");

	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	
	//����Obj�ļ�
	Vertex* pVertex;
	unsigned int vertexSize;
	unsigned int* pIndex;
	unsigned int indexSize;

	LoadObjModel("../res/model/niutou.obj", &pVertex, vertexSize, &pIndex, indexSize);

	//��������������
	GLuint vbo = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexSize, GL_STATIC_DRAW, pVertex);
	GLuint ibo = CreateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexSize, GL_STATIC_DRAW, pIndex);
	
	//VAO
	GLuint vao = CreateGLVertexArrayObject([&]()->void
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});

	//FBO
	GLuint colorBuffer, depthBuffer;
	GLuint fbo = CreateFrameBufferObject(colorBuffer, depthBuffer, width, height);

	//�������Lambert���ʽ
	auto DrawSomething = [&]()->void
	{
		//����GPU Program
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);

		glUseProgram(0);
	};
/************************************************************************/


/************************************************************************/
/*	full		                                                            */

//��������GPU Program
	GLuint fullProgram = CreateGPUProgram("../res/shader/fsq.vs", "../res/shader/fsq.fs");

	//�������λ��
	GLuint fullPosLocation, fullTexcoordLocation;
	fullPosLocation = glGetAttribLocation(fullProgram, "pos");
	fullTexcoordLocation = glGetAttribLocation(fullProgram, "texcoord");

	GLuint fullMainTextureLocation;
	fullMainTextureLocation = glGetUniformLocation(fullProgram, "U_MainTexture");

	//����Obj�ļ�
	Vertex* pFullVertex;
	unsigned int fullVertexSize;
	unsigned int* pFullIndex;
	unsigned int fullIndexSize;

	LoadObjModel("../res/model/Quad.obj", &pFullVertex, fullVertexSize, &pFullIndex, fullIndexSize);

	//��������������
	GLuint fullVBO = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * fullVertexSize, GL_STATIC_DRAW, pFullVertex);
	GLuint fullIBO = CreateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * fullIndexSize, GL_STATIC_DRAW, pFullIndex);

	//���ز�������
	GLuint fullTexture = CreateTextureFromFile("../res/image/niutou.bmp");

	//VAO
	GLuint fullVAO = CreateGLVertexArrayObject([&]()->void
	{
		glBindBuffer(GL_ARRAY_BUFFER, fullVBO);
		glEnableVertexAttribArray(fullPosLocation);
		glVertexAttribPointer(fullPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glEnableVertexAttribArray(fullTexcoordLocation);
		glVertexAttribPointer(fullTexcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});

	//�������Lambert���ʽ
	auto DrawFullScreenQuad = [&]()->void
	{
		//����GPU Program
		glUseProgram(fullProgram);

		glBindTexture(GL_TEXTURE_2D, depthBuffer);
		glUniform1i(fullMainTextureLocation, 0);

		glBindVertexArray(fullVAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fullIBO);
		glDrawElements(GL_TRIANGLES, fullIndexSize, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glUseProgram(0);
	};
/************************************************************************/


	glEnable(GL_DEPTH_TEST);

	//��ʾ�͸��´���
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//��Ϣѭ��
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

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.4f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawSomething();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.4f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawFullScreenQuad();

		glFlush();
		SwapBuffers(dc);
	}

	glDeleteProgram(program);
	return 0;
}

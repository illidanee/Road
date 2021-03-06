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


float identity[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

glm::mat4 projection = glm::perspective(45.0f, 1280.0f / 800.0f, 0.1f, 1000.0f);

struct Vertex
{
	float pos[3];
	float color[4];
};

const char* LoadFileContent(const char* path)
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

GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath)
{
	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vsCode = LoadFileContent(vsShaderPath);
	const char* fsCode = LoadFileContent(fsShaderPath);
	glShaderSource(vsShader, 1, &vsCode, 0);
	glShaderSource(fsShader, 1, &fsCode, 0);
	glCompileShader(vsShader);
	glCompileShader(fsShader);
	GLuint pragram = glCreateProgram();
	glAttachShader(pragram, vsShader);
	glAttachShader(pragram, fsShader);
	glLinkProgram(pragram);
	glDetachShader(pragram, vsShader);
	glDetachShader(pragram, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	return pragram;
}


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
	freopen("main.log", "w", stdout);

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
	GLuint program = CreateGPUProgram("sample.vs", "sample.fs");

	//计算参数位置
	GLuint posLocation, colorLocation, MLocation, VLocation, PLocation;
	posLocation = glGetAttribLocation(program, "pos");
	colorLocation = glGetAttribLocation(program, "color");
	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");

	//上传顶点数据到GPU
	Vertex vertex[3];
	vertex[0].pos[0] = 0.0f;
	vertex[0].pos[1] = 0.0f;
	vertex[0].pos[2] = -100.0f;
	vertex[0].color[0] = 1.0f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 1.0f;
	vertex[0].color[3] = 1.0f;

	vertex[1].pos[0] = 10.0f;
	vertex[1].pos[1] = 0.0f;
	vertex[1].pos[2] = -100.0f;
	vertex[1].color[0] = 1.0f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 1.0f;
	vertex[1].color[3] = 1.0f;

	vertex[2].pos[0] = 0.0f;
	vertex[2].pos[1] = 10.0f;
	vertex[2].pos[2] = -100.0f;
	vertex[2].color[0] = 1.0f;
	vertex[2].color[1] = 1.0f;
	vertex[2].color[2] = 1.0f;
	vertex[2].color[3] = 1.0f;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 100, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 3, vertex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int indexes[] = { 0, 1, 2 };
	GLuint vio;
	glGenBuffers(1, &vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, indexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);

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

		glClear(GL_COLOR_BUFFER_BIT);

		//调用GPU Program
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
		glEnableVertexAttribArray(colorLocation);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glUseProgram(0);

		SwapBuffers(dc);
	}

	fclose(stdout);
	return 0;
}

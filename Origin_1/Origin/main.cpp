#include <stdio.h>
#include <windows.h>
#include <iostream>

LRESULT CALLBACK WinPro(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//�ض��������־
	freopen("main.log", "w", stdout);

	//��ʼ������
	int width = 1280;
	int height = 800;


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

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);


	//��Ϣѭ��
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	fclose(stdout);
	return 0;
}

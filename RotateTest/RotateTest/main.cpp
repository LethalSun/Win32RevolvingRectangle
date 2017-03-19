#include <windows.h>
#include <time.h>
#include "Clock.h"
#include <math.h>
#include <atlimage.h>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

const double pi = 3.1415926535;
int count = 0;
int count1 = 0;
int ratio = 0;

int	 mouseX = 0;
int	 mouseY = 0;
LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam);

void DrawBoxByPolygon(HDC
	,int anchorX
	,int anchorY
	,int dx
	,int dy);

double RotateX(int x, int y, double rad);
double RotateY(int x, int y, double rad);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
		return 0;

	HWND	hWnd = CreateWindowEx(WS_EX_APPWINDOW
		, szWindowClass
		, szTitle
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, 800
		, 600
		, NULL
		, NULL
		, hInstance
		, NULL);

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow);

	//TODO: //NOT timer init
	Clock timer;
	if (timer.Init() == false)
	{
		MessageBoxA(hWnd, "timer problem!!", "error!", MB_OK);
		return 0;
	}
	float time = 0.f;

	//TODO: //NOT getcurrentdirectory
	WCHAR path[256];
	GetCurrentDirectory(sizeof(path), path);

	//TODO: //NOT HDC init
	HDC hDC = GetDC(hWnd);
	HDC memoryDC = CreateCompatibleDC(hDC);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(memoryDC,800,600);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC,memoryBitmap);

	//TODO: //NOT message init
	MSG			msg;
	//deltatime
	float dt;

	//
	WCHAR temp[256];
	const WCHAR* PartialBG = TEXT("/whiteBackground.png");
	wcscpy_s(temp, path);
	wcscat_s(temp, PartialBG);

	CImage backgroundImg;
	backgroundImg.Load(temp);

	while (1)
	{
		timer.ProcessTime();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			if (timer.isOneFrame(&dt))
			{
				backgroundImg.BitBlt(memoryDC, 0, 0, 800, 600, 0, 0, SRCCOPY);
				DrawBoxByPolygon(memoryDC, 400, 300, 100, 100);
				DrawBoxByPolygon(memoryDC, mouseX, mouseY, 100, 100);
				BitBlt(hDC, 0, 0, 800, 600, memoryDC, 0, 0, SRCCOPY);
			}
		}

	}

	SelectObject(memoryDC, oldBitmap);
	DeleteObject(memoryBitmap);
	DeleteDC(memoryDC);
	ReleaseDC(hWnd, hDC);

	return (int)msg.wParam;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE:

		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_MOUSEMOVE:
	{
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void DrawBoxByPolygon(HDC pHDC
	, int anchorX
	, int anchorY
	, int dx
	, int dy)
{
	//회전 비율
	count = (count++) % 500;

	ratio = count + 1;
	double rad = (2 * pi * ratio) / 500;

	//기준점으로부터 회전을 그림
	int x = anchorX + (int)RotateX(-dx,dy,rad);
	int y = anchorY + (int)RotateY(-dx, dy,rad);
	int x1 = anchorX + (int)RotateX(dx, dy,rad);
	int y1 = anchorY + (int)RotateY(dx, dy,rad);
	int x2 = anchorX + (int)RotateX(dx, -dy,rad);
	int y2 = anchorY + (int)RotateY(dx, -dy,rad);
	int x3 = anchorX + (int)RotateX(-dx, -dy,rad);
	int y3 = anchorY + (int)RotateY(-dx, -dy,rad);

	MoveToEx(pHDC, x, y, nullptr);
	LineTo(pHDC, x1, y1);
	LineTo(pHDC, x2, y2);
	LineTo(pHDC, x3, y3);
	LineTo(pHDC, x, y);

}

double RotateX(int  x, int  y, double rad)
{
	auto xr = (x*cos(rad) + y*sin(rad));

	return xr;
}

double RotateY(int x, int y, double rad)
{
	auto yr = (y*cos(rad) - x*sin(rad));

	return yr;
}





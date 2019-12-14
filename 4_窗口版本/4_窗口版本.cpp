// 4_窗口版本.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "4_窗口版本.h"
#include "CGame.h"
#include "define.h"

#define MAX_LOADSTRING 100

// 全局变量:
CGame *game;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY4));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//  目标: 注册窗口类。
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//   目标: 保存实例句柄并创建主窗口
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      20,20,WND_WIDE, WND_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//  目标: 处理主窗口的消息。
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool if_select_first = false, if_select_second = false;
	static POINT first = { 0, 0 }, second = { 0, 0 };  //鼠标选择的点
    switch (message)
    {
	case WM_CREATE:
		{
			game = new CGame;
			//重新开始游戏按钮
			HWND restart_button_hwnd = CreateWindow(TEXT("Button"), TEXT("重新开始"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT, FRAME_GAP_WIDE * 2 + BLOCK_WIDE * 5, FRAME_GAP_HEIGHT + BLOCK_WIDE * 10 + BLOCK_WIDE * 2 / 5 , BLOCK_WIDE, BLOCK_WIDE * 3 / 5, hWnd, (HMENU)1, hInst, NULL);
			break;
		}
	case WM_COMMAND:
		{
			WORD wmId = LOWORD(wParam);
			switch (wmId)
			{
			case 1:  //重新开始游戏按钮事件
				if_select_first = false;
				if_select_second = false;
				game->ReGame();
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			break;
		}
	case WM_RBUTTONDOWN:
		if_select_first = false;
		if_select_second = false;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			x -= FRAME_GAP_WIDE * 2;
			y -= FRAME_GAP_HEIGHT;
			int zx = x / BLOCK_WIDE;
			int zy = 9 - y / BLOCK_WIDE;
			if (!if_select_first && zx >= 0 && zx < 10 && zy >= 0 && zy < 10)
			{
				first.x = zx;
				first.y = zy;
				if_select_first = true;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (!if_select_second && zx >= 0 && zx < 10 && zy >= 0 && zy < 10)
			{
				second.x = zx;
				second.y = zy;
				if_select_second = true;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			if (if_select_first && if_select_second)
			{
				if (first.x == second.x && first.y - second.y == -1)
				{
					game->Change(first.x, first.y, 'u');
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else if (first.x == second.x && first.y - second.y == 1)
				{
					game->Change(first.x, first.y, 'd');
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else if (first.x - second.x == -1 && first.y == second.y)
				{
					game->Change(first.x, first.y, 'r');
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else if (first.x - second.x == 1 && first.y == second.y)
				{
					game->Change(first.x, first.y, 'l');
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if_select_first = false;
				if_select_second = false;
			}
			break;
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			game->Print(hdc);
			if (if_select_first)  //绘制选择
			{
				HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
				SelectObject(hdc, brush);
				Rectangle(hdc, FRAME_GAP_WIDE * 2 + BLOCK_WIDE * first.x + BLOCK_WIDE / 2.5,
					FRAME_GAP_HEIGHT + BLOCK_WIDE * (9 - first.y) + BLOCK_WIDE / 2.5,
					FRAME_GAP_WIDE * 2 + BLOCK_WIDE * (first.x + 1) - BLOCK_WIDE / 2.5,
					FRAME_GAP_HEIGHT + BLOCK_WIDE * ((9 - first.y) + 1) - BLOCK_WIDE / 2.5
				);
				DeleteObject(brush);
			}
			if (if_select_second)  //绘制选择
			{
				HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
				SelectObject(hdc, brush);
				Rectangle(hdc, FRAME_GAP_WIDE * 2 + BLOCK_WIDE * second.x + BLOCK_WIDE / 2.5,
					FRAME_GAP_HEIGHT + BLOCK_WIDE * (9 - second.y) + BLOCK_WIDE / 2.5,
					FRAME_GAP_WIDE * 2 + BLOCK_WIDE * (second.x + 1) - BLOCK_WIDE / 2.5,
					FRAME_GAP_HEIGHT + BLOCK_WIDE * ((9 - second.y) + 1) - BLOCK_WIDE / 2.5
				);
				DeleteObject(brush);
			}
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		delete game;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
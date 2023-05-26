#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <CommCtrl.h>
#include <time.h>
#include "resource.h"

// 用户自定义消息
const UINT WM_USERMSG = WM_USER + 1;

void print(LPCWSTR format, ...) {

	WCHAR wchar_buff[100]{ 0 };

	va_list arglist;

	// 从哪里开始
	va_start(arglist, format);

	// 打印到缓冲区
	wvsprintfW(wchar_buff, format, arglist);

	// 关闭指针
	va_end(arglist);

	// 在debug窗口输出
	OutputDebugStringW(wchar_buff);

}

BOOL CALLBACK EnumChildProc(
	_In_ HWND hwnd, 
	_In_ LPARAM lParam
) {
	
	WCHAR buff[1024]{ 0 };
	GetWindowTextW(hwnd, buff, 100);
	print(L"子窗口：%s\n", buff);
	return true;
}

LRESULT CALLBACK WindowProc(
	_In_ HWND hwnd,      // 窗口句柄
	_In_ UINT uMsg,      // 消息类型
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {

	WORD X = { 0 };
	WORD Y = { 0 };

	// 获取父窗体大小
	RECT rect = { 0 };
	GetClientRect(hwnd, &rect);

	// 获取当前程序的实例句柄
	static HINSTANCE hInstance = GetModuleHandleW(NULL);

	switch (uMsg) {
	case WM_CREATE:
		/*MessageBoxW(hwnd, L"窗口创建了", L"提示", MB_OK);*/
		// 添加控件
		//CreateWindowExW(0L, "Button", L"按钮1", 0x40000 0 00L | 0x10000000L, 10, 10, 50, 20, hnd,
		CreateWindowA(WC_BUTTON,"获取文本内容", WS_CHILD|WS_VISIBLE, 
			140, 10, 120, 40, hwnd, (HMENU)0x100, hInstance, 0);
		CreateWindowA(WC_BUTTON, "设置文本内容", WS_CHILD | WS_VISIBLE,
			270, 10, 120, 40, hwnd, (HMENU)0x102, hInstance, 0);
		CreateWindowA(WC_BUTTON, "移动按钮", WS_CHILD | WS_VISIBLE,
			10, 120, 80, 40, hwnd, (HMENU)0x101, hInstance, 0);
		CreateWindowA(WC_BUTTON, "点我呀 ^_^ ", WS_CHILD | WS_VISIBLE,
			100, 120, 80, 40, hwnd, (HMENU)0x104, hInstance, 0);
		CreateWindowA(WC_BUTTON, "移动父窗口", WS_CHILD | WS_VISIBLE,
			190, 120, 80, 40, hwnd, (HMENU)0x103, hInstance, 0);
		CreateWindowA(WC_BUTTON, "枚举子窗口", WS_CHILD | WS_VISIBLE,
			280, 120, 80, 40, hwnd, (HMENU)0x105, hInstance, 0);
		CreateWindowA(WC_BUTTON, "更换ICON", WS_CHILD | WS_VISIBLE,
			370, 120, 80, 40, hwnd, (HMENU)0x106, hInstance, 0);
		CreateWindowA(WC_EDIT, "文本框内容", WS_CHILD | WS_BORDER | WS_VISIBLE,
			10, 10, 120, 40, hwnd, (HMENU)0x200, hInstance, 0);

		break;
	case WM_CLOSE:
		MessageBoxW(hwnd, L"窗口关闭了", L"提示", MB_OK);
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		
		// lParam存放鼠标坐标，高字节
		X = LOWORD(lParam);
		Y = HIWORD(lParam);
		print(L"鼠标移动了 X:%d Y:%d\n", X, Y);
		break;
	case WM_LBUTTONDOWN:
		print(L"按下鼠标左键\n ");

		// 发送消息
		SendMessage(hwnd, WM_USERMSG, 0, 0);
		break;

	case WM_RBUTTONDOWN: {
		print(L"按下鼠标右键\n ");

		/*X = LOWORD(lParam);
		Y = HIWORD(lParam);*/
		POINT point{ 0 };
		/*point.x = X;
		point.y = Y;*/
		//ClientToScreen(hwnd, &point);
		GetCursorPos(&point);
		HMENU hMenu = LoadMenuW(hInstance, (LPCWSTR)IDR_MENU1);

		// 获取子菜单 
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, point.x, point.y, 0, hwnd, NULL);
		break;
	}
	case WM_USERMSG:
		print(L"自定义消息\n");
		break;
	case WM_COMMAND:
	{
		// 控件id
		WORD ctrlId = LOWORD(wParam);

	    // 获取文本内容
		if (ctrlId == 0x100) {

			// 获取输入框的句柄
			HWND hEdit = GetDlgItem(hwnd, 0x200);
			
			WCHAR buff[100]{ 100 };
			GetWindowTextW(hEdit, buff, 100);
			MessageBoxW(hwnd, buff, L"提示", MB_OK);
		}

	    // 移动按钮
		else if (ctrlId == 0x101) {
			MoveWindow((HWND)lParam, 
				rand() % (rect.right - 120), 
				rand() % (rect.bottom - 40), 
				120, 40, true);
		}

		// 设置文本内容
		else if (ctrlId == 0x102) {

			// 获取输入框的句柄
			HWND hEdit = GetDlgItem(hwnd, 0x200);
			SetWindowTextW(hEdit, L"我是孙悟空");
		}

		// 设置父窗体
		else if (ctrlId == 0x103) {

			// 获取桌面大小
			HWND deskHWND = GetDesktopWindow();
			RECT deskRect = { 0 };
			GetWindowRect(deskHWND, &deskRect);
			
			SetWindowPos(hwnd, NULL, 
				rand() % (deskRect.right - rect.right),
				rand() % (deskRect.bottom - rect.bottom),
				800, 600, true);
		}

		// 点我呀
		else if (ctrlId == 0x104) {
			MessageBoxW(hwnd, L"恭喜您点到我了", L"胜利", MB_OK);
		}
		else if (ctrlId == 0x105) {
			EnumChildWindows(hwnd, EnumChildProc, NULL);
		}
		else if (ctrlId == 0x106) {
			HICON hIcon = LoadIcon(hInstance, (LPCSTR)(IDI_ICON1));
			SetClassLongPtrW(hwnd, -14, (LONG)hIcon);
		}

		// 菜单 退出
		else if (ctrlId == ID_40002) {
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		
		break;
	}
	case WM_MOUSEHOVER:
	{
		print(L"鼠标划过...");

		// 控件id
		WORD ctrlId = LOWORD(wParam);
		if (ctrlId == 0x104) {
			print(L"鼠标划过控件%d", ctrlId);
		}
		break;
	}
	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

int WINAPI WinMain (
	HINSTANCE hInstance,     // 程序实例句柄
	HINSTANCE hPreInstance,  // 上一个程序的实例句柄
	LPSTR     lpCmdLine,     // char * arg[] 命令行参数
	int       mCmdShow       // 显示方式

) 
//int main()
{
	// 字符串处理
	/*CHAR char_buff[] = "123456";
	WCHAR wchar_buff[] =  L"123456";
	TCHAR tchar_buff[] = _T("123456");

	printf("sizeof char_buff = %d\n", sizeof(char_buff));
	printf("sizeof wchar_buff = %d\n", sizeof(wchar_buff));
	printf("sizeof tchar_buff = %d\n", sizeof(tchar_buff));

	printf("length char_buff = %d\n", strlen(char_buff));
	printf("length wchar_buff = %d\n", wcslen(wchar_buff));
	printf("length tchar_buff = %d\n", strlen(tchar_buff));*/

	//MessageBoxA(NULL, "Hello World", "Title", MB_OK);

	// 1. 创建一个窗口类
	WNDCLASSW myClass = { 0 };
	myClass.lpszClassName = L"第一个窗口";
	myClass.lpfnWndProc = WindowProc;
	myClass.hIcon = LoadIcon(0, (LPCSTR)IDI_SHIELD); // 0代表系统资源
	myClass.hCursor = LoadCursorA(0, (LPCSTR)IDC_IBEAM); // 换光标
	//myClass.lpszMenuName = (LPCWSTR)IDR_MENU1; 

	// 设置背景画刷
	myClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

	// 2. 注册窗口类
	RegisterClassW(&myClass);

	// 3. 创建窗口
	HMENU hMenu = LoadMenuW(hInstance, (LPCWSTR)IDR_MENU1);
	HWND hwindow = CreateWindowW(
		myClass.lpszClassName,    // 窗口类名
		L"一个窗口",               // 窗口名
		WS_OVERLAPPEDWINDOW,      // 窗口样式
		CW_USEDEFAULT,            // x
		0,                        // y
		CW_USEDEFAULT,            // width
		0,                        // height
		NULL,                     // 父窗口
		hMenu,                     // 菜单
		hInstance,                // hInstance
		0
	);

	// 4. 显示窗口
	ShowWindow(hwindow, SW_SHOWNORMAL);

	// 5. 获取消息
	MSG msg = { 0 };
	while (GetMessageW(&msg, 0, 0, 0)) {

		TranslateMessage(&msg); // 转成 WM_CHAR消息，否则编辑框不能输入

		DispatchMessageW(&msg); // 分发消息给窗口处理函数
	}

	return 0;
}
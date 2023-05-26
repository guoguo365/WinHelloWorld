#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <CommCtrl.h>
#include <time.h>
#include "resource.h"

// �û��Զ�����Ϣ
const UINT WM_USERMSG = WM_USER + 1;

void print(LPCWSTR format, ...) {

	WCHAR wchar_buff[100]{ 0 };

	va_list arglist;

	// �����￪ʼ
	va_start(arglist, format);

	// ��ӡ��������
	wvsprintfW(wchar_buff, format, arglist);

	// �ر�ָ��
	va_end(arglist);

	// ��debug�������
	OutputDebugStringW(wchar_buff);

}

BOOL CALLBACK EnumChildProc(
	_In_ HWND hwnd, 
	_In_ LPARAM lParam
) {
	
	WCHAR buff[1024]{ 0 };
	GetWindowTextW(hwnd, buff, 100);
	print(L"�Ӵ��ڣ�%s\n", buff);
	return true;
}

LRESULT CALLBACK WindowProc(
	_In_ HWND hwnd,      // ���ھ��
	_In_ UINT uMsg,      // ��Ϣ����
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {

	WORD X = { 0 };
	WORD Y = { 0 };

	// ��ȡ�������С
	RECT rect = { 0 };
	GetClientRect(hwnd, &rect);

	// ��ȡ��ǰ�����ʵ�����
	static HINSTANCE hInstance = GetModuleHandleW(NULL);

	switch (uMsg) {
	case WM_CREATE:
		/*MessageBoxW(hwnd, L"���ڴ�����", L"��ʾ", MB_OK);*/
		// ��ӿؼ�
		//CreateWindowExW(0L, "Button", L"��ť1", 0x40000 0 00L | 0x10000000L, 10, 10, 50, 20, hnd,
		CreateWindowA(WC_BUTTON,"��ȡ�ı�����", WS_CHILD|WS_VISIBLE, 
			140, 10, 120, 40, hwnd, (HMENU)0x100, hInstance, 0);
		CreateWindowA(WC_BUTTON, "�����ı�����", WS_CHILD | WS_VISIBLE,
			270, 10, 120, 40, hwnd, (HMENU)0x102, hInstance, 0);
		CreateWindowA(WC_BUTTON, "�ƶ���ť", WS_CHILD | WS_VISIBLE,
			10, 120, 80, 40, hwnd, (HMENU)0x101, hInstance, 0);
		CreateWindowA(WC_BUTTON, "����ѽ ^_^ ", WS_CHILD | WS_VISIBLE,
			100, 120, 80, 40, hwnd, (HMENU)0x104, hInstance, 0);
		CreateWindowA(WC_BUTTON, "�ƶ�������", WS_CHILD | WS_VISIBLE,
			190, 120, 80, 40, hwnd, (HMENU)0x103, hInstance, 0);
		CreateWindowA(WC_BUTTON, "ö���Ӵ���", WS_CHILD | WS_VISIBLE,
			280, 120, 80, 40, hwnd, (HMENU)0x105, hInstance, 0);
		CreateWindowA(WC_BUTTON, "����ICON", WS_CHILD | WS_VISIBLE,
			370, 120, 80, 40, hwnd, (HMENU)0x106, hInstance, 0);
		CreateWindowA(WC_EDIT, "�ı�������", WS_CHILD | WS_BORDER | WS_VISIBLE,
			10, 10, 120, 40, hwnd, (HMENU)0x200, hInstance, 0);

		break;
	case WM_CLOSE:
		MessageBoxW(hwnd, L"���ڹر���", L"��ʾ", MB_OK);
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		
		// lParam���������꣬���ֽ�
		X = LOWORD(lParam);
		Y = HIWORD(lParam);
		print(L"����ƶ��� X:%d Y:%d\n", X, Y);
		break;
	case WM_LBUTTONDOWN:
		print(L"����������\n ");

		// ������Ϣ
		SendMessage(hwnd, WM_USERMSG, 0, 0);
		break;

	case WM_RBUTTONDOWN: {
		print(L"��������Ҽ�\n ");

		/*X = LOWORD(lParam);
		Y = HIWORD(lParam);*/
		POINT point{ 0 };
		/*point.x = X;
		point.y = Y;*/
		//ClientToScreen(hwnd, &point);
		GetCursorPos(&point);
		HMENU hMenu = LoadMenuW(hInstance, (LPCWSTR)IDR_MENU1);

		// ��ȡ�Ӳ˵� 
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, point.x, point.y, 0, hwnd, NULL);
		break;
	}
	case WM_USERMSG:
		print(L"�Զ�����Ϣ\n");
		break;
	case WM_COMMAND:
	{
		// �ؼ�id
		WORD ctrlId = LOWORD(wParam);

	    // ��ȡ�ı�����
		if (ctrlId == 0x100) {

			// ��ȡ�����ľ��
			HWND hEdit = GetDlgItem(hwnd, 0x200);
			
			WCHAR buff[100]{ 100 };
			GetWindowTextW(hEdit, buff, 100);
			MessageBoxW(hwnd, buff, L"��ʾ", MB_OK);
		}

	    // �ƶ���ť
		else if (ctrlId == 0x101) {
			MoveWindow((HWND)lParam, 
				rand() % (rect.right - 120), 
				rand() % (rect.bottom - 40), 
				120, 40, true);
		}

		// �����ı�����
		else if (ctrlId == 0x102) {

			// ��ȡ�����ľ��
			HWND hEdit = GetDlgItem(hwnd, 0x200);
			SetWindowTextW(hEdit, L"���������");
		}

		// ���ø�����
		else if (ctrlId == 0x103) {

			// ��ȡ�����С
			HWND deskHWND = GetDesktopWindow();
			RECT deskRect = { 0 };
			GetWindowRect(deskHWND, &deskRect);
			
			SetWindowPos(hwnd, NULL, 
				rand() % (deskRect.right - rect.right),
				rand() % (deskRect.bottom - rect.bottom),
				800, 600, true);
		}

		// ����ѽ
		else if (ctrlId == 0x104) {
			MessageBoxW(hwnd, L"��ϲ���㵽����", L"ʤ��", MB_OK);
		}
		else if (ctrlId == 0x105) {
			EnumChildWindows(hwnd, EnumChildProc, NULL);
		}
		else if (ctrlId == 0x106) {
			HICON hIcon = LoadIcon(hInstance, (LPCSTR)(IDI_ICON1));
			SetClassLongPtrW(hwnd, -14, (LONG)hIcon);
		}

		// �˵� �˳�
		else if (ctrlId == ID_40002) {
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		
		break;
	}
	case WM_MOUSEHOVER:
	{
		print(L"��껮��...");

		// �ؼ�id
		WORD ctrlId = LOWORD(wParam);
		if (ctrlId == 0x104) {
			print(L"��껮���ؼ�%d", ctrlId);
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
	HINSTANCE hInstance,     // ����ʵ�����
	HINSTANCE hPreInstance,  // ��һ�������ʵ�����
	LPSTR     lpCmdLine,     // char * arg[] �����в���
	int       mCmdShow       // ��ʾ��ʽ

) 
//int main()
{
	// �ַ�������
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

	// 1. ����һ��������
	WNDCLASSW myClass = { 0 };
	myClass.lpszClassName = L"��һ������";
	myClass.lpfnWndProc = WindowProc;
	myClass.hIcon = LoadIcon(0, (LPCSTR)IDI_SHIELD); // 0����ϵͳ��Դ
	myClass.hCursor = LoadCursorA(0, (LPCSTR)IDC_IBEAM); // �����
	//myClass.lpszMenuName = (LPCWSTR)IDR_MENU1; 

	// ���ñ�����ˢ
	myClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

	// 2. ע�ᴰ����
	RegisterClassW(&myClass);

	// 3. ��������
	HMENU hMenu = LoadMenuW(hInstance, (LPCWSTR)IDR_MENU1);
	HWND hwindow = CreateWindowW(
		myClass.lpszClassName,    // ��������
		L"һ������",               // ������
		WS_OVERLAPPEDWINDOW,      // ������ʽ
		CW_USEDEFAULT,            // x
		0,                        // y
		CW_USEDEFAULT,            // width
		0,                        // height
		NULL,                     // ������
		hMenu,                     // �˵�
		hInstance,                // hInstance
		0
	);

	// 4. ��ʾ����
	ShowWindow(hwindow, SW_SHOWNORMAL);

	// 5. ��ȡ��Ϣ
	MSG msg = { 0 };
	while (GetMessageW(&msg, 0, 0, 0)) {

		TranslateMessage(&msg); // ת�� WM_CHAR��Ϣ������༭��������

		DispatchMessageW(&msg); // �ַ���Ϣ�����ڴ�����
	}

	return 0;
}
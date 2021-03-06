// POEPOEM.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "POEPOEM.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_POEPOEM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POEPOEM));

    MSG msg;

    // メイン メッセージ ループ:
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



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POEPOEM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_POEPOEM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char *pText, *pTemp;
	static HGLOBAL hResource;
	static HWND hScroll;
	static int iPosition, cxChar, cyChar, cyClient,
		iNumLines, xScroll;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	TEXTMETRIC tm;

    switch (message)
    {
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);

		xScroll = GetSystemMetrics(SM_CXVSCROLL);
		hScroll = CreateWindow(TEXT("scrollbar"), NULL,
			WS_CHILD | WS_VISIBLE | SBS_VERT,
			0, 0, 0, 0,
			hwnd, (HMENU)1, hInst, NULL);

		hResource = LoadResource(hInst, FindResource(hInst, TEXT("AnnabelLee"), TEXT("TEXT")));

		pText = (char *)LockResource(hResource);
		iNumLines = 0;

		while (*pText != '\\' && *pText != '\0')
		{
			if (*pText == '\n')
				iNumLines++;
			pText = AnsiNext(pText);
		}
		pText = '\0';

		SetScrollRange(hScroll, SB_CTL, 0, iNumLines, FALSE);
		SetScrollPos(hScroll, SB_CTL, 0, FALSE);
		return 0;

	case WM_SIZE:
		MoveWindow(hScroll, LOWORD(lParam) - xScroll, 0, xScroll, cyClient = HIWORD(lParam), TRUE);
		SetFocus(hwnd);
		return 0;

	case WM_SETFOCUS:
		SetFocus(hScroll);
		return 0;

	case WM_VSCROLL:
		switch (wParam)
		{
		case SB_TOP:
			iPosition = 0;
			break;
		case SB_BOTTOM:
			iPosition = iNumLines;
			break;
		case SB_LINEUP:
			iPosition -= 1;
			break;
		case SB_LINEDOWN:
			iPosition += 1;
			break;
		case SB_PAGEUP:
			iPosition -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN:
			iPosition += cyClient / cyChar;
			break;
		case SB_THUMBPOSITION:
			iPosition = LOWORD(lParam);
			break;
		}
		iPosition = max(0, min(iPosition, iNumLines));
		if (iPosition != GetScrollPos(hScroll, SB_CTL))
		{
			SetScrollPos(hScroll, SB_CTL, iPosition, TRUE);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		pText = (char *)LockResource(hResource);
		GetClientRect(hwnd, &rect);
		rect.left += cxChar;
		rect.top += cyChar * (1 - iPosition);
		DrawTextA(hdc, pText, -1, &rect, DT_EXTERNALLEADING);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		FreeResource(hResource);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

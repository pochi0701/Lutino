// Lutino.cpp : アプリケーションのエントリ ポイントを定義します。
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Lutino.h"

#include "shellapi.h"
#include "ltn.h"

// タスクトレイのアイコンに対してクリックなどをすると実行されるメッセージ定数追加
#define WM_TASKTRAY (WM_APP + 1)

// グローバル変数
HICON hIcon;

// Mutexのハンドル
HANDLE m_hMutex = NULL;

HANDLE  threadHandle;
DWORD   id;
int var = 0;
HWND g_hMainWnd = NULL;

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

extern int Lutinomain(void* argv);

//---------------------------------------------------------------------------
void LtnStart()
{
	threadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Lutinomain, NULL, 0, &id);
	//ltn_run_flag = true;   // Lutino起動フラグON
	char work[256];
	while (global_param.server_port == 0) {
		Sleep(100);
	}
	Sleep(1000);

	snprintf(work, 256, "start http://127.0.0.1:%d/", global_param.server_port);
	system(work);
	//ShellExecute(NULL, "open", "http://127.0.0.1:8000/", NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------
void LtnStop(void)
{
	DWORD dwExCode;
	DWORD dwParam;
	//detect_finalize();
	GetExitCodeThread(threadHandle, &dwParam);
	if (dwParam == STILL_ACTIVE) {
		loop_flag = 0;
		closesocket(listen_socket);
		if (WaitForSingleObject(threadHandle, INFINITE) != WAIT_OBJECT_0) {
			TerminateThread(threadHandle, id);
			while (1) {
				GetExitCodeThread(threadHandle, &dwExCode);
				if (dwExCode != STILL_ACTIVE) {
					break;
				}
			}
		}
	}
	CloseHandle(threadHandle);
	threadHandle = (HANDLE)NULL;
	//ltn_run_flag = false;   //Lutino起動フラグOFF
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。

	// インスタンスに関連付いたアイコン読み込み
	//  (AddSystemTrayIcon()で使用するため)
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUTINO));

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LUTINO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUTINO));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUTINO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LUTINO);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_LUTINO));

	return RegisterClassExW(&wcex);
}

BOOL AddSystemTrayIcon(HWND hWnd)
{
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uFlags = (NIF_ICON | NIF_MESSAGE | NIF_TIP);
	nid.hWnd = hWnd;
	nid.hIcon = hIcon;
	nid.uCallbackMessage = WM_TASKTRAY;
	lstrcpy(nid.szTip, TEXT("Lutino"));
	nid.uID = 1;  // アイコンIDを追加
	return Shell_NotifyIcon(NIM_ADD, &nid);
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
	// ここから日付チェックを追加
	time_t now = time(NULL);
	struct tm* tm_now = localtime(&now);

	// 現在日付をYYYYMMDD形式の整数に変換
	int today = (tm_now->tm_year + 1900) * 10000 + (tm_now->tm_mon + 1) * 100 + tm_now->tm_mday;

	//if (today >= 20251001) {
	//	MessageBox(NULL, _T("このアプリケーションは2025年10月01日以降は起動できません。"), _T("起動制限"), MB_OK | MB_ICONWARNING);
	//	return FALSE;
	//}
	// ここまで日付チェック
	//フルアクセス(「別のユーザーとして実行」に対応)
	SECURITY_DESCRIPTOR sd;

	if (0 == ::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) {
		// エラー
		return FALSE;
	}

	if (0 == ::SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE)) {
		// エラー
		return FALSE;
	}
	SECURITY_ATTRIBUTES secAttribute;
	secAttribute.nLength = sizeof(secAttribute);
	secAttribute.lpSecurityDescriptor = &sd;
	secAttribute.bInheritHandle = TRUE;

	m_hMutex = ::CreateMutex(&secAttribute, FALSE, _T("abcdefg"));

	if (m_hMutex == NULL) {
		// ミューテックスの取得に失敗
		return FALSE;
	}

	// 起動していたらすぐに終了させる
	if (::GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, _T("既にこのアプリケーションを起動中です。複数起動はできません。"), _T("起動制限"), MB_OK | MB_ICONWARNING);
		::CloseHandle(m_hMutex);
		return FALSE;  // FALSEを返すと終了する。
	}

	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_hMainWnd = hWnd; // ここでグローバル変数に代入

	// タスクトレイにアイコンを追加
	if (!AddSystemTrayIcon(hWnd))
	{
		DestroyWindow(hWnd);
		return FALSE;
	}

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);


	if (var == 0) {
		var = 1;
		LtnStart();
	}

	return TRUE;
}

void DellSystemTrayIcon(HWND hWnd)
{
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;  // アイコンIDを追加
	Shell_NotifyIcon(NIM_DELETE, &nid);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// タスクトレイのアイコンに対する処理
	case WM_TASKTRAY:
	{
		switch (lParam)
		{
		// 左クリック
		// 右クリック
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			// ウィンドウを非表示
			ShowWindow(hWnd, SW_HIDE);
			POINT pt;
			GetCursorPos(&pt);
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, IDM_BROWSE, TEXT("表示"));
			AppendMenu(hMenu, MF_STRING, IDM_ABOUT, TEXT("バージョン情報"));
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("終了"));
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hMenu, TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
			PostMessage(hWnd, WM_NULL, 0, 0);
			DestroyMenu(hMenu);
		}
		break;
		}
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_BROWSE:
            char work[256];
			snprintf(work, 256, "start http://127.0.0.1:%d/", global_param.server_port);
			system(work);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	//case WM_PAINT:
	//{
	//	PAINTSTRUCT ps;
	//	HDC hdc = BeginPaint(hWnd, &ps);
	//	// TODO: HDC を使用する描画コードをここに追加してください...
	//	EndPaint(hWnd, &ps);
	//}
	//break;
	//case WM_CLOSE:
	//{
	//	// タスクトレイにアイコンを追加
	//	AddSystemTrayIcon(hWnd);
	//	// タスクバー内のアプリアイコンを非表示
	//	ShowWindow(FindWindow(TEXT("Lutino"), NULL), SW_HIDE);
	//}
	break;
	case WM_DESTROY:
	{
		// タスクトレイからアイコンを削除
		DellSystemTrayIcon(hWnd);
		PostQuitMessage(0);
		LtnStop();  // Lutinoを終了
	}
	break;
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
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


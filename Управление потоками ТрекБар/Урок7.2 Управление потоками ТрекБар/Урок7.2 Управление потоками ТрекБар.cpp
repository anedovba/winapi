// ”рок7.2 ”правление потоками “рекЅар.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "”рок7.2 ”правление потоками “рекЅар.h"
#include <commctrl.h>
#include <Windows.h>
#include <iostream>
//#pragma comment(lib,"commctrl32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//=======================================================================================================================
int countThreat = 3;//кол-во потоков
HWND hSumm;//идентификатор окона сумма
HWND*hTr = new HWND[countThreat],*hData = new HWND[countThreat], *hPr = new HWND[countThreat];//hTr - трекер, hData - эдит дата, hPr- эдит прторитет

//идентификатор потоков
HANDLE *hTh = new HANDLE[countThreat];
int summ,*val=new int[countThreat];//summ - данные в поле hSumm, *val - данные шкалы трек бара(номер позиции)
//функци€ работы потоков
DWORD WINAPI Func(LPVOID ptr);

//структура дл€ пометки блока который заблокирован дл€ прерывани€ другим потоком
CRITICAL_SECTION CriticalSection;
TCHAR buf[12];//временный буфер дл€ изменени€ типов данных

//=======================================================================================================================

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY72, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY72));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY72));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY72);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
  
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

	case WM_CREATE:{

		//координаты и айди окон
		int xTR = 10;
		int xPR = 110;
		int idTR = 1000;
		int idPR = 3001;
		int xData = 110;
		int idData = 3004;

		for (int i = 0; i < countThreat; i++){
			//создаем трекбары

			hTr[i] = CreateWindowEx(0, TRACKBAR_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS |
				TBS_ENABLESELRANGE, xTR, 10, 200, 50, hWnd, HMENU(idTR), NULL, NULL);//создаем ползунок
			//устанавливаем минимальные и максимальные значени€ трекбара
			SendMessage(hTr[i], TBM_SETRANGE,
				(WPARAM)TRUE,                   // redraw flag 
				(LPARAM)MAKELONG(0, 31));  // min. & max. positions

			hPr[i] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_WANTRETURN, xPR, 60, 50, 20, hWnd, HMENU(idPR), NULL, NULL);

			hData[i] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_WANTRETURN, xData, 100, 50, 20, hWnd, HMENU(idData), NULL, NULL);

			//изменение координат и айди окон
			xTR += 210;
			idTR++;
			xPR += 210;
			idPR++;
			xData += 210;
			idData++;
		}

		CreateWindow(TEXT("STATIC"), TEXT("Priority:"), WS_CHILD | WS_VISIBLE, 10, 60, 100, 20, hWnd, HMENU(2001), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Priority:"), WS_CHILD | WS_VISIBLE, 220, 60, 100, 20, hWnd, HMENU(2002), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Priority:"), WS_CHILD | WS_VISIBLE, 430, 60, 100, 20, hWnd, HMENU(2003), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Data:"), WS_CHILD | WS_VISIBLE, 10, 100, 100, 20, hWnd, HMENU(2004), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Data:"), WS_CHILD | WS_VISIBLE, 220, 100, 100, 20, hWnd, HMENU(2005), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Data:"), WS_CHILD | WS_VISIBLE, 430, 100, 100, 20, hWnd, HMENU(2006), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Summ:"), WS_CHILD | WS_VISIBLE, 10, 130, 100, 20, hWnd, HMENU(2007), NULL, NULL);
		hSumm = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_WANTRETURN, 110, 130, 100, 20, hWnd, HMENU(3007), NULL, NULL);
		CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE, 250, 130, 100, 20, hWnd, HMENU(4001), NULL, NULL);

	}
				   break;
	case WM_NOTIFY:
	{
		//устанавливаем в эдиты числовые показатели трекбара

		for (int idTr = 1000, i = 0; i < countThreat; i++, idTr++){
			if (((LPNMHDR)lParam)->idFrom == idTr)//если изменили положение трекбара
			{
				val[i] = SendMessage(hTr[i], TBM_GETPOS, 0, 0);//получаем позицию
				wchar_t pos[3];//
				wsprintf(pos, L"%d", val[i]);//переводим в строку
				SetWindowText(hPr[i], pos);//устанавливаем в эдит

				//устанавливаем приоритеты в зависимости от показателей шкалы

				if (val[i] >= 0 && val[i] <= 6){
					val[i] = THREAD_PRIORITY_LOWEST;
				}
				else {
					if ((val[i] >= 7 && val[i] <= 12)){
						val[i] = THREAD_PRIORITY_BELOW_NORMAL;
					}
					else {
						if ((val[i] >= 13 && val[i] <= 18)){
							val[i] = THREAD_PRIORITY_NORMAL;
						}
						else {
							if ((val[i] >= 19 && val[i] <= 24)){
								val[i] = THREAD_PRIORITY_ABOVE_NORMAL;
							}
							else {
								if ((val[i] >= 25 && val[i] <= 31)){
									val[i] = THREAD_PRIORITY_HIGHEST;
								}
							}
						}
					}
				}
			}
		}
	}
	break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case 4001:{//если нажали старт

			GetWindowText(hSumm, buf, 12);//получаем максимальное значение Data
			summ = _wtoi(buf); //переводим в инт
			
			DWORD *id = new DWORD[countThreat];
			
			InitializeCriticalSection(&CriticalSection);

			//создаем потоки и устанавливаем приоритеты
			for (int i = 0, Data = 0; i < countThreat; i++, Data++){
				hTh[i] = CreateThread(NULL, NULL, Func, (void*)Data, CREATE_SUSPENDED, &id[i]); //(void*)Data - указатель передает в функцию  Func адрес потока
				SetThreadPriority(hTh[i], val[i]);

				//запускаем поток
				ResumeThread(hTh[i]);
			}
		}
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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

//функци€ потоков

DWORD WINAPI Func(LPVOID ptr){
	
	
	for (int i = 0; i < summ; ++i){

		// Request ownership of the critical section.
		EnterCriticalSection(&CriticalSection);
		
		if (i % 100 == 0){ //каждые 100 итераций
			wchar_t *Data = new wchar_t[12];
			wsprintf(Data, L"%d", i);
			SetWindowText(hData[(int)ptr], Data);//установать i кратное 100 в эдит соответствующего  потока
			//(int)ptr - это указатель предаетс€ в CreateThread как (void*)Data
		}
		
///		Sleep(1);
		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);
	}
	return 0;
}

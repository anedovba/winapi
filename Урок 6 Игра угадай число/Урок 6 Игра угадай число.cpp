// ���� 6 ���� ������ �����.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <vector>
#include <time.h>
#include "���� 6 ���� ������ �����.h"
using namespace std;
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


HMENU hmS, hmG, hMm, hmL;// hmS - ���� ��������, hmG - ���� ����, hmL - ���� �������
HWND hTimer, hEdit, hList, hCount, hPoz; /*hTimer - ������������� ������� , hEdit - ���� ��� ����� ������������ �����, 
										 hList - ������ �������, hCount-���-�� ����������, hPoz-���-�� �������*/
HWND hWNND;//���������� ���������� ��� ��������������� ��������� ���� 
BOOL Flag = TRUE;//���� ��� ���� �����/����
wchar_t strTimer[7];//������ ��������� ������� MM:SS
int level; //������� ���������
wchar_t *number;//���������� �����
BOOL CALLBACK DlgProcModal(HWND hDlg, UINT message, WPARAM WP, LPARAM lp);//������� ������ � ���������� ����� "�����"


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
	LoadString(hInstance, IDC_MY6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY6));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY6));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= /*MAKEINTRESOURCE(IDC_MY6)*/NULL;
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

   hWNND=hWnd = CreateWindow(szWindowClass, /*szTitle*/L"������ �����", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 350,500, NULL, NULL, hInstance, NULL);

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
	case WM_CREATE:
	{
		//==========================MENU==========================

		auto hmF = CreatePopupMenu();//������� ������ �� ������� ��������� ���������� ���� ����
		AppendMenu(hmF, MF_STRING, 1234, L"&New");//�������� ����� � ����� ����
		AppendMenu(hmF, MF_SEPARATOR, 0, 0);//�����������
		AppendMenu(hmF, MF_STRING, 1235, L"E&xit");
		hmG = CreatePopupMenu();
		AppendMenu(hmG, MF_STRING, 1236, L"&Start");
		hmS = CreatePopupMenu();
		AppendMenu(hmS, MF_STRING, 1238, L"&1");
		AppendMenu(hmS, MF_STRING, 1239, L"&2");
		AppendMenu(hmS, MF_STRING, 1240, L"&3");
		AppendMenu(hmS, MF_STRING, 1241, L"&4");
		AppendMenu(hmS, MF_STRING, 1242, L"&5");
		CheckMenuRadioItem(hmS, 0, 4, 0, MF_BYPOSITION);
		hmL = CreatePopupMenu();
		AppendMenu(hmL, MF_STRING, 1243, L"&1");
		AppendMenu(hmL, MF_STRING, 1244, L"&2");
		AppendMenu(hmL, MF_STRING, 1245, L"&3");
		CheckMenuRadioItem(hmL, 0, 2, 0, MF_BYPOSITION);
		hMm = CreateMenu();//������� ������� ����
		AppendMenu(hMm, MF_POPUP, (int)hmF, L"&File");
		AppendMenu(hMm, MF_POPUP, (int)hmG, L"&Game");
		AppendMenu(hMm, MF_POPUP, (int)hmS, L"&Speed");
		AppendMenu(hMm, MF_POPUP, (int)hmL, L"&Level");
		SetMenu(hWnd, hMm);

		//==========================FIELD==========================

		CreateWindow(L"BUTTON", L"Easy", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 10, 10, 100, 20, hWnd, HMENU(2001), NULL, NULL);
		CreateWindow(L"BUTTON", L"Medium", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 10, 40, 100, 20, hWnd, HMENU(2002), NULL, NULL);
		CreateWindow(L"BUTTON", L"Hard", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 10, 70, 100, 20, hWnd, HMENU(2003), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("������� �����:"), WS_CHILD | WS_VISIBLE, 10, 110, 110, 20, hWnd, HMENU(1001), NULL, NULL);
		hEdit=CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_WANTRETURN | ES_CENTER, 10, 130, 100, 20,
			hWnd, HMENU(1002), NULL, NULL);
		CreateWindow(L"BUTTON", L"Validate", WS_CHILD | WS_VISIBLE, 150, 130, 100, 20, hWnd, HMENU(2004), NULL, NULL);
		hList=CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE, 10, 170, 100, 250, hWnd, HMENU(1003), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("������� ����:"), WS_CHILD | WS_VISIBLE, 150, 170, 150, 20, hWnd, HMENU(1004), NULL, NULL);
		hCount=CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_WANTRETURN, 150, 200, 100, 20, hWnd, HMENU(1005), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("������� �������:"), WS_CHILD | WS_VISIBLE, 150, 240, 150, 20, hWnd, HMENU(1006), NULL, NULL);
		hPoz=CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_WANTRETURN, 150, 270, 100, 20, hWnd, HMENU(1007), NULL, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("���������� �����:"), WS_CHILD | WS_VISIBLE , 150, 310, 150, 20, hWnd, HMENU(1008), NULL, NULL);
		hTimer = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 150, 340, 100, 50, hWnd, HMENU(1009), NULL, NULL);

	}
	break;
	
	case WM_TIMER:{//������ �������
		
		if (GetMenuState(hmS, 0, MF_BYPOSITION)&MF_CHECKED){//���� �������� 1
			static int MM = 0, SS = 60;//����� 1 ������

		wsprintf(strTimer, L" %i : %i ", MM, SS);//������� ����� � ������
		SetWindowText(hTimer, strTimer);//������� ������ � ���� �������
		SS--;//�������� ������
		if (SS == -1){//���� ����� �����������
			KillTimer(hWnd, 3000);//������������� ������
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			MessageBox(hWnd, L"����� �����", L"", MB_OK);//������� ���������
			ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");//�������� ������ ����
			Flag = TRUE;
			MM = 0, SS = 60;//��������� �������� �������
			
		}
		}
		else {
			if (GetMenuState(hmS, 1, MF_BYPOSITION)&MF_CHECKED){//���� �������� 2
				static int MM = 1, SS = 60;
				wsprintf(strTimer, L" %i : %i ", MM, SS);
				SetWindowText(hTimer, strTimer);
				SS--;
				if (SS == 0 && MM > 0){
					MM--; SS = 60;}
				if (SS == -1){
					KillTimer(hWnd, 3000);
					SendMessage(hList, LB_RESETCONTENT, 0, 0);
					MessageBox(hWnd, L"����� �����", L"", MB_OK);
					ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");
					Flag = TRUE;
					MM = 1, SS = 60;
				}
			}
			else { 
				if (GetMenuState(hmS, 2, MF_BYPOSITION)&MF_CHECKED) {
					static int MM = 2, SS = 60;
					wsprintf(strTimer, L" %i : %i ", MM, SS);
					SetWindowText(hTimer, strTimer);
					SS--;
					if (SS == 0 && MM > 0){
						MM--; SS = 60;
					}
					if (SS == -1){
						KillTimer(hWnd, 3000);
						SendMessage(hList, LB_RESETCONTENT, 0, 0);
						MessageBox(hWnd, L"����� �����", L"", MB_OK);
						ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");
						Flag = TRUE;
						MM = 2, SS = 60;
					}
				}
				else{
					if (GetMenuState(hmS, 3, MF_BYPOSITION)&MF_CHECKED) {
						static int MM = 3, SS = 60;
						wsprintf(strTimer, L" %i : %i ", MM, SS);
						SetWindowText(hTimer, strTimer);
						SS--;
						if (SS == 0 && MM > 0){
							MM--; SS = 60;
						}
						if (SS == -1){
							KillTimer(hWnd, 3000);
							SendMessage(hList, LB_RESETCONTENT, 0, 0);
							MessageBox(hWnd, L"����� �����", L"", MB_OK);
							ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");
							Flag = TRUE;
							MM = 3, SS = 60;
						}
					}
					else {
						if (GetMenuState(hmS, 4, MF_BYPOSITION)&MF_CHECKED) {
							static int MM = 4, SS = 60;
							wsprintf(strTimer, L" %i : %i ", MM, SS);
							SetWindowText(hTimer, strTimer);
							SS--;
							if (SS == 0 && MM > 0){
								MM--; SS = 60;
							}
							if (SS == -1){
								KillTimer(hWnd, 3000);
								MessageBox(hWnd, L"����� �����", L"", MB_OK);
								SendMessage(hList, LB_RESETCONTENT, 0, 0);
								ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");
								Flag = TRUE;
								MM = 4, SS = 60;
							}
						}
					}
				}
			}
		}
	}
			break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		
		case 1236:{//start ��� stop
			
			if (GetMenuState(hmL, 0, MF_BYPOSITION)&MF_CHECKED)//��������� ������� ���������
				level = 3;
			if (GetMenuState(hmL, 1, MF_BYPOSITION)&MF_CHECKED)
				level = 5;
			if (GetMenuState(hmL, 2, MF_BYPOSITION)&MF_CHECKED)
				level = 7;

			if (Flag){//���� �������� �����
				
				srand(time(NULL));//���������� ��������� �����
				int Number;
				number = new wchar_t[level];
				wchar_t buf1[2];
				for (int i = 0; i < level; i++){
					Number = rand() % 9;
					wsprintf(buf1, L"%d", Number);//��������� ��� � ������
					if (i==0)
						wcscpy(number, buf1);
					else
					{
						wcscat(number, buf1);
					}
				}
				//wsprintf(xxx, L"%d", Number);
				//MessageBox(hWnd, number, 0, 0);

				SetTimer(hWnd, 3000, 1000, NULL);//������ �������

				ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Stop");//����� ������ �� ����
				Flag = FALSE;

			}
			else //���� �������� ����
			{	
				KillTimer(hWnd, 3000);//��������� �������
				ShowWindow(hWnd, SW_HIDE);//������ �������� ����
			 	UpdateWindow(hWnd);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProcModal);//��������� ���������� ���� � ������
			}

			
		}
			break;
			//��������� ��������� ��������
		case 1238:
			CheckMenuRadioItem(hmS, 0, 4, 0, MF_BYPOSITION);
			break;
		case 1239:
			CheckMenuRadioItem(hmS, 0, 4, 1, MF_BYPOSITION);
			break;
		case 1240:
			CheckMenuRadioItem(hmS, 0, 4, 2, MF_BYPOSITION);
			break;
		case 1241:
			CheckMenuRadioItem(hmS, 0, 4, 3, MF_BYPOSITION);
			break;
		case 1242:
			CheckMenuRadioItem(hmS, 0, 4, 4, MF_BYPOSITION);
			break;
			//��������� ��������� ������� � ������������� � �������������
		case 1243:
		case 2001:
			CheckMenuRadioItem(hmL, 0, 2, 0, MF_BYPOSITION);
			CheckRadioButton(hWnd, 2001, 2003, wmId);
			CheckDlgButton(hWnd, 2001, BST_CHECKED);
			break;
		case 1244:
		case 2002:
			CheckMenuRadioItem(hmL, 0, 2, 1, MF_BYPOSITION);
			CheckRadioButton(hWnd, 2001, 2003, wmId);
			CheckDlgButton(hWnd, 2002, BST_CHECKED);
			break;
		case 1245:	
		case 2003:
			CheckMenuRadioItem(hmL, 0, 2, 2, MF_BYPOSITION);
			CheckRadioButton(hWnd, 2001, 2003, wmId);
			CheckDlgButton(hWnd, 2003, BST_CHECKED);
			break;

			//===================�������� �����, ������� ��� ���==========================

		case 2004:{
			// �������� ���������� ������
			if (GetMenuState(hmL, 0, MF_BYPOSITION)&MF_CHECKED)
				level = 3;
			if (GetMenuState(hmL, 1, MF_BYPOSITION)&MF_CHECKED)
				level = 5;
			if (GetMenuState(hmL, 2, MF_BYPOSITION)&MF_CHECKED)
				level = 7;
			wchar_t str[10];//���������� ��� �������� ���������� ����� � �����
			GetWindowText(hEdit, str, 10);//��������� ����
			wchar_t buf[10];//����� ��� �������� ��������
			
			int i = 0, j, poz = 0, count = 0, w, e = 0, f = 0;//poz - ���������� ��������� �������,count-���������� ��������� ����

			for (; i < level; i++){
				if (str[i] == number[i]){//���������� ���������� ����� � ���������
					poz++;//���� ������� ������ - ������� �� �����
				}
				for (j = i + 1; j < level; j++){//������� ������������� ����� � ��������� �����
					if (str[i] != str[j]){
						for (w = 0; w < level; w++){
							if (str[i] == buf[w])
								f++;
						}
						if (f){
							f = 0;
							break;
						}
						else{
							buf[e] = str[i];
							e++;

						}
					}

				}
			}
			for (w = 0; w < level; w++){
				if (str[level-1] == buf[w])
					f++;
			}
			if (!f){
				buf[e] = str[i-1];
				e++;

			}
			
			for (i = 0; i < e; i++){//���������� ��������� �����(��� ����������) � ����������
				for (j = 0; j < level; j++){
					if (buf[i] == number[j]){
						count++;//���� ������� - ����������� ����������
					}
				}
			}	
			SetWindowText(hEdit, L"");//������� ����
			
			wchar_t Count[2];//��� ������ ���������� � ������
			wsprintf(Count, L"%d", count);
			SetWindowText(hCount, Count);
			
			wchar_t Poz[2];//��� ������ ���������� � ������
			wsprintf(Poz, L"%d", poz);
			SetWindowText(hPoz, Poz);

			wchar_t List[12]; //��� ������ ���������� � ��������
			wsprintf(List, L"%d %d ", count, poz);
			wcscat(List, str);
			SendMessage(hList, LB_INSERTSTRING, 0, (LPARAM)List);
			
			if (poz == level){//���� ��� ������� �� ����� - ������������ � ��������, ��������� ����
				KillTimer(hWnd, 3000);
				SendMessage(hList, LB_RESETCONTENT, 0, 0);
				MessageBox(hWnd, L"�����������, �� ��������!!!", L"", MB_OK);
				ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");
				Flag = TRUE;
			}
		}

			break;
		case 1234: //���� NEW
			KillTimer(hWnd, 3000);
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			SetWindowText(hTimer, L"");
			ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Start");
			Flag = TRUE;
			UpdateWindow(hWnd);
			break;
		case 1235://���� �����
			DestroyWindow(hWnd);
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

BOOL CALLBACK DlgProcModal(HWND hDlg, UINT message, WPARAM WP, LPARAM lp){

	switch (message){
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		//PostQuitMessage(0);
		break;

	case WM_COMMAND://��������� ������� ������� �� ������� � ���������� ����
	{
		int id = LOWORD(WP);//��������� id ������� ������ 

		if (id == IDOK){//���� ������ ������ OK

			EndDialog(hDlg, 0);//��������� ����
			ModifyMenu(hmG, 1236, MF_BYCOMMAND, 1236, L"&Stop");//��������� ������ ����
			Flag = FALSE;
			ShowWindow(hWNND, SW_SHOW);//������������ ������ ��������� ����
			UpdateWindow(hWNND);
			SetTimer(hWNND, 3000, 1000, NULL);//����� �������
			return TRUE;

		}
	}

	}
	return FALSE;
}

#include <windows.h>

/* Deklaracja wyprzedzająca: funkcja obsługi okna */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
/* Nazwa klasy okna */
char szClassName[] = "PRZYKLAD";
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hwnd; /* Uchwyt okna */
	MSG messages; /* Komunikaty okna */
	WNDCLASSEX wincl; /* Struktura klasy okna */
	/* Klasa okna */
	wincl.hInstance = hInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure; // wskaźnik na funkcję obsługi okna
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);
	/* Domyślna ikona i wskaźnik myszy */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	/* Jasnoszare tło */
	//wincl.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wincl.hbrBackground = (HBRUSH)GetStockObject(CTLCOLOR_EDIT);
	/* Rejestruj klasę okna */
	if (!RegisterClassEx(&wincl)) return 0;
	/* Twórz okno */
	hwnd = CreateWindowEx(0, szClassName, "Wybór uczelni", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 512, 430, HWND_DESKTOP, NULL, hInstance, NULL);
	ShowWindow(hwnd, nShowCmd);
	// przyciski:
	HWND button1 = CreateWindowEx(0, "BUTTON", "Akceptuj", WS_CHILD | WS_VISIBLE, 10,  350, 100, 30, hwnd, NULL, hInstance, NULL);
	HWND button2 = CreateWindowEx(0, "BUTTON", "Anuluj",   WS_CHILD | WS_VISIBLE, 388, 350, 100, 30, hwnd, NULL, hInstance, NULL);

	// groupbox uczelnia:
	HWND groupbox1 = CreateWindow("Button", "Uczelnia", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 478, 160, hwnd, 0, GetModuleHandle(0), 0);
	ShowWindow(groupbox1, nShowCmd);

	// editbox + textbox nazwa, adres:
	HWND textbox1 = CreateWindow("STATIC", "Nazwa uczelni: ", WS_CHILD | BS_TEXT | WS_VISIBLE, 30, 50, 100, 20, hwnd, 0, GetModuleHandle(0), 0);
	HWND editbox1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 50, 312, 20, hwnd, NULL, hInstance, NULL);
	SetWindowText(editbox1, "wypełnij nazwę uczelni");

	HWND textbox2 = CreateWindow("STATIC", "Adres uczelni: ", WS_CHILD | BS_TEXT | WS_VISIBLE, 30, 100, 100, 20, hwnd, 0, GetModuleHandle(0), 0);
	HWND editbox2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 100, 312, 20, hwnd, NULL, hInstance, NULL);
	SetWindowText(editbox2, "wypełnij adres uczelni");
	
	// groupbox rodzaj studiow:
	HWND groupbox2 = CreateWindow("Button", "Rodzaj studiow", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 180, 478, 160, hwnd, 0, GetModuleHandle(0), 0);
	ShowWindow(groupbox2, nShowCmd);

	// textbox cykl nauki:
	HWND textbox3 = CreateWindow("STATIC", "Cykl studiów: ", WS_CHILD | BS_TEXT | WS_VISIBLE, 30, 220, 100, 20, hwnd, 0, GetModuleHandle(0), 0);

	// combobox cykl nauki:
	HWND combobox = CreateWindow("COMBOBOX", "mycombo", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 150, 220, 312, 20, hwnd, (HMENU)1, hInstance, NULL);
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)"3-letnie   (licencjackie)");
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)"3.5-letnie (inzynierskie)");
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)"5-letnie   (magisterskie)");

	// checkboxy:
	HWND checkbox1 = CreateWindow("BUTTON", "dzienne",		 WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 255, 100, 20, hwnd, NULL, hInstance, NULL);
	HWND checkbox2 = CreateWindow("BUTTON", "uzupełniające", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 255, 255, 130, 20, hwnd, NULL, hInstance, NULL);

	/* Pętla obsługi komunikatów */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Tłumacz kody rozszerzone */
		TranslateMessage(&messages);
		/* Obsłuż komunikat */
		DispatchMessage(&messages);
	}

	/* Zwróć parametr podany w PostQuitMessage( ) */
	return messages.wParam;
}
/* Tę funkcję woła DispatchMessage( ) */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
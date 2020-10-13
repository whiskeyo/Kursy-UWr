#include <windows.h>
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
char szClassName[] = "Zadanie 3.1.1";
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
	LPSTR lpszArgument, int nFunsterStil)
{
	HWND hwnd;
	MSG messages;
	WNDCLASSEX wincl;

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);

	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;

	wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassEx(&wincl)) return 0;

	hwnd = CreateWindowEx(
		0,
		szClassName,
		"Zadanie 3.1.1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		512,
		512,
		HWND_DESKTOP,
		NULL,
		hThisInstance,
		NULL
	);
	ShowWindow(hwnd, nFunsterStil);

	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return messages.wParam;
}
int xSize, ySize;


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
	HPEN hPen;
	HBRUSH hBrush;
	RECT r;
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		xSize = LOWORD(lParam);
		ySize = HIWORD(lParam);
		GetClientRect(hwnd, &r);
		InvalidateRect(hwnd, &r, 1);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// osie
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, xSize / 2, 0, NULL);
		LineTo(hdc, xSize / 2, ySize);
		MoveToEx(hdc, 0, ySize / 2, NULL);
		LineTo(hdc, xSize, ySize / 2);
		DeleteObject(hPen);

		// wykres |x|
		hPen = CreatePen(PS_DASH, 1, RGB(100, 100, 0));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, xSize / 2, ySize / 2, NULL);

		for (i = 1; i * 2 < ySize; ++i) {
			LineTo(hdc, xSize / 2 + i, ySize / 2 - i);
		}
		LineTo(hdc, xSize / 2 + i, ySize / 2 - i);
		MoveToEx(hdc, xSize / 2, ySize / 2, NULL);

		for (i = 1; i * 2 < ySize; ++i) {
			LineTo(hdc, xSize / 2 - i, ySize / 2 - i);
		}
		LineTo(hdc, xSize / 2 - i, ySize / 2 - i);

		DeleteObject(hPen);

		// wykres x^2
		hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 100));
		SelectObject(hdc, hPen);
		MoveToEx(hdc, xSize / 2, ySize / 2, NULL);

		for (i = 1; i*i * 2 < ySize; ++i) {
			LineTo(hdc, xSize / 2 + i, ySize / 2 - i * i);
		}
		LineTo(hdc, xSize / 2 + i, ySize / 2 - i * i);
		MoveToEx(hdc, xSize / 2, ySize / 2, NULL);

		for (i = 1; i*i * 2 < ySize; ++i) {
			LineTo(hdc, xSize / 2 - i, ySize / 2 - i * i);
		}
		LineTo(hdc, xSize / 2 - i, ySize / 2 - i * i);

		DeleteObject(hPen);

		EndPaint(hwnd, &ps);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
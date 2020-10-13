#include <Windows.h>

int main()
{
	// zadanie to uzywa skryptu datetime.cmd
	ShellExecute(0, "open", "cmd.exe", "/C date /t > time.txt", 0, SW_HIDE);
}
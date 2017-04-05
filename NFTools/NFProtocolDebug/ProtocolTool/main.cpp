#include "stdwx.h"
#include <stdio.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <time.h>
#include <random>

int main(int argc, char* argv[])
{
	srand(time(NULL));

	// get HINSTANCE of current application
	HINSTANCE hInstance = GetModuleHandle(NULL), hPrevInstance = NULL;
	// get command line
	wxCmdLineArgType lpCmdLine = (wxCmdLineArgType)GetCommandLine();

	// create GUI window
	return wxEntry(hInstance, NULL, lpCmdLine, SW_SHOW);
}
// For compilers that don't support precompilation, include "wx/wx.h"
#include "stdwx.h"
#include "app.h"

#ifdef _DEBUG
IMPLEMENT_APP_NO_MAIN(MyApp)
#else
IMPLEMENT_APP(MyApp)
#endif // _DEBUG

// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool MyApp::OnInit()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	Dlg* pDlg = new Dlg(NULL, wxID_ANY, L"ProtocolTool", wxDefaultPosition, wxSize(1024, 768), wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER);
	pDlg->Centre();

	// Show it and tell the application that it's our main window
	pDlg->Show(TRUE);
	SetTopWindow(pDlg);

	return true;
}

int MyApp::OnExit()
{	
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

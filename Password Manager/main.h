
#pragma once


//Including header files
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif


//Ids
namespace windowIDs {
	enum ids {
		MAINWINDOWID = wxID_HIGHEST + 1,
		ENTERBUTTONID = 2,
		USERINPUTFIELD = 3,
		PASSINPUTFIELD = 4,
		SHOWBUTTONID = 5
	};
}
 
//App
class loginSoftware : public wxApp {
	public:
		loginSoftware();

		bool OnInit();

};

//Frame
class mainWindow : public wxFrame {
	//For handeling events
	private:
		void OnButtonClicked(wxCommandEvent &evt);
		void ShowPass(wxCommandEvent& evt);
		wxDECLARE_EVENT_TABLE();

		//TextCtrls
		void OnPassChanged(wxCommandEvent& evt);
		void OnUserChanged(wxCommandEvent& evt);


	public:
		mainWindow(wxWindow* parent,
				   wxWindowID id,
				   const wxString& title,
				   const wxPoint& position = wxDefaultPosition,
				   const wxSize& size = wxDefaultSize,
				   long style = wxDEFAULT_FRAME_STYLE,
				   const wxString& name = wxASCII_STR(wxFrameNameStr));
};

DECLARE_APP(loginSoftware);
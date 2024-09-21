
#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

//IDs
namespace ids {
  enum ui_ids {
    SEARCHFIELD_ID = 1,
    SEARCHRESULTS_ID = 2
  };
}

bool restart = false;

#pragma region Windows

//Calling .exe app
class webBrowser : public wxApp {
  public:
    webBrowser(); //Constructor
    
    ~webBrowser() {
      if (restart) {
        wxExecute(argv[0]);

        restart = false;
      }
    }

    bool OnInit();
};

//Managing the main frame
class mainFrame : public wxFrame {
  //Event manager
  void GetKeyWord(wxCommandEvent& evt);
  wxDECLARE_EVENT_TABLE();

  public:
    //Constructor
    mainFrame(wxWindow* parent,
      wxWindowID id,
      const wxString& title,
      const wxPoint& position = wxDefaultPosition,
      const wxSize&  = wxDefaultSize,
      long style = wxDEFAULT_FRAME_STYLE,
      const wxString& name = wxASCII_STR(wxFrameNameStr));
};

//Managing the results frame
class resultsFrame : public wxFrame {
  private:
    void menuEvent(wxCommandEvent& evt);
    wxDECLARE_EVENT_TABLE();


  public:

    //Constructor
    resultsFrame(wxWindow* parent,
      wxWindowID id,
      const wxString& title,
      const wxPoint& position = wxDefaultPosition,
      const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_FRAME_STYLE,
      const wxString& name = wxASCII_STR(wxFrameNameStr));
};

#pragma endregion


DECLARE_APP(webBrowser);
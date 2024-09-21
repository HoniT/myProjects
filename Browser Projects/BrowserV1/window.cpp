
#include "window.h"
#include "main.h"

IMPLEMENT_APP(webBrowser);

//Events
wxBEGIN_EVENT_TABLE(mainFrame, wxFrame)
  EVT_TEXT(ids::ui_ids::SEARCHFIELD_ID, mainFrame::GetKeyWord)
wxEND_EVENT_TABLE();

wxBEGIN_EVENT_TABLE(resultsFrame, wxFrame)
  EVT_MENU(wxID_NEW, resultsFrame::menuEvent)
wxEND_EVENT_TABLE();

//Function references
void OnEnterPressed();
void createSearchWindow();

//Global Variables
mainFrame* window;
  wxTextCtrl* searchField;
  wxString searchedKeyWord;
  std::string std_keyWord;
  wxPanel* mainPanel;

#pragma region Managing The Window

webBrowser::webBrowser() {}

//Showing the window
bool webBrowser::OnInit() {
  if (!wxApp::OnInit()) return false;

  window = new mainFrame(nullptr, wxID_ANY, _("Web Browser"), wxDefaultPosition, wxSize(500, 400),
    wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
  window->Show();

  return true;
}

#pragma endregion

#pragma region Window

//Main window
mainFrame::mainFrame(wxWindow* parent,
  wxWindowID id,
  const wxString& title,
  const wxPoint& position,
  const wxSize& size,
  long style,
  const wxString& name) :
  wxFrame(parent, id, title, position, size, style, name) {

    //UI elements
    mainPanel = new wxPanel(this);
    mainPanel->SetBackgroundColour(wxColour(240, 240, 240, 240));

    wxStaticText* titleText = new wxStaticText(mainPanel, wxID_ANY, "Web Browser", wxPoint(125, 40), wxSize(250, 50));
    titleText->SetFont(wxFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    searchField = new wxTextCtrl(mainPanel, ids::ui_ids::SEARCHFIELD_ID, "", wxPoint(75, 100), wxSize(350, 20), wxTE_PROCESS_ENTER | wxTE_AUTO_URL);

    searchField->Bind(wxEVT_TEXT_ENTER, [](wxCommandEvent&) {
      //Adding "https://" if needed
      std_keyWord = std::string(searchedKeyWord.mb_str());
      checkURL(std_keyWord);

      OnEnterPressed();
    });

    CreateStatusBar();
}

#pragma endregion


bool pressed_enter = false;
std::string http_response;

#pragma region events

void OnEnterPressed() {
  if (std_keyWord != "") {
    wxLogStatus("Inputed Info: %s", std_keyWord);

    PassKeyWord(std_keyWord);
    http_response = PassKeyWord(std_keyWord);

    pressed_enter = true;
    searchField->Clear();

    createSearchWindow();

    return;
  }

  wxLogStatus("Please Enter a Search Keyword");
}

void mainFrame::GetKeyWord(wxCommandEvent& evt) {
  if (!pressed_enter)
    searchedKeyWord = wxString::Format(evt.GetString());
}

#pragma endregion

//Creating a window for search results

#pragma region resultsWindow

resultsFrame* searchResultsWindow;
void createSearchWindow() {
  searchResultsWindow = new resultsFrame(nullptr, wxID_ANY, "Results", window->GetPosition(), wxSize(500, 400),
    wxDEFAULT_FRAME_STYLE);
  delete window;
  searchResultsWindow->Show();

  return;
}
wxHtmlWindow* htmlView;

resultsFrame::resultsFrame(
  wxWindow* parent,
  wxWindowID id,
  const wxString& title,
  const wxPoint& position,
  const wxSize& size,
  long style, 
  const wxString& name) :
  wxFrame(parent, id, title, position, size, style, name) {


  pressed_enter = false;
  wxInitAllImageHandlers();
  wxFileSystem::AddHandler(new wxMemoryFSHandler);
  wxFileSystem::AddHandler(new wxInternetFSHandler);

  htmlView = new wxHtmlWindow(this);
  htmlView->SetRelatedFrame(this, searchedKeyWord);
  htmlView->SetRelatedStatusBar(0);

  htmlView->SetPage(http_response);

  wxMenuBar* menuBar = new wxMenuBar();
  wxMenu* fileMenu = new wxMenu();
  fileMenu->Append(wxID_NEW);

  menuBar->Append(fileMenu, "File");
  SetMenuBar(menuBar);
  
  

  CreateStatusBar();
}

void resultsFrame::menuEvent(wxCommandEvent& evt) {

  delete searchResultsWindow;
  restart = true;
}

#pragma endregion
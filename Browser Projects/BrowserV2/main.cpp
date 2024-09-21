
#include "main.h"

DECLARE_APP(WebBrowser);


// Static Events
wxBEGIN_EVENT_TABLE(ResultsFrame, wxFrame)
	EVT_MENU(wxID_NEW, ResultsFrame::menuEvent)
	EVT_MENU(ids::menu_ids::BACK_ID, ResultsFrame::OnBack)
	EVT_MENU(ids::menu_ids::FORWARD_ID, ResultsFrame::OnForward)
wxEND_EVENT_TABLE();

// Function References
void OnEnterPressed();

// Global Variables
MainFrame* mainFrame = nullptr;
ResultsFrame* resultsFrame = nullptr;

wxTextCtrl* searchField = nullptr;
std::string keyWord;

#pragma region Windows

// Initializing the App

WebBrowser::WebBrowser() {}

bool WebBrowser::OnInit() {
	if (!wxApp::OnInit()) return false;

	// (Else)

	// Creating and Showing Main Window
	mainFrame = new MainFrame(_("Web Browser"));
	mainFrame->Show(true);

	return true;
}

// Main Frame Elements

MainFrame::MainFrame(const wxString& title) :
	wxFrame(NULL, wxID_ANY, _("Web Browser"), wxDefaultPosition, wxSize(700, 500), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {

	wxPanel* mainPanel = new wxPanel(this);
	mainPanel->SetBackgroundColour(wxColour(240, 240, 240, 240));

	// Search Field
	searchField = new wxTextCtrl(mainPanel, ids::ui_ids::SEARCH_FIELD_ID, "", wxPoint(100, 100), wxSize(500, 30), wxTE_PROCESS_ENTER | wxTE_AUTO_URL);

	// Title
	wxStaticText* titleText = new wxStaticText(mainPanel, wxID_ANY, _("Web Browser"), wxPoint(225, 40), wxSize(250, 50));
	titleText->SetFont(wxFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	// Search Field Enter Function
	searchField->Bind(wxEVT_TEXT_ENTER, [](wxCommandEvent& evt) {

		// Getting Keyword/URL
		wxString wxKeyWord = wxString::Format(evt.GetString());
		keyWord = std::string(wxKeyWord.mb_str());

		// Adding any Necessary Prefixes to the entered URL
		CheckURL(keyWord);

		OnEnterPressed();

		});

	CreateStatusBar();
}

// Result Frame Elements

ResultsFrame::ResultsFrame(const wxString& title) :
	wxFrame(NULL, wxID_ANY, keyWord, wxDefaultPosition, wxSize(700, 500)) {

	// Loading URL
	m_webView = wxWebView::New(this, wxID_ANY);
	m_webView->LoadURL(keyWord);


	// Menu
	wxMenuBar* menuBar = new wxMenuBar();

	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW);
	fileMenu->AppendSeparator();
	fileMenu->Append(ids::menu_ids::BACK_ID, "Back");
	fileMenu->Append(ids::menu_ids::FORWARD_ID, "Forward");


	menuBar->Append(fileMenu, "File");
	SetMenuBar(menuBar);

	CreateStatusBar();
}

#pragma endregion


#pragma region Event Functions 

void OnEnterPressed() {

	// Checking if the Keyword is Empty
	if (keyWord != "") {
		wxLogStatus("Inputed Info: %s", keyWord);

		// Clearing the Search Field
		searchField->Clear();

		// Creating New Window and Deleting Old Window
		resultsFrame = new ResultsFrame(keyWord);
		delete mainFrame;
		resultsFrame->Show(true);

		return;
	}

	wxLogStatus("Please Enter a Search Keyword");
}

// Restarting App
void ResultsFrame::menuEvent(wxCommandEvent& evt) {
	delete resultsFrame;
	restart = true;
}

#pragma endregion
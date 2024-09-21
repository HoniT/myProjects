
#include "main.h"
#include "encrypt.h"

IMPLEMENT_APP(loginSoftware);

//Events
wxBEGIN_EVENT_TABLE(mainWindow, wxFrame)
	EVT_BUTTON(windowIDs::ids::ENTERBUTTONID, mainWindow::OnButtonClicked)
	EVT_BUTTON(windowIDs::ids::SHOWBUTTONID, mainWindow::ShowPass)

	EVT_TEXT(windowIDs::ids::PASSINPUTFIELD, mainWindow::OnPassChanged)
	EVT_TEXT(windowIDs::ids::USERINPUTFIELD, mainWindow::OnUserChanged)

wxEND_EVENT_TABLE();
  
 
#pragma region managingSoftware

loginSoftware::loginSoftware() {}

//Showing a window
bool loginSoftware::OnInit() {
	if (!wxApp::OnInit())
		return false;

	mainWindow* window = new mainWindow(nullptr, windowIDs::ids::MAINWINDOWID, _("Password Manager"), wxDefaultPosition, wxSize(300, 500), 
										wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
	window->Show();

	return true;
}

#pragma endregion


//Global UI Elements
wxTextCtrl* userInputField;
wxTextCtrl* passInputField;
wxPanel* inputPanel;

//Customizing the window
#pragma region mainFrame

mainWindow::mainWindow(wxWindow* parent,
					   wxWindowID id,
					   const wxString& title,
					   const wxPoint& position,
					   const wxSize& size,
					   long style,
					   const wxString& name) :
			wxFrame(parent, id, title, position, size, style, name) {
	//Window components

	wxPanel* panel = new wxPanel(this);

	inputPanel = new wxPanel(panel, wxID_ANY, wxPoint(17, 50), wxSize(250, 200));
	inputPanel->SetBackgroundColour(wxColour(220, 220, 220, 220));

	wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "Password Manager", wxPoint(100, 10), wxSize(100, 15));
	wxStaticText* passText = new wxStaticText(inputPanel, wxID_ANY, "Enter Password Here: ", wxPoint(50, 70), wxSize(200, 20));
	wxStaticText* userText = new wxStaticText(inputPanel, wxID_ANY, "Enter Username Here: ", wxPoint(50, 15), wxSize(200, 20));


	//UI to enter password
	userInputField = new wxTextCtrl(inputPanel, windowIDs::ids::USERINPUTFIELD, "", wxPoint(25, 35), wxSize(200, 20));
	passInputField = new wxTextCtrl(inputPanel, windowIDs::ids::PASSINPUTFIELD, "", wxPoint(25, 90), wxSize(200, 20), wxTE_PASSWORD);
	wxButton* enterPassButton = new wxButton(inputPanel, windowIDs::ids::ENTERBUTTONID, "Enter", wxPoint(70, 140), wxSize(100, 30));

	//Button to show password
	wxButton* showPassButton = new wxButton(inputPanel, windowIDs::ids::SHOWBUTTONID, "", wxPoint(230, 90), wxSize(15, 20));


	//Creating statusBar
	CreateStatusBar();
}

#pragma endregion 

wxString pass;
wxString user;


#pragma region eventSystem

void mainWindow::OnPassChanged(wxCommandEvent& evt) {
	pass = wxString::Format(evt.GetString());
}
void mainWindow::OnUserChanged(wxCommandEvent& evt) {
	user = wxString::Format(evt.GetString());
	wxLogStatus("User: ", user);
}

bool is_showing = false;

bool checkUser(wxString user) {

	if (user.size() < 5) {
		wxLogStatus("Username must be 5 or more characters long");
		return false;
	}


	//Checking if the usernames first character is a letter
	if (!(user[0] >= 65 && user[0] <= 90) && (!(user[0] >= 97 && user[0] <= 122))) {
		wxLogStatus("Username must start with a letter");
		return false;
	}

	//Checking if it contains any spaces
	for (int i = 1; i < user.size(); i++) {
		if (user[i] == ' ') {
			wxLogStatus("Username must not contain any spaces");
			return false;
		}
	}

	return true;
}

bool checkPass(wxString pass) {

	if (pass.size() < 5) {
		wxLogStatus("Password must be 5 or more characters long");

		return false;
	}

	bool containsNeededChars[4]; //0 - Lower Letters, 1 - Upper Letters, 2 - Numbers, 3 - Special Characters

	//Checking if it contains any spaces
	for (int i = 0; i < pass.size(); i++) {
		if (pass[i] == ' ') {
			wxLogStatus("Password must not contain any spaces");

			return false;
		}

		//Checking if all the needed characters are here for a strong password
		if (pass[i] >= 97 && pass[i] <= 122) containsNeededChars[0] = true;
		else if (pass[i] >= 65 && pass[i] <= 90) containsNeededChars[1] = true;
		else if (pass[i] >= 48 && pass[i] <= 57) containsNeededChars[2] = true;
		else if (pass[i] >= 33 && pass[i] <= 126) containsNeededChars[3] = true;

	}

	for (int i = 0; i < 4; i++) {
		if (containsNeededChars[i] != true) {
			wxLogStatus("Password must contain upper and lower case letters, numbers and special characters");

			return false;
		}
	}

	return true;
}

void mainWindow::OnButtonClicked(wxCommandEvent& evt) {

	//Checking pass and username
	checkUser(user);
	checkPass(pass);

	if (checkUser(user) && checkPass(pass)) {

		if (encrypt::checkUsers(user)) wxLogStatus("Username already exists");

		else {
			if (user != "" && pass != "") {


				encrypt::logInfo(user, pass);

				//Deleting Info 
				userInputField->Clear();
				passInputField->Clear();
				pass = user = "";

				wxLogStatus("Entered Info");

			}
			else wxLogStatus("Please enter password and username!");
		}
	}
}

void mainWindow::ShowPass(wxCommandEvent& evt) {
	HWND hwnd = (HWND) passInputField->GetHandle();
	SendMessage(hwnd, EM_SETPASSWORDCHAR, is_showing ? 0x25cf : 0, 0); // 0x25cf is ? character
	passInputField->Refresh();

	//Swaping value
	is_showing = !is_showing;
}


#pragma endregion 
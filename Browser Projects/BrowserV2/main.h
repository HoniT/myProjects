

#pragma region Headers and Macros
#pragma once

#pragma warning(disable : 4996)

#include <string>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/webview.h>

#ifndef BROWSER
	#define BROWSER
#endif

#pragma endregion

// Enums for IDs

namespace ids {
	enum ui_ids {
		SEARCH_FIELD_ID = 1
	};

	enum menu_ids {
		BACK_ID = 2,
		FORWARD_ID = 3
	};
}

#pragma region Windows and App Management

bool restart = false;

class WebBrowser : public wxApp {
public:
	bool OnInit();

	// Constructors and Destructors
	WebBrowser();

	~WebBrowser() {
		// Restart Variable Makes Sure That the App Doesn't go in a Loop

		if (restart) {

			// Restarting
			wxExecute(argv[0]);

			restart = false;
		}
	}

};


class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
};

class ResultsFrame : public wxFrame {
private:
	// Menu Events
	void menuEvent(wxCommandEvent& evt);

	// Go Back and Forward Functions
	void OnBack(wxCommandEvent& event) {
		if (m_webView->CanGoBack()) {
			m_webView->GoBack();
		}
	}

	void OnForward(wxCommandEvent& event) {
		if (m_webView->CanGoForward()) {
			m_webView->GoForward();
		}
	}

	wxDECLARE_EVENT_TABLE();

	// WebView
	wxWebView* m_webView = nullptr;

public:
	ResultsFrame(const wxString& title);
};

#pragma endregion


// Adding Necessary Prefixes to the entered URL

void CheckURL(std::string& url) {
	// Checking If the keyword is a URL, and if so, We Add "https://" / "www."


	if (url.find("https://") == std::string::npos && url.find(" ") == std::string::npos && url.find(".") != std::string::npos) {
		url.insert(0, "https://");
	}

}

wxIMPLEMENT_APP(WebBrowser);
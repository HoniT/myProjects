
#pragma once
#pragma region Headers

#pragma warning(disable : 4996)

//HTML Window
#ifndef WXHTMLWINDOWMY_H
  #define WXHTMLWINDOWMY_H
#endif
#include <wx/wxhtml.h>

//Libcurl
#define CURL_STATICLIB
#include <curl/curl.h>

//wxWidgets

#include <wx/string.h>
#include <wx/panel.h>

#include <wx/fs_mem.h>
#include <wx/fs_inet.h>

#if !defined(_WX_FS_MEM_H_) || !defined(_WX_FS_MEM_H_)
#include <wx/wx.h>
#endif

#pragma endregion



//Function References
std::string PassKeyWord(wxString keyWord); //Passing the keyword to the backend functions (in main.cpp)
void checkURL(std::string& url);


#include <string>
#include "main.h"

typedef std::string string;

string httpResponse;

size_t write_to_string(void* ptr, size_t size, size_t count, void* stream);
void checkURL(string& url);

//Using Curl's API to get HTTP request
string set_http_request(const string std_keyWord) {

  CURL* curl = nullptr;
  CURLcode res;

  curl = curl_easy_init();
  if (curl == NULL) return "ERROR! HTTP request failed. Curl = NULL";

  curl_easy_setopt(curl, CURLOPT_URL, std_keyWord.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpResponse);

  res = curl_easy_perform(curl);
  string tempReturn(curl_easy_strerror(res));

  if (res != CURLE_OK) return "ERROR! " + tempReturn;

  curl_easy_cleanup(curl);


  return httpResponse;
}

//Function Connecting Frontend to Backend
string PassKeyWord(wxString keyWord) {

  string std_keyWord = string(keyWord.mb_str()); //Converting wxString to std::string

  return set_http_request(std_keyWord);
}


//Fills the url with "https://" / "www." if needed
void checkURL(string& url) {
  if (url.find("www.") == std::string::npos && url.find(" ") == std::string::npos && url.find(".") != std::string::npos) {
    url.insert(0, "www.");
  }
  if (url.find("https://") == std::string::npos && url.find(" ") == std::string::npos && url.find(".") != std::string::npos) {
    url.insert(0, "https://");
  }
}


//Function for appending string
size_t write_to_string(void* ptr, size_t size, size_t count, void* stream) {
  ((string*)stream)->append((char*)ptr, 0, size * count);
  return size * count;
}
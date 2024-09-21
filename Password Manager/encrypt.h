
#include <wx/string.h>
#include <fstream>
#include <ctime>
#include <string>

class encrypt {
	private:
		static void encryptInfo(wxString &pass) {
			srand(time(NULL));

			
			wxString tempString = "";
			int count = 0, pCount = 0;

			//Setting and filling a random size
			int stopPar = (pass.size() * (rand() % (9 - 2) + 2));

			for (int i = 0; i < stopPar; i++) {
				tempString.append((char)(rand() % (127 - 33) + 33));
			}
			int debug = tempString.size() / pass.size();

			for (int i = 0; i < tempString.size(); i++) {
				if (debug == count) {
					tempString.insert(i, pass[pCount]);

					i++;
					pCount++;
					count = 0;
				}

				count++;
			}

			tempString.append(pass[pass.size() - 1]);
			pass = tempString;
			pass.append((char)(debug + 48));
		}

	public:
		static bool checkUsers(const wxString user) {
			std::ifstream fin("dataBase.txt");

			std::string text1, username, text2, pass;

			while (fin >> text1 >> username >> text2 >> pass) {

				if (username == user) {
					return true;
				}
			}

			return false;
		}

		static void logInfo(const wxString user, wxString pass) {

			if (!checkUsers(user)) {
				//Logging info
				std::ofstream fout("dataBase.txt", std::fstream::app);

				encryptInfo(pass);
				fout << "User: " << user << " Password: " << pass << '\n';
				fout.close();

			}

		}
};
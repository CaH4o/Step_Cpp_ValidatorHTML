#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;
using std::cin;

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::size_t;

enum eTeg { error, open, close, selfClose };
enum eStatus { Non, OpenLeft, CloseLeft };

/*
<html>
	<body>To be or not to be</body>
	<br/>
</html>
*/

class Validator {
public:
	explicit Validator(string content, vector <string> tegs) {
		_sContent = _sContent;
		_tegOpen = _tegClose = _tegSelfClose = 0;
		_isValide = true;
		_tegs = tegs; 
		ValidCheck(content);
		CheckOpenandCloseTegs();
		Show();
	}

	string ValidCheck(string s) {

		do
		{
			if (s == "") return ""; //stop case;

			eTeg nextTeg = CheckOrder(s);

			if (nextTeg == error) {
				_isValide = false;
				return "";
			}

			if (nextTeg == selfClose) {
				s = CheckTeg(s);
				continue;
			}

			if (nextTeg == open)
			{
				s = CheckTeg(s);
				s = ValidCheck(s); //recurs case
			}

			if (nextTeg == close)
			{
				return CheckTeg(s);
			}

		} while (true);

		return "";
	}

private:

	eTeg CheckOrder(const string& s) {
		eStatus status = Non;

		for (size_t i = 0; i < s.length(); ++i)
		{
			switch (status) {
			case Non:
				if (s[i] == '<' && s[i + 1] == '/') {
					status = CloseLeft;
					++i;
					continue;
				}
				if (s[i] == '<') {
					status = OpenLeft;
					continue;
				}
				if (s[i] == '>' || s[i] == '/') return error;
				continue;
			case CloseLeft:
				if (s[i] == '>') {
					++_tegClose;
					return close;
				}
				else if (s[i] == '<' || s[i] == '/') return error;
				continue;
			case OpenLeft:
				if (s[i] == '/' && s[i + 1] == '>') { 
					++_tegSelfClose;
					return selfClose; 
				}
				if (s[i] == '>') {
					++_tegOpen;
					return open;
				}
				if (s[i] == '/' || s[i] == '<') return error;
				continue;
			default:
				break;
			}
		}
	}

	string CheckTeg(string s) {
		string sTeg = "";
		bool isNotCompletedTeg = false;

		for (size_t i = 0; i < s.length(); ++i)
		{
			if (s[i] == '<') {
				s = s.substr(i);
				break;
			}
		}

		for (size_t i = 0; i < s.length(); ++i)
		{
			if (sTeg == "" && s[i] == '<' && s[i + 1] == '/') { // Left </teg>
				++i;
				isNotCompletedTeg = true;
				continue;
			}
			if (sTeg == "" && s[i] == '<') { // Left <teg>, <teg/>
				isNotCompletedTeg = true;
				continue;
			}
			if (sTeg != "" && s[i] == '/' && s[i + 1] == '>') { // Right <teg/>
				s = s.substr(i + 2);
				break;
			}
			if (sTeg != "" && s[i] == '>') { // Right <teg>, </teg>
				s = s.substr(i + 1);
				break;
			}

			if (isNotCompletedTeg && s[i] == ' ') isNotCompletedTeg = false;
			if (isNotCompletedTeg) sTeg += s[i];
		}

		for (size_t i = 0; i < _tegs.size(); ++i)
		{
			if (sTeg == _tegs[i]) return s;
		}

		_isValide = false;
		return "";
	}

	void CheckOpenandCloseTegs() {
		if (_tegOpen != _tegClose) _isValide = false;
	}

	void Show() {
		if (_isValide) { 
			cout << "Valid\nOpen tegs: " << _tegOpen << "\nClose tegs: " << _tegClose << "\nSelf close tegs: " << _tegSelfClose << "\n";
		}
		else cout << "Invalid.\n";
	}

	string _sContent;
	int _tegOpen;
	int _tegClose;
	int _tegSelfClose;
	bool _isValide;
	vector <string> _tegs;
};

void main()
{
	setlocale(LC_ALL, "rus");

	try
	{
		ifstream inFstream;
		string sPath;
		vector <string> tegs = { "HTML","BODY","BR","html","body","br" };

		cout << "Enter the path of \"HTML\" file:\n";
		cin >> sPath;
		//sPath = "html.txt";
		cout << sPath;
		getchar();

		inFstream.open(sPath);


		if (inFstream.is_open())
		{
			string sFileContent;
			string sTemp;

			cout << "\nThe file upload.\n\n";

			while (getline(inFstream, sTemp))
				sFileContent.append(sTemp);

			inFstream.close();

			Validator valid = Validator(sFileContent, tegs);
		}
		else
		{
			cout << "\nFile is not found.\n";
		}
	}
	catch (const std::exception&)
	{
		cout << "\nThere is some misstake.\n";
	}

	getchar();
}
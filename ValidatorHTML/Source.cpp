#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum eTeg { error, open, close, selfClose};
enum eStatus { Non, OpenLeft, CloseLeft };

vector <string> tegs = { "HTML","BODY","BR","html","body","br" };
bool isValide = true;

eTeg CheckOrder(const string& s);
string CheckTeg(string s);

//Проверка на валидность.
string Validator(string s) {

	do
	{
		if (s == "") return ""; //stop case;

		eTeg nextTeg = CheckOrder(s);

		if (nextTeg == error) {
			isValide = false;
			return "";
		}

		if (nextTeg == selfClose) {
			s = CheckTeg(s);
			continue;
		}

		if (nextTeg == open)
		{
			s = CheckTeg(s);
			s = Validator(s);
		}

		if (nextTeg == close)
		{
			return CheckTeg(s);
		}

	} while (true);

	return "";
}

//проверка тега на правильную очередность управляющих знаков "<", ">" и "/".
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
			if (s[i] == '>') return close;
			else if (s[i] == '<' || s[i] == '/') return error;
			continue;
		case OpenLeft:
			if (s[i] == '/' && s[i + 1] == '>') return selfClose;
			if (s[i] == '>') return open;
			if (s[i] == '/' || s[i] == '<') return error;
			continue;
		default:
			break;
		}
	}
}

//проверка тега на корректное написание относительно списка тегов.
string CheckTeg(string s) {
	string teg = "";
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
		if (teg == "" && s[i] == '<' && s[i + 1] == '/') { // Left </teg>
			++i;
			isNotCompletedTeg = true;
			continue;
		}
		if (teg == "" && s[i] == '<') { // Left <teg>, <teg/>
			isNotCompletedTeg = true;
			continue;
		}
		if (teg != "" && s[i] == '/' && s[i + 1] == '>') { // Right <teg/>
			s = s.substr(i + 2);
			break;
		}
		if (teg != "" && s[i] == '>') { // Right <teg>, </teg>
			s = s.substr(i + 1);
			break;
		}

		if (isNotCompletedTeg && s[i] == ' ') isNotCompletedTeg = false;
		if (isNotCompletedTeg) teg += s[i];
	}

	for (size_t i = 0; i < tegs.size(); ++i)
	{
		if (teg == tegs[i]) return s;
	}

	isValide = false;
	return "";
}


void main() {

	string code = "<HTML key=1><BODY>Some text</BODY><BR/></HTML>";
	Validator(code);
	
	(isValide) ? cout << "Valid\n" : cout << "Invalid\n" ;

	system("pause");
}


#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
#include"scanner.h"

using namespace std;

string source;
int index = 0;//index of characters not tokens


/*toString of the types
	print
*/
string typetostring(type t) {
	switch (t)
	{
	case RESERVED:
		return "reserved";
	case NUMBER:
		return "number";
	case ID:
		return "identifier";
	case SPECIAL:
		return "special character";
	default:
		break;
	}
	return "token value is invalid";
}
/*
	returns true if the passed character is an accepted special character
*/
bool isspecial(char ch) {
	for (int i = 0; i < SPECIALCHARACTERS.length(); i++) {
		if (ch == SPECIALCHARACTERS[i])
			return true;
	}
	return false;
}
/*
	returns true if the passed string is a reserved word
*/
bool isreserved(string str) {
	for (int i = 0; i < RESERVEDWORDSLENGTH; i++)
	{
		if (str == RESERVEDWORDS[i])
			return true;
	}
	return false;
}
/*
	pops a character from the start of the string
*/
char consume() {
	return source[index++];
}
/*
	reads a character from the start of the string
*/
char read() {
	return source[index];
}
/*
	returns true if the string is empty
*/
bool done() {
	return index == source.length();
}

token getToken() {
	token result;
	result.value = "";
	char tmp;
	state cs = START;
	
	while(!done() && cs != DONE && cs !=ERROR){
		switch (cs)
		{
		case START:
			tmp = consume();
			if (isdigit(tmp)) {
				cs = INNUM;
				result.t = NUMBER;
				result.value.push_back(tmp);
			}
			else if (isalpha(tmp)) {
				cs = INID;
				result.t = ID;
				result.value.push_back(tmp);
			}
			else if (tmp == ':') {
				cs = INASSIGN;
				result.t = SPECIAL;
				result.value.push_back(tmp);
			}
			else if (isspecial(tmp)) {
				cs = DONE;
				result.t = SPECIAL;
				result.value.push_back(tmp);
			}
			else if (tmp == '{')
				cs = INCOMMENT;

			break;
		case INID:
			tmp = read();
			if (!isalpha(tmp))
				cs = DONE;
			else
				result.value.push_back(consume());
			break;
		case INNUM:
			tmp = read();
			if (!isdigit(tmp))
				cs = DONE;
			else
				result.value.push_back(consume());
			break;
		case INASSIGN:
			tmp = read();
			if (tmp != '=')
				cs = ERROR;
			else {
				result.value.push_back(consume());
				cs = DONE;
			}

			break;
		case INCOMMENT:
			if (consume() == '}')
				cs = START;
			break;
		default:
			break;
		}
	}

	if (cs == ERROR || cs == START) {
		result.value.append(" this token is an error");
		result.t = INVALID;
	}

	if (cs == DONE && result.t == ID)
		result.t = isreserved(result.value)? RESERVED : result.t;

	return result;
}
/*
	returns a list of all tokens
*/
vector<token> getTokens(){
	vector<token> tokens;
	while (!done()) {
		tokens.push_back(getToken());
	}
	return tokens;
}

/*
	this a testing function that prints tokens
*/
void printTokens() {
	ofstream fout("output.txt");
	vector<token> tokens = getTokens();
	for (auto i = tokens.begin(); i != tokens.end(); i++) {
		if (i->t == INVALID)
			continue;
		cout << "TYPE: " << setw(25) << left << typetostring(i->t) << "\tVALUE: " << i->value << endl << endl;
		fout << "TYPE: " << setw(25) << left << typetostring(i->t) << "\tVALUE: " << i->value << endl << endl;
	}
	fout.close();
}
/*
	this function reads a whole file and puts it in a string
*/
void readFile(string filename) {
	ifstream file("input.txt");
	string str;
	while (getline(file, str))
	{
		source.append(str);
	}
}
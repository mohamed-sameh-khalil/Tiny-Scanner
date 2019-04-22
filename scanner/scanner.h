#include <vector>
#pragma once
const std::string RESERVEDWORDS[] = { "if", "then", "else", "end", "repeat", "until", "read", "write" };
const std::string SPECIALCHARACTERS = "+-*/=<();:";
const int RESERVEDWORDSLENGTH = 8;

enum type {
	RESERVED,
	NUMBER,
	ID,
	SPECIAL,
	INVALID
};

enum state {
	START,
	INID,
	INNUM,
	INASSIGN,
	INCOMMENT,
	DONE,
	ERROR
};
struct token {
	type t;
	std::string value;
};

std::string typetostring(type);
bool isspecial(char ch);
bool isreserved(std::string str);
token getToken();
std::vector<token> getTokens();
void readFile(std::string filename);
void printTokens();
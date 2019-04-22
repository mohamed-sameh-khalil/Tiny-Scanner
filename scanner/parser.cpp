#include"parser.h"
#include"scanner.h"
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;

vector<token> tokens;
int parse_index = 0;
void init() {
	tokens = getTokens();
}
void printAll() {
	init();
	for (int i = 0; i < tokens.size(); i++) {
		cout << tokens[i].value << endl;
	}
}
bool parse_done() {
	return parse_index >= tokens.size();
}
string wrap(string answer, string wrapper){
	return "<" + wrapper + ">" + answer + "</" + wrapper + ">\n";
}
token current() {
	token t = { INVALID, "" };
	return parse_done() ? t : tokens[parse_index];
}
void log(string x) {}
string match(string matcher) {
	token tmp = current();
		if (tmp.value != matcher)
			tmp.value = "errrror expecting " + matcher;
	parse_index++;
	return tmp.value;
}

string program() {
	init();
	string answer = "";
	answer += stmt_seq();
	return wrap(answer, "program");
}

string stmt_seq() {
	string answer = "";
	answer += statement();
	while (current().value == ";") {
		match(";");
		answer += statement();
	}
	return wrap(answer, "stmt-sequence");
}

string statement() {
	string answer = "", choice = current().value;
	if (choice == "if")
		answer += if_stmt();
	else if (choice == "repeat")
		answer += repeat_stmt();
	else if (current().t == ID)
		answer += assign_stmt();
	else if (choice == "read")
		answer += read_stmt();
	else if (choice == "write")
		answer += write_stmt();
	else
		answer += "errrorrr in statement not expecting " + choice;
	// return wrap(answer, "statement");
	return answer;
}

string if_stmt() {
	string answer = "";
	match("if");
	answer += exp(); //test part
	match("then");
	answer += stmt_seq(); //then part
	if (current().value == "else") {
		match("else");
		answer += stmt_seq(); //else_part
	}
	match("end");

	return wrap(answer, "if");
}
string repeat_stmt() {
	string answer = "";
	match("repeat");
	answer += stmt_seq(); //body
	match("until");
	answer += exp(); //test
	return wrap(answer, "repeat");
}
string assign_stmt() {
	string answer = "";
	if (current().t == ID) {
		answer = "(" + match(current().value) + ")"; //the identifier
		match(":=");
		answer += exp(); //expression
	}
	else
		answer += "problem in assign statement not expecting " + current().t;
	return wrap(answer, "assign");
}
string read_stmt() {
	string answer = "";
	match("read");
	if (current().t == ID)
		answer += match(current().value);// the identifier
	else
		answer += "error in read stmt not expecting " + current().value;
	return wrap(answer, "read");
}
string write_stmt() {
	string answer = "";
	match("write");
	answer += exp(); //expression
	return wrap(answer, "write");
}
string exp() {
	string answer = "", title = "const";
	answer += simple_exp(); // simple exp or term
	if (current().value == "<" || current().value == "=") {
		title = "op(" + comparison_op() + ")";
		answer += simple_exp();//other simple exp or term
	}
	return wrap(answer, title);//title is comparison op or just a constant
}
std::string comparison_op() {
	string answer = "";
	if (current().value == "<") {
		match("<");
		answer += "&lt";//html escape character for angle brackets
	}
	else
		answer += match("=");
	return answer;
}
std::string simple_exp() {
	string answer = "";
	answer += term();
	while (current().value == "+" || current().value == "-") {
		answer += match(current().value);
		answer += term();
	}
	return wrap(answer, "simple-exp");
}
std::string addop() {
	string answer = "";
	if (current().value == "+")
		answer += match("+");
	else
		answer += match("-");
	return wrap(answer, "addop");
}
std::string term() {
	string answer = "";
	answer += factor();
	while(current().value == "*" || current().value == "/"){
		answer += match(current().value);
		answer += factor();
	}
	return wrap(answer, "term");
}
std::string mulop() {
	string answer = "";
	if (current().value == "*")
		answer += match("*");
	else
		answer += match("/");
	return wrap(answer, "mulop");
}
std::string factor() {
	string answer = "";
	if (current().t == ID) {
		answer += match(current().value);
	}
	else if (current().t == NUMBER) {
		answer += match(current().value);
	}
	else {
		answer += match("(");
		answer += exp();
		answer += match("(");
	}
	return wrap(answer, "factor");
}



string parse2file() {
	ofstream fout("parse_out.txt");
	string answer = program();
	fout << answer;
	fout.close();
	return answer;
}




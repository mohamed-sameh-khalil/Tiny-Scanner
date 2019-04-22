#include"parser.h"
#include"scanner.h"
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;
bool fit_to_xml = 1;
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
	return answer;
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
	string answer = "", title="";
	if (current().t == ID) {
		title = "assign_" + match(current().value); //the identifier
		match(":=");
		answer += exp(); //expression
	}
	else
		answer += "problem in assign statement not expecting " + current().t;
	return wrap(answer, title);
}
string read_stmt() {
	string answer = "";
	match("read");
	if (current().t == ID)
		answer += wrap("", "read_" + match(current().value));// the read(identifier) becomes the root
	else
		answer += "error in read stmt not expecting " + current().value;
	return answer;
}
string write_stmt() {
	string answer = "";
	match("write");
	answer += exp(); //expression
	return wrap(answer, "write");
}
string exp() {
	string answer = "";
	answer += simple_exp(); // simple exp or term
	if (current().value == "<" || current().value == "=") {
		string title = "op_" + comparison_op();
		answer += simple_exp();//other simple exp or term
		answer = wrap(answer, title);//op is the parent for the 2 simple exp
		//if there is an op we want it to be the parent
		//otherwise we accept that the simple exp will handle this node completely
	}
	return answer;//title is comparison op or just a constant
}
std::string comparison_op() {
	string answer = "";
	if (current().value == "<") {
		match("<");
		answer += fit_to_xml ? "lessthan" : "<";
	}
	else {
		match("=");
		answer += fit_to_xml ? "equals" : "=";
	}
	return answer;
}
std::string simple_exp() {
	string answer = "", title = "";
	answer += term();//first term could be the root
	while (current().value == "+" || current().value == "-") {
		string title = "op_" + addop();//if there is addop it will be the root to the two terms
		answer += term();//second term
		answer = wrap(answer, title);//making the op be the root
	}
	return answer;
}
std::string addop() {
	string answer = "";
	if (current().value == "+") {
		match("+");
		answer += fit_to_xml ? "plus" : "+";
	}
	else
		answer += match("-");
	return answer;
}
std::string term() {
	string answer = "";
	answer += factor();//first factor, could be the root
	while(current().value == "*" || current().value == "/"){
		string title = "op_" + mulop();//if the mulop exists it will be the root
		answer += factor();//second factor
		answer = wrap(answer, title);//making the mulop become the root
	}
	return answer;
}
std::string mulop() {
	string answer = "";
	if (current().value == "*") {
		match("*");
		answer += fit_to_xml ? "multiply" : "*";
	}
	else {
		match("/");
		answer += fit_to_xml ? "divide" : "/";
	}
	return answer;
}
std::string factor() {
	string answer = "";
	if (current().t == ID) {
		string title= "ID_" + match(current().value);
		answer = wrap("", title);//wrap the identifier as the root
	}
	else if (current().t == NUMBER) {
		string title = "CONST_" + match(current().value);
		answer = wrap("", title);//wrap the number as the root
	}
	else {
		match("(");
		answer += exp();//exp should handle the root
		match("(");
	}
	return answer;
}



string parse2file() {
	ofstream fout("parse_out.txt");
	string answer = program();
	fout << answer;
	fout.close();
	return answer;
}




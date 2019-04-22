#include<iostream>
#include"scanner.h"
#include"parser.h"

using namespace std;



int main() {
	
	readFile("input.txt");
	//printTokens();
	cout << parse2file();
	//printAll();
	
	cin.get();
	return 0;
}
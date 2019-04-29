#include<iostream>
#include"scanner.h"

using namespace std;



int main() {
	
	readFile("tiny.txt");
	//printTokens();
	printTokens("scanned.txt");
	//printAll();
	
	cin.get();
	return 0;
}
#include<iostream>
#include"scanner.h"

using namespace std;



int main() {
	
	readFile("input.txt");
	//printTokens();
	printTokens("output.txt");
	//printAll();
	
	cin.get();
	return 0;
}
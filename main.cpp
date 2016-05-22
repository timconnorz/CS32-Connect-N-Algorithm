// main.cpp

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
//	string name;
	//cout << "What's your name?  ";
//	getline(cin, name);
	HumanPlayer hp("name");
	SmartPlayer sp("Computer");
	Game g1(5, 5, 3, &hp, &sp);
	g1.play();
	
}

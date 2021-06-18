/*

Elias Sims, Section 1, elisims@byu.edu
Lab 1: Lexical Analyzer

*/

#include <string>
#include <fstream>
#include <iostream>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main(int argc, const char* argv[]) {

	//string files[] = {
	//	"input1.txt",
	//	"input2.txt",
	//	"input3.txt",
	//	"input4.txt",
	//	"input5.txt",
	//	"input6.txt",
	//	"input7.txt",
	//	"input8.txt",
	//	"input9.txt",
	//	"input10.txt"
	//};

//	for (unsigned int i = 0; i < 10; i++) {
		//cout << "---------+" << endl;
		//cout << "Input: " << i + 1 << " |" << endl;
		//cout << "---------+" << endl;

		Lexer input(argv[1]);
		vector<Token> automata = input.generateTokens();

		/* Lab 1 */
		//for (unsigned int i = 0; i < automata.size(); i++) {
		//	cout << automata[i].toString();
		//}

		//cout << "Total Tokens = " << automata.size() << endl;

		/* Lab 2 */
		Parser parser = Parser(automata);
		bool success = parser.parse();
		if (!success) {
			cout << "Failure!" << endl;
			cout << "  " << parser.getLastToken().toString();
		}
		else {
			cout << "Success!" << endl;
			cout << parser.getDataLog().toString();
		}
		cout << endl;
//}
	return 0;
}
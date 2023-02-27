
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include "lex.h"
//#include "lex.cpp"

using namespace std;

int main(int argc, char* argv[])
{
	ifstream inFile;
	string fileName;
	string line, word;
	bool oneFile = true;
	LexItem token;
	int lineNum = 0;
	int tokCount = 0;
	set <string> idents;
	set <int> iconsts;
	set <float> rconsts;
	set <string> strliterals;
	set <string> bconsts;
	bool v = false;
	bool iconst = false;
	bool rconst = false;
	bool sconst = false;
	bool ident = false;
	bool bconst = false;
	bool badarg = false;
	bool errors = false;



	if (argc == 1) {
		cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
		exit(1);
	}
	else {
		fileName = argv[1];
		inFile.open(fileName.c_str());

		if (!inFile.is_open()) {
			cerr << "CANNOT OPEN THE FILE " << fileName << endl;
			exit(1);
		} // while
		if (argc >= 3) {
			for (int i = 2; i < argc; i++) {
				string comm = argv[i];
				if (comm[0] != '-') {
					oneFile = false;
					std::cout << "ONLY ONE FILE NAME ALLOWED." << endl;

				}

			}
			if (oneFile == true) {

				for (int i = 2; i < argc; i++) {
					string arrayHolder = argv[i];
					if (arrayHolder != "-v" && arrayHolder != "-iconst" && arrayHolder != "-rconst" && arrayHolder != "-sconst" && arrayHolder != "-ident" && arrayHolder != "-bconst") {
						std::cout << "UNRECOGNIZED FLAG " << arrayHolder << endl;
						badarg = true;
					}
					if (arrayHolder == "-v") {
						v = true;
					}
					if (arrayHolder == "-iconst") {
						iconst = true;
					}
					if (arrayHolder == "-rconst") {
						rconst = true;
					}
					if (arrayHolder == "-sconst") {
						sconst = true;
					}
					if (arrayHolder == "-ident") {
						ident = true;
					}
					if (arrayHolder == "-bconst") {
						bconst = true;
					}
				}
			}

		}
	}
	if (inFile.is_open() && oneFile == true) {
		bool isEmpty = (inFile.peek() == std::ifstream::traits_type::eof());
		if (isEmpty == true) {
			std::cout << "Lines: 0" << endl;
			exit(1);
		}
		//while (getline(inFile, line)) {

		token = getNextToken(inFile, lineNum);

		while (token != DONE || token != ERR) {

			//while (iss >> word) {



			if (token == DONE || token == ERR) {
				if (token == ERR) {
					errors = true;
					cout << token;
				}
				break;
			}
			tokCount++;

			if (token.GetToken() == IDENT) {
				idents.insert(token.GetLexeme());

			}
			if (token.GetToken() == ICONST) {
				iconsts.insert(stoi(token.GetLexeme()));

			}
			if (token.GetToken() == RCONST) {
				rconsts.insert(stof(token.GetLexeme()));

			}
			if (token.GetToken() == SCONST) {
				strliterals.insert(token.GetLexeme());

			}
			if (token.GetToken() == BCONST) {
				bconsts.insert(token.GetLexeme());
			}

			//}
			if (v)
				cout << token;

			token = getNextToken(inFile, lineNum);


		}
		if (oneFile == true && badarg == false && errors == false) {
			cout << "Lines: " << lineNum << endl;
			cout << "Tokens: " << tokCount << endl;
		}
		if (sconst && errors == false) {
			cout << "STRINGS:" << endl;
			for (string i : strliterals) {
				cout << "\"" << i << "\"" << endl;
			}
		}

		if (iconst && errors == false)
		{
			cout << "INTEGERS:" << endl;
			for (int i : iconsts) {
				cout << i << endl;
			}
		}
		if (rconst && errors == false) {
			cout << "REALS:" << endl;
			for (float i : rconsts) {
				cout << i << endl;
			}
		}
		if (bconst && errors == false) {
			cout << "Booleans:" << endl;
			for (string i : bconsts) {
				cout << i << endl;
			}
		}
		if (ident && errors == false) {
			cout << "IDENTIFIERS:" << endl;
			int j = 0;
			for (string i : idents) {

				if (j != 0)
					cout << ", ";
				cout << i;
				j++;
			}
			cout << endl;

		}

	}
}
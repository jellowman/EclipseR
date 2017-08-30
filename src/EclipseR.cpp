//============================================================================
// Name        : EclipseR.cpp
// Author      : Trevor Fisher
// Version     : 0.1
// Copyright   : N/A
// Description : Initial i/o implementation for EclipseR Software
//============================================================================

#include <iostream>
#include <string>
#include "EclipseR.h"
using namespace std;



int main() {

	//Variable used to hold next line from the eclipse data text file
	string nextLine;
	int lineNum = 0;

	//Read past the "junk lines" at the beginning of the file
	for(int i = 0; i < 10; i++) {
		getline(cin, nextLine);
		lineNum++;
	}

	//getline(cin, nextLine);
	//lineNum++;
	//cout << nextLine << endl;
	//hoi

	while(getline(cin, nextLine)) {
	string columns[20];
	lineNum++;
	int numCols = ColumnSplitter(columns, nextLine);
	//Check for correct # of columns based on eclipse type
	if (columns[9].at(0) == 'P') {
		if(numCols != 16) {
			cerr << "INCORRECT COLUMNS for P-Type Eclipse on line: " << lineNum << endl;
		}
	} else {
		if(numCols != 18) {
			cerr << "INCORRECT COLUMNS for Non-P-Type Eclipse on line: " << lineNum << endl;
		}
	}

	//Check for unique catalog number
	//Update offset from id and lineNum

	//Print out in CSV format, loop except last column
	for(int i = 0; i < numCols-1; i++) {
		cout << columns[i] << ",";
	}
	cout << columns[numCols-1] << endl;

	//Read next line
	//getline(cin, nextLine);
	//lineNum++;
	//cout << "Before if" << endl;


	//cout << "After if" << endl;
	}

	//cout << "Done" << endl;

	return 0;
}

int ColumnSplitter(string columns[], const string nextLine) {

	//int charPos = 0;
	int columnPos = -1;
	int partStart = 0;
	int partEnd = 0;
	bool isWriting = false;
	for (unsigned int i = 0; i < nextLine.length(); i++) {
		//If the next character is a space...
		if(nextLine.at(i) == ' ') {

			if(isWriting) //Will run if the previous characters were part of a column
			{             //Helps ignore space between columns, regardless of number of spaces

				//Previously read char was the last in the column
				partEnd = i-1;
				//Writes the characters from the eclipse file that correspond to the next column
				columnPos += 1;
				columns[columnPos] = nextLine.substr(partStart, (partEnd-partStart+1));

				//cout << nextLine.substr(partStart, (partEnd-partStart+1)) << endl;

				//Indicate that the column characters are done being read, prepare for next column
				isWriting = false;
			}
			continue;
		}
		else { //If the next char isn't a space...

			if(isWriting) {
				//If this isn't the first char in the next column, keep reading the next char
				continue;
			} else {
				//This is the first char in the new column, so set start bound
				partStart = i;
				isWriting=true;
			}
		}
	}

	//Check to see if a column was still writing when the line ended
	if(isWriting) {
		partEnd = nextLine.size()-1;
		columnPos += 1;
		columns[columnPos] = nextLine.substr(partStart, (partEnd-partStart+1));
	}

	return columnPos+1;
}

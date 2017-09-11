//============================================================================
// Name        : EclipseR.cpp
// Author      : Trevor Fisher
// Version     : 0.1
// Description : Initial i/o implementation for EclipseR Software
//============================================================================

#include <iostream>
#include <string>
#include "EclipseR.h"
using namespace std;


int main() {
	//Instantiate array holding used IDs to false
	for(int i = 0; i < 20000; i++)
	{
		eclipseID[i] = false;
	}

	//Variable used to hold next line from the eclipse data text file
	string nextLine;
	//Keeps track of the line number from input file
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

	//Loop through input file until the end of file is reached (Does not work properly, processing getline() never returns)
	while(getline(cin, nextLine)) { //Must CTRL+Z to end program
		lineNum++;

		//string array to hold individual columns from file
		string columns[20];

		//Read nextLine, put parts into columns
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
		int idNum = stoi(columns[0]);
		if(eclipseID[idNum] == true) { //true means it's a duplicate ID
			cerr << "DUPLICATE ID FOUND: ID no. " << idNum << endl;
		} else {
			//Indicate an ID has been used
			eclipseID[idNum] = true;

			//Print out in CSV format, loop except last column
			for(int i = 0; i < numCols-1; i++) {
				cout << columns[i] << ",";
			}
			cout << columns[numCols-1] << endl;
		}

		if(!cin.good()) //DOES NOT WORK, attempt to break out if end of file is reached
		{
			break;
		}

	} //END while loop

	//cout << "Done" << endl;

	return 0;
} //END main() method

//Function to take the whitespace separated columns in nextLine and
//put them in individual string indexes in columns.
int ColumnSplitter(string columns[], const string nextLine) {

	//The data column being read (0 indexed)
	int columnPos = -1;
	//Char index in nextLine where column starts
	int partStart = 0;
	//Char index in nextLine where column ends
	int partEnd = 0;
	//Keeps track if a column or whitespace is currently traversed by char index
	bool isWriting = false;

	//Traverse the characters in nextLine
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

				//Indicate that the column characters are done being read, prepare for next column
				isWriting = false;
			}
			continue;
		} //END checking if next char is space
		else { //If the next char isn't a space...

			if(isWriting) {
				//If this isn't the first char in the next column, keep reading the next char
				continue;
			} else {
				//This is the first char in the new column, so set start bound
				partStart = i;
				isWriting=true;
			}
		} //END checking if character is not a space
	} //END for loop

	//Check to see if a column was still writing when the line ended
	if(isWriting) {
		partEnd = nextLine.size()-1;
		columnPos += 1;
		columns[columnPos] = nextLine.substr(partStart, (partEnd-partStart+1));
	}

	return columnPos+1; //Return number of columns as 1-indexed
} //END Function ColumnSplitter()

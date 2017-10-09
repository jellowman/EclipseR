//============================================================================
// Name        : EclipseR.cpp
// Author      : Trevor Fisher
// Version     : 0.2
// Description : Initial i/o implementation for EclipseR Software
//============================================================================


#include <iostream>
#include <string>
#include "EclipseR.h"
#include "Tarray.h"
#include "Eclipse.h"
using namespace std;


int main() {
	//Unit testing
	TestArrayTemplate();

	//Used to hold unique eclipse IDS, can't use Tarray until it has sorting implemented
	bool eclipseID[20000];

	//Instantiate array holding used IDs to false
	for(int i = 0; i < 20000; i++)
	{
		eclipseID[i] = false;
	}

	//Keep track of eclipses (just a string array of strings for now)
	Tarray<Eclipse> *eclipses = new Tarray<Eclipse>(10);

	//Variable used to hold next line from the eclipse data text file
	string nextLine;
	//Keeps track of the line number from input file, not including header lines
	int lineNum = 0;

	//Read past the "junk lines" at the beginning of the file
	for(int i = 0; i < 10; i++) {
		getline(cin, nextLine);
		//lineNum++;
	}

	//Loop through input file until the end of file is reached (Does not work properly, processing getline() never returns)
	//while(getline(cin, nextLine)) { //Must CTRL+Z to end program
	while(lineNum < 26) {
		getline(cin, nextLine);
		lineNum++;
		//string array to hold individual columns from file
		Tarray<string> *columnStrings = new Tarray<string>();

		//Read nextLine, put parts into columns
		int numCols = ColumnSplitter(columnStrings, nextLine);

		//Begin error checking content of lines
		//Hold type of eclipse
		int eclipseType = -1;

		//Check for correct # of columns based on eclipse type
		if ((columnStrings->Get(9)).at(0) == 'P') {
			//Set eclipse to partial type
			eclipseType = 0;
			if(numCols != 16) {
				cerr << "Error in data row " << lineNum << ": " << numCols <<
						" columns found. Should be 16." << endl;
				//Move to next line
				continue;
			}
		} else {
			//Set eclipse type to 'other'
			eclipseType = 1;
			if(numCols != 18) {
				cerr << "Error in data row " << lineNum << ": " << numCols <<
						" columns found. Should be 18." << endl;
				//Move to next line
				continue;
			}
		}

		//Test to see if Integer columns have integers
		bool isBad = false;
		switch(eclipseType) {
		case 1:
			//Check if column 17 is integer
			isBad = IsColumnNumber(columnStrings, 17, true, lineNum);
			//intentionally no break
		case 0:
			//Check if columns 1,2,3,5,7,8,9,15,and 16 are Integers
			isBad = IsColumnNumber(columnStrings, 1, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 2, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 3, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 5, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 7, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 8, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 9, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 15, true, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 16, true, lineNum) || isBad;

			//Check if 11 and 12 are decimals
			isBad = IsColumnNumber(columnStrings, 11, false, lineNum) || isBad;
			isBad = IsColumnNumber(columnStrings, 12, false, lineNum) || isBad;
			break;
		default:
			break;
		}

		//Break out to read next line if any number formatting failed
		if(isBad)
			continue;

		//Check for unique catalog number
		int idNum = 0;
		idNum = stoi(columnStrings->Get(0));
		if(eclipseID[idNum] == true) { //true means it's a duplicate ID
			cerr << "Error in data row " << lineNum << ": Duplicate catalog number "
					<< idNum << "." << endl;
		} else {
			//Indicate an ID has been used
			eclipseID[idNum] = true;

			//Add to eclipse array
			Eclipse *newEclipse = new Eclipse();
			newEclipse->SetParts(*columnStrings);
			eclipses->Add(*newEclipse);
		}

		if(!cin.good()) //DOES NOT WORK, attempt to break out if end of file is reached
		{
			break;
		}

	} //END while loop
	//Now, print out eclipses in reverse order
	for(int i = eclipses->Size() - 1; i >= 0; i--) {
		Eclipse anEclipse = eclipses->Get(i);
		//Tarray<string> anEclipse = eclipses->Get(i);
		//Print out each column in CSV format
		cout << anEclipse << endl;
		/*for(int j = 0; j < anEclipse.Size()-1; j++) {
			cout << anEclipse.Get(j) << ",";
		}
		cout << anEclipse.Get(anEclipse.Size()-1) << endl;*/
	}

	return 0;

} //END main() method

//Function to take the whitespace separated columns in nextLine and
//put them in individual string indexes in columns.
int ColumnSplitter(Tarray<string> *columnStrings, const string nextLine) {

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
				columnStrings->AddCopy(nextLine.substr(partStart, (partEnd-partStart+1)));

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
		columnStrings->AddCopy(nextLine.substr(partStart, (partEnd-partStart+1)));
	}
	/*if(columnPos+1 != columnStrings->Size())
	{
		cerr << "OH NO" << endl;
	}*/
	return columnPos+1; //Return number of columns as 1-indexed
} //END Function ColumnSplitter()

//Checks to see if column entry is numeric, returns true if bad formatting
bool IsColumnNumber(Tarray<string> *colStrings, int column, bool isInt, int lineNum) {
	bool isBad = false;
	if(isInt) { //Checking for integer
		//Iterate through string
		for(char c : colStrings->Get(column-1)) {
			if(!isdigit(c) && (c != '-')) {
				isBad = true;
				cerr << "Error in data row " << lineNum << ": Column " << column << " is not a whole number." << endl;
				break;
			}
		}
	} else { //Checking for double-formatted number
		int numDecimals = 0;
		//Iterate through string
		for(char c : colStrings->Get(column-1)) {
			if(c == '.')
				numDecimals++;
			if((!isdigit(c) && (c != '.') && (c != '-')) || numDecimals > 1) {
				isBad = true;
				cerr << "Error in data row " << lineNum << ": Column " << column << " is not a decimal number." << endl;
				break;
			}
		}
	}
	return isBad;
}

//UNIT TESTING for Tarray template
void TestArrayTemplate() {

	//Test on primitive data type
	Tarray<double> *testArray;
	testArray = new Tarray<double>(6);

	int i = testArray->ACTUAL_SIZE();
	if(i != 6)
	{
		cerr << "Specified size and/or Size() method not working" << endl;
	}

	delete testArray;

	testArray = new Tarray<double>();
	i = testArray->ACTUAL_SIZE();
	if(i != 10)
		cerr << "Default constructor not initializing to 10" << endl;

	double one = 1.0;
	double two = 2.3;
	double three = 3.5;

	testArray->Add(one);
	testArray->Add(two);
	testArray->Add(three);

	double diff = testArray->Get(1) - 2.3;
	if((diff < -0.001) || (diff > 0.001))
		cerr << "Get method not working" << endl;

	//Add 14 more numbers
	for(int i = 0; i < 14; i++) {
		//string test;
		//test = "test" + to_string(i);
		double tdouble = 4.1+i;
		testArray->Add(tdouble);
	}

	i = testArray->Size();
	//Array should have 17 elements now
	if(i != 17) {
		cerr << "dynamic sizing did not work" << endl;
		cerr << "Expected 17, size returned " << i << endl;
	}

	i = testArray->ACTUAL_SIZE();
	//Low level array should have 20 elements
	if(i != 20) {
		cerr << "low level sizing did not work" << endl;
		cerr << "Expected 20, size returned " << i << endl;
	}

	//Test Remove functions, remove 10 elements
	for(int j = 0; j < 10; j++) {
		testArray->RemoveAt(testArray->Size()-1);
	}

	i = testArray->ACTUAL_SIZE();
	//Low level array should have decreased back to size of 10
	if(i != 10) {
		cerr << "Remove method did not resize properly" << endl;
		cerr << "Expected 10, actual size is " << i << endl;
	}

	i = testArray->Size();
	//Array should only have 7 elements now
	if(i != 7) {
		cerr << "Improper size" << endl;
		cerr << "Expected 7, size returned is " << i << endl;
	}

	//Test print method
	cout << "Testing << operator format for array: ";
	cout << "[" << *testArray << "]" << endl;

	delete testArray;
	//Demonstrate template ability
	Tarray<Eclipse> *array2 = new Tarray<Eclipse>(3);

	Eclipse *ec1 = new Eclipse("one");
	Eclipse *ec2 = new Eclipse("two");

	array2->Add(*ec1);
	array2->Add(*ec1);
	array2->AddCopy(*ec1);

	ec1->SetName("yeah");
	array2->Add(*ec2);

	cout << (array2->Get(0)).GetName() << endl;
	cout << (array2->Get(1)).GetName() << endl;
	cout << (array2->Get(2)).GetName() << endl;
	cout << (array2->Get(3)).GetName() << endl;
	cout << ec1->GetName() << endl;

	cout << "Finished unit testing for array" << endl;
}

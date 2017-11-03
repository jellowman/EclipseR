//============================================================================
// Name        : EclipseR.cpp
// Author      : Trevor Fisher
// Version     : 1.0
// Description : Initial i/o implementation for EclipseR Software
//============================================================================


#include <iostream>
#include <string>
#include <fstream>
#include "EclipseR.h"
#include "Tarray.h"
#include "Eclipse.h"
#include "TList.h"
using namespace std;


int main() {
	//Unit testing
	//TestArrayTemplate();
	TestLinkedList();

	//Used to hold unique eclipse IDS, can't use Tarray until it has sorting implemented
	bool eclipseID[20000];

	//Instantiate array holding used IDs to false
	for(int i = 0; i < 20000; i++)
	{
		eclipseID[i] = false;
	}

	//Keep track of eclipses (Array of Eclispes)
	Tarray<Eclipse> *eclipses = new Tarray<Eclipse>(10);
	//Hold trash at beginning of file
	Tarray<string> *header = new Tarray<string>();

	//Keep track of valid eclipses and num of data lines read
	int dataTally = 0; int validTally = 0;

	//Enter data input loop
	bool anyData = false;
	bool exit = false;
	do {
		cout << "Enter the name of an eclipse data file." << endl;
		string fileName;
		getline(cin, fileName);

		if(!fileName.empty()) {
			ifstream inFS;
			inFS.open(fileName);
			if(!inFS.is_open()) {
				cout << "File is not available." << endl;
			} else {
				//Read file contents
				delete header;
				header = new Tarray<string>(10);
				bool hasData = ReadFile(inFS, eclipses, header, dataTally, validTally);
				//Data has been read at least once
				if(hasData) {
					anyData = true;
				}
			}

			inFS.close();
		} else { //Exit loop
			exit = true;
		}
	} while (!exit);

	if(!anyData) {
		return 0;
	}

	//Enter data manipulation loop
	int sortedBy = -1;
	exit = false;
	do {
		cout << "Choose to (O)utput, (S)ort, (F)ind, or (Q)uit" << endl;
		string userChoice;
		getline(cin, userChoice);
		char firstLetter = userChoice[0];

		switch (firstLetter) {
		case 'o':
		case 'O':
			//Output data
			OutputValues(eclipses, header, dataTally, validTally);
			break;
		case 's':
		case 'S':
			//Sort data
			SortValues(eclipses, sortedBy);
			break;
		case 'f':
		case 'F':
			//Find data
			FindValues(eclipses, sortedBy, header);
			break;
		case 'q':
		case 'Q':
			//Quit Program
			cout << "~~~Goodbye~~~" << endl;
			exit = true;
			break;
		default:
			cout << "Invalid action, please enter O, S, F, or Q." << endl;
			break;
		}
	} while (!exit);

	//END PROGRAM
}

//Reads in
bool ReadFile(ifstream& inFS, Tarray<Eclipse> *eclipses, Tarray<string> *header, int& dataTally, int& validTally) {

	//bool eclipseID[20000];
	//Variable used to hold next line from the eclipse data text file
	string nextLine;
	//Keeps track of the line number from input file, not including header lines
	int lineNum = 0;

	//Read past the "junk lines" at the beginning of the file
	for(int i = 0; i < 10; i++) {
		getline(inFS, nextLine);
		header->add(nextLine);
		//lineNum++;
	}

	//Loop through input file until the end of file is reached
	while(getline(inFS, nextLine)) {
	//while(lineNum < 26) {
		//getline(inFS, nextLine);
		lineNum++;
		dataTally++;
		//string array to hold individual columns from file
		Tarray<string> *columnStrings = new Tarray<string>();

		//Read nextLine, put parts into columns
		int numCols = ColumnSplitter(columnStrings, nextLine);

		//Begin error checking content of lines
		//Hold type of eclipse
		bool isBad = false;
		int eclipseType = -1;

		//Check for correct # of columns based on eclipse type
		if ((columnStrings->get(9)).at(0) == 'P') {
			//Set eclipse to partial type
			eclipseType = 0;
			if(numCols != 16) {
				cerr << "Error in data row " << lineNum << ": " << numCols <<
						" columns found. Should be 16." << endl;
				isBad = true;
				continue;
			}
		} else {
			//Set eclipse type to 'other'
			eclipseType = 1;
			if(numCols != 18) {
				cerr << "Error in data row " << lineNum << ": " << numCols <<
						" columns found. Should be 18." << endl;
				isBad = true;
				continue;
			}
		}
		//Test to see if Integer columns have integers
		switch(eclipseType) {
		case 1:
			//Check if column 17 is integer
			isBad = IsColumnNumber(columnStrings, 17, true, lineNum) || isBad;
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

		validTally++;
		//Check for unique catalog number
		int idNum = 0;
		idNum = stoi(columnStrings->get(0));
		//Iterate through eclipses to compare idNums
		bool uniqueID = true;
		int indexOfDuplicate = 0;
		for(int i = 0; i < eclipses->size(); i++) {
			if(idNum == eclipses->get(i).getID()) {
				uniqueID = false;
				indexOfDuplicate = i;
			}
		}
		if(!uniqueID) { //Duplicate ID found, replace
			cerr << "Error in data row " << lineNum << ": Duplicate catalog number "
					<< idNum << "." << endl;

			Eclipse *newEclipse = new Eclipse();
			newEclipse->setParts(*columnStrings);
			eclipses->replaceAt(*newEclipse, indexOfDuplicate);
		} else {
			//Indicate an ID has been used
			//eclipseID[idNum] = true;

			//Add to eclipse array
			Eclipse *newEclipse = new Eclipse();
			newEclipse->setParts(*columnStrings);
			eclipses->add(*newEclipse);
		}

		if(!inFS.good()) //DOES NOT WORK, attempt to break out if end of file is reached
		{
			cerr << "Standard input not good" << endl;
			break;
		}

	} //END while loop

	bool hasData = (eclipses->size() > 0) ? true : false;
	return hasData;
} //END ReadFile method

void OutputValues(Tarray<Eclipse>* eclipses, Tarray<string>* header, int& dataTally, int& validTally) {
	cout << "Enter the name of the output file:" << endl;
	string outFileName;
	getline(cin, outFileName);
	if(!outFileName.empty()) {
		ofstream outFile;
		outFile.open(outFileName);
		if(!outFile.is_open()) {
			cerr << "File is not available." << endl;
			return;
		}
		//Now, print out eclipses
		for(int i = 0; i < header->size(); i++) {
			outFile << header->get(i) << endl;
		}
		for(int i = 0; i < eclipses->size(); i++) {
			Eclipse anEclipse = eclipses->get(i);
			outFile << anEclipse << endl;
		}
		outFile << "Data lines read: " << dataTally << "; Valid eclipses read: " << validTally <<
				"; Eclipses in memory: " << eclipses->size() << endl;
		outFile.close();
	} else {
		//Now, print out eclipses
		for(int i = 0; i < header->size(); i++) {
			cout << header->get(i) << endl;
		}
		for(int i = 0; i < eclipses->size(); i++) {
			Eclipse anEclipse = eclipses->get(i);
			cout << anEclipse << endl;
		}
		cout << "Data lines read: " << dataTally << "; Valid eclipses read: " << validTally <<
			"; Eclipses in memory: " << eclipses->size() << endl;
	}

} //END OutputValues method

void SortValues(Tarray<Eclipse>* eclipses, int& sortBy) {

	cout << "Select a data field to sort from 1-18" << endl;
	string nextLine;
	getline(cin, nextLine);
	try{
		int colNum = stoi(nextLine);
		if(colNum > 18 || colNum < 1) {
			cout << "Invalid column specified." << endl;
			return;
		}
		ColumnSort(*eclipses, colNum-1);
		sortBy = colNum;
	} catch(invalid_argument& ar) {
		cout << "Invalid column specified." << endl;
	}
}

void FindValues(Tarray<Eclipse>* eclipses, int& sortedBy, Tarray<string>* header) {

	cout<< "Select a data field to find from 1-18" << endl;
	string nextLine;
	string searchTerm;
	getline(cin, nextLine);
	try{
		int min = -1;
		int max = -1;
		Tarray<int>* matches = new Tarray<int>();
		int colNum = stoi(nextLine);
		//Check for valid column number
		if(colNum > 18 || colNum < 1) {
					cout << "Invalid column specified." << endl;
					return;
		}
		//Check if column was month entry
		if(colNum == 4) {
			cout << "Enter a month abbreviation to search for." << endl;
			getline(cin, searchTerm);
			Month mon = StrMo(searchTerm);
			if(mon >= 12) {
				cout << "Invalid month specified" << endl;
				return;
			}
		}
		//Get any value to search for
		else {
		cout << "Pick a value to search for." << endl;
		getline(cin, searchTerm);
		}

		if(searchTerm.empty() && ((colNum == 17) || (colNum == 18))) {
			searchTerm = "P";
			colNum = 10;
		} else if(searchTerm.empty()) {
			return;
		}
		//Determine to sort by binary or linear
		if(colNum == sortedBy) {
			ColumnSearchBinary(*eclipses, searchTerm, colNum-1, min, max);

			//Print out values between min and max, if there were any found
			for(int i = 0; i < header->size(); i++) {
				cout << header->get(i) << endl;
			}
			if(min != -1) {
				for(int i = min; i <= max; i++) {
					cout << eclipses->get(i) << endl;
				}
			}
			cout << "Eclipses found: " << max-min << endl;
		} else {
			ColumnSearch(*eclipses, searchTerm, colNum-1, *matches);

			//Print out values specified in array
			for(int i = 0; i < header->size(); i++) {
				cout << header->get(i) << endl;
			}
			for(int i = 0; i < matches->size(); i++) {
				cout << eclipses->get(matches->get(i)) << endl;
			}
			cout << "Eclipses found: " << matches->size() << endl;
		}
	} catch(invalid_argument& ar) {

	}
}

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
				columnStrings->addCopy(nextLine.substr(partStart, (partEnd-partStart+1)));

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
		columnStrings->addCopy(nextLine.substr(partStart, (partEnd-partStart+1)));
	}

	return columnPos+1; //Return number of columns as 1-indexed
} //END Function ColumnSplitter()

//Checks to see if column entry is numeric, returns true if bad formatting
bool IsColumnNumber(Tarray<string> *colStrings, int column, bool isInt, int lineNum) {
	bool isBad = false;
	int i = 0;

	if(isInt) { //Checking for integer
		//Iterate through string
		for(char c : colStrings->get(column-1)) {
			if(i == 0) {
				if(!isdigit(c) && (c != '-')) {
					isBad = true;
					cerr << "Error in data row " << lineNum << ": Column " << column << " is not a whole number." << endl;
					break;
				}
			} else {
				if(!isdigit(c)) {
					isBad = true;
					cerr << "Error in data row " << lineNum << ": Column " << column << " is not a whole number." << endl;
					break;
				}
			}
			i++;
		}
	} else { //Checking for double-formatted number
		int numDecimals = 0;
		//Iterate through string
		for(char c : colStrings->get(column-1)) {

			if(i == 0) {
				if(c == '.')
					numDecimals++;
				if((!isdigit(c) && (c != '.') && (c != '-')) || numDecimals > 1) {
					isBad = true;
					cerr << "Error in data row " << lineNum << ": Column " << column << " is not a decimal number." << endl;
					break;
				}
			}
			else {
				if(c == '.')
					numDecimals++;
				if((!isdigit(c) && (c != '.')) || numDecimals > 1) {
					isBad = true;
					cerr << "Error in data row " << lineNum << ": Column " << column << " is not a decimal number." << endl;
					break;
				}
			}
			i++;
		}
	}
	return isBad;
}

//UNIT TESTING for Tarray template
void TestArrayTemplate() {

	//Test on primitive data type
	Tarray<double> *testArray;
	testArray = new Tarray<double>(6);

	int i = testArray->DEBUG_ACTUAL_SIZE();
	if(i != 6)
	{
		cerr << "Specified size and/or Size() method not working" << endl;
	}

	delete testArray;

	testArray = new Tarray<double>();
	i = testArray->DEBUG_ACTUAL_SIZE();
	if(i != 10)
		cerr << "Default constructor not initializing to 10" << endl;

	double one = 1.0;
	double two = 2.3;
	double three = 3.5;

	testArray->add(one);
	testArray->add(two);
	testArray->add(three);

	double diff = testArray->get(1) - 2.3;
	if((diff < -0.001) || (diff > 0.001))
		cerr << "Get method not working" << endl;

	//Add 14 more numbers
	for(int i = 0; i < 14; i++) {
		//string test;
		//test = "test" + to_string(i);
		double tdouble = 4.1+i;
		testArray->add(tdouble);
	}

	i = testArray->size();
	//Array should have 17 elements now
	if(i != 17) {
		cerr << "dynamic sizing did not work" << endl;
		cerr << "Expected 17, size returned " << i << endl;
	}

	i = testArray->DEBUG_ACTUAL_SIZE();
	//Low level array should have 20 elements
	if(i != 20) {
		cerr << "low level sizing did not work" << endl;
		cerr << "Expected 20, size returned " << i << endl;
	}

	//Test Remove functions, remove 10 elements
	for(int j = 0; j < 10; j++) {
		testArray->removeAt(testArray->size()-1);
	}

	i = testArray->DEBUG_ACTUAL_SIZE();
	//Low level array should have decreased back to size of 10
	if(i != 10) {
		cerr << "Remove method did not resize properly" << endl;
		cerr << "Expected 10, actual size is " << i << endl;
	}

	i = testArray->size();
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

	array2->add(*ec1);
	array2->add(*ec1);
	Eclipse* ec1Copy = new Eclipse(*ec1);
	array2->add(*ec1Copy);

	Tarray<Eclipse> *array3 = new Tarray<Eclipse>(*array2);
	ec1->setName("yeah");
	array2->add(*ec2);

	cout << (array2->get(0)).getName() << endl;
	cout << (array2->get(1)).getName() << endl;
	cout << (array2->get(2)).getName() << endl;
	cout << (array2->get(3)).getName() << endl;
	cout << ec1->getName() << endl;

	cout << "Other array:" << endl;

	cout << (array3->get(0)).getName() << endl;
	cout << (array3->get(1)).getName() << endl;
	cout << (array3->get(2)).getName() << endl;


	cout << "Finished unit testing for array" << endl;
}

//Unit testing for templated linked list
void TestLinkedList() {
	cout << "Begin LL Testing:" << endl;
	TList<string>* eclipses = new TList<string>();

	string one = "One";
	eclipses->add(one);

	string oneCopy = eclipses->getAt(0);
	cout << "The item received from the list is: " << oneCopy << endl;

	string two = "Two";
	string three = "Three";
	eclipses->add(two);
	eclipses->add(three);

	cout << "---Printing out Linked List---" << endl;
	cout << eclipses << endl;
	cout << "---End of Linked List Printing---" << endl;

	cout << "Size: " << eclipses->size() << endl;

	string four = "Four";
	eclipses->insertAt(four, 4);

	string five = "Five";
	eclipses->insertAt(five, 10);

	cout << "---Printing out Linked List---" << endl;
	cout << eclipses << endl;
	cout << "---End of Linked List Printing---" << endl;

	cout << "Size: " << eclipses->size() << endl;
	eclipses->remove();

	cout << "---Printing out Linked List---" << endl;
	cout << eclipses << endl;
	cout << "---End of Linked List Printing---" << endl;

	cout << "Size: " << eclipses->size() << endl;

	cout << "---Test Finding---" << endl;
	string six = "Six";

	cout << "Index of six is: " << eclipses->find(six) << endl;

	eclipses->insertAt(six, 3);
	cout << eclipses << endl;
	cout << "Now index of six is: " << eclipses->find(six) << endl;

	eclipses->removeAt(eclipses->find(six));
	eclipses->removeAt(eclipses->find(one));

	cout << eclipses << endl;
	cout << eclipses->size() << endl;

	cout << "End LL Testing:" << endl;
}

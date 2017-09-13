/*
 * Author: Trevor Fisher
 * Version: 0.2
 * Description: Initializes variables for the main method for EclipseR
 */
#include "Tarray.h"
using namespace std;

//Main method of program, handles input and output for eclipse data
int main();

//Function to take the whitespace separated columns in nextLine and
//put them in individual string indexes in columns.
//stringColumns - Array to hold individual columns in their own separated index
//nextLine  - String containing the next line from the input file to parse into columns[]
//returns - the number of columns in nextLine
int ColumnSplitter(Tarray<string> *stringColumns, const string nextLine);

//Function to scan a column to confirm if it is the right numeric type
//colString - The column in string form
//column - The column number
//isInt - Determine if the string is an int or double, float, etc.
//lineNum - The line in the input file
bool IsColumnNumber(Tarray<string> *colStrings, int column, bool isInt, int lineNum);



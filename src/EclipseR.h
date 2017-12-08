/*
 * Author: Trevor Fisher
 * Version: 0.2
 * Description: Initializes variables for the main method for EclipseR
 */
#include "Tarray.h"
#include "Eclipse.h"
#include "TList.h"
#include "THashMap.h"
#include "TavlTree.h"
using namespace std;

//Main method of program, handles input and output for eclipse data
int main();

//Reads from a file specified by inFS and parses data into eclipse Linked List
bool ReadFile(ifstream& inFS, TAVLTree<Eclipse> *eclipseTree, Tarray<string> *header, int& dataTally, int& validTally);

//Reads from a file specified by inFS and parses eclipseData. Removes matching eclipses from Linked List
bool ReadFileDel(ifstream& inFS, TAVLTree<Eclipse> *eclipseTree, Tarray<string> *header);

//Merges new eclipse data into the Linked List
void MergeData(TAVLTree<Eclipse> *eclipseTree, int& dataTally, int& validTally);

//Deletes eclipse data from the Linked List that matches eclipses specified in the purge data file
void PurgeData(TAVLTree<Eclipse> *eclipseTree, int& dataTally, int& validTally);

//Output values to a file
void OutputValues(Tarray<Eclipse>* eclipses, Tarray<string>* header, int& dataTally, int& validTally);

//Sort values
void SortValues(Tarray<Eclipse>* eclipses, int& sortBy);

//Find values
void FindValues(Tarray<Eclipse>* eclipses, int& sortBy, Tarray<string>* header, THashMap<Eclipse>* eMap);

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

//Unit test for templated array class
void TestArrayTemplate();

//Unit test for templated Linked List class
void TestLinkedList();



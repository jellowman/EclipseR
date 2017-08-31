/*
 * Author: Trevor Fisher
 * Version: 0.1
 * Description: Initializes variables for the main method for EclipseR
 */
using namespace std;

//Array used to hold already entered eclipse IDs
bool eclipseID[20000];

//Function to take the whitespace separated columns in nextLine and
//put them in individual string indexes in columns.
int ColumnSplitter(string columns[], const string nextLine);

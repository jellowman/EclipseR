/*
 * Eclipse.h
 *
 *  Created on: Sep 18, 2017
 *      Author: trevorfisher
 */

#ifndef ECLIPSE_H_
#define ECLIPSE_H_

#include <string>
#include "Tarray.h"
using namespace std;

enum Month {Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, other};
class Eclipse {
public:
	Eclipse();
	~Eclipse();
	Eclipse(string name);
	Eclipse(const Eclipse& otherEcl);
	void operator=(const Eclipse& otherEcl);
	bool operator==(const Eclipse& otherEcl) const;
	bool operator>(const Eclipse& otherEcl) const;
	bool operator<(const Eclipse& otherEcl) const;
	string getName();
	string getCol(int i) const;
	bool isEmpty() const;
	int getID();
	void setName(string newName);
	void setParts(const Tarray<string>& otherParts);
	Month getMonth() const;
	int compareTo(const Eclipse& otherEclipse, int numCol) const;
	int compare(const Eclipse& otherEclipse) const;
	int compareToStr(const string& term, int numCol) const;
	int getKey() const;
	friend ostream& operator << (ostream& os, const Eclipse& theEclipse);
private:
	string* name;
	Tarray<string>* parts;

};

//Convert from string to Month enum
Month StrMo(string mon);

//Sort eclipses based on the specified data column
void ColumnSort(Tarray<Eclipse>& eclipses, int colNum);

//Search eclipses based on specified data column using binary search. Stores min and max range of matching value
//In respective integers passed in
void ColumnSearchBinary(Tarray<Eclipse>& eclipses, string searchTerm, int colNum, int& min, int& max);

//Search eclipses linearly. Holds array of integers for matching indexes
void ColumnSearch(Tarray<Eclipse>& eclipses, string searchTerm, int colNum, Tarray<int>& matches);

#endif /* ECLIPSE_H_ */

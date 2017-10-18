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
	Eclipse& operator=(const Eclipse& otherEcl);
	string getName();
	string getCol(int i) const;
	int getID();
	void setName(string newName);
	void setParts(const Tarray<string>& otherParts);
	Month getMonth() const;
	int compareTo(const Eclipse& otherEclipse, int numCol);
	friend ostream& operator << (ostream& os, const Eclipse& theEclipse);
private:
	string* name;
	Tarray<string>* parts;

};

void ColumnSort(Tarray<Eclipse>& eclipses, int colNum);

#endif /* ECLIPSE_H_ */

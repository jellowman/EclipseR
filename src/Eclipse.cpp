/*
 * Eclipse.cpp
 *
 *  Created on: Sep 18, 2017
 *      Author: trevorfisher
 */

#include "Eclipse.h"
#include <string>
#include "Tarray.h"

Eclipse::Eclipse() {
	name = new string("");
	parts = new Tarray<string>(18);
}

Eclipse::~Eclipse() {
	delete name;
	delete parts;
}

Eclipse::Eclipse(string name) {
	this->name = new string;
	*(this->name) = name;
	parts = new Tarray<string>(18);
	return;
}

//Copy constructor
Eclipse::Eclipse(const Eclipse& otherEclipse) {
	name = new string;
	name->assign(*(otherEclipse.name));
	parts = new Tarray<string>(*(otherEclipse.parts));
	return;
}

//Equals assignment operator
Eclipse& Eclipse::operator=(const Eclipse& otherEclipse) {
	delete name;
	delete parts;
	this->name = otherEclipse.name;
	this->parts = otherEclipse.parts;
	return *this;
}
void Eclipse::setName(string newName) {
	name->assign(newName);
}

void Eclipse::setParts(const Tarray<string>& otherParts) {
	delete this->parts;
	parts = new Tarray<string>(otherParts);
	return;
}

string Eclipse::getName() {
	return *name;
}

string Eclipse::getCol(int colNum) const{
	return parts->get(colNum);
}

int Eclipse::getID() {
	return stoi(parts->get(0));
}

Month Eclipse::getMonth() const{
	if(parts->get(3) == "Jan") {
		return Jan;
	} else if(parts->get(3) == "Feb") {
		return Feb;
	} else if(parts->get(3) == "Mar") {
		return Feb;
	} else if(parts->get(3) == "Apr") {
		return Feb;
	} else if(parts->get(3) == "May") {
		return Feb;
	} else if(parts->get(3) == "Jun") {
		return Feb;
	} else if(parts->get(3) == "Jul") {
		return Feb;
	} else if(parts->get(3) == "Aug") {
		return Feb;
	} else if(parts->get(3) == "Sep") {
		return Feb;
	} else if(parts->get(3) == "Oct") {
		return Feb;
	} else if(parts->get(3) == "Nov") {
		return Feb;
	} else if(parts->get(3) == "Dec") {
		return Feb;
	} else {
		return other;
	}
}

//Comparison method for sorting
int Eclipse::compareTo(const Eclipse& otherEclipse, int numCol) {
	int diff;
	double dubDiff;
	switch(numCol) {
	//Look at integer cases
	case 0:
	case 1:
	case 2:
	case 4:
	case 6:
	case 7:
	case 8:
	case 14:
	case 15:
		diff = stoi(this->getCol(numCol));
		diff = diff - stoi(otherEclipse.getCol(numCol));
		return diff;
		break;
	//Look at double cases
	case 10:
	case 11:
		dubDiff = stod(this->getCol(numCol)) - stod(otherEclipse.getCol(numCol));
		if(dubDiff > 0.000001) {
			return 1;
		} else if (dubDiff < 0.000001) {
			return -1;
		} else {
			return 0;
		}
		break;
	//Month Comparison
	case 3:
		return this->getMonth() - otherEclipse.getMonth();
		break;
	//16 not in P type, check int
	case 16:
		if((this->getCol(9) != "P") && (otherEclipse.getCol(9) == "P")) {
			return -1;
		} else if((this->getCol(9) == "P") && (otherEclipse.getCol(9) != "P")) {
			return 1;
		} else if((this->getCol(9) == "P") && (otherEclipse.getCol(9) == "P")) {
			return 0;
		} else {
			diff = stoi(this->getCol(numCol));
			diff = diff - stoi(otherEclipse.getCol(numCol));
			return diff;
		}
		break;
	//17 not in P type, check string
	case 17:
		if((this->getCol(9) != "P") && (otherEclipse.getCol(9) == "P")) {
			return -1;
		} else if((this->getCol(9) == "P") && (otherEclipse.getCol(9) != "P")) {
			return 1;
		} else if((this->getCol(9) == "P") && (otherEclipse.getCol(9) == "P")) {
			return 0;
		} else {
			return this->getCol(numCol).compare(otherEclipse.getCol(numCol));
		}
		break;
	//Other cases are string
	default:
		return this->getCol(numCol).compare(otherEclipse.getCol(numCol));
		break;
	}
	if(numCol == 0) {

	}
	else if(numCol == 3) {
		return (this->getMonth() - otherEclipse.getMonth());
	}
}

std::ostream& operator << (ostream& os, const Eclipse& theEclipse) {
	os << *(theEclipse.parts);
	return os;
}

//Implements Binary Insertion Sort and compares based on specified column
void ColumnSort(Tarray<Eclipse>& eclipses, int colNum) {
	//For the ith element
	for(int i = 1; i < eclipses.size(); i++) {
		//Start at midpoint
		int comparison;
		int insertAt = i;
		int min = 0;
		int max = i;
		int compareVal = (min+max)/2;
		do {
		comparison = eclipses.get(i).compareTo(eclipses.get(compareVal), colNum);
		if (comparison == 0) {
			break;
		}
		else if(comparison < 0) {
			//Reduce to left half, min stays same
			max = compareVal;
			compareVal = (compareVal+min)/2;

		} else {
			//Reduce to right half, max stays same
			min = compareVal;
			compareVal = (max+compareVal)/2;
		}
		} while(min-max > 1);
		insertAt = compareVal+1;

		//Move elements
		for(int j = i; j > insertAt; j--) {
			eclipses.swap(j-1, j);
		}
	} //End Loop for element
}

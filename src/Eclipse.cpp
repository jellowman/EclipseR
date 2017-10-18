/*
 * Eclipse.cpp
 *
 *  Created on: Sep 18, 2017
 *      Author: trevorfisher
 */

#include "Eclipse.h"
#include <string>
#include "Tarray.h"
#include <iomanip>

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
	//delete name;
	//delete parts;
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
		return Mar;
	} else if(parts->get(3) == "Apr") {
		return Apr;
	} else if(parts->get(3) == "May") {
		return May;
	} else if(parts->get(3) == "Jun") {
		return Jun;
	} else if(parts->get(3) == "Jul") {
		return Jul;
	} else if(parts->get(3) == "Aug") {
		return Aug;
	} else if(parts->get(3) == "Sep") {
		return Sep;
	} else if(parts->get(3) == "Oct") {
		return Oct;
	} else if(parts->get(3) == "Nov") {
		return Nov;
	} else if(parts->get(3) == "Dec") {
		return Dec;
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
		try {
		diff = stoi(this->getCol(numCol));
		diff = diff - stoi(otherEclipse.getCol(numCol));
		return diff;
		} catch (invalid_argument ar) {
			cerr << "Error converting integers:" << this->getCol(numCol) << " and " << otherEclipse.getCol(numCol) << endl;
			return 0;
		}
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
		return (this->getMonth() - otherEclipse.getMonth());
		break;
	//16 not in P type, check int
	case 16:
		if((this->getCol(9).at(0) != 'P') && (otherEclipse.getCol(9).at(0) == 'P')) {
			return -1;
		} else if((this->getCol(9).at(0) == 'P') && (otherEclipse.getCol(9).at(0) != 'P')) {
			return 1;
		} else if((this->getCol(9).at(0) == 'P') && (otherEclipse.getCol(9).at(0) == 'P')) {
			return 0;
		} else {
			diff = stoi(this->getCol(numCol));
			diff = diff - stoi(otherEclipse.getCol(numCol));
			return diff;
		}
		break;
	//17 not in P type, check string
	case 17:
		if((this->getCol(9).at(0) != 'P') && (otherEclipse.getCol(9).at(0) == 'P')) {
			return -1;
		} else if((this->getCol(9).at(0) == 'P') && (otherEclipse.getCol(9).at(0) != 'P')) {
			return 1;
		} else if((this->getCol(9).at(0) == 'P') && (otherEclipse.getCol(9).at(0) == 'P')) {
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
	//os << *(theEclipse.parts);
	os << setw(5) << right << theEclipse.parts->get(0) << setw(5) << theEclipse.parts->get(1);
	os << setw(7) << theEclipse.parts->get(2) << setw(4) << theEclipse.parts->get(3);
	os << setw(3) << theEclipse.parts->get(4) << setw(10) << theEclipse.parts->get(5);
	os << setw(7) << theEclipse.parts->get(6) << setw(7) << theEclipse.parts->get(7);
	os << setw(5) << theEclipse.parts->get(8) << setw(4) << theEclipse.parts->get(9);
	os << setw(10) << theEclipse.parts->get(10) << setw(8) << theEclipse.parts->get(11);
	os << setw(7) << theEclipse.parts->get(12) << setw(7) << theEclipse.parts->get(13);
	os << setw(4) << theEclipse.parts->get(14) << setw(5) << theEclipse.parts->get(15);
	if(theEclipse.parts->get(9).at(0) != 'P') {
		os << setw(5) << theEclipse.parts->get(16) << setw(8) << theEclipse.parts->get(17);
	}

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
			//cout << eclipses.get(i) << "} is equal to{" << eclipses.get(compareVal) << endl;
			insertAt = compareVal;
			break;
		}
		else if(comparison < 0) {
			//Reduce to left half, min stays same
			//cout << eclipses.get(i) << "} is less than{" << eclipses.get(compareVal).getCol(colNum) << endl;
			max = compareVal;
			compareVal = (compareVal+min)/2;

		} else {
			//Reduce to right half, max stays same
			//cout << eclipses.get(i) << "} is greater than{" << eclipses.get(compareVal) << endl;
			min = compareVal;
			compareVal = (max+compareVal)/2;
		}
		//cout << "Max is: " << max << " | Min is: " << min << endl;
		} while((max-min) > 1);
		comparison = eclipses.get(i).compareTo(eclipses.get(compareVal), colNum);
		if(compareVal == min) {
			insertAt = (comparison <= 0) ? compareVal : compareVal+1;
		} else if(compareVal == max) {
			insertAt = (comparison <= 0) ? compareVal-1 : compareVal;
		}
		//cout << "inserting at: " << insertAt << endl;

		//Move elements
		Eclipse *temp = new Eclipse();
		*temp = eclipses.get(i);
		for(int j = i; j > insertAt; j--) {
			eclipses.replaceAt(eclipses.get(j-1), j);
		}
		eclipses.replaceAt(*temp, insertAt);
		temp = 0;
	} //End Loop for element
}

//Search by binary
void ColumnSearchBinary(Tarray<Eclipse>& eclipses, string searchTerm, int colNum, int& min, int& max) {
	int lmin; int lmax; int lmid;
	if(colNum > 15) {

	} else {
		lmin = 0; lmax = eclipses.size(); lmid = (lmin+lmax)/2;
		//Search for min value
		while((min == -1) && (lmin-lmax >= 1)) {
			if(eclipses.get(lmid).getCol(colNum).compare(searchTerm) == 0) {
				if(lmid == 0) {
					min = 0;
				} else if (eclipses.get(lmid-1).getCol(colNum).compare(searchTerm) != 0) {
					min = lmid;
				} else {
					lmax = lmid;
					lmid = (lmin+lmax)/2;
				}
			} else if(eclipses.get(lmid).getCol(colNum).compare(searchTerm) < 0) {
				lmax = lmid-1;
				lmid = (lmin+lmax)/2;
			} else {
				lmin = lmid + 1;
				lmid = (lmin+lmax)/2;
			}
		}
		lmin = min; lmax = eclipses.size()-1; lmid = (lmin+lmax)/2;
		//Search for max val
		while((max == -1) && (lmin-lmax >= 1)) {
			if(eclipses.get(lmid).getCol(colNum).compare(searchTerm) == 0) {
				if(lmid == eclipses.size()-1) {
					max = lmax;
				} else if (eclipses.get(lmid+1).getCol(colNum).compare(searchTerm) != 0) {
					max = lmax;
				} else {
					lmin = lmid;
					lmid = (lmin+lmax)/2;
				}
			} else if(eclipses.get(lmid).getCol(colNum).compare(searchTerm) < 0) {
				lmax = lmid-1;
				lmid = (lmin+lmax)/2;
			} else {
				lmin = lmid+1;
				lmid = (lmin+lmax)/2;
			}
		}
	}
}

//Search Linearly
void ColumnSearch(Tarray<Eclipse>& eclipses, string searchTerm, int colNum, Tarray<int>& matches) {
	if(colNum > 15) {
		for(int i = 0; i < eclipses.size(); i++) {
			if(eclipses.get(i).getCol(9).at(0) == 'P') {

			} else if (eclipses.get(i).getCol(colNum).compare(searchTerm) == 0) {
				matches.add(i);
			}
		}
	} else {
		for(int i = 0; i < eclipses.size(); i++) {
			if(eclipses.get(i).getCol(colNum).compare(searchTerm) == 0) {
				matches.add(i);
			}
		}
	}
}

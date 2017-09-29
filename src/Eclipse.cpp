/*
 * Eclipse.cpp
 *
 *  Created on: Sep 18, 2017
 *      Author: trevorfisher
 */

#include "Eclipse.h"
#include <string>

Eclipse::Eclipse() {
	name = new string("");
	parts = new Tarray<string>(18);
}

Eclipse::Eclipse(string name) {
	this->name = new string;
	*(this->name) = name;
	parts = new Tarray<string>(18);

}

//Copy constructor
Eclipse::Eclipse(const Eclipse& otherEclipse) {
	name = new string;
	this->name->assign(*otherEclipse.name);
	this->parts = new Tarray<string>(otherEclipse.parts);
	return;
}

//Equals assignment operator
Eclipse& Eclipse::operator=(const Eclipse& otherEclipse) {
	this->name = otherEclipse.name;
	return *this;
}
void Eclipse::SetName(string newName) {
	delete name;
	name = new string(newName);
}

void Eclipse::SetParts(Tarray<string>& otherParts) {
	delete this->parts;
	*(this->parts) = otherParts;
}

string Eclipse::GetName() {
	return *name;
}

std::ostream& operator << (ostream& os, Eclipse& theEclipse) {
	os << *(theEclipse.parts);
	return os;
}

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
}

Eclipse::Eclipse(string name) {
	this->name = new string;
	*(this->name) = name;
}

Eclipse::Eclipse(const Eclipse& otherEclipse) {
	name = new string;
	this->name->assign(*otherEclipse.name);
	return;
}

Eclipse& Eclipse::operator=(const Eclipse& otherEclipse) {
	this->name = otherEclipse.name;
	return *this;
}
void Eclipse::SetName(string newName) {
	delete name;
	name = new string(newName);
}

string Eclipse::GetName() {
	return *name;
}

std::ostream& operator << (ostream& os, Eclipse& theEclipse) {
	os << *(theEclipse.name);
	return os;
}

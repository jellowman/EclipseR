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

class Eclipse {
public:
	Eclipse();
	Eclipse(string name);
	Eclipse(const Eclipse& otherEcl);
	Eclipse& operator=(const Eclipse& otherEcl);
	string GetName();
	void SetName(string newName);
	void SetParts(const Tarray<string>& otherParts);
	friend ostream& operator << (ostream& os, const Eclipse& theEclipse);
private:
	string* name;
	Tarray<string>* parts;

};


#endif /* ECLIPSE_H_ */

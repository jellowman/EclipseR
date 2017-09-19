/*
 * Eclipse.h
 *
 *  Created on: Sep 18, 2017
 *      Author: trevorfisher
 */

#ifndef ECLIPSE_H_
#define ECLIPSE_H_

#include <string>
using namespace std;

class Eclipse {
	friend ostream& operator << (ostream& os, Eclipse& theEclipse);
public:
	Eclipse();
	Eclipse(string name);
	Eclipse(const Eclipse& otherEcl);
	Eclipse& operator=(const Eclipse& otherEcl);
	string GetName();
	void SetName(string newName);
private:
	string* name;
};


#endif /* ECLIPSE_H_ */

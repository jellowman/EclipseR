/*
 * Bool.h
 *
 *  Created on: Sep 12, 2017
 *      Author: trevorfisher
 */
/*
=====================UNUSED====================
#ifndef BOOL_H_
#define BOOL_H_

class Bool {
private:
	bool theBool;
public:
	Bool();
	Bool(bool theBool);
	bool Get();
	void operator=(bool otherBool);
};

Bool::Bool() {
	theBool = false;
	return;
}

Bool::Bool(bool theBool) {
	this->theBool = theBool;
	return;
}

bool Bool::Get() {
	return theBool;
}

void Bool::operator=(bool otherBool) {
	theBool = otherBool;
	return;
}







#endif /* BOOL_H_ */

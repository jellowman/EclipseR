//============================================================================
// Name        : Tarray.h
// Author      : Trevor Fisher
// Version     : 0.2
// Description : Tarray (Terrific Array) is a generic template to handle
//               an array of any class.
//========================================================================
#ifndef TARRAY_H
#define TARRAY_H

#include <iostream>
#include <string>
using namespace std;

//Template class to hold functions for handling arrays
template<typename T>
class Tarray {
public:
	Tarray();
	Tarray(int initSize);
	Tarray(const Tarray<T>& copyArray);
	~Tarray();
	Tarray<T>& operator=(const Tarray<T>& otherArray);
	void Add(T& newT);
	void AddCopy(T newT);
	void AddAt(T& newT, int index);
	void RemoveDel(int index);
	void RemoveAt(int index);
	T Get(int i);
	T& operator[](int i);
	void ReplaceAt(T& value, int index);
	bool HasValue(int index);
	int Size();
	int ACTUAL_SIZE();
	template<typename U> friend std::ostream& operator<< (std::ostream& os, const Tarray<U>& thisObject);

private:
	static const int INIT_SIZE = 10;
	//Actual size of the low level array
	int currentSize;
	//Used as an effective size of the dynamic Tarray (How many elements have been added)
	int nextOpenSlot;
	//The low level array used within the Tarray class
	T* array;
};

//Default Constructor
template<typename T>
Tarray<T>::Tarray() {
	currentSize = INIT_SIZE;
	nextOpenSlot = 0;
	array = new T[INIT_SIZE];

	//for(int i = 0; i < INIT_SIZE; i++) {
	//array[i] = NULL;
	//}
	return;
}

//Default Destructor
template<typename T>
Tarray<T>::~Tarray() {
	delete [] array;
	return;
}

//Overloaded constructor to specify initial size
template<typename T>
Tarray<T>::Tarray(int initSize) {
	currentSize = initSize;
	nextOpenSlot = 0;
	array = new T[initSize];

	//for(int i = 0; i < initSize; i++) {
	//	array[i] = new T;
	//}
	return;
}

//Clone constructor
template<typename T>
Tarray<T>::Tarray(const Tarray<T>& copyArray) {
	this->nextOpenSlot = copyArray.nextOpenSlot;
	this->currentSize = copyArray.currentSize;
	this->array = new T[copyArray.currentSize];

	for(int i = 0; i < this->nextOpenSlot; i++) {
		this->array[i] = new T(copyArray.array[i]);
	}
	return;
}

//Copy overloader to deep copy
template<typename T>
Tarray<T>& Tarray<T>::operator=(const Tarray<T>& otherArray) {
	delete [] array;
	array = new T[otherArray.currentSize];
	//Copy all elements in internal array to the other Tarray
	for(int i = 0; i < otherArray.nextOpenSlot; i++) {
		array[i] = otherArray.array[i];
	}
	//*array = *(otherArray.array);
	currentSize = otherArray.currentSize;
	nextOpenSlot = otherArray.nextOpenSlot;
	return *this;
}

//Basic add function by reference
template<typename T>
void Tarray<T>::Add(T& newT) {
	if(nextOpenSlot < currentSize) {
		array[nextOpenSlot] = newT;
		nextOpenSlot++;
	} else {
		T *newArray = new T[currentSize*2];// = new T[currentSize*2];
		for(int i = 0; i < currentSize; i++) {
			newArray[i] = array[i];
			//cout << "Copying: " << array[i] << endl;
		}
		//for(int i = currentSize; i < currentSize * 2; i++) {
		//	*newArray[i] = NULL;
		//}
		//cout << "Before copy: " << array[1] << endl;
		//delete [] array;
		array = newArray;
		//cout << "After copy: " << array[1] << endl;

		currentSize = currentSize*2;
		array[nextOpenSlot] = newT;
		nextOpenSlot++;
	}
	return;
}

//Adds to an array a copy of T passed
template<typename T>
void Tarray<T>::AddCopy(T newT) {
	if(nextOpenSlot < currentSize) {
		array[nextOpenSlot] = newT;
		nextOpenSlot++;
	} else {
		T *newArray = new T[currentSize*2];
		for(int i = 0; i < currentSize; i++) {
			newArray[i] = array[i];
		}
		//for(int i = currentSize; i < currentSize * 2; i++) {
		//	*newArray[i] = NULL;
		//}
		//delete [] array;
		array = newArray;
		//delete [] newArray;

		currentSize = currentSize*2;
		array[nextOpenSlot] = newT;
		nextOpenSlot++;
	}
	return;
}

template<typename T>
void Tarray<T>::AddAt(T& newT, int index) {
	if(nextOpenSlot < currentSize) {
			//Push elements at index forward
			for (int i = nextOpenSlot; i > index; i--) {
				array[i] = array[i-1];
			}
			array[index] = newT;
			nextOpenSlot++;
		} else {
			T *newArray = new T[currentSize*2];// = new T[currentSize*2];
			for(int i = 0; i < currentSize; i++) {
				newArray[i] = array[i];
				//cout << "Copying: " << array[i] << endl;
			}
			//for(int i = currentSize; i < currentSize * 2; i++) {
			//	*newArray[i] = NULL;
			//}
			//cout << "Before copy: " << array[1] << endl;
			//delete [] array;
			array = newArray;
			//cout << "After copy: " << array[1] << endl;

			currentSize = currentSize*2;

			//Push elements at index forward
			for (int i = nextOpenSlot; i > index; i--) {
				array[i] = array[i-1];
			}
			array[index] = newT;
			nextOpenSlot++;
		}
		return;
}

//Removes an object from specified index and deletes it
template<typename T>
void Tarray<T>::RemoveDel(int index) {
	if(index < nextOpenSlot) {
		//Delete and move pointers
		delete *array[index];
		for(int i = index; i < nextOpenSlot - 1; i++) {
			array[i] = array[i+1];
		}
		nextOpenSlot--;
		//Check to see if there are fewer than half elements in actual sized array
		if(nextOpenSlot < (currentSize/2)) {
			//Create new array of half size and fill it with existing elements
			T *newArray = new T[currentSize/2];
			for(int i = 0; i < nextOpenSlot; i++) {
				newArray[i] = array[i];
			}
			delete [] array;
			array = newArray;
			currentSize = currentSize/2;
		}
	} else {
		cerr << "INVALID REMOVE RANGE: Size specified: " << index << ". Max size: "
				<< nextOpenSlot-1 << endl;
	}
	return;
}

//Removes an object from specified index
template<typename T>
void Tarray<T>::RemoveAt(int index) {
	if(index < nextOpenSlot) {
		//Move pointers, so object is not deleted, but not pointed to in this array
		for(int i = index; i < nextOpenSlot - 1; i++) {
			array[i] = array[i+1];
		}
		nextOpenSlot--;
		//Check to see if there are fewer than half elements in actual sized array
		if(nextOpenSlot < (currentSize/2)) {
			//Create new array of half size and fill it with existing elements
			T *newArray = new T[currentSize/2];
			for(int i = 0; i < nextOpenSlot; i++) {
				newArray[i] = array[i];
			}
			delete [] array;
			array = newArray;
			currentSize = currentSize/2;
		}
	} else {
		cerr << "INVALID REMOVE RANGE: Size specified: " << index << ". Max size: "
				<< nextOpenSlot-1 << endl;
	}
	return;
}

//Returns OBJECT T
template<typename T>
T Tarray<T>::Get(int i) {
	if(i >= nextOpenSlot) {
			cerr << "INVALID ACCESS RANGE: Size specified: " << i << ". Max size: "
							<< nextOpenSlot-1 << endl;
			exit(1);
		} else {
			return array[i];
		}
}

//Returns ADDRESS to T (Not tested)
template<typename T>
T& Tarray<T>::operator[] (int i) {
	return array[i];
}

//Set a value at an index in Tarray to T, DELETES OLD VALUE
template<typename T>
void Tarray<T>::ReplaceAt(T& value, int index) {
	if(index >= nextOpenSlot) {
		cerr << "INVALID SET RANGE: Size specified: " << index << ". Max size: "
						<< nextOpenSlot-1 << endl;
		return;
	} else {
	delete array[index];
	array[index] = value;
	return;
	}
}

//Checks if there is a value stored at this index
template<typename T>
bool Tarray<T>::HasValue(int index) {
	if(index >= nextOpenSlot) {
		return false;
	} else {
		return true;
	}
}

//Returns effective size of dynamic array (not actual internal array size)
template<typename T>
int Tarray<T>::Size() {
	return nextOpenSlot;
}

//Returns actual size (USE ONLY FOR DEBUGGING PURPOSES)
template<typename T>
int Tarray<T>::ACTUAL_SIZE() {
	return currentSize;
}

//Support for printing to ostream in CSV format
template<typename T>
std::ostream& operator<< (std::ostream& os, const Tarray<T>& thisObject) {
	for(int i = 0; i < thisObject.nextOpenSlot-1; i++) {
		os << (thisObject.array)[i] << ",";
	}
	os << (thisObject.array)[thisObject.nextOpenSlot-1];
	return os;
}

#endif

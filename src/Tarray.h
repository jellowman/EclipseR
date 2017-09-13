//============================================================================
// Name        : Tarray.h
// Author      : Trevor Fisher
// Version     : 0.2
// Description : Tarray (Teriffic Array) is a generic template to handle
//               an array of any class.
//========================================================================
#ifndef TARRAY_H
#define TARRAY_H

#include <string>
using namespace std;

//Template class to hold functions for handling arrays
template<typename T>
class Tarray {
public:
	Tarray();
	Tarray(int initSize);
	~Tarray();
	Tarray<T>& operator=(const Tarray<T>& otherArray);
	void Add(T& newT);
	void AddCopy(T newT);
	void Remove(int index);
	T Get(int i);
	T& operator[](int i);
	void Set(int i, T value);
	bool HasValue(int index);
	int Size();

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

//Copy overloader to deep copy
template<typename T>
Tarray<T>& Tarray<T>::operator=(const Tarray<T>& otherArray) {
	delete [] array;
	array = new T[otherArray.currentSize];
	//Copy all elements in internal array to the other Tarray
	for(int i = 0; i < otherArray.nextOpenSlot; i++) {
		array[i] = otherArray.array[i];
	}
	*array = *(otherArray.array);
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
		delete [] array;
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
			//cout << "Copying: " << array[i] << endl;
		}
		//for(int i = currentSize; i < currentSize * 2; i++) {
		//	*newArray[i] = NULL;
		//}
		//cout << "Before copy: " << array[1] << endl;
		delete [] array;
		array = newArray;
		//delete [] newArray;
		//cout << "After copy: " << array[1] << endl;

		currentSize = currentSize*2;
		array[nextOpenSlot] = newT;
		nextOpenSlot++;
	}
	return;
}

//Removes an object from specified index
template<typename T>
void Tarray<T>::Remove(int index) {
	if(index < nextOpenSlot) {
		for(int i = index; i < nextOpenSlot - 1; i++) {
			array[i] = array[i+1];
		}
		nextOpenSlot--;
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

//Set a value at an index in Tarray to T
template<typename T>
void Tarray<T>::Set(int i, T value) {
	if(i >= nextOpenSlot) {
		cerr << "INVALID SET RANGE: Size specified: " << i << ". Max size: "
						<< nextOpenSlot-1 << endl;
		return;
	} else {
	array[i] = value;
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

#endif

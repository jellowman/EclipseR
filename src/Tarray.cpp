/*
 * Tarray.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: trevorfisher
 */
/*
#include <iostream>
#include "Tarray.h"
using namespace std;

template<typename T>
Tarray<T>::Tarray() {
	currentSize = INIT_SIZE;
	nextOpenSlot = 0;
	*array = new T[INIT_SIZE];

	for(int i = 0; i < INIT_SIZE; i++) {
	array[i] = NULL;
	}
}

template<typename T>
Tarray<T>::Tarray(int initSize) {
	currentSize = initSize;
	nextOpenSlot = 0;
	array = new T[initSize];

	for(int i = 0; i < initSize; i++) {
		array[i] = NULL;
	}
}

template<typename T>
void Tarray<T>::Add(T newT) {
	if(nextOpenSlot < currentSize) {
		array[nextOpenSlot] = &newT;
		nextOpenSlot++;
	} else {
		T *newArray = new T[currentSize*2];
		for(int i = 0; i < currentSize; i++) {
			newArray[i] = *array[i];
		}
		//for(int i = currentSize; i < currentSize * 2; i++) {
		//	*newArray[i] = NULL;
		//}

		*array = newArray;
		currentSize = currentSize*2;
		array[nextOpenSlot] = &newT;
		nextOpenSlot++;
	}
	return;
}

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

template<typename T>
T Tarray<T>::Get(int i) {
	return *array[i];
}

template<typename T>
int Tarray<T>::Size() {
	return nextOpenSlot;
} */

#ifndef STOREITEM_H
#define STOREITEM_H

#include <string>
using namespace std;

template<typename T>
class Tarray {
public:
	Tarray();
	Tarray(int initSize);
	~Tarray();
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
	int currentSize;
	int nextOpenSlot;
	T* array;
};

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

template<typename T>
Tarray<T>::~Tarray() {
	delete [] array;
}

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

template<typename T>
void Tarray<T>::Add(T& newT) {
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
		//cout << "After copy: " << array[1] << endl;

		currentSize = currentSize*2;
		array[nextOpenSlot] = newT;
		nextOpenSlot++;
	}
	return;
}

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
		//cout << "After copy: " << array[1] << endl;

		currentSize = currentSize*2;
		array[nextOpenSlot] = newT;
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

//Returns OBJECT
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

//Returns ADDRESS
template<typename T>
T& Tarray<T>::operator[] (int i) {
	return array[i];
}

template<typename T>
void Tarray<T>::Set(int i, T value) {
	if(i >= nextOpenSlot) {
		cerr << "INVALID REMOVE RANGE: Size specified: " << index << ". Max size: "
						<< nextOpenSlot-1 << endl;
		return;
	} else {
	*array[i] = value;
	return;
	}
}

template<typename T>
bool Tarray<T>::HasValue(int index) {
	if(index >= nextOpenSlot) {
		return false;
	} else {
		return true;
	}
}

template<typename T>
int Tarray<T>::Size() {
	return nextOpenSlot;
}

#endif

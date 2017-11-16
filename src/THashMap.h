/*
 * THashMap.h
 * A generic linked hash map
 * Ideas for implementation come from
 * Dr. Radhakrishnan's Data Structures Slides
 *  Created on: Nov 15, 2017
 *      Author: trevorfisher
 */

#ifndef SRC_THASHMAP_H_
#define SRC_THASHMAP_H_

#include <iostream>
#include "TList.h"
#include "Tarray.h"
using namespace std;

template<typename T>
class THashMap {
public:
	THashMap();
	~THashMap();
	THashMap(int size);
	THashMap(const THashMap<T>& otherMap);
	void operator=(const THashMap<T>& otherMap);
	void add(T& obj);
	T remove(T& obj);
	T find(int key) const;
	T find(const T& objToFind) const;
	int size() const;
	template<typename U> friend std::ostream& operator<< (std::ostream& os, const THashMap<U>& thisObject);

private:
	//Tarray<TList<T> >* _hashTable;
	T* _table;
	TList<T>* _mapList;
	int _size;

	int hashFunction(int key) const;
	int hashFunction2(int key) const;
};

template<typename T>
THashMap<T>::THashMap() {
	//_hashTable = new Tarray<TList<T> >();
	_table = NULL;
	_mapList = NULL;
	_size = 0;
}

template<typename T>
THashMap<T>::~THashMap() {
	/*if(_hashTable != NULL) {
		delete _hashTable;
		_hashTable = NULL;
	}*/
	if(_table != NULL) {
		delete [] _table;
		_table = NULL;
	}

	if(_mapList != NULL) {
		delete _mapList;
		_mapList = NULL;
	}
}

template<typename T>
THashMap<T>::THashMap(int size) {
	//_hashTable = new Tarray<TList<T> >(size);
	_size = (size*1.25);
	_table = new T[_size];
	for(int i = 0; i < _size; i++) {
		//_hashTable->add(new TList<T>());
		_table[i] = nullptr;
	}
	_mapList = new TList<T>();
}

template<typename T>
THashMap<T>::THashMap(const THashMap<T>& otherMap) {
	/*if(otherMap._hashTable == NULL) {
		this->_hashTable = new Tarray<TList <T> >();
	} else {
		this->_hashTable = new Tarray<TList <T> >(*otherMap._hashTable);
	}*/
	_size = otherMap.size();
	_table = new T[otherMap._size];
	for(int i = 0; i < _size; i++) {
		_table[i] = new T(*(otherMap._table[i]));
	}

	if(otherMap._mapList == NULL) {
		_mapList == new TList<T>();
	} else {
		_mapList = new TList<T>(*otherMap._mapList);
	}
}

template<typename T>
void THashMap<T>::operator=(const THashMap<T>& otherMap) {
	/*if(_hashTable != NULL) {
		delete _hashTable;
	}*/

	if(_table != NULL) {
		delete [] _table;
	}
	if(_mapList != NULL) {
		delete _mapList;
	}

	/*if(otherMap._hashTable == NULL) {
		_hashTable = NULL;
	} else {
		_hashTable = otherMap._hashTable;
	}*/
	if(otherMap._table == NULL) {
		_table = NULL;
	} else {
		_table = otherMap._table;
	}

	if(otherMap._mapList == NULL) {
		_mapList = NULL;
	} else {
		_mapList = otherMap._mapList;
	}
}

template<typename T>
void THashMap<T>::add(T& obj) {
	int key = obj.getKey();
	int hashVal = hashFunction(key);
	int dubHash = hashFunction2(key);

	//TList<T>* temp = _hashTable->get(hashVal);
	//temp->add(obj);
	//Add to keyed bucket, tries primary hash function, then resorts to double hashing
	for(int i = 0; i < _size; i++) {
		if(_table[hashVal] == nullptr) {
			_table[hashVal] = new T(obj);
			break;
		} else {
			hashVal = (hashVal + dubHash) % _size;
		}
	}

	_mapList->add(obj);
}

template<typename T>
T THashMap<T>::remove(T& obj) {
	int key = obj.getKey();
	int hashVal = hashFunction(key);
	int dubHash = hashFunction2(key);

	//TList<T>* matchingList = _hashTable->get(hashVal);
	//T toRem = matchingList->remove(obj);
	T toRem = nullptr;

	//Perform search to remove data
	bool wasDeleted = false;
	for(int i = 0; i < _size; i++) {
		if(_table[hashVal] != nullptr) {
			if(key == _table[hashVal].getKey()) {
				delete _table[i];
				_table[i] == nullptr;
				wasDeleted = true;
				break;
			}
		} else {
			hashVal = (hashVal + dubHash) % _size;
		}
	}

	if(wasDeleted) { //Remove from Linked List
		toRem = _mapList->remove(obj);
	}

	return toRem;
}

template<typename T>
T THashMap<T>::find(int key) const{
	int hashVal = hashFunction(key);
	int dubHash = hashFunction2(key);

	//TList<T>* matchingList = _hashTable->get(hashVal);
	T toRem = nullptr;

	for(int i = 0; i < _size; i++) {
		if(_table[hashVal] != nullptr) {
			if(key == _table[hashVal].getKey()) {
				return *_table[hashVal];
			}
		}
	}
	return toRem;
}

template<typename T>
T THashMap<T>::find(const T& obj) const{
	return find(obj.getKey());
}


template<typename T>
int THashMap<T>::hashFunction(int key) const{
	return key % size();
}

//Double hashing implementation to add an increment to search for unoccupied bin.
//Ensures that the offset provided is not a multiple of the number of buckets, allowing for
//traversal of all possible buckets
template<typename T>
int THashMap<T>::hashFunction2(int key) const{
	int key2 = (key % (size()/3)) + 1;

	//If the number of buckets is divisible by the key, increment key
	//Or if both the buckets and the key have the same parity, increment key
	if(key2 != 1) {
		while(((size() % key2) == 0) || ((key2 % 2) == (size() % 2))) {
			key2 = key2 + 1;
		}
	}
	return key2;
}

template<typename T>
int THashMap<T>::size() const {
	return _hashTable->size();
}


#endif /* SRC_THASHMAP_H_ */

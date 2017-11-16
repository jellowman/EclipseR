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
	Tarray<TList<T> >* _hashTable;
	TList<T>* _mapList;

	int hashFunction(int key) const;
};

template<typename T>
THashMap<T>::THashMap() {
	_hashTable = new Tarray<TList<T> >();
	_mapList = new TList<T>();
}

template<typename T>
THashMap<T>::~THashMap() {
	if(_hashTable != NULL) {
		delete _hashTable;
		_hashTable = NULL;
	}
	if(_mapList != NULL) {
		delete _mapList;
		_mapList = NULL;
	}
}

template<typename T>
THashMap<T>::THashMap(int size) {
	_hashTable = new Tarray<TList<T> >(size);
	for(int i = 0; i < size; i++) {
		_hashTable->get() = new TList<T>();
	}
	_mapList = new TList<T>();

}

template<typename T>
THashMap<T>::THashMap(const THashMap<T>& otherMap) {
	if(otherMap._hashTable == NULL) {
		this->_hashTable = new Tarray<TList <T> >();
	} else {
		this->_hashTable = new Tarray<TList <T> >(*otherMap._hashTable);
	}

	if(otherMap._mapList == NULL) {
		_mapList == new TList<T>();
	} else {
		_mapList = new TList<T>(*otherMap._mapList);
	}
}

template<typename T>
void THashMap<T>::operator=(const THashMap<T>& otherMap) {
	if(_hashTable != NULL) {
		delete _hashTable;
	}
	if(_mapList != NULL) {
		delete _mapList;
	}

	if(otherMap._hashTable == NULL) {
		_hashTable = NULL;
	} else {
		_hashTable = otherMap._hashTable;
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

	TList<T>* temp = _hashTable->get(hashVal);
	temp->add(obj);

	_mapList->add(obj);
}

template<typename T>
T THashMap<T>::remove(T& obj) {
	int key = obj.getKey();
	int hashVal = hashFunction(key);

	TList<T>* matchingList = _hashTable->get(hashVal);

	T toRem = matchingList->remove(obj);
	if(toRem.getKey() != -1) {
		_mapList->remove(obj);
	}

	return toRem;
}

template<typename T>
T THashMap<T>::find(int key) const{
	int hashVal = hashFunction(key);

	TList<T>* matchingList = _hashTable->get(hashVal);
	T toRem;

	for(int i = 0; i < matchingList->size(); i++) {
		if(key == matchingList->item().getKey()) {
			toRem = matchingList->item();
			break;
		}
		matchingList = matchingList->next();
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

template<typename T>
int THashMap<T>::size() const {
	return _hashTable->size();
}


#endif /* SRC_THASHMAP_H_ */

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
	THashMap(const Tarray<T>& otherArray);
	void operator=(const THashMap<T>& otherMap);
	void add(T& obj);
	T remove(T& obj);
	T find(int key) const;
	T find(const T& objToFind) const;
	int size() const;
	int numElements() const;
	double load() const;
	std::ostream& displayList(std::ostream& os);
	template<typename U> friend std::ostream& operator<< (std::ostream& os, const THashMap<U>* thisObject);

private:
	//Tarray<TList<T> >* _hashTable;
	T** _table;
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
		for(int i = 0; i < _size; i++) {
			if(_table[i] != nullptr) {
				delete _table[i];
				_table[i] = nullptr;
			}
		}
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
	_table = new T*[_size];
	for(int i = 0; i < _size; i++) {
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
	if(otherMap.size() != NULL) {
		_size = otherMap.size();
	} else {
		_size = NULL;
	}
	if(otherMap._table != NULL) {
		_table = new T*[otherMap._size];
		for(int i = 0; i < _size; i++) {
			if(otherMap._table[i] != nullptr) {
				_table[i] = new T(*(otherMap._table[i]));
			} else {
				_table[i] = nullptr;
			}
		}
	} else {
		_table = NULL;
	}

	if(otherMap._mapList == NULL) {
		_mapList == NULL;
	} else {
		_mapList = new TList<T>(*otherMap._mapList);
	}
}

template<typename T>
THashMap<T>::THashMap(const Tarray<T>& otherArray) {
	_size = otherArray.size()*1.25;
	_table = new T*[_size];
	for(int i = 0; i < _size; i++) {
		_table[i] = nullptr;
	}

	_mapList = new TList<T>();

	for(int i = 0; i < otherArray.size(); i++) {
		this->add(otherArray.get(i));
	}
}

template<typename T>
void THashMap<T>::operator=(const THashMap<T>& otherMap) {
	/*if(_hashTable != NULL) {
		delete _hashTable;
	}*/

	if(_table != NULL) {
		for(int i = 0; i < _size; i++) {
			delete _table[i];
			_table[i] = nullptr;
		}
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

	//cout << "Key1: " << hashVal << endl;
	//cout << "Key2: " << dubHash << endl;

	//TList<T>* temp = _hashTable->get(hashVal);
	//temp->add(obj);
	//Add to keyed bucket, tries primary hash function, then resorts to double hashing
	for(int i = 0; i < _size; i++) {
		if(_table[hashVal] == nullptr) {
			//cout << "Adding " << obj << " to index " << hashVal << endl;
			//cout << "Took " << i+1 << " looks" << endl;
			_table[hashVal] = new T(obj);
			break;
		} else {
			hashVal = (hashVal + dubHash) % _size;
		}
	}

	//Insert object into end of linked list
	_mapList->insertAt(obj, _mapList->size());
}

template<typename T>
T THashMap<T>::remove(T& obj) {
	int key = obj.getKey();
	int hashVal = hashFunction(key);
	int dubHash = hashFunction2(key);

	//TList<T>* matchingList = _hashTable->get(hashVal);
	//T toRem = matchingList->remove(obj);
	T toRem = nullptr;

	//Look at primary and double hash points to remove data
	bool wasDeleted = false;
	for(int i = 0; i < _size; i++) {
		if(_table[hashVal] != nullptr) {
			if(key == _table[hashVal]->getKey()) {
				delete _table[i];
				_table[i] = nullptr;
				wasDeleted = true;
				break;
			}
		}

		hashVal = (hashVal + dubHash) % _size;
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
	T found = T();

	for(int i = 0; i < _size; i++) {
		if(_table[hashVal] != nullptr) {
			if(key == _table[hashVal]->getKey()) {
				return *(_table[hashVal]);
			}
		}
		hashVal = (hashVal + dubHash) % _size;
	}
	return found;
}

template<typename T>
T THashMap<T>::find(const T& obj) const{
	return find(obj.getKey());
}


template<typename T>
int THashMap<T>::hashFunction(int key) const{
	return key % _size;
}

//Double hashing implementation to add an increment to search for unoccupied bin.
//Ensures that the offset provided is not a multiple of the number of buckets, allowing for
//traversal of all possible buckets
template<typename T>
int THashMap<T>::hashFunction2(int key) const{
	int key2 = (key % (_size/3)) + 1;

	//If the number of buckets is divisible by the key, increment key
	//Or if both the buckets and the key have the same parity, increment key
	if(key2 != 1) {
		while(((_size % key2) == 0) || ((key2 % 2) == (_size % 2))) {
			key2 = key2 + 1;
		}
	}
	return key2;
}

template<typename T>
int THashMap<T>::size() const {
	return _size;
}

template<typename T>
int THashMap<T>::numElements() const {
	return _mapList->size();
}

template<typename T>
double THashMap<T>::load() const {
	return static_cast<double>(numElements()) / static_cast<double>(_size);
}

template<typename T>
std::ostream& THashMap<T>::displayList(std::ostream& os) {
	os << _mapList;
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const THashMap<T>* thisObject) {
	for(int i = 0; i < thisObject->_size; i++) {
		os << "Bucket: " << i << " - Content [";
		if(thisObject->_table[i] == nullptr) {
			os << "NULL";
		} else {
			os << *(thisObject->_table[i]);
		}
		os << "]" << endl << endl;
	}

	return os;
}



#endif /* SRC_THASHMAP_H_ */

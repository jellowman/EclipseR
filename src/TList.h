/*
 * TList.h
 * A Generic Linked List.
 * Many of the ideas for methods and implementation come from
 * Dr. Radhakrishnan's Data Structures Slides
 *
 *  Created on: Oct 28, 2017
 *      Author: trevorfisher
 */

#ifndef TLIST_H_
#define TLIST_H_

#include <iostream>
#include <string>
using namespace std;

template<typename T>
class AbstractTList {
public:
	virtual T& info() = 0;
	virtual AbstractTList<T>* next() = 0;
	virtual bool isEmpty() = 0;
	virtual void add(T& newT) = 0;
	virtual AbstractTList<T>* setNext(AbstractTList<T>* nextT) = 0;
	virtual void insertAt(AbstractTList<T>& newT, int pos) = 0;
	virtual T& getAt(int pos) = 0;
	virtual T& find(T& key) = 0;
	virtual T remove() = 0;
	virtual T remove(T& key) = 0;
	virtual T removeAt(int pos) = 0;
	virtual int size() = 0;
	virtual void display(ostream& s);
};

class LinkedListException : public std::exception { };
class LinkedListBounds : public LinkedListException { };
class LinkedListNotFound : public LinkedListException { };

//A simple, single abstract linked list
template<typename T>
class TList : AbstractTList<T> {
protected:
	T* _item;
	TList<T>* _next;
	int* _size;
public:
	TList();
	TList(T& item);
	TList(T& item, TList<T>* next);
	~TList();
	TList(const TList<T>& other);
	T& info();
	TList<T>* next();
	bool isEmpty();
	void add(T& newT);//
	TList<T>* setNext(TList<T>* nextT);//
	void insertAt(TList<T>& newT, int pos);//
	T& getAt(int pos);//
	T& find(T& key);//
	T remove();
	T remove(T& key);//
	T removeAt(int pos);//
	int size();//
};

template<typename T>
TList<T>::TList() {
	_item = NULL;
	_next = NULL;
	_size = 0;
}

template<typename T>
TList<T>::TList(T& item) {
	_item = item;
	_next = NULL;
	_size = 0;
}

template<typename T>
TList<T>::TList(T& item, TList<T>* next) {
	_item = item;
	_next = next;
	this->_size = next->_size;
}

template<typename T>
TList<T>::~TList() {
	if(_item != NULL) {
		delete _item;
		_info = NULL;
	}
	if (_next != NULL) {
		delete _next;

		_next = NULL;
	}
	if (_size != NULL) {
			delete _size;

			_size = NULL;
		}
}

template<typename T>
TList<T>::TList(const TList<T>& other) {
	if(other._item == NULL) {
		_item = NULL;
	} else {
		_item = new T(*other._item);
	}

	if(other._next == NULL) {
		_next = NULL;
	} else {
		_next = new T(*other._next);
	}

	if(other._size == NULL) {
			_size = NULL;
	} else {
			_size = other._size;
	}

}

template<typename T>
T& TList<T>::info() {
	return *_info;
}

template<typename T>
TList<T>* TList<T>::next() {
	return _next;
}

template<typename T>
int TList<T>::size() {
	return *_size;
}

template<typename T>
bool TList<T>::isEmpty() {
	return (_item == NULL);
}

//Adds a new element to the current chain of the LinkedList in front
//of the current LinkedList item
 template<typename T>
 void TList<T>::add(T& newT) {
	 if(_item == NULL) {
		 _item = new T(newT);
	 } else {
		 //Store this current linked list object's contents
		 TList<T>* oldList = new TList<T>(*_info, _next);

		 //Store new data in current list, put new linked object after the current list item
		 _item = newT;
		 _next = oldList;
	 }
 }

//Recursively goes through linked list to add a new element to the specified position
template<typename T>
void TList<T>::insertAt(TList<T>& newT, int pos) {
	if(pos == 0) {
		add(newT);
	} else {
		if(_next == NULL) {
			_next = new TList<T>(newT);
		} else {
			_next->insertAt(newT, pos-1);
		}
	}
}

//Replaces the _next pointer with the specified pointer, returns
//the pointer to the old _next address
template<typename T>
TList<T>* TList<T>::setNext(TList<T>* newTPointer) {
	TList<T>* oldNext = _next;
	_next = newTPointer;
	return oldNext;
}

template<typename T>
T& TList<T>::getAt(int pos) {
	if(pos == 0) {
		return *_item;
	} else {
		if(_next == NULL) {
			cerr << "Index out of bounds" << endl;
			return NULL;
		} else {
			return _next->getAt(pos-1);
		}
	}
}

template<typename T>
T& TList<T>::find(T& key) {
	if(isEmpty()) {
		throw LinkedListNotFound();
	} else {
		if(*_item == key) {
			return *_item;
		} else if(_next == NULL) {
			return NULL;
		} else {
			return _next->find(key);
		}
	}
}

template<typename T>
T TList<T>::remove() {
	T temp = *_item;
	delete _item;

	//Set info to NULL if it's the last chain, otherwise, make the data
	//In this chain the data in the next one to re-link the chain
	if(_next == NULL) {
		_item == NULL;
	} else {
		TList<T>* oldNext = _next;
		_item = _next->_item;
		_next = _next->_next;

		//Free pointers from old chain link
		oldNext->_item = NULL;
		oldNext->_next = NULL;
		oldNext->_size = NULL;

	}

	return temp;
}

template<typename T>
T TList<T>::remove(T& key) {
	if(isEmpty()) {
		throw LinkedListNotFound();
	} else {
		if(*_info == key) {
			return this->remove();
		} else if (_next == NULL) {
			return NULL;
		} else {
			return _next->remove(key);
		}
	}
}

template<typename T>
T TList<T>::removeAt(int pos) {
	if(isEmpty()) {
		throw LinkedListNotFound();
	} else {
		if(pos == 0) {
			return this->remove();
		} else if(_next == NULL) {
			throw LinkedListBounds();
		} else {
			return _next->removeAt(pos-1);
		}
	}
}

#endif /* TLIST_H_ */

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
#include "Tarray.h"
using namespace std;

template<typename T>
class AbstractTList {
public:
	virtual T& item() = 0;
	virtual AbstractTList<T>* next() = 0;
	virtual bool isEmpty() = 0;
	virtual void add(T& newT) = 0;
	//virtual AbstractTList<T>* setNext(AbstractTList<T>* nextT) = 0;
	//virtual void insertAt(AbstractTList<T>& newT, int pos) = 0;
	virtual T& getAt(int pos) = 0;
	virtual int find(T& key) = 0;
	virtual T remove() = 0;
	virtual T remove(T& key) = 0;
	virtual T removeAt(int pos) = 0;
	virtual int size() = 0;
	//virtual void display(ostream& s);
};

class LinkedListException : public std::exception { };
class LinkedListBounds : public LinkedListException { };
class LinkedListNotFound : public LinkedListException { };

//A simple, single abstract linked list
template<typename T>
class TList :public AbstractTList<T> {
protected:
	T* _item;
	TList<T>* _next;
	int* _size;
public:
	TList();
	TList(T& item);
	TList(int* size);
	TList(T& item, TList<T>* next, int* size);
	~TList();
	TList(const TList<T>& other);
	TList(const Tarray<T>& otherArray);
	Tarray<T>& toArray();
	void operator=(const TList<T>& other);
	T& item();
	TList<T>* next();
	bool isEmpty();
	void add(T& newT);//
	TList<T>* setNext(TList<T>* nextT);//
	void insertAt(T& newT, int pos);//
	int insertSorted(T& newT);
	T& getAt(int pos);//
	int find(T& key);//
	T remove();
	T remove(T& key);//
	T removeAt(int pos);//
	int size();//
	void setSize(int* size);
	template<typename U> friend std::ostream& operator<< (std::ostream& os, const TList<U>* headList);
};

template<typename T>
TList<T>::TList() {
	_item = NULL;
	_next = NULL;
	_size = new int(0);
}

template<typename T>
TList<T>::TList(int* size) {
	_item = NULL;
	_next = NULL;
	_size = size;
}

//Creates a new linked list with a copy of the same object
//Passed in. The next is set to null
template<typename T>
TList<T>::TList(T& item) {
	_item = new T(item);
	_next = NULL;
	_size = new int(0);
}

//Creates a linked list by shallow assigning to the
//Components of a linked list
template<typename T>
TList<T>::TList(T& item, TList<T>* next, int* size) {
	_item = &item;
	_next = next;
	_size = size;
}

//TODO
template<typename T>
TList<T>::~TList() {
	/*if(_item != NULL) {
		delete _item;
		_item = NULL;
	}

	if (_next != NULL) {
		delete _next;

		_next = NULL;
	}

	if (_size != NULL) {
		delete _size;

		_size = NULL;
	}*/

	TList<T>* temp = this;
	TList<T>* tempNext = this->_next;
	bool exit = false;
	do {
		if(temp->_item != NULL) {
				delete temp->_item;
				temp->_item = NULL;
		}

		if (temp->_next != NULL) {
				tempNext = temp->_next;

				temp->_next = NULL;
				temp->_size = NULL;

				temp = tempNext;
		} else { //Delete the size on the last link of the chain
			if(temp->_size != NULL) {
				delete _size;
				_size = NULL;
			}
			exit = true;
		}
	} while (!exit);
}

//Deep copy
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
		//Recursive call to assign all chains past it
		_next = new TList<T>(*other._next);
	}

	if(other._size == NULL) {
			_size = NULL;
	} else {
		if(_next->_size == NULL) {
			int newSize = *(other._size);
			_size = &newSize;
		} else {
			_size = _next->_size;
		}
	}
}

//Copies elements from a Tarray (Array List) into
//A Linked List
template<typename T>
TList<T>::TList(const Tarray<T>& otherArray) {
	_item = NULL;
	_next = NULL;
	_size = new int(0);

	for(int i = 0; i < otherArray.size(); i++) {
		//temp->_item = otherArray.get(i);
		//temp->_next = new TList<T>(_size);

		//temp = temp->_next;
		insertSorted(*(new T(otherArray.get(i))));
	}
}

template<typename T>
Tarray<T>& TList<T>::toArray() {
	Tarray<T>* newArray = new Tarray<T>();
	TList<T>* temp = this;

	for(int i = 0; i < this->size(); i++) {
		newArray->add(*(temp->_item));
		temp = temp->_next;
	}

	return *newArray;
}

//Performs shallow assignment
template<typename T>
void TList<T>::operator=(const TList<T>& other) {
	if(_item != NULL) {
		 delete _item;
	}
	if(_next != NULL) {
		delete _next;
	}
	if(_size != NULL) {
		//This is shared along all the chain, so don't delete
	}

	if(other._item == NULL) {
		_item == NULL;
	} else {
		_item = other._item;
	}

	if(other._next == NULL) {
		_next == NULL;
	} else {
		_next = other._next;
	}

	if(other._size == NULL) {
			_size == NULL;
		} else {
			_size = other._size;
		}
}

template<typename T>
T& TList<T>::item() {
	return *_item;
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
void TList<T>::setSize(int* size) {
	_size = size;
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
		 TList<T>* oldList = new TList<T>(*_item, _next, _size);

		 //Store new data in current list, put new linked object after the current list item
		 _item = new T(newT);
		 _next = oldList;
	 }

	 //Increment size of chain
	 *(_size) = *(_size)+1;
 }

//Recursively goes through linked list to add a new element to the specified position
template<typename T>
void TList<T>::insertAt(T& newT, int pos) {
	/*if(pos == 0) {
		add(newT);
	} else {
		if(_next == NULL) {
			_next = new TList<T>(newT);
			_next->_size = this->_size;
		} else {
			_next->insertAt(newT, pos-1);
		}
	}*/

	if(pos > this->size()) {
		pos = *_size;
	}

	TList<T>* temp = this;
	bool toEnd = false;
	for(int i = 0; i < pos; i++) {
		if(temp->_next == NULL) {
			temp->_next = new TList<T>(newT);
			temp->_next->_size = temp->_size;
			toEnd = true;
			//Increment chain size
			*(temp->_size) = *(temp->_size)+1;
		} else {
		temp = temp->_next;
		}
	}

	if(!toEnd) {
		temp->add(newT);
		//Adds 1 to size in add()
	}
}

template<typename T>
int TList<T>::insertSorted(T& newT) {
	TList<T>* temp = this;
	int pos = -1;
	if(_item == NULL) {
		add(newT);
		return 0;
	} else {
		for(int i = 0; i < this->size(); i++) {
			if(temp->isEmpty()) {
				throw LinkedListNotFound();
			} else {
				//cout << "Comparing " << newT << " to " << *(temp->_item) << endl;
				if(newT.compare(*(temp->_item)) > 0) {
					if(temp->_next == NULL) {
						temp->_next = new TList<T>(newT);
						temp->_next->_size = temp->_size;
						*(temp->_size) = *(temp->_size) + 1;
						return i+1;
					} else {
						temp = temp->_next;
					}
				} else {
					temp->add(newT);
					//size incremented in method
					return i;
				}
			}
		}
		return pos;
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
	/*if(pos == 0) {
		return *_item;
	} else {
		if(_next == NULL) {
			cerr << "Index out of bounds" << endl;
			return NULL;
		} else {
			return _next->getAt(pos-1);
		}
	}*/
	if(pos > *_size) {
		throw LinkedListBounds();
		pos = *_size;
	}

	TList<T>* temp = this;
	for(int i = 0; i < pos; i++) {
		if(temp->next() == NULL) {
			throw LinkedListBounds();
		} else {
			temp = temp->_next;
		}
	}

	return *(temp->_item);
}

template<typename T>
int TList<T>::find(T& key) {
	/*if(isEmpty()) {
		throw LinkedListNotFound();
	} else {
		if(*_item == key) {
			return *_item;
		} else if(_next == NULL) {
			return NULL;
		} else {
			return _next->find(key);
		}
	}*/
	TList<T>* temp = this;
	for (int i = 0; i < this->size(); i++) {
		if(temp->isEmpty()) {
			throw LinkedListNotFound();
		} else {
			if(*(temp->_item) == key) {
				return i;
			} else if(this->_next == NULL) {
				return -1;
			} else {
				temp = temp->_next;
			}
		}
	}
	return -1;
}

template<typename T>
T TList<T>::remove() {
	T* temp = new T(*_item);
	delete _item;

	//Set item to NULL if it's the last chain, otherwise, make the data
	//In this chain the data in the next one to re-link the chain
	if(_next == NULL) {
		_item = NULL;
	} else {
		//Create new pointer to next object
		TList<T>* oldNext = _next;
		//Assign current list's items to next list object
		_item = _next->_item;
		_next = _next->_next;

		//Free pointers from old chain link
		oldNext->_item = NULL;
		oldNext->_next = NULL;
		oldNext->_size = NULL;

	}
	*(_size) = *(_size)-1;

	return *temp;
}

template<typename T>
T TList<T>::remove(T& key) {
	if(isEmpty()) {
		throw LinkedListNotFound();
	} else {
		if(*_item == key) {
			return this->remove();
		} else if (_next == NULL) {
			return *(new T());
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

template<typename T>
std::ostream& operator<< (std::ostream& os, const TList<T>* headList) {
	const TList<T>* temp = headList;
	//os << "<";
	if(temp->_item == NULL) {
		//os << ">";
		return os;
	}

	do {
		os << *(temp->_item);
		os << endl;
		temp = temp->_next;
		if(temp != NULL) {
			//os << ",";
		}
	} while(temp != NULL);
	//os << ">";
	return os;
}

#endif /* TLIST_H_ */

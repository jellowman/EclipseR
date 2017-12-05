/*
 * TavlTree.h
 * A generic AVL tree
 * Coding Implementation ideas
 * are from Dr. Radhakrishnan's Data Structure lecture slides
 *
 *  Created on: Nov 29, 2017
 *      Author: trevorfisher
 */

#ifndef TAVLTREE_H_
#define TAVLTREE_H_

#include <iostream>
using namespace std;

template<typename T>
class BinarySearchTree {
protected:
	T* _info;
	BinarySearchTree<T>* _left;
	BinarySearchTree<T>* _right;
	bool _subtree;

	virtual BinarySearchTree<T>* makeSubtree();
	virtual void copyTree(BinarySearchTree<T>* otherTree);
	virtual void makeNull();
	BinarySearchTree<T>* _find(const T& data);
public:
	BinarySearchTree();
	BinarySearchTree(T& data);
	virtual ~BinarySearchTree();
	virtual bool isEmpty();
	virtual int height();
	virtual int size();
	virtual T& root();
	virtual BinarySearchTree<T>* left();
	virtual BinarySearchTree<T>* right();
	virtual bool subtree();
	virtual void makeEmpty();
	virtual T find(const T& data);
	virtual void remove(const T& data);
	//virtual void rangeSearch(const T& low, const T& high);
	virtual void insert(const T& data);
};
class BinarySearchTreeException : std::exception { };
class BinarySearchTreeEmpty : BinarySearchTreeException { };
class BinarySearchTreeChangeSubtree : BinarySearchTreeException {	 };
class BinarySearchTreeNotFound : BinarySearchTreeException { };
/*
 * Standard Constructor
 */
template<typename T>
BinarySearchTree<T>::BinarySearchTree() {
	_info = NULL;
	_left = NULL;
	_right = NULL;
	_subtree = false;
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(T& otherData) {
	_info = new T(otherData);
	_left = makeSubtree();
	_right = makeSubtree();
	_subtree = false;
}

template<typename T>
BinarySearchTree<T>* BinarySearchTree<T>::makeSubtree() {
	BinarySearchTree<T>* bst = new BinarySearchTree<T>();
	bst->_subtree = true;
	return bst;
}

template<typename T>
void BinarySearchTree<T>::copyTree(BinarySearchTree<T>* otherTree) {
	_info = otherTree->_info;
	_left = otherTree->_left;
	_right = otherTree->_right;
	//Don't change _subtree since the root node may copy data from a leaf
	//_subtree = otherTree->_subtree;
}

/*template<typename T>
BinarySearchTree<T>::~BinarySearchTree() {
	if(_info != NULL) {
		_info == NULL;
	}
	if(_left != NULL) {
		_left == NULL;
	}
	if(_right != NULL) {
		_right == NULL;
	}
}*/

template<typename T>
bool BinarySearchTree<T>::isEmpty() {
	return (_info == NULL);
}

template<typename T>
int BinarySearchTree<T>::height() {
	if(isEmpty()) {
		return 0;
	}

	return (1 + max(_left->height(), _right->height()));
}

template<typename T>
int BinarySearchTree<T>::size() {
	if(isEmpty()) {
		return 0;
	}

	return (1 + _left->size() + _right->size());
}

template<typename T>
T& BinarySearchTree<T>::root() {
	if(isEmpty()	) {
		throw BinarySearchTreeEmpty();
	}
	return *_info;
}

template<typename T>
BinarySearchTree<T>* BinarySearchTree<T>::left() {
	return _left;
}

template<typename T>
BinarySearchTree<T>* BinarySearchTree<T>::right() {
	return _right;
}

template<typename T>
bool BinarySearchTree<T>::subtree() {
	return _subtree;
}

template<typename T>
void BinarySearchTree<T>::makeEmpty() {
	if(_subtree) throw BinarySearchTreeException();
	if(_info != NULL) {
		delete _info;
		_info = NULL;
	}
	if(_left != NULL) {
		delete _left;
		_left = NULL;
	}
	if(_right != NULL) {
		delete _right;
		_right = NULL;
	}
}

template<typename T>
void BinarySearchTree<T>::makeNull() {
	_info = NULL;
	_left = NULL;
	_right = NULL;
}

template<typename T>
T BinarySearchTree<T>::find(const T& data) {
	BinarySearchTree<T>* bst = this;

	while(true) {
		if(bst->isEmpty()) throw BinarySearchTreeEmpty();
		if( *(bst->_info) < data) {
			bst = bst->_right;
		} else if(*(bst->_info) > data) {
			bst = bst->_left;
		} else {
			return bst->root();
		}
	}
}

template<typename T>
BinarySearchTree<T>* BinarySearchTree<T>::_find(const T& data) {
	BinarySearchTree<T>* bst = this;

	while(true) {
		if(bst->isEmpty()) {
			return bst;
		}
		if( *(bst->_info) < data) {
			bst = bst->_right;
		} else if(*(bst->_info) > data) {
			bst = bst->_left;
		} else {
			return bst;
		}
	}
}

template<typename T>
void BinarySearchTree<T>::insert(const T& data) {
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	BinarySearchTree<T>* bst = _find(data);
	if(bst->isEmpty()) {
		bst->_info = new T(data);
		bst->_left = makeSubtree();
		bst->_right = makeSubtree();
	} else {
		delete bst->_info;
		bst->_info = new T(data);
	}
}

template<typename T>
void BinarySearchTree<T>::remove(const T& data) {
	//Ensure this method isn't accessed from a subtree
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	BinarySearchTree<T>* bst1;
	BinarySearchTree<T>* bst2;
	BinarySearchTree<T>* bst3;

	//Find pointer to element to remove, otherwise handle if the data wasn't present
	bst1->find(data);
	if(bst1->isEmpty()) throw BinarySearchTreeNotFound();
	delete bst1->_info;

	if(bst1->_left->isEmpty()) {
		//Make node with removed element equal to right subtree
		delete bst1->_left;
		bst2 = bst1->_right;
		bst1->copyTree(bst2);

		bst2->makeNull();
		delete bst2;
	} else if(bst1->_right->isEmpty()) {
		//Make node with removed element equal to left subtree
		delete bst1->_right;
		bst2 = bst1->_left;
		bst1->copyTree(bst2);

		bst2->makeNull();
		delete bst2;
	} else {
		//Make node with removed element equal to next largest element
		bst2 = bst1->_right;
		while(!bst2->_left->isEmpty()) {
			bst2 = bst2->_left;
		}

		bst1->_info = bst2->_info;

		//Replace the old location with the right node, if it exists
		delete bst2->_left;
		if(bst2->_right->isEmpty()) {
			delete bst2->_right;
			bst2->makeNull();
		} else {
			bst3 = bst2->_right;
			bst2->copyTree(bst3);

			bst3->makeNull();
			delete bst3;
		}
	}
}

//BEGIN SELF_MODIFYING_BST_CLASSES
template<typename T>
class SelfModifyingBST : public BinarySearchTree<T> {
protected:
	BinarySearchTree<T>* makeSubtree();
	virtual void zig();
	virtual void zag();
	virtual void zigzig();
	virtual void zagzag();
	virtual void zigzag();
	virtual void zagzig();
public:
	SelfModifyingBST();
	SelfModifyingBST(T& data);
	virtual ~SelfModifyingBST();
};

template<typename T>
SelfModifyingBST<T>::SelfModifyingBST() : BinarySearchTree<T>() { }

template<typename T>
SelfModifyingBST<T>::SelfModifyingBST(T& data) : BinarySearchTree<T>(data) { }

template<typename T>
SelfModifyingBST<T>::~SelfModifyingBST() { }

template<typename T>
BinarySearchTree<T>* SelfModifyingBST<T>::makeSubtree() {
	BinarySearchTree<T>* bst = new SelfModifyingBST<T>();
	((SelfModifyingBST<T>*)bst)->_subtree = true;
	return bst;
}

template<typename T>
void SelfModifyingBST<T>::zag() {
	if(isEmpty()	) return;
	if(_right->isEmpty()) return;

	SelfModifyingBST<T>* rightChild = (SelfModifyingBST<T>*)_right;
	_right = rightChild->_right;
	rightChild->_right = rightChild->_left;
	rightChild->_left = _left;
	_left = rightChild;
	swap (_root, rightChild->_root);
}

template<typename T>
void SelfModifyingBST<T>::zig() {
	if(isEmpty()) return;
	if(_left->isEmpty()) return;

	SelfModifyingBST<T>* leftChild = (SelfModigyingBST<T>*)_left;
	_left = leftChild->_left;
	leftChild->_left = leftChild->_right;
	leftChild->_right = _right;
	_right = leftChild;
	swap (_root, leftChild->_root);
}

template<typename T>
void SelfModifyingBST<T>::zagzig() {
	if(isEmpty()) return;
	((SelfModifyingBST<T>*)_right)->zig();
	zag();
}

template<typename T>
void SelfModifyingBST<T>::zigzag() {
	if(isEmpty()) return;
	((SelfModifyingBST<T>*)_left)->zag();
	zig();
}

template<typename T>
void SelfModifyingBST<T>::zigzig() {
	zig();
	zig();
}

template<typename T>
void SelfModifyingBST<T>::zagzag() {
	zag();
	zag();
}

template<typename T>
class TAVLTree : public SelfModifyingBST<T> {
protected:
	int _diff;
	virtual BinarySearchTree<T>* makeSubtree();
	virtual void makeNull();
	virtual void _makeEmpty();
public:
	virtual ~TAVLTree();
	virtual void makeEmpty();
};



#endif /* TAVLTREE_H_ */

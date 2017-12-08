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
#include "TList.h"
using namespace std;

template<typename T>
class BinarySearchTree {
protected:
	//Contains the abstract data stored in this node
	T* _root;
	//Left subtree
	BinarySearchTree<T>* _left;
	//Right Subtree
	BinarySearchTree<T>* _right;
	//Ensures that only certain methods are called on the head of the tree
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
	virtual void preOrder(ostream& os);
	virtual void postOrder(ostream& os);
	virtual void inOrder(ostream& os);
	virtual void buildList(TList<T>* list);
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
	_root = NULL;
	_left = NULL;
	_right = NULL;
	_subtree = false;
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(T& otherData) {
	_root = new T(otherData);
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
	_root = otherTree->_root;
	_left = otherTree->_left;
	_right = otherTree->_right;
	//Don't change _subtree since the root node may copy data from a leaf
	//_subtree = otherTree->_subtree;
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree() {
	if(_root != NULL) {
		_root = NULL;
	}
	if(_left != NULL) {
		_left = NULL;
	}
	if(_right != NULL) {
		_right = NULL;
	}
}

template<typename T>
bool BinarySearchTree<T>::isEmpty() {
	return (_root == NULL);
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
	return *_root;
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

/*
 * Used to remove all data from the tree
 */
template<typename T>
void BinarySearchTree<T>::makeEmpty() {
	if(_subtree) throw BinarySearchTreeException();
	if(_root != NULL) {
		delete _root;
		_root = NULL;
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

/*
 * Remove pointers from a node, used when moving data
 */
template<typename T>
void BinarySearchTree<T>::makeNull() {
	_root = NULL;
	_left = NULL;
	_right = NULL;
}

template<typename T>
T BinarySearchTree<T>::find(const T& data) {
	BinarySearchTree<T>* bst = this;

	while(true) {
		if(bst->isEmpty()) throw BinarySearchTreeEmpty();
		if( *(bst->_root) < data) {
			bst = bst->_right;
		} else if(*(bst->_root) > data) {
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
		if( *(bst->_root) < data) {
			bst = bst->_right;
		} else if(*(bst->_root) > data) {
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
		bst->_root = new T(data);
		bst->_left = makeSubtree();
		bst->_right = makeSubtree();
	} else {
		delete bst->_root;
		bst->_root = new T(data);
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
	bst1 = _find(data);
	if(bst1->isEmpty()) throw BinarySearchTreeNotFound();
	delete bst1->_root;

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

		bst1->_root = bst2->_root;

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

/*
 * ---Printing Methods---
 */
template<typename T>
void BinarySearchTree<T>::preOrder(ostream& os){
	if(isEmpty()) return;
	os << root() << endl;
	_left->preOrder(os);
	_right->preOrder(os);
}

template<typename T>
void BinarySearchTree<T>::postOrder(ostream& os){
	if(isEmpty()) return;
	_left->postOrder(os);
	_right->postOrder(os);
	os << root() << endl;
}

template<typename T>
void BinarySearchTree<T>::inOrder(ostream& os) {
	if(isEmpty()) return;
	_left->inOrder(os);
	os << root() << endl;
	_right->inOrder(os);
}

/*
 * Construct a linked list from the data in the Tree. Adds elements in order into the list
 */
template<typename T>
void BinarySearchTree<T>::buildList(TList<T>* list) {
	if(isEmpty()) return;
	_right->buildList(list);
	list->add(*_root);
	_left->buildList(list);
}

//BEGIN SELF_MODIFYING_BST_CLASSES
template<typename T>
class SelfModifyingBST : public BinarySearchTree<T> {
protected:
	using BinarySearchTree<T>::_root;
	using BinarySearchTree<T>::_left;
	using BinarySearchTree<T>::_right;
	using BinarySearchTree<T>::_subtree;

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

/*
 * Rotate tree to the right at a given node
 */
template<typename T>
void SelfModifyingBST<T>::zag() {
	if(this->isEmpty()) return;
	if(_right->isEmpty()) return;

	SelfModifyingBST<T>* rightChild = (SelfModifyingBST<T>*)_right;
	_right = rightChild->_right;
	rightChild->_right = rightChild->_left;
	rightChild->_left = _left;
	_left = rightChild;
	swap (_root, rightChild->_root);
}

/*
 * Rotate tree to the left at a given node
 */
template<typename T>
void SelfModifyingBST<T>::zig() {
	if(this->isEmpty()) return;
	if(_left->isEmpty()) return;

	SelfModifyingBST<T>* leftChild = (SelfModifyingBST<T>*)_left;
	_left = leftChild->_left;
	leftChild->_left = leftChild->_right;
	leftChild->_right = _right;
	_right = leftChild;
	swap (_root, leftChild->_root);
}

/*
 * Left rotation followed by a right rotation
 */
template<typename T>
void SelfModifyingBST<T>::zagzig() {
	if(this->isEmpty()) return;
	((SelfModifyingBST<T>*)_right)->zig();
	zag();
}

/*
 * Right rotation followed by a left rotation
 */
template<typename T>
void SelfModifyingBST<T>::zigzag() {
	if(this->isEmpty()) return;
	((SelfModifyingBST<T>*)_left)->zag();
	zig();
}

/*
 * CURRNETLY UNUSED
 */
template<typename T>
void SelfModifyingBST<T>::zigzig() {
	zig();
	zig();
}

/*
 * CURRENTLY UNUSED
 */
template<typename T>
void SelfModifyingBST<T>::zagzag() {
	zag();
	zag();
}

template<typename T>
class TAVLTree : public SelfModifyingBST<T> {
protected:
	using BinarySearchTree<T>::_root;
	using BinarySearchTree<T>::_left;
	using BinarySearchTree<T>::_right;
	using BinarySearchTree<T>::_subtree;

	//Keep track of the balance of a node (right - left)
	int _diff;

	virtual BinarySearchTree<T>* makeSubtree();
	virtual void makeNull();
	virtual void _makeEmpty();
	TAVLTree<T>* avlleft();
	TAVLTree<T>* avlright();
	virtual void _insert(const T& data);
	virtual void zig();
	virtual void zag();
	virtual void rebalance();
	virtual void _remove(const T& data);
public:
	TAVLTree();
	TAVLTree(T& data);
	virtual ~TAVLTree();
	virtual void makeEmpty();
	int difference();
	virtual void insert(const T& data);
	virtual void remove(const T& data);
};

template<typename T>
TAVLTree<T>::TAVLTree() : SelfModifyingBST<T>() {
	_diff = 0;
}

template<typename T>
TAVLTree<T>::TAVLTree(T& data) : SelfModifyingBST<T>(data) {
	_diff = 0;
}

template<typename T>
TAVLTree<T>::~TAVLTree() {
	if(_root != NULL) {
		_root = NULL;
	}
	if(_left != NULL) {
		_left = NULL;
	}
	if(_right != NULL) {
		_right = NULL;
	}
}

template<typename T>
BinarySearchTree<T>* TAVLTree<T>::makeSubtree() {
	TAVLTree<T>* bst = new TAVLTree();
	bst->_subtree = true;
	return bst;
}

template<typename T>
void TAVLTree<T>::_makeEmpty() {
	if(_root != NULL) {
		delete _root;
		_root = NULL;
	}
	if(_left != NULL) {
		delete _left;
		_left = NULL;
	}
	if(_right != NULL) {
		delete _right;
		_right = NULL;
	}
	_diff = 0;
}

template<typename T>
void TAVLTree<T>::makeEmpty() {
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	_makeEmpty();
}

template<typename T>
void TAVLTree<T>::makeNull() {
	SelfModifyingBST<T>::makeNull();
	_diff = 0;
}

template<typename T>
int TAVLTree<T>::difference() {
	return _diff;
}

template<typename T>
TAVLTree<T>* TAVLTree<T>::avlleft() {
	return ((TAVLTree<T>*)_left);
}

template<typename T>
TAVLTree<T>* TAVLTree<T>::avlright() {
	return ((TAVLTree<T>*)_right);
}

template<typename T>
void TAVLTree<T>::insert(const T& data) {
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	_insert(data);
}

template<typename T>
void TAVLTree<T>::_insert(const T& data) {
	//If no data is stored in current node, enter data and create new leafs
	if(this->isEmpty()) { //If current node is empty
		_root = new T(data);
		_left = makeSubtree();
		_right = makeSubtree();
		_diff = 0;
		return;
	}

	if(*(_root) == data) { //If data equals the root data
		delete _root;
		_root = new T(data);
		return;
	}

	int oldDiff;
	if((*_root) > data) { //If data to be inserted is less than root
		if(avlleft()->isEmpty()) {
			avlleft()->_insert(data);
			_diff--;
		} else {
			oldDiff = avlleft()->_diff;
			avlleft()->_insert(data);

			if((oldDiff != avlleft()->_diff) && (avlleft()->_diff != 0)) {
				_diff--;
			}
		}
	} else { //If data to be inserted is greater than root
		if(avlright()->isEmpty()) {
			avlright()->_insert(data);
			_diff++;
		} else {
			oldDiff = avlright()->_diff;
			avlright()->_insert(data);

			if((oldDiff != avlright()->_diff) && (avlright()->_diff != 0)) {
				_diff++;
			}
		}
	}

	rebalance();
}

template<typename T>
void TAVLTree<T>::zig() {
	if(this->isEmpty()) return;
	if(_left->isEmpty()) return;

	int grandDiff = _diff;
	int parentDiff = avlleft()->_diff;

	SelfModifyingBST<T>::zig();

	if(parentDiff < 0) { //Left of grandDiff is defined by the left of parentDiff
		avlright()->_diff = grandDiff - parentDiff + 1;
		_diff = grandDiff + 2;
	} else {
		avlright()->_diff = 1 + grandDiff;
		_diff = 1 + parentDiff;
	}
}

template<typename T>
void TAVLTree<T>::zag() {
	if(this->isEmpty()) return;
	if(_right->isEmpty()) return;

	int grandDiff = _diff;
	int parentDiff = avlright()->_diff;

	SelfModifyingBST<T>::zag();

	if(parentDiff > 0) { //Right of grandDiff is defined by the right of parentDiff
		avlleft()->_diff = grandDiff - parentDiff - 1;
		_diff = grandDiff - 2;
	} else {
		avlleft()->_diff = grandDiff - 1;
		_diff = parentDiff - 1;
	}
}

template<typename T>
void TAVLTree<T>::rebalance() {
	if((_diff >= -1) && (_diff <= 1)) {
		return;
	}
	else if((_diff < 0) && (avlleft()->_diff <= 0)) {
		this->zig(); //left-left violation
	}
	else if((_diff < 0) && (avlleft()->_diff) > 0) {
		this->zigzag(); //left-right violation
	}
	else if((_diff > 0) && (avlright()->_diff) < 0) {
		this->zagzig(); //right-left violation
	}
	else if((_diff > 0) && (avlright()->_diff) >= 0) {
		this->zag(); //right-right violation
	}

}

template<typename T>
void TAVLTree<T>::remove(const T& data) {
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	_remove(data);
}

template<typename T>
void TAVLTree<T>::_remove(const T& data) {
	if(this->isEmpty()) throw BinarySearchTreeNotFound();
	int oldDiff;

	if((*_root) > data) {
		oldDiff = avlleft()->_diff;
		avlleft()->_remove(data);

		if((_left->isEmpty()) || ((avlleft()->_diff != oldDiff) && (avlleft()->_diff == 0))) {
			_diff++;
		}
	}
	else if((*_root) < data) {
		oldDiff = avlright()->_diff;
		avlright()->_remove(data);

		if((_right->isEmpty()) || ((avlright()->_diff != oldDiff) && (avlright()->_diff == 0))) {
			_diff--;
		}
	}
	else {
		if(_right->isEmpty()) {
			TAVLTree<T>* oldLeft = avlleft();
			delete _right;
			delete _root;
			this->copyTree(_left);
			oldLeft->makeNull();
			delete oldLeft;
			_diff = 0;
		}
		else if(_left->isEmpty()) {
			TAVLTree<T>* oldRight = avlright();
			delete _left;
			delete _root;
			this->copyTree(_right);
			oldRight->makeNull();
			delete oldRight;
			_diff = 0;
		}
		else {
			TAVLTree<T>* succ = avlright();
			while(!succ->_left->isEmpty()) {
				succ = succ->avlleft();
			}
			delete _root;
			_root = new T(*(succ->_root));

			oldDiff = avlright()->_diff;
			avlright()->_remove(*(succ->_root));
			if((_right->isEmpty()) || ((avlright()->_diff != oldDiff) && (avlright()->_diff == 0))) {
				_diff--;
			}
		}
	}

	rebalance();
}



#endif /* TAVLTREE_H_ */

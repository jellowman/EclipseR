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
	TAVLTree<T>* avlleft();
	TAVLTree<T>* avlright();
	virtual void _insert(T& data);
	virtual void zig();
	virtual void zag();
	virtual void rebalance();
	virtual void _remove(T& data);
public:
	TAVLTree();
	TAVLTree(T& data);
	virtual ~TAVLTree();
	virtual void makeEmpty();
	virtual void printTree(ostream& os, int level);
	int difference();
	virtual void insert(T& data);
	virtual void remove(T& data);
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
BinarySearchTree<T>* TAVLTree<T>::makeSubtree() {
	TAVLTree<T>* bst = new TAVLTree();
	bst->subtree() = true;
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
void TAVLTree<T>::insert(T& data) {
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	_insert(data);
}

template<typename T>
void TAVLTree<T>::_insert(T& data) {
	//If no data is stored in current node, enter data and create new leafs
	if(isEmpty()) { //If current node is empty
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
			oldDiff = avlleft()->_diff
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
	if(isEmpty()) return;
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
	if(isEmpty()) return;
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
		zig(); //left-left violation
	}
	else if((_diff < 0) && (avlleft()->_diff) > 0) {
		zigzag(); //left-right violation
	}
	else if((_diff > 0) && (avlright()->_diff) < 0) {
		zagzig(); //right-left violation
	}
	else if((_diff > 0) && (avlright()->_diff) >= 0) {
		zag(); //right-right violation
	}

}

template<typename T>
void TAVLTree<T>::remove(T& data) {
	if(_subtree) throw BinarySearchTreeChangeSubtree();
	_remove(data);
}

template<typename T>
void TAVLTree<T>::_remove(T& data) {
	if(isEmpty()) throw BinarySearchTreeNotFound();
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
			copyTree(_left);
			oldLeft->makeNull();
			delete oldLeft;
			_diff = 0;
		}
		else if(_left->isEmpty()) {
			TAVLTree<T>* oldRight = avlright();
			delete _left;
			delete _root;
			copyTree(_right);
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

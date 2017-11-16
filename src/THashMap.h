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
	THashMap(const THashMap<T>& otherMap);
	void operator=(const THashMap<T>& otherMap);
	void add(T& obj, int key);

	void remove(int key);
	template<typename U> friend std::ostream& operator<< (std::ostream& os, const THashMap<U>& thisObject);

private:
	Tarray<TList<T> >* hashTable;
	TList<T>* mapList;
};




#endif /* SRC_THASHMAP_H_ */

#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


//static functions - start
//%1 - projected attributes
inline static void hashCode(int&h, int& user1Id ) __attribute__((always_inline));
static void increaseArraySize(Follower1ProjectedTupleMap* follower1ProjectedTupleEntryArray);
inline static Follower1ProjectedTupleEntry* next() __attribute__((always_inline));
static void ensureSize(Follower1ProjectedTupleMap* follower1ProjectedTupleMap);
//static functions - end

//iterator - start
static int currentCapacity;
static int index1 = 0;
static Follower1ProjectedTupleEntry* currentP = NULL;
static Follower1ProjectedTupleEntry** currentProjectedTupleEntryArray;
//iterator - end

__inline static void ensureSize(Follower1ProjectedTupleMap* follower1ProjectedTupleMap) {
    if(follower1ProjectedTupleMap->size >= follower1ProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(follower1ProjectedTupleMap);
    }
}
static void increaseArraySize(Follower1ProjectedTupleMap* follower1ProjectedTupleMap) {
	int newcapacity = follower1ProjectedTupleMap->capacity * 2;
	Follower1ProjectedTupleEntry** newprojectedTupleEntryArray = new Follower1ProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < follower1ProjectedTupleMap->capacity; i++) {
		Follower1ProjectedTupleEntry* p = follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31; 
			hashCode(h,
			 //%1 - projected attributes
			 p->user1Id 
			);
			h = h & (newcapacity - 1);
			do {
                Follower1ProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray);
	follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray = newprojectedTupleEntryArray;
	follower1ProjectedTupleMap->capacity = newcapacity;
}
//%3 - projected attributes
Follower1ProjectedTupleEntry* putFollower1ProjectedTuple(Follower1ProjectedTupleMap* follower1ProjectedTupleMap, int& user1Id ) {
	int h = 31;
	hashCode(h, user1Id ); 
	h = h & (follower1ProjectedTupleMap->capacity - 1);
	Follower1ProjectedTupleEntry* candidate = follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[h];
	Follower1ProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
		  //%1 - projected attributes
				candidate->user1Id == (user1Id)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Follower1ProjectedTupleEntry* p = new Follower1ProjectedTupleEntry;
	//%1 - projected attributes
	p->user1Id = user1Id;
	if(!lastP)
		follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	follower1ProjectedTupleMap->size++;
	ensureSize(follower1ProjectedTupleMap);
	return p;
}
//arguments based on key
//%1 - projected attributes
__inline static void hashCode(int& h, int& user1Id ) {
    h ^=  user1Id + 0x9e3779b9 + (h<<6) + (h>>2);
}
//arguments based on key
//void remove(%1$1sProjectedTupleMap* %2$1sProjectedTupleMap, int userId, int %2$1sId, std::string %2$1sDate) {
	//implement later
	//if count = 0, delete auto
//	if(--%2$1sProjectedTupleMap->size == 0)
//		free(&%2$1sProjectedTupleMap);
//}

Follower1ProjectedTupleIteratorNext follower1ProjectedTupleIterator(Follower1ProjectedTupleMap* follower1ProjectedTupleMap) {
	currentProjectedTupleEntryArray = follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray;
	index1 = 0;
	currentP = NULL;
	currentCapacity = follower1ProjectedTupleMap->capacity;
	return &next;
}

__inline static Follower1ProjectedTupleEntry* next() {
	for(; index1 < currentCapacity; index1++, currentP = NULL) {
		if(currentProjectedTupleEntryArray[index1]) {
			if(currentP) {
				currentP = currentP->next;
				if(currentP)
					return currentP;
			} else
				return (currentP = currentProjectedTupleEntryArray[index1]);
		}
	}
	return NULL;
}

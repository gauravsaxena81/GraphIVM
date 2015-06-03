#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& follower1Id ) __attribute__((always_inline));
static void increaseArraySize(Follower1ProjectedTupleMap* follower1ProjectedTupleEntryArray);
static void ensureSize(Follower1ProjectedTupleMap* follower1ProjectedTupleMap);


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
			do {
    			int h = 31; 
                hashCode(h,
                 p->follower1Id 
                );
                h = h & (newcapacity - 1);
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

__inline static void hashCode(int& h, int& follower1Id ) {
    h ^=  follower1Id + 0x9e3779b9 + (h<<6) + (h>>2);
}
Follower1ProjectedTupleEntry* putFollower1ProjectedTuple(Follower1ProjectedTupleMap* follower1ProjectedTupleMap, int& follower1Id ) {
	int h = 31;
	hashCode(h, follower1Id ); 
	h = h & (follower1ProjectedTupleMap->capacity - 1);
	Follower1ProjectedTupleEntry* candidate = follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[h];
	Follower1ProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->follower1Id == (follower1Id)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Follower1ProjectedTupleEntry* p = new Follower1ProjectedTupleEntry;
	p->follower1Id = follower1Id;
	if(!lastP)
		follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	follower1ProjectedTupleMap->size++;
	ensureSize(follower1ProjectedTupleMap);
	return p;
}

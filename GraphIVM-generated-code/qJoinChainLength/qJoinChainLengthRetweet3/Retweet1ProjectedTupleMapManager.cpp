#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& r1TweetId ) __attribute__((always_inline));
static void increaseArraySize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleEntryArray);
static void ensureSize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap);


__inline static void ensureSize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap) {
    if(retweet1ProjectedTupleMap->size >= retweet1ProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(retweet1ProjectedTupleMap);
    }
}
static void increaseArraySize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap) {
	int newcapacity = retweet1ProjectedTupleMap->capacity * 2;
	Retweet1ProjectedTupleEntry** newprojectedTupleEntryArray = new Retweet1ProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < retweet1ProjectedTupleMap->capacity; i++) {
		Retweet1ProjectedTupleEntry* p = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[i];
		if(p) {
			do {
    			int h = 31; 
                hashCode(h,
                 p->r1TweetId 
                );
                h = h & (newcapacity - 1);
                Retweet1ProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray);
	retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray = newprojectedTupleEntryArray;
	retweet1ProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& r1TweetId ) {
    h ^=  r1TweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
Retweet1ProjectedTupleEntry* putRetweet1ProjectedTuple(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap, int& r1TweetId ) {
	int h = 31;
	hashCode(h, r1TweetId ); 
	h = h & (retweet1ProjectedTupleMap->capacity - 1);
	Retweet1ProjectedTupleEntry* candidate = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[h];
	Retweet1ProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->r1TweetId == (r1TweetId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet1ProjectedTupleEntry* p = new Retweet1ProjectedTupleEntry;
	p->r1TweetId = r1TweetId;
	if(!lastP)
		retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	retweet1ProjectedTupleMap->size++;
	ensureSize(retweet1ProjectedTupleMap);
	return p;
}

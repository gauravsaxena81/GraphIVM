#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


//static functions - start
//%1 - projected attributes
inline static void hashCode(int&h, int& tweetId ) __attribute__((always_inline));
static void increaseArraySize(TweetProjectedTupleMap* tweetProjectedTupleEntryArray);
inline static TweetProjectedTupleEntry* next() __attribute__((always_inline));
static void ensureSize(TweetProjectedTupleMap* tweetProjectedTupleMap);
//static functions - end

//iterator - start
static int currentCapacity;
static int index1 = 0;
static TweetProjectedTupleEntry* currentP = NULL;
static TweetProjectedTupleEntry** currentProjectedTupleEntryArray;
//iterator - end

__inline static void ensureSize(TweetProjectedTupleMap* tweetProjectedTupleMap) {
    if(tweetProjectedTupleMap->size >= tweetProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(tweetProjectedTupleMap);
    }
}
static void increaseArraySize(TweetProjectedTupleMap* tweetProjectedTupleMap) {
	int newcapacity = tweetProjectedTupleMap->capacity * 2;
	TweetProjectedTupleEntry** newprojectedTupleEntryArray = new TweetProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < tweetProjectedTupleMap->capacity; i++) {
		TweetProjectedTupleEntry* p = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31; 
			hashCode(h,
			 //%1 - projected attributes
			 p->tweetId 
			);
			h = h & (newcapacity - 1);
			do {
                TweetProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(tweetProjectedTupleMap->tweetProjectedTupleEntryArray);
	tweetProjectedTupleMap->tweetProjectedTupleEntryArray = newprojectedTupleEntryArray;
	tweetProjectedTupleMap->capacity = newcapacity;
}
//%3 - projected attributes
TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap, int& tweetId ) {
	int h = 31;
	hashCode(h, tweetId ); 
	h = h & (tweetProjectedTupleMap->capacity - 1);
	TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h];
	TweetProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
		  //%1 - projected attributes
				candidate->tweetId == (tweetId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	TweetProjectedTupleEntry* p = new TweetProjectedTupleEntry;
	//%1 - projected attributes
	p->tweetId = tweetId;
	if(!lastP)
		tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	tweetProjectedTupleMap->size++;
	ensureSize(tweetProjectedTupleMap);
	return p;
}
//arguments based on key
//%1 - projected attributes
__inline static void hashCode(int& h, int& tweetId ) {
    h ^=  tweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
//arguments based on key
//void remove(%1$1sProjectedTupleMap* %2$1sProjectedTupleMap, int userId, int %2$1sId, std::string %2$1sDate) {
	//implement later
	//if count = 0, delete auto
//	if(--%2$1sProjectedTupleMap->size == 0)
//		free(&%2$1sProjectedTupleMap);
//}

TweetProjectedTupleIteratorNext tweetProjectedTupleIterator(TweetProjectedTupleMap* tweetProjectedTupleMap) {
	currentProjectedTupleEntryArray = tweetProjectedTupleMap->tweetProjectedTupleEntryArray;
	index1 = 0;
	currentP = NULL;
	currentCapacity = tweetProjectedTupleMap->capacity;
	return &next;
}

__inline static TweetProjectedTupleEntry* next() {
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

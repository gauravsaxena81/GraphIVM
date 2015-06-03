#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& followerId, int& retweet2UserIdFollowerUserId ) __attribute__((always_inline));
static void increaseArraySize(FollowerProjectedTupleMap* followerProjectedTupleEntryArray);
static void ensureSize(FollowerProjectedTupleMap* followerProjectedTupleMap);


__inline static void ensureSize(FollowerProjectedTupleMap* followerProjectedTupleMap) {
    if(followerProjectedTupleMap->size >= followerProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(followerProjectedTupleMap);
    }
}
static void increaseArraySize(FollowerProjectedTupleMap* followerProjectedTupleMap) {
	int newcapacity = followerProjectedTupleMap->capacity * 2;
	FollowerProjectedTupleEntry** newprojectedTupleEntryArray = new FollowerProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < followerProjectedTupleMap->capacity; i++) {
		FollowerProjectedTupleEntry* p = followerProjectedTupleMap->followerProjectedTupleEntryArray[i];
		if(p) {
			do {
			    int h = 31; 
                hashCode(h
                 , p->followerId 
                 , p->retweet2UserIdFollowerUserId 
                );
                h = h & (newcapacity - 1);
                FollowerProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(followerProjectedTupleMap->followerProjectedTupleEntryArray);
	followerProjectedTupleMap->followerProjectedTupleEntryArray = newprojectedTupleEntryArray;
	followerProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& followerId, int& retweet2UserIdFollowerUserId ) {
    h ^=  followerId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  retweet2UserIdFollowerUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}
FollowerProjectedTupleEntry* putFollowerProjectedTuple(FollowerProjectedTupleMap* followerProjectedTupleMap, int& followerId, int& retweet2UserIdFollowerUserId ) {
	int h = 31;
	hashCode(h
	, followerId
	, retweet2UserIdFollowerUserId
	); 
	h = h & (followerProjectedTupleMap->capacity - 1);
	FollowerProjectedTupleEntry* candidate = followerProjectedTupleMap->followerProjectedTupleEntryArray[h];
	FollowerProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->followerId == (followerId)
&& 				candidate->retweet2UserIdFollowerUserId == (retweet2UserIdFollowerUserId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	FollowerProjectedTupleEntry* p = new FollowerProjectedTupleEntry;
	p->followerId = followerId;
	p->retweet2UserIdFollowerUserId = retweet2UserIdFollowerUserId;
	if(!lastP)
		followerProjectedTupleMap->followerProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	followerProjectedTupleMap->size++;
	ensureSize(followerProjectedTupleMap);
	return p;
}
int deleteFollowerProjectedTupleIfZero(FollowerProjectedTupleMap* followerProjectedTupleMap
, int& followerId 
, int& retweet2UserIdFollowerUserId 
) {
    int h = 31;
    hashCode(h
    , followerId
    , retweet2UserIdFollowerUserId
    );
    h = h & (followerProjectedTupleMap->capacity - 1);
    FollowerProjectedTupleEntry* candidate = followerProjectedTupleMap->followerProjectedTupleEntryArray[h];
    FollowerProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
              candidate->followerId == followerId
            && candidate->retweet2UserIdFollowerUserId == retweet2UserIdFollowerUserId
        ) {
            if(--candidate->count == 0) {
                if(lastP) {
                    lastP->next = candidate->next;
                } else
                    followerProjectedTupleMap->followerProjectedTupleEntryArray[h] = NULL;
                delete candidate;
            }
            return --(followerProjectedTupleMap->size);
        } else {
            candidate = candidate->next;
        }
    }
    return -1;
}



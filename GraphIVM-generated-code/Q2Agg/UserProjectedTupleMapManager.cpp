#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
inline static void hashCode(int&h, int& followerId ) __attribute__((always_inline));
static void increaseArraySize(UserProjectedTupleMap* userProjectedTupleEntryArray);
static void ensureSize(UserProjectedTupleMap* userProjectedTupleMap);


__inline static void ensureSize(UserProjectedTupleMap* userProjectedTupleMap) {
    if(userProjectedTupleMap->size >= userProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(userProjectedTupleMap);
    }
}
static void increaseArraySize(UserProjectedTupleMap* userProjectedTupleMap) {
	int newcapacity = userProjectedTupleMap->capacity * 2;
	UserProjectedTupleEntry** newprojectedTupleEntryArray = new UserProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < userProjectedTupleMap->capacity; i++) {
		UserProjectedTupleEntry* p = userProjectedTupleMap->userProjectedTupleEntryArray[i];
		if(p) {
		    int h = 31; 
            hashCode(h
             , p->followerId 
            );
            h = h & (newcapacity - 1);
			do {
                UserProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(userProjectedTupleMap->userProjectedTupleEntryArray);
	userProjectedTupleMap->userProjectedTupleEntryArray = newprojectedTupleEntryArray;
	userProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& followerId ) {
    h ^=  followerId + 0x9e3779b9 + (h<<6) + (h>>2);
}
UserProjectedTupleEntry* putUserProjectedTuple(UserProjectedTupleMap* userProjectedTupleMap, int& followerId ) {
	int h = 31;
	hashCode(h
	, followerId
	); 
	h = h & (userProjectedTupleMap->capacity - 1);
	UserProjectedTupleEntry* candidate = userProjectedTupleMap->userProjectedTupleEntryArray[h];
	UserProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->followerId == (followerId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	UserProjectedTupleEntry* p = new UserProjectedTupleEntry;
	p->followerId = followerId;
	if(!lastP)
		userProjectedTupleMap->userProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	userProjectedTupleMap->size++;
	ensureSize(userProjectedTupleMap);
	return p;
}
int deleteUserProjectedTupleIfZero(UserProjectedTupleMap* userProjectedTupleMap
, int& followerId 
) {
    int h = 31;
    hashCode(h
    , followerId
    );
    h = h & (userProjectedTupleMap->capacity - 1);
    UserProjectedTupleEntry* candidate = userProjectedTupleMap->userProjectedTupleEntryArray[h];
    UserProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
            candidate->followerId == followerId
        ) {
            if(--candidate->count == 0) {
                if(lastP) {
                    lastP->next = candidate->next;
                } else
                    userProjectedTupleMap->userProjectedTupleEntryArray[h] = NULL;
                delete candidate;
            }
            return --(userProjectedTupleMap->size);
        } else {
            candidate = candidate->next;
        }
    }
    return -1;
}



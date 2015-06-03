#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& retweet2UserIdFollowerUserId) __attribute__((always_inline));
static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap);
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap);
static void addToVertexValues(int& retweet2UserIdFollowerUserId , FollowerJoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap);



__inline static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap) {
    if(followerJoinTupleMap->size >= followerJoinTupleMap->capacity * 0.8) {
        increaseArraySize(followerJoinTupleMap);
    }
}
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap) {
    int newcapacity = followerJoinTupleMap->capacity * 2;
    FollowerJoinTupleEntry** newJoinTupleEntryArray = new FollowerJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < followerJoinTupleMap->capacity; i++) {
        FollowerJoinTupleEntry* p = followerJoinTupleMap->followerJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h
                , p->retweet2FollowerVertexValue->retweet2UserIdFollowerUserId
                );
                h = h & (newcapacity - 1);
                FollowerJoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete followerJoinTupleMap->followerJoinTupleEntryArray;
    followerJoinTupleMap->followerJoinTupleEntryArray = newJoinTupleEntryArray;
    followerJoinTupleMap->capacity = newcapacity;
}
FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& retweet2UserIdFollowerUserId, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, retweet2UserIdFollowerUserId ); 
    h = h & (followerJoinTupleMap->capacity - 1);
    FollowerJoinTupleEntry* candidate = followerJoinTupleMap->followerJoinTupleEntryArray[h];
    FollowerJoinTupleEntry* lastP = NULL;
    while(candidate) {
        if(
                candidate->retweet2FollowerVertexValue->retweet2UserIdFollowerUserId == retweet2UserIdFollowerUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    FollowerJoinTupleEntry* p = new FollowerJoinTupleEntry;
    addToVertexValues(retweet2UserIdFollowerUserId , p, retweet2FollowerVertexValueMap);
    if(!lastP)
        followerJoinTupleMap->followerJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    followerJoinTupleMap->size++;
    ensureSize(followerJoinTupleMap);
    return p;
}
__inline static void addToVertexValues(int& retweet2UserIdFollowerUserId , FollowerJoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap) {
    Retweet2FollowerVertexValueEntry* vertexValue = putRetweet2FollowerVertexValueIfAbsent(retweet2FollowerVertexValueMap, retweet2UserIdFollowerUserId );
    p->retweet2FollowerVertexValue = vertexValue;
    vertexValue->followerJoinTupleList.push_back(p);
}

inline static void hashCode(int&h, int& retweet2UserIdFollowerUserId) {
    h ^=  retweet2UserIdFollowerUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



FollowerJoinTupleEntry** getFollowerJoinTuplePointer(FollowerJoinTupleMap* followerJoinTupleMap
, int& retweet2UserIdFollowerUserId
)  {
    int h = 31;
    hashCode(h
    , retweet2UserIdFollowerUserId  
    );
    h = h & (followerJoinTupleMap->capacity - 1);
    FollowerJoinTupleEntry* candidate = followerJoinTupleMap->followerJoinTupleEntryArray[h];
    FollowerJoinTupleEntry** address = &(followerJoinTupleMap->followerJoinTupleEntryArray[h]);
    while(candidate) {
        if(
              candidate->retweet2FollowerVertexValue->retweet2UserIdFollowerUserId == retweet2UserIdFollowerUserId
        ) {
            return address;
        } else {
            address = &(candidate->next);
            candidate = candidate->next;
        }
    }
    return NULL;
}
void removeJoinTupleEntry(FollowerJoinTupleMap* followerJoinTupleMap, FollowerJoinTupleEntry** entryPointer) {
    if((*entryPointer)->next) {
        (*entryPointer)->next = (*entryPointer)->next->next;
    } else {
        delete *entryPointer;
        (*entryPointer) = NULL;
    }
    followerJoinTupleMap->size--;
}

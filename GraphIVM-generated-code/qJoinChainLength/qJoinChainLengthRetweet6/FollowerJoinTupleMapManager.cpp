#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r5UserIdFUserId) __attribute__((always_inline));
static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r5UserIdFUserId);
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r5UserIdFUserId);
static void addToVertexValues(int& r5UserIdFUserId , FollowerJoinTupleEntry* p, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap);



__inline static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r5UserIdFUserId) {
    if(followerJoinTupleMap->size >= followerJoinTupleMap->capacity * 0.8) {
        increaseArraySize(followerJoinTupleMap, r5UserIdFUserId );
    }
}
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r5UserIdFUserId) {
    int newcapacity = followerJoinTupleMap->capacity * 2;
    FollowerJoinTupleEntry** newJoinTupleEntryArray = new FollowerJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < followerJoinTupleMap->capacity; i++) {
        FollowerJoinTupleEntry* p = followerJoinTupleMap->followerJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r5UserIdFUserId );
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
FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& r5UserIdFUserId, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r5UserIdFUserId ); 
    h = h & (followerJoinTupleMap->capacity - 1);
    FollowerJoinTupleEntry* candidate = followerJoinTupleMap->followerJoinTupleEntryArray[h];
    FollowerJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet5FollowerVertexValue->r5UserIdFUserId == r5UserIdFUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    FollowerJoinTupleEntry* p = new FollowerJoinTupleEntry;
    if(!lastP)
        followerJoinTupleMap->followerJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    followerJoinTupleMap->size++;
    ensureSize(followerJoinTupleMap, r5UserIdFUserId );
    addToVertexValues(r5UserIdFUserId , p, retweet5FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r5UserIdFUserId , FollowerJoinTupleEntry* p, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap) {
    Retweet5FollowerVertexValueEntry* vertexValue = putRetweet5FollowerVertexValueIfAbsent(retweet5FollowerVertexValueMap, r5UserIdFUserId );
    p->retweet5FollowerVertexValue = vertexValue;
    vertexValue->followerJoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& r5UserIdFUserId) {
    h ^=  r5UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



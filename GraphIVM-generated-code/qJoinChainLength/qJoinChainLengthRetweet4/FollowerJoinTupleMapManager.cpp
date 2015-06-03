#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r3UserIdFUserId) __attribute__((always_inline));
static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r3UserIdFUserId);
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r3UserIdFUserId);
static void addToVertexValues(int& r3UserIdFUserId , FollowerJoinTupleEntry* p, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap);



__inline static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r3UserIdFUserId) {
    if(followerJoinTupleMap->size >= followerJoinTupleMap->capacity * 0.8) {
        increaseArraySize(followerJoinTupleMap, r3UserIdFUserId );
    }
}
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r3UserIdFUserId) {
    int newcapacity = followerJoinTupleMap->capacity * 2;
    FollowerJoinTupleEntry** newJoinTupleEntryArray = new FollowerJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < followerJoinTupleMap->capacity; i++) {
        FollowerJoinTupleEntry* p = followerJoinTupleMap->followerJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r3UserIdFUserId );
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
FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& r3UserIdFUserId, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r3UserIdFUserId ); 
    h = h & (followerJoinTupleMap->capacity - 1);
    FollowerJoinTupleEntry* candidate = followerJoinTupleMap->followerJoinTupleEntryArray[h];
    FollowerJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet3FollowerVertexValue->r3UserIdFUserId == r3UserIdFUserId
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
    ensureSize(followerJoinTupleMap, r3UserIdFUserId );
    addToVertexValues(r3UserIdFUserId , p, retweet3FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r3UserIdFUserId , FollowerJoinTupleEntry* p, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap) {
    Retweet3FollowerVertexValueEntry* vertexValue = putRetweet3FollowerVertexValueIfAbsent(retweet3FollowerVertexValueMap, r3UserIdFUserId );
    p->retweet3FollowerVertexValue = vertexValue;
    vertexValue->followerJoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->retweet3JoinTupleList.size() > 0);
}
inline static void hashCode(int&h, int& r3UserIdFUserId) {
    h ^=  r3UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



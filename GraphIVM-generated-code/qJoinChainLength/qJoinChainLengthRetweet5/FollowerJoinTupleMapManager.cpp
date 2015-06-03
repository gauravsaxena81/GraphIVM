#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r4UserIdFUserId) __attribute__((always_inline));
static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r4UserIdFUserId);
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r4UserIdFUserId);
static void addToVertexValues(int& r4UserIdFUserId , FollowerJoinTupleEntry* p, Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap);



__inline static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r4UserIdFUserId) {
    if(followerJoinTupleMap->size >= followerJoinTupleMap->capacity * 0.8) {
        increaseArraySize(followerJoinTupleMap, r4UserIdFUserId );
    }
}
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r4UserIdFUserId) {
    int newcapacity = followerJoinTupleMap->capacity * 2;
    FollowerJoinTupleEntry** newJoinTupleEntryArray = new FollowerJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < followerJoinTupleMap->capacity; i++) {
        FollowerJoinTupleEntry* p = followerJoinTupleMap->followerJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r4UserIdFUserId );
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
FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& r4UserIdFUserId, Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r4UserIdFUserId ); 
    h = h & (followerJoinTupleMap->capacity - 1);
    FollowerJoinTupleEntry* candidate = followerJoinTupleMap->followerJoinTupleEntryArray[h];
    FollowerJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet4FollowerVertexValue->r4UserIdFUserId == r4UserIdFUserId
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
    ensureSize(followerJoinTupleMap, r4UserIdFUserId );
    addToVertexValues(r4UserIdFUserId , p, retweet4FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r4UserIdFUserId , FollowerJoinTupleEntry* p, Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap) {
    Retweet4FollowerVertexValueEntry* vertexValue = putRetweet4FollowerVertexValueIfAbsent(retweet4FollowerVertexValueMap, r4UserIdFUserId );
    p->retweet4FollowerVertexValue = vertexValue;
    vertexValue->followerJoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->retweet4JoinTupleList.size() > 0);
}
inline static void hashCode(int&h, int& r4UserIdFUserId) {
    h ^=  r4UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



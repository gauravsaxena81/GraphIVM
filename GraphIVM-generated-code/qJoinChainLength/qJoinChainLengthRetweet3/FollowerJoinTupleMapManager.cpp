#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r2UserIdFUserId) __attribute__((always_inline));
static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r2UserIdFUserId);
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r2UserIdFUserId);
static void addToVertexValues(int& r2UserIdFUserId , FollowerJoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap);



__inline static void ensureSize(FollowerJoinTupleMap* followerJoinTupleMap, int& r2UserIdFUserId) {
    if(followerJoinTupleMap->size >= followerJoinTupleMap->capacity * 0.8) {
        increaseArraySize(followerJoinTupleMap, r2UserIdFUserId );
    }
}
static void increaseArraySize(FollowerJoinTupleMap* followerJoinTupleMap, int& r2UserIdFUserId) {
    int newcapacity = followerJoinTupleMap->capacity * 2;
    FollowerJoinTupleEntry** newJoinTupleEntryArray = new FollowerJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < followerJoinTupleMap->capacity; i++) {
        FollowerJoinTupleEntry* p = followerJoinTupleMap->followerJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r2UserIdFUserId );
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
FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& r2UserIdFUserId, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r2UserIdFUserId ); 
    h = h & (followerJoinTupleMap->capacity - 1);
    FollowerJoinTupleEntry* candidate = followerJoinTupleMap->followerJoinTupleEntryArray[h];
    FollowerJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet2FollowerVertexValue->r2UserIdFUserId == r2UserIdFUserId
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
    ensureSize(followerJoinTupleMap, r2UserIdFUserId );
    addToVertexValues(r2UserIdFUserId , p, retweet2FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r2UserIdFUserId , FollowerJoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap) {
    Retweet2FollowerVertexValueEntry* vertexValue = putRetweet2FollowerVertexValueIfAbsent(retweet2FollowerVertexValueMap, r2UserIdFUserId );
    p->retweet2FollowerVertexValue = vertexValue;
    vertexValue->followerJoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->retweet2JoinTupleList.size() > 0);
}
inline static void hashCode(int&h, int& r2UserIdFUserId) {
    h ^=  r2UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



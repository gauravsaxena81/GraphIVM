#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& user1IdFollower2Id) __attribute__((always_inline));
static void ensureSize(Follower1JoinTupleMap* follower1JoinTupleMap, int& user1IdFollower2Id);
static void increaseArraySize(Follower1JoinTupleMap* follower1JoinTupleMap, int& user1IdFollower2Id);
static void addToVertexValues(int& user1IdFollower2Id , Follower1JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);



__inline static void ensureSize(Follower1JoinTupleMap* follower1JoinTupleMap, int& user1IdFollower2Id) {
    if(follower1JoinTupleMap->size >= follower1JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower1JoinTupleMap, user1IdFollower2Id );
    }
}
static void increaseArraySize(Follower1JoinTupleMap* follower1JoinTupleMap, int& user1IdFollower2Id) {
    int newcapacity = follower1JoinTupleMap->capacity * 2;
    Follower1JoinTupleEntry** newJoinTupleEntryArray = new Follower1JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower1JoinTupleMap->capacity; i++) {
        Follower1JoinTupleEntry* p = follower1JoinTupleMap->follower1JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, user1IdFollower2Id );
                h = h & (newcapacity - 1);
                Follower1JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete follower1JoinTupleMap->follower1JoinTupleEntryArray;
    follower1JoinTupleMap->follower1JoinTupleEntryArray = newJoinTupleEntryArray;
    follower1JoinTupleMap->capacity = newcapacity;
}
Follower1JoinTupleEntry* putFollower1JoinTupleIfAbsent(Follower1JoinTupleMap* follower1JoinTupleMap, int& user1IdFollower2Id, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap ) {
    int h = 31;
    hashCode(h, user1IdFollower2Id ); 
    h = h & (follower1JoinTupleMap->capacity - 1);
    Follower1JoinTupleEntry* candidate = follower1JoinTupleMap->follower1JoinTupleEntryArray[h];
    Follower1JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower1Follower2VertexValue->user1IdFollower2Id == user1IdFollower2Id
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Follower1JoinTupleEntry* p = new Follower1JoinTupleEntry;
    if(!lastP)
        follower1JoinTupleMap->follower1JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower1JoinTupleMap->size++;
    ensureSize(follower1JoinTupleMap, user1IdFollower2Id );
    addToVertexValues(user1IdFollower2Id , p, follower1Follower2VertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& user1IdFollower2Id , Follower1JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
    Follower1Follower2VertexValueEntry* vertexValue = putFollower1Follower2VertexValueIfAbsent(follower1Follower2VertexValueMap, user1IdFollower2Id );
    p->follower1Follower2VertexValue = vertexValue;
    vertexValue->follower1JoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& user1IdFollower2Id) {
    h ^=  user1IdFollower2Id + 0x9e3779b9 + (h<<6) + (h>>2);
}



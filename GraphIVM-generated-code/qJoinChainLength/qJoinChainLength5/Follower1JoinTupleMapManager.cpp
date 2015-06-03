#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower1IdUser2Id) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(Follower1JoinTupleMap* follower1JoinTupleMap, int& follower1IdUser2Id);
//%2 - vertex value attribute values 
static void increaseArraySize(Follower1JoinTupleMap* follower1JoinTupleMap, int& follower1IdUser2Id);
inline static Follower1JoinTupleEntry* next() __attribute__((always_inline));
//%1$1s - join attributes
static void addToVertexValues(int& follower1IdUser2Id , Follower1JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);

//iterator
static int index1 = 0;
static Follower1JoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static Follower1JoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(Follower1JoinTupleMap* follower1JoinTupleMap, int& follower1IdUser2Id) {
    if(follower1JoinTupleMap->size >= follower1JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower1JoinTupleMap, follower1IdUser2Id );
    }
}
//%2 - join attributes
static void increaseArraySize(Follower1JoinTupleMap* follower1JoinTupleMap, int& follower1IdUser2Id) {
    int newcapacity = follower1JoinTupleMap->capacity * 2;
    Follower1JoinTupleEntry** newJoinTupleEntryArray = new Follower1JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower1JoinTupleMap->capacity; i++) {
        Follower1JoinTupleEntry* p = follower1JoinTupleMap->follower1JoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower1IdUser2Id );
            h = h & (newcapacity - 1);
            do {
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
//%3 - join attributes, %4 - vertex values
Follower1JoinTupleEntry* putFollower1JoinTupleIfAbsent(Follower1JoinTupleMap* follower1JoinTupleMap, int& follower1IdUser2Id, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap ) {
    int h = 31;
    hashCode(h, follower1IdUser2Id ); 
    h = h & (follower1JoinTupleMap->capacity - 1);
    Follower1JoinTupleEntry* candidate = follower1JoinTupleMap->follower1JoinTupleEntryArray[h];
    Follower1JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower1Follower2VertexValue->follower1IdUser2Id == follower1IdUser2Id
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    Follower1JoinTupleEntry* p = new Follower1JoinTupleEntry;
    if(!lastP)
        follower1JoinTupleMap->follower1JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower1JoinTupleMap->size++;
    ensureSize(follower1JoinTupleMap, follower1IdUser2Id );
    //%2 - join attribute values
    addToVertexValues(follower1IdUser2Id , p, follower1Follower2VertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower1IdUser2Id , Follower1JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
    //%4 - vertex attribute values
    Follower1Follower2VertexValueEntry* vertexValue = putFollower1Follower2VertexValueIfAbsent(follower1Follower2VertexValueMap, follower1IdUser2Id );
    p->follower1Follower2VertexValue = vertexValue;
    vertexValue->follower1JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower1IdUser2Id) {
    h ^=  follower1IdUser2Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

Follower1JoinTupleIteratorNext follower1JoinTupleIterator(Follower1JoinTupleMap* follower1JoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = follower1JoinTupleMap->capacity;
    currentJoinTupleEntryArray = follower1JoinTupleMap->follower1JoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

__inline static Follower1JoinTupleEntry* next() {
    for(; index1 < currentCapacity; index1++, currentP = NULL) {
        if(currentJoinTupleEntryArray[index1]) {
            if(currentP) {
                currentP = currentP->next;
                if(currentP)
                    return currentP;
            } else
                return (currentP = currentJoinTupleEntryArray[index1]);
        }
    }
    return NULL;
}

#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower1IdUser2Id,int& follower2IdUser3Id) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdUser3Id);
//%2 - vertex value attribute values 
static void increaseArraySize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdUser3Id);
inline static Follower2JoinTupleEntry* next() __attribute__((always_inline));
//%1$1s - join attributes
static void addToVertexValues(int& follower1IdUser2Id , Follower2JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);
static void addToVertexValues(int& follower2IdUser3Id , Follower2JoinTupleEntry* p, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap);

//iterator
static int index1 = 0;
static Follower2JoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static Follower2JoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdUser3Id) {
    if(follower2JoinTupleMap->size >= follower2JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower2JoinTupleMap, follower1IdUser2Id, follower2IdUser3Id );
    }
}
//%2 - join attributes
static void increaseArraySize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdUser3Id) {
    int newcapacity = follower2JoinTupleMap->capacity * 2;
    Follower2JoinTupleEntry** newJoinTupleEntryArray = new Follower2JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower2JoinTupleMap->capacity; i++) {
        Follower2JoinTupleEntry* p = follower2JoinTupleMap->follower2JoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower1IdUser2Id, follower2IdUser3Id );
            h = h & (newcapacity - 1);
            do {
                Follower2JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete follower2JoinTupleMap->follower2JoinTupleEntryArray;
    follower2JoinTupleMap->follower2JoinTupleEntryArray = newJoinTupleEntryArray;
    follower2JoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
Follower2JoinTupleEntry* putFollower2JoinTupleIfAbsent(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdUser3Id, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap ) {
    int h = 31;
    hashCode(h, follower1IdUser2Id, follower2IdUser3Id ); 
    h = h & (follower2JoinTupleMap->capacity - 1);
    Follower2JoinTupleEntry* candidate = follower2JoinTupleMap->follower2JoinTupleEntryArray[h];
    Follower2JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower1Follower2VertexValue->follower1IdUser2Id == follower1IdUser2Id
        &&         candidate->follower2Follower3VertexValue->follower2IdUser3Id == follower2IdUser3Id
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    Follower2JoinTupleEntry* p = new Follower2JoinTupleEntry;
    if(!lastP)
        follower2JoinTupleMap->follower2JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower2JoinTupleMap->size++;
    ensureSize(follower2JoinTupleMap, follower1IdUser2Id, follower2IdUser3Id );
    //%2 - join attribute values
    addToVertexValues(follower1IdUser2Id , p, follower1Follower2VertexValueMap);
    addToVertexValues(follower2IdUser3Id , p, follower2Follower3VertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower1IdUser2Id , Follower2JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
    //%4 - vertex attribute values
    Follower1Follower2VertexValueEntry* vertexValue = putFollower1Follower2VertexValueIfAbsent(follower1Follower2VertexValueMap, follower1IdUser2Id );
    p->follower1Follower2VertexValue = vertexValue;
    vertexValue->follower2JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
__inline static void addToVertexValues(int& follower2IdUser3Id , Follower2JoinTupleEntry* p, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap) {
    //%4 - vertex attribute values
    Follower2Follower3VertexValueEntry* vertexValue = putFollower2Follower3VertexValueIfAbsent(follower2Follower3VertexValueMap, follower2IdUser3Id );
    p->follower2Follower3VertexValue = vertexValue;
    vertexValue->follower2JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower1IdUser2Id,int& follower2IdUser3Id) {
    h ^=  follower1IdUser2Id + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  follower2IdUser3Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

Follower2JoinTupleIteratorNext follower2JoinTupleIterator(Follower2JoinTupleMap* follower2JoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = follower2JoinTupleMap->capacity;
    currentJoinTupleEntryArray = follower2JoinTupleMap->follower2JoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

__inline static Follower2JoinTupleEntry* next() {
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

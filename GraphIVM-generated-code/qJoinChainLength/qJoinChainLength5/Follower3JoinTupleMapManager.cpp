#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower2IdUser3Id,int& follower3IdUser4Id) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(Follower3JoinTupleMap* follower3JoinTupleMap, int& follower2IdUser3Id,int& follower3IdUser4Id);
//%2 - vertex value attribute values 
static void increaseArraySize(Follower3JoinTupleMap* follower3JoinTupleMap, int& follower2IdUser3Id,int& follower3IdUser4Id);
inline static Follower3JoinTupleEntry* next() __attribute__((always_inline));
//%1$1s - join attributes
static void addToVertexValues(int& follower2IdUser3Id , Follower3JoinTupleEntry* p, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap);
static void addToVertexValues(int& follower3IdUser4Id , Follower3JoinTupleEntry* p, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap);

//iterator
static int index1 = 0;
static Follower3JoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static Follower3JoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(Follower3JoinTupleMap* follower3JoinTupleMap, int& follower2IdUser3Id,int& follower3IdUser4Id) {
    if(follower3JoinTupleMap->size >= follower3JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower3JoinTupleMap, follower2IdUser3Id, follower3IdUser4Id );
    }
}
//%2 - join attributes
static void increaseArraySize(Follower3JoinTupleMap* follower3JoinTupleMap, int& follower2IdUser3Id,int& follower3IdUser4Id) {
    int newcapacity = follower3JoinTupleMap->capacity * 2;
    Follower3JoinTupleEntry** newJoinTupleEntryArray = new Follower3JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower3JoinTupleMap->capacity; i++) {
        Follower3JoinTupleEntry* p = follower3JoinTupleMap->follower3JoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower2IdUser3Id, follower3IdUser4Id );
            h = h & (newcapacity - 1);
            do {
                Follower3JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete follower3JoinTupleMap->follower3JoinTupleEntryArray;
    follower3JoinTupleMap->follower3JoinTupleEntryArray = newJoinTupleEntryArray;
    follower3JoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
Follower3JoinTupleEntry* putFollower3JoinTupleIfAbsent(Follower3JoinTupleMap* follower3JoinTupleMap, int& follower2IdUser3Id,int& follower3IdUser4Id, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap ) {
    int h = 31;
    hashCode(h, follower2IdUser3Id, follower3IdUser4Id ); 
    h = h & (follower3JoinTupleMap->capacity - 1);
    Follower3JoinTupleEntry* candidate = follower3JoinTupleMap->follower3JoinTupleEntryArray[h];
    Follower3JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower2Follower3VertexValue->follower2IdUser3Id == follower2IdUser3Id
        &&         candidate->follower3Follower4VertexValue->follower3IdUser4Id == follower3IdUser4Id
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    Follower3JoinTupleEntry* p = new Follower3JoinTupleEntry;
    if(!lastP)
        follower3JoinTupleMap->follower3JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower3JoinTupleMap->size++;
    ensureSize(follower3JoinTupleMap, follower2IdUser3Id, follower3IdUser4Id );
    //%2 - join attribute values
    addToVertexValues(follower2IdUser3Id , p, follower2Follower3VertexValueMap);
    addToVertexValues(follower3IdUser4Id , p, follower3Follower4VertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower2IdUser3Id , Follower3JoinTupleEntry* p, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap) {
    //%4 - vertex attribute values
    Follower2Follower3VertexValueEntry* vertexValue = putFollower2Follower3VertexValueIfAbsent(follower2Follower3VertexValueMap, follower2IdUser3Id );
    p->follower2Follower3VertexValue = vertexValue;
    vertexValue->follower3JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
__inline static void addToVertexValues(int& follower3IdUser4Id , Follower3JoinTupleEntry* p, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap) {
    //%4 - vertex attribute values
    Follower3Follower4VertexValueEntry* vertexValue = putFollower3Follower4VertexValueIfAbsent(follower3Follower4VertexValueMap, follower3IdUser4Id );
    p->follower3Follower4VertexValue = vertexValue;
    vertexValue->follower3JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower2IdUser3Id,int& follower3IdUser4Id) {
    h ^=  follower2IdUser3Id + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  follower3IdUser4Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

Follower3JoinTupleIteratorNext follower3JoinTupleIterator(Follower3JoinTupleMap* follower3JoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = follower3JoinTupleMap->capacity;
    currentJoinTupleEntryArray = follower3JoinTupleMap->follower3JoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

__inline static Follower3JoinTupleEntry* next() {
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

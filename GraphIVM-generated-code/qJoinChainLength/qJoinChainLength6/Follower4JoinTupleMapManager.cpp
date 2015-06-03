#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower3IdUser4Id,int& follower4IdUser5Id) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(Follower4JoinTupleMap* follower4JoinTupleMap, int& follower3IdUser4Id,int& follower4IdUser5Id);
//%2 - vertex value attribute values 
static void increaseArraySize(Follower4JoinTupleMap* follower4JoinTupleMap, int& follower3IdUser4Id,int& follower4IdUser5Id);
static Follower4JoinTupleEntry* next();
//%1$1s - join attributes
static void addToVertexValues(int& follower3IdUser4Id , Follower4JoinTupleEntry* p, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap);
static void addToVertexValues(int& follower4IdUser5Id , Follower4JoinTupleEntry* p, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap);

//iterator
static int index1 = 0;
static Follower4JoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static Follower4JoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(Follower4JoinTupleMap* follower4JoinTupleMap, int& follower3IdUser4Id,int& follower4IdUser5Id) {
    if(follower4JoinTupleMap->size >= follower4JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower4JoinTupleMap, follower3IdUser4Id, follower4IdUser5Id );
    }
}
//%2 - join attributes
static void increaseArraySize(Follower4JoinTupleMap* follower4JoinTupleMap, int& follower3IdUser4Id,int& follower4IdUser5Id) {
    int newcapacity = follower4JoinTupleMap->capacity * 2;
    Follower4JoinTupleEntry** newJoinTupleEntryArray = new Follower4JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower4JoinTupleMap->capacity; i++) {
        Follower4JoinTupleEntry* p = follower4JoinTupleMap->follower4JoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower3IdUser4Id, follower4IdUser5Id );
            h = h & (newcapacity - 1);
            newJoinTupleEntryArray[h] = p;
        }
    }
    delete follower4JoinTupleMap->follower4JoinTupleEntryArray;
    follower4JoinTupleMap->follower4JoinTupleEntryArray = newJoinTupleEntryArray;
    follower4JoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
Follower4JoinTupleEntry* putFollower4JoinTupleIfAbsent(Follower4JoinTupleMap* follower4JoinTupleMap, int& follower3IdUser4Id,int& follower4IdUser5Id, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap ) {
    int h = 31;
    hashCode(h, follower3IdUser4Id, follower4IdUser5Id ); 
    h = h & (follower4JoinTupleMap->capacity - 1);
    Follower4JoinTupleEntry* candidate = follower4JoinTupleMap->follower4JoinTupleEntryArray[h];
    Follower4JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower3Follower4VertexValue->follower3IdUser4Id == follower3IdUser4Id
        &&         candidate->follower4Follower5VertexValue->follower4IdUser5Id == follower4IdUser5Id
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    ensureSize(follower4JoinTupleMap, follower3IdUser4Id, follower4IdUser5Id );
    Follower4JoinTupleEntry* p = new Follower4JoinTupleEntry;
    if(!lastP)
        follower4JoinTupleMap->follower4JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower4JoinTupleMap->size++;
    //%2 - join attribute values
    addToVertexValues(follower3IdUser4Id , p, follower3Follower4VertexValueMap);
    addToVertexValues(follower4IdUser5Id , p, follower4Follower5VertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower3IdUser4Id , Follower4JoinTupleEntry* p, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap) {
    //%4 - vertex attribute values
    Follower3Follower4VertexValueEntry* vertexValue = putFollower3Follower4VertexValueIfAbsent(follower3Follower4VertexValueMap, follower3IdUser4Id );
    p->follower3Follower4VertexValue = vertexValue;
    vertexValue->follower4JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
__inline static void addToVertexValues(int& follower4IdUser5Id , Follower4JoinTupleEntry* p, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap) {
    //%4 - vertex attribute values
    Follower4Follower5VertexValueEntry* vertexValue = putFollower4Follower5VertexValueIfAbsent(follower4Follower5VertexValueMap, follower4IdUser5Id );
    p->follower4Follower5VertexValue = vertexValue;
    vertexValue->follower4JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower3IdUser4Id,int& follower4IdUser5Id) {
    h ^=  follower3IdUser4Id + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  follower4IdUser5Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

Follower4JoinTupleIteratorNext follower4JoinTupleIterator(Follower4JoinTupleMap* follower4JoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = follower4JoinTupleMap->capacity;
    currentJoinTupleEntryArray = follower4JoinTupleMap->follower4JoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

static Follower4JoinTupleEntry* next() {
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

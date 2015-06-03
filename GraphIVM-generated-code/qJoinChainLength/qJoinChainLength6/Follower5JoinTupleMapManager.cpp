#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower4IdUser5Id,int& follower5IdTweetUserId) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(Follower5JoinTupleMap* follower5JoinTupleMap, int& follower4IdUser5Id,int& follower5IdTweetUserId);
//%2 - vertex value attribute values 
static void increaseArraySize(Follower5JoinTupleMap* follower5JoinTupleMap, int& follower4IdUser5Id,int& follower5IdTweetUserId);
static Follower5JoinTupleEntry* next();
//%1$1s - join attributes
static void addToVertexValues(int& follower4IdUser5Id , Follower5JoinTupleEntry* p, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap);
static void addToVertexValues(int& follower5IdTweetUserId , Follower5JoinTupleEntry* p, Follower5TweetVertexValueMap* follower5TweetVertexValueMap);

//iterator
static int index1 = 0;
static Follower5JoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static Follower5JoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(Follower5JoinTupleMap* follower5JoinTupleMap, int& follower4IdUser5Id,int& follower5IdTweetUserId) {
    if(follower5JoinTupleMap->size >= follower5JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower5JoinTupleMap, follower4IdUser5Id, follower5IdTweetUserId );
    }
}
//%2 - join attributes
static void increaseArraySize(Follower5JoinTupleMap* follower5JoinTupleMap, int& follower4IdUser5Id,int& follower5IdTweetUserId) {
    int newcapacity = follower5JoinTupleMap->capacity * 2;
    Follower5JoinTupleEntry** newJoinTupleEntryArray = new Follower5JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower5JoinTupleMap->capacity; i++) {
        Follower5JoinTupleEntry* p = follower5JoinTupleMap->follower5JoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower4IdUser5Id, follower5IdTweetUserId );
            h = h & (newcapacity - 1);
            newJoinTupleEntryArray[h] = p;
        }
    }
    delete follower5JoinTupleMap->follower5JoinTupleEntryArray;
    follower5JoinTupleMap->follower5JoinTupleEntryArray = newJoinTupleEntryArray;
    follower5JoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
Follower5JoinTupleEntry* putFollower5JoinTupleIfAbsent(Follower5JoinTupleMap* follower5JoinTupleMap, int& follower4IdUser5Id,int& follower5IdTweetUserId, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap, Follower5TweetVertexValueMap* follower5TweetVertexValueMap ) {
    int h = 31;
    hashCode(h, follower4IdUser5Id, follower5IdTweetUserId ); 
    h = h & (follower5JoinTupleMap->capacity - 1);
    Follower5JoinTupleEntry* candidate = follower5JoinTupleMap->follower5JoinTupleEntryArray[h];
    Follower5JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower4Follower5VertexValue->follower4IdUser5Id == follower4IdUser5Id
        &&         candidate->follower5TweetVertexValue->follower5IdTweetUserId == follower5IdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    ensureSize(follower5JoinTupleMap, follower4IdUser5Id, follower5IdTweetUserId );
    Follower5JoinTupleEntry* p = new Follower5JoinTupleEntry;
    if(!lastP)
        follower5JoinTupleMap->follower5JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower5JoinTupleMap->size++;
    //%2 - join attribute values
    addToVertexValues(follower4IdUser5Id , p, follower4Follower5VertexValueMap);
    addToVertexValues(follower5IdTweetUserId , p, follower5TweetVertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower4IdUser5Id , Follower5JoinTupleEntry* p, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap) {
    //%4 - vertex attribute values
    Follower4Follower5VertexValueEntry* vertexValue = putFollower4Follower5VertexValueIfAbsent(follower4Follower5VertexValueMap, follower4IdUser5Id );
    p->follower4Follower5VertexValue = vertexValue;
    vertexValue->follower5JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
__inline static void addToVertexValues(int& follower5IdTweetUserId , Follower5JoinTupleEntry* p, Follower5TweetVertexValueMap* follower5TweetVertexValueMap) {
    //%4 - vertex attribute values
    Follower5TweetVertexValueEntry* vertexValue = putFollower5TweetVertexValueIfAbsent(follower5TweetVertexValueMap, follower5IdTweetUserId );
    p->follower5TweetVertexValue = vertexValue;
    vertexValue->follower5JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower4IdUser5Id,int& follower5IdTweetUserId) {
    h ^=  follower4IdUser5Id + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  follower5IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

Follower5JoinTupleIteratorNext follower5JoinTupleIterator(Follower5JoinTupleMap* follower5JoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = follower5JoinTupleMap->capacity;
    currentJoinTupleEntryArray = follower5JoinTupleMap->follower5JoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

static Follower5JoinTupleEntry* next() {
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

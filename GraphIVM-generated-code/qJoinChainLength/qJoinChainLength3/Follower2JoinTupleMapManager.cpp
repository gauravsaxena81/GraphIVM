#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower1IdUser2Id,int& follower2IdTweetUserId) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdTweetUserId);
//%2 - vertex value attribute values 
static void increaseArraySize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdTweetUserId);
static Follower2JoinTupleEntry* next();
//%1$1s - join attributes
static void addToVertexValues(int& follower1IdUser2Id , Follower2JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);
static void addToVertexValues(int& follower2IdTweetUserId , Follower2JoinTupleEntry* p, Follower2TweetVertexValueMap* follower2TweetVertexValueMap);

//iterator
static int index1 = 0;
static Follower2JoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static Follower2JoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdTweetUserId) {
    if(follower2JoinTupleMap->size >= follower2JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower2JoinTupleMap, follower1IdUser2Id, follower2IdTweetUserId );
    }
}
//%2 - join attributes
static void increaseArraySize(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdTweetUserId) {
    int newcapacity = follower2JoinTupleMap->capacity * 2;
    Follower2JoinTupleEntry** newJoinTupleEntryArray = new Follower2JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower2JoinTupleMap->capacity; i++) {
        Follower2JoinTupleEntry* p = follower2JoinTupleMap->follower2JoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower1IdUser2Id, follower2IdTweetUserId );
            h = h & (newcapacity - 1);
            newJoinTupleEntryArray[h] = p;
        }
    }
    delete follower2JoinTupleMap->follower2JoinTupleEntryArray;
    follower2JoinTupleMap->follower2JoinTupleEntryArray = newJoinTupleEntryArray;
    follower2JoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
Follower2JoinTupleEntry* putFollower2JoinTupleIfAbsent(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdTweetUserId, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, Follower2TweetVertexValueMap* follower2TweetVertexValueMap ) {
    int h = 31;
    hashCode(h, follower1IdUser2Id, follower2IdTweetUserId ); 
    h = h & (follower2JoinTupleMap->capacity - 1);
    Follower2JoinTupleEntry* candidate = follower2JoinTupleMap->follower2JoinTupleEntryArray[h];
    Follower2JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower1Follower2VertexValue->follower1IdUser2Id == follower1IdUser2Id
        &&         candidate->follower2TweetVertexValue->follower2IdTweetUserId == follower2IdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    ensureSize(follower2JoinTupleMap, follower1IdUser2Id, follower2IdTweetUserId );
    Follower2JoinTupleEntry* p = new Follower2JoinTupleEntry;
    if(!lastP)
        follower2JoinTupleMap->follower2JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower2JoinTupleMap->size++;
    //%2 - join attribute values
    addToVertexValues(follower1IdUser2Id , p, follower1Follower2VertexValueMap);
    addToVertexValues(follower2IdTweetUserId , p, follower2TweetVertexValueMap);
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
__inline static void addToVertexValues(int& follower2IdTweetUserId , Follower2JoinTupleEntry* p, Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
    //%4 - vertex attribute values
    Follower2TweetVertexValueEntry* vertexValue = putFollower2TweetVertexValueIfAbsent(follower2TweetVertexValueMap, follower2IdTweetUserId );
    p->follower2TweetVertexValue = vertexValue;
    vertexValue->follower2JoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower1IdUser2Id,int& follower2IdTweetUserId) {
    h ^=  follower1IdUser2Id + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  follower2IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

Follower2JoinTupleIteratorNext follower2JoinTupleIterator(Follower2JoinTupleMap* follower2JoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = follower2JoinTupleMap->capacity;
    currentJoinTupleEntryArray = follower2JoinTupleMap->follower2JoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

static Follower2JoinTupleEntry* next() {
	if(currentP) {
		currentP = currentP->next;
		if(currentP)
			return currentP;
	}
    for(; index1 < currentCapacity; index1++) {
        if(currentJoinTupleEntryArray[index1]) {
			return (currentP = currentJoinTupleEntryArray[index1]);
        }
    }
    return NULL;
}

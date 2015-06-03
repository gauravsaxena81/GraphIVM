#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower5IdTweetUserId) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower5IdTweetUserId);
//%2 - vertex value attribute values 
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower5IdTweetUserId);
static TweetJoinTupleEntry* next();
//%1$1s - join attributes
static void addToVertexValues(int& follower5IdTweetUserId , TweetJoinTupleEntry* p, Follower5TweetVertexValueMap* follower5TweetVertexValueMap);

//iterator
static int index1 = 0;
static TweetJoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static TweetJoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower5IdTweetUserId) {
    if(tweetJoinTupleMap->size >= tweetJoinTupleMap->capacity * 0.8) {
        increaseArraySize(tweetJoinTupleMap, follower5IdTweetUserId );
    }
}
//%2 - join attributes
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower5IdTweetUserId) {
    int newcapacity = tweetJoinTupleMap->capacity * 2;
    TweetJoinTupleEntry** newJoinTupleEntryArray = new TweetJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < tweetJoinTupleMap->capacity; i++) {
        TweetJoinTupleEntry* p = tweetJoinTupleMap->tweetJoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower5IdTweetUserId );
            h = h & (newcapacity - 1);
            newJoinTupleEntryArray[h] = p;
        }
    }
    delete tweetJoinTupleMap->tweetJoinTupleEntryArray;
    tweetJoinTupleMap->tweetJoinTupleEntryArray = newJoinTupleEntryArray;
    tweetJoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& follower5IdTweetUserId, Follower5TweetVertexValueMap* follower5TweetVertexValueMap ) {
    int h = 31;
    hashCode(h, follower5IdTweetUserId ); 
    h = h & (tweetJoinTupleMap->capacity - 1);
    TweetJoinTupleEntry* candidate = tweetJoinTupleMap->tweetJoinTupleEntryArray[h];
    TweetJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower5TweetVertexValue->follower5IdTweetUserId == follower5IdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    ensureSize(tweetJoinTupleMap, follower5IdTweetUserId );
    TweetJoinTupleEntry* p = new TweetJoinTupleEntry;
    if(!lastP)
        tweetJoinTupleMap->tweetJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    tweetJoinTupleMap->size++;
    //%2 - join attribute values
    addToVertexValues(follower5IdTweetUserId , p, follower5TweetVertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower5IdTweetUserId , TweetJoinTupleEntry* p, Follower5TweetVertexValueMap* follower5TweetVertexValueMap) {
    //%4 - vertex attribute values
    Follower5TweetVertexValueEntry* vertexValue = putFollower5TweetVertexValueIfAbsent(follower5TweetVertexValueMap, follower5IdTweetUserId );
    p->follower5TweetVertexValue = vertexValue;
    vertexValue->tweetJoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower5IdTweetUserId) {
    h ^=  follower5IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

TweetJoinTupleIteratorNext tweetJoinTupleIterator(TweetJoinTupleMap* tweetJoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = tweetJoinTupleMap->capacity;
    currentJoinTupleEntryArray = tweetJoinTupleMap->tweetJoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

static TweetJoinTupleEntry* next() {
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

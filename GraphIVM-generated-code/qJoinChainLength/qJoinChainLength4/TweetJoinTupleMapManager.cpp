#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - join attributes
inline static void hashCode(int& h, int& follower3IdTweetUserId) __attribute__((always_inline));
//%2 - vertex value attributes
static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower3IdTweetUserId);
//%2 - vertex value attribute values 
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower3IdTweetUserId);
inline static TweetJoinTupleEntry* next() __attribute__((always_inline));
//%1$1s - join attributes
static void addToVertexValues(int& follower3IdTweetUserId , TweetJoinTupleEntry* p, Follower3TweetVertexValueMap* follower3TweetVertexValueMap);

//iterator
static int index1 = 0;
static TweetJoinTupleEntry* currentP = NULL;
static int currentCapacity = 0;
static TweetJoinTupleEntry** currentJoinTupleEntryArray;
static int currentIndex = 0;


__inline static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower3IdTweetUserId) {
    if(tweetJoinTupleMap->size >= tweetJoinTupleMap->capacity * 0.8) {
        increaseArraySize(tweetJoinTupleMap, follower3IdTweetUserId );
    }
}
//%2 - join attributes
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap, int& follower3IdTweetUserId) {
    int newcapacity = tweetJoinTupleMap->capacity * 2;
    TweetJoinTupleEntry** newJoinTupleEntryArray = new TweetJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < tweetJoinTupleMap->capacity; i++) {
        TweetJoinTupleEntry* p = tweetJoinTupleMap->tweetJoinTupleEntryArray[i];
        if(p) {
            //Store the first element of p to the newJoinTupleEntryArray array. Rest are chained to this
            int h = 31;
            hashCode(h, follower3IdTweetUserId );
            h = h & (newcapacity - 1);
            do {
                TweetJoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete tweetJoinTupleMap->tweetJoinTupleEntryArray;
    tweetJoinTupleMap->tweetJoinTupleEntryArray = newJoinTupleEntryArray;
    tweetJoinTupleMap->capacity = newcapacity;
}
//%3 - join attributes, %4 - vertex values
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& follower3IdTweetUserId, Follower3TweetVertexValueMap* follower3TweetVertexValueMap ) {
    int h = 31;
    hashCode(h, follower3IdTweetUserId ); 
    h = h & (tweetJoinTupleMap->capacity - 1);
    TweetJoinTupleEntry* candidate = tweetJoinTupleMap->tweetJoinTupleEntryArray[h];
    TweetJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower3TweetVertexValue->follower3IdTweetUserId == follower3IdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    //%2 - join attributes
    TweetJoinTupleEntry* p = new TweetJoinTupleEntry;
    if(!lastP)
        tweetJoinTupleMap->tweetJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    tweetJoinTupleMap->size++;
    ensureSize(tweetJoinTupleMap, follower3IdTweetUserId );
    //%2 - join attribute values
    addToVertexValues(follower3IdTweetUserId , p, follower3TweetVertexValueMap);
    return p;
}
//%1 - join attributes, %3 - vertex values
    //%4 - vertex attribute values
__inline static void addToVertexValues(int& follower3IdTweetUserId , TweetJoinTupleEntry* p, Follower3TweetVertexValueMap* follower3TweetVertexValueMap) {
    //%4 - vertex attribute values
    Follower3TweetVertexValueEntry* vertexValue = putFollower3TweetVertexValueIfAbsent(follower3TweetVertexValueMap, follower3IdTweetUserId );
    p->follower3TweetVertexValue = vertexValue;
    vertexValue->tweetJoinTupleList.push_back(p);
}
//arguments based on key
//void remove(%1$1sJoinTupleMap* %1$1sJoinTupleMap, int userId) {
    //implement later
    //if count = 0, delete auto
    //%1$1sJoinTupleMap->size--;
//}
//%1 - join attributes
inline static void hashCode(int&h, int& follower3IdTweetUserId) {
    h ^=  follower3IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

TweetJoinTupleIteratorNext tweetJoinTupleIterator(TweetJoinTupleMap* tweetJoinTupleMap) {
    index1 = 0;
    currentP = NULL;
    currentCapacity = tweetJoinTupleMap->capacity;
    currentJoinTupleEntryArray = tweetJoinTupleMap->tweetJoinTupleEntryArray;
    currentIndex = 0;
    return &next;
}

__inline static TweetJoinTupleEntry* next() {
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

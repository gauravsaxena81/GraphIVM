#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& tweetTweetIdRetweet1RetweetTweetId,int& retweet1TweetIdRetweet2RetweetTweetId) __attribute__((always_inline));
static void ensureSize(Retweet1JoinTupleMap* retweet1JoinTupleMap);
static void increaseArraySize(Retweet1JoinTupleMap* retweet1JoinTupleMap);
static void addToVertexValues(int& tweetTweetIdRetweet1RetweetTweetId , Retweet1JoinTupleEntry* p, TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap);
static void addToVertexValues(int& retweet1TweetIdRetweet2RetweetTweetId , Retweet1JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);



__inline static void ensureSize(Retweet1JoinTupleMap* retweet1JoinTupleMap) {
    if(retweet1JoinTupleMap->size >= retweet1JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet1JoinTupleMap);
    }
}
static void increaseArraySize(Retweet1JoinTupleMap* retweet1JoinTupleMap) {
    int newcapacity = retweet1JoinTupleMap->capacity * 2;
    Retweet1JoinTupleEntry** newJoinTupleEntryArray = new Retweet1JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet1JoinTupleMap->capacity; i++) {
        Retweet1JoinTupleEntry* p = retweet1JoinTupleMap->retweet1JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h
                , p->tweetRetweet1VertexValue->tweetTweetIdRetweet1RetweetTweetId
                , p->retweet1Retweet2VertexValue->retweet1TweetIdRetweet2RetweetTweetId
                );
                h = h & (newcapacity - 1);
                Retweet1JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete retweet1JoinTupleMap->retweet1JoinTupleEntryArray;
    retweet1JoinTupleMap->retweet1JoinTupleEntryArray = newJoinTupleEntryArray;
    retweet1JoinTupleMap->capacity = newcapacity;
}
Retweet1JoinTupleEntry* putRetweet1JoinTupleIfAbsent(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& tweetTweetIdRetweet1RetweetTweetId,int& retweet1TweetIdRetweet2RetweetTweetId, TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap ) {
    int h = 31;
    hashCode(h, tweetTweetIdRetweet1RetweetTweetId, retweet1TweetIdRetweet2RetweetTweetId ); 
    h = h & (retweet1JoinTupleMap->capacity - 1);
    Retweet1JoinTupleEntry* candidate = retweet1JoinTupleMap->retweet1JoinTupleEntryArray[h];
    Retweet1JoinTupleEntry* lastP = NULL;
    while(candidate) {
        if(
                candidate->tweetRetweet1VertexValue->tweetTweetIdRetweet1RetweetTweetId == tweetTweetIdRetweet1RetweetTweetId
        &&         candidate->retweet1Retweet2VertexValue->retweet1TweetIdRetweet2RetweetTweetId == retweet1TweetIdRetweet2RetweetTweetId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet1JoinTupleEntry* p = new Retweet1JoinTupleEntry;
    addToVertexValues(tweetTweetIdRetweet1RetweetTweetId , p, tweetRetweet1VertexValueMap);
    addToVertexValues(retweet1TweetIdRetweet2RetweetTweetId , p, retweet1Retweet2VertexValueMap);
    if(!lastP)
        retweet1JoinTupleMap->retweet1JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet1JoinTupleMap->size++;
    ensureSize(retweet1JoinTupleMap);
    return p;
}
__inline static void addToVertexValues(int& tweetTweetIdRetweet1RetweetTweetId , Retweet1JoinTupleEntry* p, TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap) {
    TweetRetweet1VertexValueEntry* vertexValue = putTweetRetweet1VertexValueIfAbsent(tweetRetweet1VertexValueMap, tweetTweetIdRetweet1RetweetTweetId );
    p->tweetRetweet1VertexValue = vertexValue;
    vertexValue->retweet1JoinTupleList.push_back(p);
}

__inline static void addToVertexValues(int& retweet1TweetIdRetweet2RetweetTweetId , Retweet1JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
    Retweet1Retweet2VertexValueEntry* vertexValue = putRetweet1Retweet2VertexValueIfAbsent(retweet1Retweet2VertexValueMap, retweet1TweetIdRetweet2RetweetTweetId );
    p->retweet1Retweet2VertexValue = vertexValue;
    vertexValue->retweet1JoinTupleList.push_back(p);
}

inline static void hashCode(int&h, int& tweetTweetIdRetweet1RetweetTweetId,int& retweet1TweetIdRetweet2RetweetTweetId) {
    h ^=  tweetTweetIdRetweet1RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  retweet1TweetIdRetweet2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}



Retweet1JoinTupleEntry** getRetweet1JoinTuplePointer(Retweet1JoinTupleMap* retweet1JoinTupleMap
, int& tweetTweetIdRetweet1RetweetTweetId
, int& retweet1TweetIdRetweet2RetweetTweetId
)  {
    int h = 31;
    hashCode(h
    , tweetTweetIdRetweet1RetweetTweetId  
    , retweet1TweetIdRetweet2RetweetTweetId  
    );
    h = h & (retweet1JoinTupleMap->capacity - 1);
    Retweet1JoinTupleEntry* candidate = retweet1JoinTupleMap->retweet1JoinTupleEntryArray[h];
    Retweet1JoinTupleEntry** address = &(retweet1JoinTupleMap->retweet1JoinTupleEntryArray[h]);
    while(candidate) {
        if(
              candidate->tweetRetweet1VertexValue->tweetTweetIdRetweet1RetweetTweetId == tweetTweetIdRetweet1RetweetTweetId
            &&  candidate->retweet1Retweet2VertexValue->retweet1TweetIdRetweet2RetweetTweetId == retweet1TweetIdRetweet2RetweetTweetId
        ) {
            return address;
        } else {
            address = &(candidate->next);
            candidate = candidate->next;
        }
    }
    return NULL;
}
void removeJoinTupleEntry(Retweet1JoinTupleMap* retweet1JoinTupleMap, Retweet1JoinTupleEntry** entryPointer) {
    if((*entryPointer)->next) {
        (*entryPointer)->next = (*entryPointer)->next->next;
    } else {
        delete *entryPointer;
        (*entryPointer) = NULL;
    }
    retweet1JoinTupleMap->size--;
}

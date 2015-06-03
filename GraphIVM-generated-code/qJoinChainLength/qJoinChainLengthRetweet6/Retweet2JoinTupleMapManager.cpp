#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId) __attribute__((always_inline));
static void ensureSize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId);
static void increaseArraySize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId);
static void addToVertexValues(int& r1TweetIdR2RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);
static void addToVertexValues(int& r2TweetIdR3RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap);



__inline static void ensureSize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId) {
    if(retweet2JoinTupleMap->size >= retweet2JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet2JoinTupleMap, r1TweetIdR2RetweetTweetId, r2TweetIdR3RetweetTweetId );
    }
}
static void increaseArraySize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId) {
    int newcapacity = retweet2JoinTupleMap->capacity * 2;
    Retweet2JoinTupleEntry** newJoinTupleEntryArray = new Retweet2JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet2JoinTupleMap->capacity; i++) {
        Retweet2JoinTupleEntry* p = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r1TweetIdR2RetweetTweetId, r2TweetIdR3RetweetTweetId );
                h = h & (newcapacity - 1);
                Retweet2JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete retweet2JoinTupleMap->retweet2JoinTupleEntryArray;
    retweet2JoinTupleMap->retweet2JoinTupleEntryArray = newJoinTupleEntryArray;
    retweet2JoinTupleMap->capacity = newcapacity;
}
Retweet2JoinTupleEntry* putRetweet2JoinTupleIfAbsent(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap ) {
    int h = 31;
    hashCode(h, r1TweetIdR2RetweetTweetId, r2TweetIdR3RetweetTweetId ); 
    h = h & (retweet2JoinTupleMap->capacity - 1);
    Retweet2JoinTupleEntry* candidate = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h];
    Retweet2JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet1Retweet2VertexValue->r1TweetIdR2RetweetTweetId == r1TweetIdR2RetweetTweetId
        &&         candidate->retweet2Retweet3VertexValue->r2TweetIdR3RetweetTweetId == r2TweetIdR3RetweetTweetId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet2JoinTupleEntry* p = new Retweet2JoinTupleEntry;
    if(!lastP)
        retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet2JoinTupleMap->size++;
    ensureSize(retweet2JoinTupleMap, r1TweetIdR2RetweetTweetId, r2TweetIdR3RetweetTweetId );
    addToVertexValues(r1TweetIdR2RetweetTweetId , p, retweet1Retweet2VertexValueMap);
    addToVertexValues(r2TweetIdR3RetweetTweetId , p, retweet2Retweet3VertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r1TweetIdR2RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
    Retweet1Retweet2VertexValueEntry* vertexValue = putRetweet1Retweet2VertexValueIfAbsent(retweet1Retweet2VertexValueMap, r1TweetIdR2RetweetTweetId );
    p->retweet1Retweet2VertexValue = vertexValue;
    vertexValue->retweet2JoinTupleList.push_back(p);
}
__inline static void addToVertexValues(int& r2TweetIdR3RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap) {
    Retweet2Retweet3VertexValueEntry* vertexValue = putRetweet2Retweet3VertexValueIfAbsent(retweet2Retweet3VertexValueMap, r2TweetIdR3RetweetTweetId );
    p->retweet2Retweet3VertexValue = vertexValue;
    vertexValue->retweet2JoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId) {
    h ^=  r1TweetIdR2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  r2TweetIdR3RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}



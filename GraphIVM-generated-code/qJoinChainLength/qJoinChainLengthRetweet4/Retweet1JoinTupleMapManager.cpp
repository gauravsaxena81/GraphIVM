#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r1TweetIdR2RetweetTweetId) __attribute__((always_inline));
static void ensureSize(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId);
static void increaseArraySize(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId);
static void addToVertexValues(int& r1TweetIdR2RetweetTweetId , Retweet1JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);



__inline static void ensureSize(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId) {
    if(retweet1JoinTupleMap->size >= retweet1JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet1JoinTupleMap, r1TweetIdR2RetweetTweetId );
    }
}
static void increaseArraySize(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId) {
    int newcapacity = retweet1JoinTupleMap->capacity * 2;
    Retweet1JoinTupleEntry** newJoinTupleEntryArray = new Retweet1JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet1JoinTupleMap->capacity; i++) {
        Retweet1JoinTupleEntry* p = retweet1JoinTupleMap->retweet1JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r1TweetIdR2RetweetTweetId );
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
Retweet1JoinTupleEntry* putRetweet1JoinTupleIfAbsent(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap ) {
    int h = 31;
    hashCode(h, r1TweetIdR2RetweetTweetId ); 
    h = h & (retweet1JoinTupleMap->capacity - 1);
    Retweet1JoinTupleEntry* candidate = retweet1JoinTupleMap->retweet1JoinTupleEntryArray[h];
    Retweet1JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet1Retweet2VertexValue->r1TweetIdR2RetweetTweetId == r1TweetIdR2RetweetTweetId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet1JoinTupleEntry* p = new Retweet1JoinTupleEntry;
    if(!lastP)
        retweet1JoinTupleMap->retweet1JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet1JoinTupleMap->size++;
    ensureSize(retweet1JoinTupleMap, r1TweetIdR2RetweetTweetId );
    addToVertexValues(r1TweetIdR2RetweetTweetId , p, retweet1Retweet2VertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r1TweetIdR2RetweetTweetId , Retweet1JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
    Retweet1Retweet2VertexValueEntry* vertexValue = putRetweet1Retweet2VertexValueIfAbsent(retweet1Retweet2VertexValueMap, r1TweetIdR2RetweetTweetId );
    p->retweet1Retweet2VertexValue = vertexValue;
    vertexValue->retweet1JoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->retweet2JoinTupleList.size() > 0);
}
inline static void hashCode(int&h, int& r1TweetIdR2RetweetTweetId) {
    h ^=  r1TweetIdR2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}



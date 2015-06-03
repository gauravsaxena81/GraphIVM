#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId) __attribute__((always_inline));
static void ensureSize(Retweet4JoinTupleMap* retweet4JoinTupleMap, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId);
static void increaseArraySize(Retweet4JoinTupleMap* retweet4JoinTupleMap, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId);
static void addToVertexValues(int& r3TweetIdR4RetweetTweetId , Retweet4JoinTupleEntry* p, Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap);
static void addToVertexValues(int& r4TweetIdR5RetweetTweetId , Retweet4JoinTupleEntry* p, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap);



__inline static void ensureSize(Retweet4JoinTupleMap* retweet4JoinTupleMap, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId) {
    if(retweet4JoinTupleMap->size >= retweet4JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet4JoinTupleMap, r3TweetIdR4RetweetTweetId, r4TweetIdR5RetweetTweetId );
    }
}
static void increaseArraySize(Retweet4JoinTupleMap* retweet4JoinTupleMap, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId) {
    int newcapacity = retweet4JoinTupleMap->capacity * 2;
    Retweet4JoinTupleEntry** newJoinTupleEntryArray = new Retweet4JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet4JoinTupleMap->capacity; i++) {
        Retweet4JoinTupleEntry* p = retweet4JoinTupleMap->retweet4JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r3TweetIdR4RetweetTweetId, r4TweetIdR5RetweetTweetId );
                h = h & (newcapacity - 1);
                Retweet4JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete retweet4JoinTupleMap->retweet4JoinTupleEntryArray;
    retweet4JoinTupleMap->retweet4JoinTupleEntryArray = newJoinTupleEntryArray;
    retweet4JoinTupleMap->capacity = newcapacity;
}
Retweet4JoinTupleEntry* putRetweet4JoinTupleIfAbsent(Retweet4JoinTupleMap* retweet4JoinTupleMap, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId, Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap ) {
    int h = 31;
    hashCode(h, r3TweetIdR4RetweetTweetId, r4TweetIdR5RetweetTweetId ); 
    h = h & (retweet4JoinTupleMap->capacity - 1);
    Retweet4JoinTupleEntry* candidate = retweet4JoinTupleMap->retweet4JoinTupleEntryArray[h];
    Retweet4JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet3Retweet4VertexValue->r3TweetIdR4RetweetTweetId == r3TweetIdR4RetweetTweetId
        &&         candidate->retweet4Retweet5VertexValue->r4TweetIdR5RetweetTweetId == r4TweetIdR5RetweetTweetId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet4JoinTupleEntry* p = new Retweet4JoinTupleEntry;
    if(!lastP)
        retweet4JoinTupleMap->retweet4JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet4JoinTupleMap->size++;
    ensureSize(retweet4JoinTupleMap, r3TweetIdR4RetweetTweetId, r4TweetIdR5RetweetTweetId );
    addToVertexValues(r3TweetIdR4RetweetTweetId , p, retweet3Retweet4VertexValueMap);
    addToVertexValues(r4TweetIdR5RetweetTweetId , p, retweet4Retweet5VertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r3TweetIdR4RetweetTweetId , Retweet4JoinTupleEntry* p, Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap) {
    Retweet3Retweet4VertexValueEntry* vertexValue = putRetweet3Retweet4VertexValueIfAbsent(retweet3Retweet4VertexValueMap, r3TweetIdR4RetweetTweetId );
    p->retweet3Retweet4VertexValue = vertexValue;
    vertexValue->retweet4JoinTupleList.push_back(p);
}
__inline static void addToVertexValues(int& r4TweetIdR5RetweetTweetId , Retweet4JoinTupleEntry* p, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap) {
    Retweet4Retweet5VertexValueEntry* vertexValue = putRetweet4Retweet5VertexValueIfAbsent(retweet4Retweet5VertexValueMap, r4TweetIdR5RetweetTweetId );
    p->retweet4Retweet5VertexValue = vertexValue;
    vertexValue->retweet4JoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId) {
    h ^=  r3TweetIdR4RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  r4TweetIdR5RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}



#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId) __attribute__((always_inline));
static void ensureSize(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId);
static void increaseArraySize(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId);
static void addToVertexValues(int& r2TweetIdR3RetweetTweetId , Retweet3JoinTupleEntry* p, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap);
static void addToVertexValues(int& r3UserIdFUserId , Retweet3JoinTupleEntry* p, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap);



__inline static void ensureSize(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId) {
    if(retweet3JoinTupleMap->size >= retweet3JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet3JoinTupleMap, r2TweetIdR3RetweetTweetId, r3UserIdFUserId );
    }
}
static void increaseArraySize(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId) {
    int newcapacity = retweet3JoinTupleMap->capacity * 2;
    Retweet3JoinTupleEntry** newJoinTupleEntryArray = new Retweet3JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet3JoinTupleMap->capacity; i++) {
        Retweet3JoinTupleEntry* p = retweet3JoinTupleMap->retweet3JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r2TweetIdR3RetweetTweetId, r3UserIdFUserId );
                h = h & (newcapacity - 1);
                Retweet3JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete retweet3JoinTupleMap->retweet3JoinTupleEntryArray;
    retweet3JoinTupleMap->retweet3JoinTupleEntryArray = newJoinTupleEntryArray;
    retweet3JoinTupleMap->capacity = newcapacity;
}
Retweet3JoinTupleEntry* putRetweet3JoinTupleIfAbsent(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r2TweetIdR3RetweetTweetId, r3UserIdFUserId ); 
    h = h & (retweet3JoinTupleMap->capacity - 1);
    Retweet3JoinTupleEntry* candidate = retweet3JoinTupleMap->retweet3JoinTupleEntryArray[h];
    Retweet3JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet2Retweet3VertexValue->r2TweetIdR3RetweetTweetId == r2TweetIdR3RetweetTweetId
        &&         candidate->retweet3FollowerVertexValue->r3UserIdFUserId == r3UserIdFUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet3JoinTupleEntry* p = new Retweet3JoinTupleEntry;
    if(!lastP)
        retweet3JoinTupleMap->retweet3JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet3JoinTupleMap->size++;
    ensureSize(retweet3JoinTupleMap, r2TweetIdR3RetweetTweetId, r3UserIdFUserId );
    addToVertexValues(r2TweetIdR3RetweetTweetId , p, retweet2Retweet3VertexValueMap);
    addToVertexValues(r3UserIdFUserId , p, retweet3FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r2TweetIdR3RetweetTweetId , Retweet3JoinTupleEntry* p, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap) {
    Retweet2Retweet3VertexValueEntry* vertexValue = putRetweet2Retweet3VertexValueIfAbsent(retweet2Retweet3VertexValueMap, r2TweetIdR3RetweetTweetId );
    p->retweet2Retweet3VertexValue = vertexValue;
    vertexValue->retweet3JoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->retweet2JoinTupleList.size() > 0);
}
__inline static void addToVertexValues(int& r3UserIdFUserId , Retweet3JoinTupleEntry* p, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap) {
    Retweet3FollowerVertexValueEntry* vertexValue = putRetweet3FollowerVertexValueIfAbsent(retweet3FollowerVertexValueMap, r3UserIdFUserId );
    p->retweet3FollowerVertexValue = vertexValue;
    vertexValue->retweet3JoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->followerJoinTupleList.size() > 0);
}
inline static void hashCode(int&h, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId) {
    h ^=  r2TweetIdR3RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  r3UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



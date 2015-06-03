#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId) __attribute__((always_inline));
static void ensureSize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId);
static void increaseArraySize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId);
static void addToVertexValues(int& r1TweetIdR2RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);
static void addToVertexValues(int& r2UserIdFUserId , Retweet2JoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap);



__inline static void ensureSize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId) {
    if(retweet2JoinTupleMap->size >= retweet2JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet2JoinTupleMap, r1TweetIdR2RetweetTweetId, r2UserIdFUserId );
    }
}
static void increaseArraySize(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId) {
    int newcapacity = retweet2JoinTupleMap->capacity * 2;
    Retweet2JoinTupleEntry** newJoinTupleEntryArray = new Retweet2JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet2JoinTupleMap->capacity; i++) {
        Retweet2JoinTupleEntry* p = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r1TweetIdR2RetweetTweetId, r2UserIdFUserId );
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
Retweet2JoinTupleEntry* putRetweet2JoinTupleIfAbsent(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r1TweetIdR2RetweetTweetId, r2UserIdFUserId ); 
    h = h & (retweet2JoinTupleMap->capacity - 1);
    Retweet2JoinTupleEntry* candidate = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h];
    Retweet2JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet1Retweet2VertexValue->r1TweetIdR2RetweetTweetId == r1TweetIdR2RetweetTweetId
        &&         candidate->retweet2FollowerVertexValue->r2UserIdFUserId == r2UserIdFUserId
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
    ensureSize(retweet2JoinTupleMap, r1TweetIdR2RetweetTweetId, r2UserIdFUserId );
    addToVertexValues(r1TweetIdR2RetweetTweetId , p, retweet1Retweet2VertexValueMap);
    addToVertexValues(r2UserIdFUserId , p, retweet2FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r1TweetIdR2RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
    Retweet1Retweet2VertexValueEntry* vertexValue = putRetweet1Retweet2VertexValueIfAbsent(retweet1Retweet2VertexValueMap, r1TweetIdR2RetweetTweetId );
    p->retweet1Retweet2VertexValue = vertexValue;
    vertexValue->retweet2JoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->retweet1JoinTupleList.size() > 0);
}
__inline static void addToVertexValues(int& r2UserIdFUserId , Retweet2JoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap) {
    Retweet2FollowerVertexValueEntry* vertexValue = putRetweet2FollowerVertexValueIfAbsent(retweet2FollowerVertexValueMap, r2UserIdFUserId );
    p->retweet2FollowerVertexValue = vertexValue;
    vertexValue->retweet2JoinTupleList.push_back(p);
    vertexValue->localLookAhead = (vertexValue->followerJoinTupleList.size() > 0);
}
inline static void hashCode(int&h, int& r1TweetIdR2RetweetTweetId,int& r2UserIdFUserId) {
    h ^=  r1TweetIdR2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  r2UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



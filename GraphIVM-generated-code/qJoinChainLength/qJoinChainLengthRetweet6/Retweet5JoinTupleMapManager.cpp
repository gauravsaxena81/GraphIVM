#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId) __attribute__((always_inline));
static void ensureSize(Retweet5JoinTupleMap* retweet5JoinTupleMap, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId);
static void increaseArraySize(Retweet5JoinTupleMap* retweet5JoinTupleMap, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId);
static void addToVertexValues(int& r4TweetIdR5RetweetTweetId , Retweet5JoinTupleEntry* p, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap);
static void addToVertexValues(int& r5UserIdFUserId , Retweet5JoinTupleEntry* p, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap);



__inline static void ensureSize(Retweet5JoinTupleMap* retweet5JoinTupleMap, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId) {
    if(retweet5JoinTupleMap->size >= retweet5JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet5JoinTupleMap, r4TweetIdR5RetweetTweetId, r5UserIdFUserId );
    }
}
static void increaseArraySize(Retweet5JoinTupleMap* retweet5JoinTupleMap, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId) {
    int newcapacity = retweet5JoinTupleMap->capacity * 2;
    Retweet5JoinTupleEntry** newJoinTupleEntryArray = new Retweet5JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet5JoinTupleMap->capacity; i++) {
        Retweet5JoinTupleEntry* p = retweet5JoinTupleMap->retweet5JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, r4TweetIdR5RetweetTweetId, r5UserIdFUserId );
                h = h & (newcapacity - 1);
                Retweet5JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete retweet5JoinTupleMap->retweet5JoinTupleEntryArray;
    retweet5JoinTupleMap->retweet5JoinTupleEntryArray = newJoinTupleEntryArray;
    retweet5JoinTupleMap->capacity = newcapacity;
}
Retweet5JoinTupleEntry* putRetweet5JoinTupleIfAbsent(Retweet5JoinTupleMap* retweet5JoinTupleMap, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, r4TweetIdR5RetweetTweetId, r5UserIdFUserId ); 
    h = h & (retweet5JoinTupleMap->capacity - 1);
    Retweet5JoinTupleEntry* candidate = retweet5JoinTupleMap->retweet5JoinTupleEntryArray[h];
    Retweet5JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->retweet4Retweet5VertexValue->r4TweetIdR5RetweetTweetId == r4TweetIdR5RetweetTweetId
        &&         candidate->retweet5FollowerVertexValue->r5UserIdFUserId == r5UserIdFUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet5JoinTupleEntry* p = new Retweet5JoinTupleEntry;
    if(!lastP)
        retweet5JoinTupleMap->retweet5JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet5JoinTupleMap->size++;
    ensureSize(retweet5JoinTupleMap, r4TweetIdR5RetweetTweetId, r5UserIdFUserId );
    addToVertexValues(r4TweetIdR5RetweetTweetId , p, retweet4Retweet5VertexValueMap);
    addToVertexValues(r5UserIdFUserId , p, retweet5FollowerVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& r4TweetIdR5RetweetTweetId , Retweet5JoinTupleEntry* p, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap) {
    Retweet4Retweet5VertexValueEntry* vertexValue = putRetweet4Retweet5VertexValueIfAbsent(retweet4Retweet5VertexValueMap, r4TweetIdR5RetweetTweetId );
    p->retweet4Retweet5VertexValue = vertexValue;
    vertexValue->retweet5JoinTupleList.push_back(p);
}
__inline static void addToVertexValues(int& r5UserIdFUserId , Retweet5JoinTupleEntry* p, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap) {
    Retweet5FollowerVertexValueEntry* vertexValue = putRetweet5FollowerVertexValueIfAbsent(retweet5FollowerVertexValueMap, r5UserIdFUserId );
    p->retweet5FollowerVertexValue = vertexValue;
    vertexValue->retweet5JoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId) {
    h ^=  r4TweetIdR5RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  r5UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



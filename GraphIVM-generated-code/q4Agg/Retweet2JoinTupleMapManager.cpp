#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& retweet1TweetIdRetweet2RetweetTweetId,int& retweet2UserIdFollowerUserId) __attribute__((always_inline));
static void ensureSize(Retweet2JoinTupleMap* retweet2JoinTupleMap);
static void increaseArraySize(Retweet2JoinTupleMap* retweet2JoinTupleMap);
static void addToVertexValues(int& retweet1TweetIdRetweet2RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);
static void addToVertexValues(int& retweet2UserIdFollowerUserId , Retweet2JoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap);



__inline static void ensureSize(Retweet2JoinTupleMap* retweet2JoinTupleMap) {
    if(retweet2JoinTupleMap->size >= retweet2JoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweet2JoinTupleMap);
    }
}
static void increaseArraySize(Retweet2JoinTupleMap* retweet2JoinTupleMap) {
    int newcapacity = retweet2JoinTupleMap->capacity * 2;
    Retweet2JoinTupleEntry** newJoinTupleEntryArray = new Retweet2JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweet2JoinTupleMap->capacity; i++) {
        Retweet2JoinTupleEntry* p = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h
                , p->retweet1Retweet2VertexValue->retweet1TweetIdRetweet2RetweetTweetId
                , p->retweet2FollowerVertexValue->retweet2UserIdFollowerUserId
                );
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
Retweet2JoinTupleEntry* putRetweet2JoinTupleIfAbsent(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& retweet1TweetIdRetweet2RetweetTweetId,int& retweet2UserIdFollowerUserId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap ) {
    int h = 31;
    hashCode(h, retweet1TweetIdRetweet2RetweetTweetId, retweet2UserIdFollowerUserId ); 
    h = h & (retweet2JoinTupleMap->capacity - 1);
    Retweet2JoinTupleEntry* candidate = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h];
    Retweet2JoinTupleEntry* lastP = NULL;
    while(candidate) {
        if(
                candidate->retweet1Retweet2VertexValue->retweet1TweetIdRetweet2RetweetTweetId == retweet1TweetIdRetweet2RetweetTweetId
        &&         candidate->retweet2FollowerVertexValue->retweet2UserIdFollowerUserId == retweet2UserIdFollowerUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Retweet2JoinTupleEntry* p = new Retweet2JoinTupleEntry;
    addToVertexValues(retweet1TweetIdRetweet2RetweetTweetId , p, retweet1Retweet2VertexValueMap);
    addToVertexValues(retweet2UserIdFollowerUserId , p, retweet2FollowerVertexValueMap);
    if(!lastP)
        retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweet2JoinTupleMap->size++;
    ensureSize(retweet2JoinTupleMap);
    return p;
}
__inline static void addToVertexValues(int& retweet1TweetIdRetweet2RetweetTweetId , Retweet2JoinTupleEntry* p, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
    Retweet1Retweet2VertexValueEntry* vertexValue = putRetweet1Retweet2VertexValueIfAbsent(retweet1Retweet2VertexValueMap, retweet1TweetIdRetweet2RetweetTweetId );
    p->retweet1Retweet2VertexValue = vertexValue;
    vertexValue->retweet2JoinTupleList.push_back(p);
}

__inline static void addToVertexValues(int& retweet2UserIdFollowerUserId , Retweet2JoinTupleEntry* p, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap) {
    Retweet2FollowerVertexValueEntry* vertexValue = putRetweet2FollowerVertexValueIfAbsent(retweet2FollowerVertexValueMap, retweet2UserIdFollowerUserId );
    p->retweet2FollowerVertexValue = vertexValue;
    vertexValue->retweet2JoinTupleList.push_back(p);
}

inline static void hashCode(int&h, int& retweet1TweetIdRetweet2RetweetTweetId,int& retweet2UserIdFollowerUserId) {
    h ^=  retweet1TweetIdRetweet2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  retweet2UserIdFollowerUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



Retweet2JoinTupleEntry** getRetweet2JoinTuplePointer(Retweet2JoinTupleMap* retweet2JoinTupleMap
, int& retweet1TweetIdRetweet2RetweetTweetId
, int& retweet2UserIdFollowerUserId
)  {
    int h = 31;
    hashCode(h
    , retweet1TweetIdRetweet2RetweetTweetId  
    , retweet2UserIdFollowerUserId  
    );
    h = h & (retweet2JoinTupleMap->capacity - 1);
    Retweet2JoinTupleEntry* candidate = retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h];
    Retweet2JoinTupleEntry** address = &(retweet2JoinTupleMap->retweet2JoinTupleEntryArray[h]);
    while(candidate) {
        if(
              candidate->retweet1Retweet2VertexValue->retweet1TweetIdRetweet2RetweetTweetId == retweet1TweetIdRetweet2RetweetTweetId
            &&  candidate->retweet2FollowerVertexValue->retweet2UserIdFollowerUserId == retweet2UserIdFollowerUserId
        ) {
            return address;
        } else {
            address = &(candidate->next);
            candidate = candidate->next;
        }
    }
    return NULL;
}
void removeJoinTupleEntry(Retweet2JoinTupleMap* retweet2JoinTupleMap, Retweet2JoinTupleEntry** entryPointer) {
    if((*entryPointer)->next) {
        (*entryPointer)->next = (*entryPointer)->next->next;
    } else {
        delete *entryPointer;
        (*entryPointer) = NULL;
    }
    retweet2JoinTupleMap->size--;
}

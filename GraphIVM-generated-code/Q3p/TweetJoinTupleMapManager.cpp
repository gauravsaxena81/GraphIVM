#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& userIdTweetUserId,int& tweetTweetIdRetweetRetweetTweetId) __attribute__((always_inline));
static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap);
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap);
static void addToVertexValues(int& userIdTweetUserId , TweetJoinTupleEntry* p, UserTweetVertexValueMap* userTweetVertexValueMap);
static void addToVertexValues(int& tweetTweetIdRetweetRetweetTweetId , TweetJoinTupleEntry* p, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap);



__inline static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap) {
    if(tweetJoinTupleMap->size >= tweetJoinTupleMap->capacity * 0.8) {
        increaseArraySize(tweetJoinTupleMap);
    }
}
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap) {
    int newcapacity = tweetJoinTupleMap->capacity * 2;
    TweetJoinTupleEntry** newJoinTupleEntryArray = new TweetJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < tweetJoinTupleMap->capacity; i++) {
        TweetJoinTupleEntry* p = tweetJoinTupleMap->tweetJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h
                , p->userTweetVertexValue->userIdTweetUserId
                , p->tweetRetweetVertexValue->tweetTweetIdRetweetRetweetTweetId
                );
                h = h & (newcapacity - 1);
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
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& userIdTweetUserId,int& tweetTweetIdRetweetRetweetTweetId, UserTweetVertexValueMap* userTweetVertexValueMap, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap ) {
    int h = 31;
    hashCode(h, userIdTweetUserId, tweetTweetIdRetweetRetweetTweetId ); 
    h = h & (tweetJoinTupleMap->capacity - 1);
    TweetJoinTupleEntry* candidate = tweetJoinTupleMap->tweetJoinTupleEntryArray[h];
    TweetJoinTupleEntry* lastP = NULL;
    while(candidate) {
        if(
                candidate->userTweetVertexValue->userIdTweetUserId == userIdTweetUserId
        &&         candidate->tweetRetweetVertexValue->tweetTweetIdRetweetRetweetTweetId == tweetTweetIdRetweetRetweetTweetId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    TweetJoinTupleEntry* p = new TweetJoinTupleEntry;
    addToVertexValues(userIdTweetUserId , p, userTweetVertexValueMap);
    addToVertexValues(tweetTweetIdRetweetRetweetTweetId , p, tweetRetweetVertexValueMap);
    if(!lastP)
        tweetJoinTupleMap->tweetJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    tweetJoinTupleMap->size++;
    ensureSize(tweetJoinTupleMap);
    return p;
}
__inline static void addToVertexValues(int& userIdTweetUserId , TweetJoinTupleEntry* p, UserTweetVertexValueMap* userTweetVertexValueMap) {
    UserTweetVertexValueEntry* vertexValue = putUserTweetVertexValueIfAbsent(userTweetVertexValueMap, userIdTweetUserId );
    p->userTweetVertexValue = vertexValue;
    vertexValue->tweetJoinTupleList.push_back(p);
}

__inline static void addToVertexValues(int& tweetTweetIdRetweetRetweetTweetId , TweetJoinTupleEntry* p, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap) {
    TweetRetweetVertexValueEntry* vertexValue = putTweetRetweetVertexValueIfAbsent(tweetRetweetVertexValueMap, tweetTweetIdRetweetRetweetTweetId );
    p->tweetRetweetVertexValue = vertexValue;
    vertexValue->tweetJoinTupleList.push_back(p);
}

inline static void hashCode(int&h, int& userIdTweetUserId,int& tweetTweetIdRetweetRetweetTweetId) {
    h ^=  userIdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  tweetTweetIdRetweetRetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}



TweetJoinTupleEntry** getTweetJoinTuplePointer(TweetJoinTupleMap* tweetJoinTupleMap
, int& userIdTweetUserId
, int& tweetTweetIdRetweetRetweetTweetId
)  {
    int h = 31;
    hashCode(h
    , userIdTweetUserId  
    , tweetTweetIdRetweetRetweetTweetId  
    );
    h = h & (tweetJoinTupleMap->capacity - 1);
    TweetJoinTupleEntry* candidate = tweetJoinTupleMap->tweetJoinTupleEntryArray[h];
    TweetJoinTupleEntry** address = &(tweetJoinTupleMap->tweetJoinTupleEntryArray[h]);
    while(candidate) {
        if(
              candidate->userTweetVertexValue->userIdTweetUserId == userIdTweetUserId
            &&  candidate->tweetRetweetVertexValue->tweetTweetIdRetweetRetweetTweetId == tweetTweetIdRetweetRetweetTweetId
        ) {
            return address;
        } else {
            address = &(candidate->next);
            candidate = candidate->next;
        }
    }
    return NULL;
}
void removeJoinTupleEntry(TweetJoinTupleMap* tweetJoinTupleMap, TweetJoinTupleEntry** entryPointer) {
    if((*entryPointer)->next) {
        (*entryPointer)->next = (*entryPointer)->next->next;
    } else {
        delete *entryPointer;
        (*entryPointer) = NULL;
    }
    tweetJoinTupleMap->size--;
}

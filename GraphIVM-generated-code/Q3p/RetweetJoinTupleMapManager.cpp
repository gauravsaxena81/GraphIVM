#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& tweetTweetIdRetweetRetweetTweetId) __attribute__((always_inline));
static void ensureSize(RetweetJoinTupleMap* retweetJoinTupleMap);
static void increaseArraySize(RetweetJoinTupleMap* retweetJoinTupleMap);
static void addToVertexValues(int& tweetTweetIdRetweetRetweetTweetId , RetweetJoinTupleEntry* p, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap);



__inline static void ensureSize(RetweetJoinTupleMap* retweetJoinTupleMap) {
    if(retweetJoinTupleMap->size >= retweetJoinTupleMap->capacity * 0.8) {
        increaseArraySize(retweetJoinTupleMap);
    }
}
static void increaseArraySize(RetweetJoinTupleMap* retweetJoinTupleMap) {
    int newcapacity = retweetJoinTupleMap->capacity * 2;
    RetweetJoinTupleEntry** newJoinTupleEntryArray = new RetweetJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < retweetJoinTupleMap->capacity; i++) {
        RetweetJoinTupleEntry* p = retweetJoinTupleMap->retweetJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h
                , p->tweetRetweetVertexValue->tweetTweetIdRetweetRetweetTweetId
                );
                h = h & (newcapacity - 1);
                RetweetJoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete retweetJoinTupleMap->retweetJoinTupleEntryArray;
    retweetJoinTupleMap->retweetJoinTupleEntryArray = newJoinTupleEntryArray;
    retweetJoinTupleMap->capacity = newcapacity;
}
RetweetJoinTupleEntry* putRetweetJoinTupleIfAbsent(RetweetJoinTupleMap* retweetJoinTupleMap, int& tweetTweetIdRetweetRetweetTweetId, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap ) {
    int h = 31;
    hashCode(h, tweetTweetIdRetweetRetweetTweetId ); 
    h = h & (retweetJoinTupleMap->capacity - 1);
    RetweetJoinTupleEntry* candidate = retweetJoinTupleMap->retweetJoinTupleEntryArray[h];
    RetweetJoinTupleEntry* lastP = NULL;
    while(candidate) {
        if(
                candidate->tweetRetweetVertexValue->tweetTweetIdRetweetRetweetTweetId == tweetTweetIdRetweetRetweetTweetId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    RetweetJoinTupleEntry* p = new RetweetJoinTupleEntry;
    addToVertexValues(tweetTweetIdRetweetRetweetTweetId , p, tweetRetweetVertexValueMap);
    if(!lastP)
        retweetJoinTupleMap->retweetJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    retweetJoinTupleMap->size++;
    ensureSize(retweetJoinTupleMap);
    return p;
}
__inline static void addToVertexValues(int& tweetTweetIdRetweetRetweetTweetId , RetweetJoinTupleEntry* p, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap) {
    TweetRetweetVertexValueEntry* vertexValue = putTweetRetweetVertexValueIfAbsent(tweetRetweetVertexValueMap, tweetTweetIdRetweetRetweetTweetId );
    p->tweetRetweetVertexValue = vertexValue;
    vertexValue->retweetJoinTupleList.push_back(p);
}

inline static void hashCode(int&h, int& tweetTweetIdRetweetRetweetTweetId) {
    h ^=  tweetTweetIdRetweetRetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}



RetweetJoinTupleEntry** getRetweetJoinTuplePointer(RetweetJoinTupleMap* retweetJoinTupleMap
, int& tweetTweetIdRetweetRetweetTweetId
)  {
    int h = 31;
    hashCode(h
    , tweetTweetIdRetweetRetweetTweetId  
    );
    h = h & (retweetJoinTupleMap->capacity - 1);
    RetweetJoinTupleEntry* candidate = retweetJoinTupleMap->retweetJoinTupleEntryArray[h];
    RetweetJoinTupleEntry** address = &(retweetJoinTupleMap->retweetJoinTupleEntryArray[h]);
    while(candidate) {
        if(
              candidate->tweetRetweetVertexValue->tweetTweetIdRetweetRetweetTweetId == tweetTweetIdRetweetRetweetTweetId
        ) {
            return address;
        } else {
            address = &(candidate->next);
            candidate = candidate->next;
        }
    }
    return NULL;
}
void removeJoinTupleEntry(RetweetJoinTupleMap* retweetJoinTupleMap, RetweetJoinTupleEntry** entryPointer) {
    if((*entryPointer)->next) {
        (*entryPointer)->next = (*entryPointer)->next->next;
    } else {
        delete *entryPointer;
        (*entryPointer) = NULL;
    }
    retweetJoinTupleMap->size--;
}

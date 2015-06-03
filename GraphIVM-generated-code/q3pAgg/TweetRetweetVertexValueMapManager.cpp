#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& tweetTweetIdRetweetRetweetTweetId ) __attribute__((always_inline));
static void ensureSize(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap);
static void increaseArraySize(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap);

__inline static void ensureSize(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap) {
	if(tweetRetweetVertexValueMap->size >= tweetRetweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(tweetRetweetVertexValueMap);
	}
}
static void increaseArraySize(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap) {
	int newcapacity = tweetRetweetVertexValueMap->capacity * 2;
	TweetRetweetVertexValueEntry** newVertexValueEntryArray = new TweetRetweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < tweetRetweetVertexValueMap->capacity; i++) {
		TweetRetweetVertexValueEntry* p = tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[i];
		if(p) {
		    do {
			    int h = 31;
			    hashCode(h, p->tweetTweetIdRetweetRetweetTweetId );
			    h = h & (newcapacity - 1);
			TweetRetweetVertexValueEntry* nextP = p->next;
			p->next = newVertexValueEntryArray[h];
			newVertexValueEntryArray[h] = p;
			p = nextP;
		    } while (p);
		}
	}
	delete(tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray);
	tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray = newVertexValueEntryArray;
	tweetRetweetVertexValueMap->capacity = newcapacity;
}
TweetRetweetVertexValueEntry* putTweetRetweetVertexValueIfAbsent(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap, int& tweetTweetIdRetweetRetweetTweetId ) {
	int h = 31;
	hashCode(h, tweetTweetIdRetweetRetweetTweetId ); 
	h = h & (tweetRetweetVertexValueMap->capacity - 1);
	TweetRetweetVertexValueEntry* candidate = tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[h];
	TweetRetweetVertexValueEntry* lastP = NULL;
	while(candidate) {
		if(
		  candidate->tweetTweetIdRetweetRetweetTweetId == (tweetTweetIdRetweetRetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	TweetRetweetVertexValueEntry* p = new TweetRetweetVertexValueEntry;
	p->tweetTweetIdRetweetRetweetTweetId = tweetTweetIdRetweetRetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[h] = p;
	tweetRetweetVertexValueMap->size++;
	ensureSize(tweetRetweetVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& tweetTweetIdRetweetRetweetTweetId ) {
	h ^=  tweetTweetIdRetweetRetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
void deleteTweetRetweetVertexValueIfZero(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& tweetTweetIdRetweetRetweetTweetId
) {
    std::vector<TweetJoinTupleEntry*> *vec = &(tweetJoinTuple->tweetRetweetVertexValue->tweetJoinTupleList);
    TweetRetweetVertexValueEntry* entry = tweetJoinTuple->tweetRetweetVertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), tweetJoinTuple));
    if(vec->size() == 0 && entry->retweetJoinTupleList.size() == 0) {
        if(entry->next) {
            TweetRetweetVertexValueEntry* next = entry->next;
            entry->tweetTweetIdRetweetRetweetTweetId = next->tweetTweetIdRetweetRetweetTweetId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->tweetTweetIdRetweetRetweetTweetId 
            );
            h = h & (tweetRetweetVertexValueMap->capacity - 1);
            delete tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[h];
            tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[h] = NULL;
        }
        tweetRetweetVertexValueMap->size--;
    }
}
void deleteTweetRetweetVertexValueIfZero(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap, RetweetJoinTupleEntry* retweetJoinTuple
, int& tweetTweetIdRetweetRetweetTweetId
) {
    std::vector<RetweetJoinTupleEntry*> *vec = &(retweetJoinTuple->tweetRetweetVertexValue->retweetJoinTupleList);
    TweetRetweetVertexValueEntry* entry = retweetJoinTuple->tweetRetweetVertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), retweetJoinTuple));
    if(vec->size() == 0 && entry->tweetJoinTupleList.size() == 0) {
        if(entry->next) {
            TweetRetweetVertexValueEntry* next = entry->next;
            entry->tweetTweetIdRetweetRetweetTweetId = next->tweetTweetIdRetweetRetweetTweetId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->tweetTweetIdRetweetRetweetTweetId 
            );
            h = h & (tweetRetweetVertexValueMap->capacity - 1);
            delete tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[h];
            tweetRetweetVertexValueMap->tweetRetweetVertexValueEntryArray[h] = NULL;
        }
        tweetRetweetVertexValueMap->size--;
    }
}

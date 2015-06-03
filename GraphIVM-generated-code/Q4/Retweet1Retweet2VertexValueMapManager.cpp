#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& retweet1TweetIdRetweet2RetweetTweetId ) __attribute__((always_inline));
static void ensureSize(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);
static void increaseArraySize(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap);

__inline static void ensureSize(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
	if(retweet1Retweet2VertexValueMap->size >= retweet1Retweet2VertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet1Retweet2VertexValueMap);
	}
}
static void increaseArraySize(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap) {
	int newcapacity = retweet1Retweet2VertexValueMap->capacity * 2;
	Retweet1Retweet2VertexValueEntry** newVertexValueEntryArray = new Retweet1Retweet2VertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet1Retweet2VertexValueMap->capacity; i++) {
		Retweet1Retweet2VertexValueEntry* p = retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[i];
		if(p) {
			do {
			    int h = 31;
                hashCode(h, p->retweet1TweetIdRetweet2RetweetTweetId );
                h = h & (newcapacity - 1);
                Retweet1Retweet2VertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray);
	retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray = newVertexValueEntryArray;
	retweet1Retweet2VertexValueMap->capacity = newcapacity;
}
Retweet1Retweet2VertexValueEntry* putRetweet1Retweet2VertexValueIfAbsent(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, int& retweet1TweetIdRetweet2RetweetTweetId ) {
	int h = 31;
	hashCode(h, retweet1TweetIdRetweet2RetweetTweetId ); 
	h = h & (retweet1Retweet2VertexValueMap->capacity - 1);
	Retweet1Retweet2VertexValueEntry* candidate = retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h];
	Retweet1Retweet2VertexValueEntry* lastP = NULL;
	while(candidate) {
		if(
		  candidate->retweet1TweetIdRetweet2RetweetTweetId == (retweet1TweetIdRetweet2RetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet1Retweet2VertexValueEntry* p = new Retweet1Retweet2VertexValueEntry;
	p->retweet1TweetIdRetweet2RetweetTweetId = retweet1TweetIdRetweet2RetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h] = p;
	retweet1Retweet2VertexValueMap->size++;
	ensureSize(retweet1Retweet2VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& retweet1TweetIdRetweet2RetweetTweetId ) {
	h ^=  retweet1TweetIdRetweet2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
void deleteRetweet1Retweet2VertexValueIfZero(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet1JoinTupleEntry* retweet1JoinTuple
, int& retweet1TweetIdRetweet2RetweetTweetId
) {
    std::vector<Retweet1JoinTupleEntry*> *vec = &(retweet1JoinTuple->retweet1Retweet2VertexValue->retweet1JoinTupleList);
    Retweet1Retweet2VertexValueEntry* entry = retweet1JoinTuple->retweet1Retweet2VertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), retweet1JoinTuple));
    if(vec->size() == 0 && entry->retweet2JoinTupleList.size() == 0) {
        if(entry->next) {
            Retweet1Retweet2VertexValueEntry* next = entry->next;
            entry->retweet1TweetIdRetweet2RetweetTweetId = next->retweet1TweetIdRetweet2RetweetTweetId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->retweet1TweetIdRetweet2RetweetTweetId 
            );
            h = h & (retweet1Retweet2VertexValueMap->capacity - 1);
            delete retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h];
            retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h] = NULL;
        }
        retweet1Retweet2VertexValueMap->size--;
    }
}
void deleteRetweet1Retweet2VertexValueIfZero(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2JoinTupleEntry* retweet2JoinTuple
, int& retweet1TweetIdRetweet2RetweetTweetId
) {
    std::vector<Retweet2JoinTupleEntry*> *vec = &(retweet2JoinTuple->retweet1Retweet2VertexValue->retweet2JoinTupleList);
    Retweet1Retweet2VertexValueEntry* entry = retweet2JoinTuple->retweet1Retweet2VertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), retweet2JoinTuple));
    if(vec->size() == 0 && entry->retweet1JoinTupleList.size() == 0) {
        if(entry->next) {
            Retweet1Retweet2VertexValueEntry* next = entry->next;
            entry->retweet1TweetIdRetweet2RetweetTweetId = next->retweet1TweetIdRetweet2RetweetTweetId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->retweet1TweetIdRetweet2RetweetTweetId 
            );
            h = h & (retweet1Retweet2VertexValueMap->capacity - 1);
            delete retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h];
            retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h] = NULL;
        }
        retweet1Retweet2VertexValueMap->size--;
    }
}

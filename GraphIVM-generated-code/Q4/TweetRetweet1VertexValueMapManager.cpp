#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& tweetTweetIdRetweet1RetweetTweetId ) __attribute__((always_inline));
static void ensureSize(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap);
static void increaseArraySize(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap);

__inline static void ensureSize(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap) {
	if(tweetRetweet1VertexValueMap->size >= tweetRetweet1VertexValueMap->capacity * 0.8) {
		increaseArraySize(tweetRetweet1VertexValueMap);
	}
}
static void increaseArraySize(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap) {
	int newcapacity = tweetRetweet1VertexValueMap->capacity * 2;
	TweetRetweet1VertexValueEntry** newVertexValueEntryArray = new TweetRetweet1VertexValueEntry*[newcapacity]();
	for(int i = 0; i < tweetRetweet1VertexValueMap->capacity; i++) {
		TweetRetweet1VertexValueEntry* p = tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[i];
		if(p) {
			do {
			    int h = 31;
                hashCode(h, p->tweetTweetIdRetweet1RetweetTweetId );
                h = h & (newcapacity - 1);
                TweetRetweet1VertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray);
	tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray = newVertexValueEntryArray;
	tweetRetweet1VertexValueMap->capacity = newcapacity;
}
TweetRetweet1VertexValueEntry* putTweetRetweet1VertexValueIfAbsent(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, int& tweetTweetIdRetweet1RetweetTweetId ) {
	int h = 31;
	hashCode(h, tweetTweetIdRetweet1RetweetTweetId ); 
	h = h & (tweetRetweet1VertexValueMap->capacity - 1);
	TweetRetweet1VertexValueEntry* candidate = tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[h];
	TweetRetweet1VertexValueEntry* lastP = NULL;
	while(candidate) {
		if(
		  candidate->tweetTweetIdRetweet1RetweetTweetId == (tweetTweetIdRetweet1RetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	TweetRetweet1VertexValueEntry* p = new TweetRetweet1VertexValueEntry;
	p->tweetTweetIdRetweet1RetweetTweetId = tweetTweetIdRetweet1RetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[h] = p;
	tweetRetweet1VertexValueMap->size++;
	ensureSize(tweetRetweet1VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& tweetTweetIdRetweet1RetweetTweetId ) {
	h ^=  tweetTweetIdRetweet1RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
void deleteTweetRetweet1VertexValueIfZero(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& tweetTweetIdRetweet1RetweetTweetId
) {
    std::vector<TweetJoinTupleEntry*> *vec = &(tweetJoinTuple->tweetRetweet1VertexValue->tweetJoinTupleList);
    TweetRetweet1VertexValueEntry* entry = tweetJoinTuple->tweetRetweet1VertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), tweetJoinTuple));
    if(vec->size() == 0 && entry->retweet1JoinTupleList.size() == 0) {
        if(entry->next) {
            TweetRetweet1VertexValueEntry* next = entry->next;
            entry->tweetTweetIdRetweet1RetweetTweetId = next->tweetTweetIdRetweet1RetweetTweetId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->tweetTweetIdRetweet1RetweetTweetId 
            );
            h = h & (tweetRetweet1VertexValueMap->capacity - 1);
            delete tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[h];
            tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[h] = NULL;
        }
        tweetRetweet1VertexValueMap->size--;
    }
}
void deleteTweetRetweet1VertexValueIfZero(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, Retweet1JoinTupleEntry* retweet1JoinTuple
, int& tweetTweetIdRetweet1RetweetTweetId
) {
    std::vector<Retweet1JoinTupleEntry*> *vec = &(retweet1JoinTuple->tweetRetweet1VertexValue->retweet1JoinTupleList);
    TweetRetweet1VertexValueEntry* entry = retweet1JoinTuple->tweetRetweet1VertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), retweet1JoinTuple));
    if(vec->size() == 0 && entry->tweetJoinTupleList.size() == 0) {
        if(entry->next) {
            TweetRetweet1VertexValueEntry* next = entry->next;
            entry->tweetTweetIdRetweet1RetweetTweetId = next->tweetTweetIdRetweet1RetweetTweetId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->tweetTweetIdRetweet1RetweetTweetId 
            );
            h = h & (tweetRetweet1VertexValueMap->capacity - 1);
            delete tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[h];
            tweetRetweet1VertexValueMap->tweetRetweet1VertexValueEntryArray[h] = NULL;
        }
        tweetRetweet1VertexValueMap->size--;
    }
}

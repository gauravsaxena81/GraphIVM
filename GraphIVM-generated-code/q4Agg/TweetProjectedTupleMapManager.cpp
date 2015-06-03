#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& userId, int& tweetTweetIdRetweet1RetweetTweetId ) __attribute__((always_inline));
static void increaseArraySize(TweetProjectedTupleMap* tweetProjectedTupleEntryArray);
static void ensureSize(TweetProjectedTupleMap* tweetProjectedTupleMap);


__inline static void ensureSize(TweetProjectedTupleMap* tweetProjectedTupleMap) {
    if(tweetProjectedTupleMap->size >= tweetProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(tweetProjectedTupleMap);
    }
}
static void increaseArraySize(TweetProjectedTupleMap* tweetProjectedTupleMap) {
	int newcapacity = tweetProjectedTupleMap->capacity * 2;
	TweetProjectedTupleEntry** newprojectedTupleEntryArray = new TweetProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < tweetProjectedTupleMap->capacity; i++) {
		TweetProjectedTupleEntry* p = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[i];
		if(p) {
			do {
			    int h = 31; 
                hashCode(h
                 , p->userId 
                 , p->tweetTweetIdRetweet1RetweetTweetId 
                );
                h = h & (newcapacity - 1);
                TweetProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(tweetProjectedTupleMap->tweetProjectedTupleEntryArray);
	tweetProjectedTupleMap->tweetProjectedTupleEntryArray = newprojectedTupleEntryArray;
	tweetProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& userId, int& tweetTweetIdRetweet1RetweetTweetId ) {
    h ^=  userId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  tweetTweetIdRetweet1RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap, int& userId, int& tweetTweetIdRetweet1RetweetTweetId ) {
	int h = 31;
	hashCode(h
	, userId
	, tweetTweetIdRetweet1RetweetTweetId
	); 
	h = h & (tweetProjectedTupleMap->capacity - 1);
	TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h];
	TweetProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->userId == (userId)
&& 				candidate->tweetTweetIdRetweet1RetweetTweetId == (tweetTweetIdRetweet1RetweetTweetId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	TweetProjectedTupleEntry* p = new TweetProjectedTupleEntry;
	p->userId = userId;
	p->tweetTweetIdRetweet1RetweetTweetId = tweetTweetIdRetweet1RetweetTweetId;
	if(!lastP)
		tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	tweetProjectedTupleMap->size++;
	ensureSize(tweetProjectedTupleMap);
	return p;
}
int deleteTweetProjectedTupleIfZero(TweetProjectedTupleMap* tweetProjectedTupleMap
, int& userId 
, int& tweetTweetIdRetweet1RetweetTweetId 
) {
    int h = 31;
    hashCode(h
    , userId
    , tweetTweetIdRetweet1RetweetTweetId
    );
    h = h & (tweetProjectedTupleMap->capacity - 1);
    TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h];
    TweetProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
              candidate->userId == userId
            && candidate->tweetTweetIdRetweet1RetweetTweetId == tweetTweetIdRetweet1RetweetTweetId
        ) {
            if(--candidate->count == 0) {
                if(lastP) {
                    lastP->next = candidate->next;
                } else
                    tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h] = NULL;
                delete candidate;
            }
            return --(tweetProjectedTupleMap->size);
        } else {
            candidate = candidate->next;
        }
    }
    return -1;
}

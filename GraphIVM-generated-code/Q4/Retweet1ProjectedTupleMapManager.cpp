#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId ) __attribute__((always_inline));
static void increaseArraySize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleEntryArray);
static void ensureSize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap);


__inline static void ensureSize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap) {
    if(retweet1ProjectedTupleMap->size >= retweet1ProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(retweet1ProjectedTupleMap);
    }
}
static void increaseArraySize(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap) {
	int newcapacity = retweet1ProjectedTupleMap->capacity * 2;
	Retweet1ProjectedTupleEntry** newprojectedTupleEntryArray = new Retweet1ProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < retweet1ProjectedTupleMap->capacity; i++) {
		Retweet1ProjectedTupleEntry* p = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[i];
		if(p) {
			do {
			    int h = 31; 
                hashCode(h
                 , p->tweetTweetIdRetweet1RetweetTweetId 
                 , p->retweet1TweetIdRetweet2RetweetTweetId 
                );
                h = h & (newcapacity - 1);
                Retweet1ProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray);
	retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray = newprojectedTupleEntryArray;
	retweet1ProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId ) {
    h ^=  tweetTweetIdRetweet1RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  retweet1TweetIdRetweet2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}
Retweet1ProjectedTupleEntry* putRetweet1ProjectedTuple(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap, int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId ) {
	int h = 31;
	hashCode(h
	, tweetTweetIdRetweet1RetweetTweetId
	, retweet1TweetIdRetweet2RetweetTweetId
	); 
	h = h & (retweet1ProjectedTupleMap->capacity - 1);
	Retweet1ProjectedTupleEntry* candidate = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[h];
	Retweet1ProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->tweetTweetIdRetweet1RetweetTweetId == (tweetTweetIdRetweet1RetweetTweetId)
&& 				candidate->retweet1TweetIdRetweet2RetweetTweetId == (retweet1TweetIdRetweet2RetweetTweetId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet1ProjectedTupleEntry* p = new Retweet1ProjectedTupleEntry;
	p->tweetTweetIdRetweet1RetweetTweetId = tweetTweetIdRetweet1RetweetTweetId;
	p->retweet1TweetIdRetweet2RetweetTweetId = retweet1TweetIdRetweet2RetweetTweetId;
	if(!lastP)
		retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	retweet1ProjectedTupleMap->size++;
	ensureSize(retweet1ProjectedTupleMap);
	return p;
}
int deleteRetweet1ProjectedTupleIfZero(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap
, int& tweetTweetIdRetweet1RetweetTweetId 
, int& retweet1TweetIdRetweet2RetweetTweetId 
) {
    int h = 31;
    hashCode(h
    , tweetTweetIdRetweet1RetweetTweetId
    , retweet1TweetIdRetweet2RetweetTweetId
    );
    h = h & (retweet1ProjectedTupleMap->capacity - 1);
    Retweet1ProjectedTupleEntry* candidate = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[h];
    Retweet1ProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
              candidate->tweetTweetIdRetweet1RetweetTweetId == tweetTweetIdRetweet1RetweetTweetId
            && candidate->retweet1TweetIdRetweet2RetweetTweetId == retweet1TweetIdRetweet2RetweetTweetId
        ) {
            if(--candidate->count == 0) {
                if(lastP) {
                    lastP->next = candidate->next;
                } else
                    retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[h] = NULL;
                delete candidate;
            }
            return --(retweet1ProjectedTupleMap->size);
        } else {
            candidate = candidate->next;
        }
    }
    return -1;
}



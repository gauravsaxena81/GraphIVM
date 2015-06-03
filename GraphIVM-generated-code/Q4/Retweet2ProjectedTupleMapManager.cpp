#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId ) __attribute__((always_inline));
static void increaseArraySize(Retweet2ProjectedTupleMap* retweet2ProjectedTupleEntryArray);
static void ensureSize(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap);


__inline static void ensureSize(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap) {
    if(retweet2ProjectedTupleMap->size >= retweet2ProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(retweet2ProjectedTupleMap);
    }
}
static void increaseArraySize(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap) {
	int newcapacity = retweet2ProjectedTupleMap->capacity * 2;
	Retweet2ProjectedTupleEntry** newprojectedTupleEntryArray = new Retweet2ProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < retweet2ProjectedTupleMap->capacity; i++) {
		Retweet2ProjectedTupleEntry* p = retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[i];
		if(p) {
			do {
			    int h = 31; 
                hashCode(h
                 , p->retweet1TweetIdRetweet2RetweetTweetId 
                 , p->retweet2TweetId 
                 , p->retweet2UserIdFollowerUserId 
                );
                h = h & (newcapacity - 1);
                Retweet2ProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray);
	retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray = newprojectedTupleEntryArray;
	retweet2ProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId ) {
    h ^=  retweet1TweetIdRetweet2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  retweet2TweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  retweet2UserIdFollowerUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}
Retweet2ProjectedTupleEntry* putRetweet2ProjectedTuple(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap, int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId ) {
	int h = 31;
	hashCode(h
	, retweet1TweetIdRetweet2RetweetTweetId
	, retweet2TweetId
	, retweet2UserIdFollowerUserId
	); 
	h = h & (retweet2ProjectedTupleMap->capacity - 1);
	Retweet2ProjectedTupleEntry* candidate = retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[h];
	Retweet2ProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->retweet1TweetIdRetweet2RetweetTweetId == (retweet1TweetIdRetweet2RetweetTweetId)
&& 				candidate->retweet2TweetId == (retweet2TweetId)
&& 				candidate->retweet2UserIdFollowerUserId == (retweet2UserIdFollowerUserId)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet2ProjectedTupleEntry* p = new Retweet2ProjectedTupleEntry;
	p->retweet1TweetIdRetweet2RetweetTweetId = retweet1TweetIdRetweet2RetweetTweetId;
	p->retweet2TweetId = retweet2TweetId;
	p->retweet2UserIdFollowerUserId = retweet2UserIdFollowerUserId;
	if(!lastP)
		retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	retweet2ProjectedTupleMap->size++;
	ensureSize(retweet2ProjectedTupleMap);
	return p;
}
int deleteRetweet2ProjectedTupleIfZero(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap
, int& retweet1TweetIdRetweet2RetweetTweetId 
, int& retweet2TweetId 
, int& retweet2UserIdFollowerUserId 
) {
    int h = 31;
    hashCode(h
    , retweet1TweetIdRetweet2RetweetTweetId
    , retweet2TweetId
    , retweet2UserIdFollowerUserId
    );
    h = h & (retweet2ProjectedTupleMap->capacity - 1);
    Retweet2ProjectedTupleEntry* candidate = retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[h];
    Retweet2ProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
              candidate->retweet1TweetIdRetweet2RetweetTweetId == retweet1TweetIdRetweet2RetweetTweetId
            && candidate->retweet2TweetId == retweet2TweetId
            && candidate->retweet2UserIdFollowerUserId == retweet2UserIdFollowerUserId
        ) {
            if(--candidate->count == 0) {
                if(lastP) {
                    lastP->next = candidate->next;
                } else
                    retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[h] = NULL;
                delete candidate;
            }
            return --(retweet2ProjectedTupleMap->size);
        } else {
            candidate = candidate->next;
        }
    }
    return -1;
}



#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& userIdTweetUserId, int& tweetId, std::string& tweetDate ) __attribute__((always_inline));
static void increaseArraySize(TweetProjectedTupleMap* tweetProjectedTupleEntryArray);
static void ensureSize(TweetProjectedTupleMap* tweetProjectedTupleMap);


__inline static void ensureSize(TweetProjectedTupleMap* tweetProjectedTupleMap) {
    if(tweetProjectedTupleMap->size >= tweetProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(tweetProjectedTupleMap);
    }
}
static void increaseArraySize(TweetProjectedTupleMap* tweetProjectedTupleMap) {
	int newcapacity = tweetProjectedTupleMap->capacity * 32;
	TweetProjectedTupleEntry** newprojectedTupleEntryArray = new TweetProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < tweetProjectedTupleMap->capacity; i++) {
		TweetProjectedTupleEntry* p = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[i];
		if(p) {
			do {
			    int h = 31; 
                hashCode(h
                 , p->userIdTweetUserId 
                 , p->tweetId 
                 , p->tweetDate 
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

__inline static void hashCode(int& h, int& userIdTweetUserId, int& tweetId, std::string& tweetDate ) {
    h ^=  userIdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  tweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  std::hash<std::string>()(tweetDate) + 0x9e3779b9 + (h<<6) + (h>>2);
}
TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap, int& userIdTweetUserId, int& tweetId, std::string& tweetDate ) {
	int h = 31;
	hashCode(h
	, userIdTweetUserId
	, tweetId
	, tweetDate
	); 
	h = h & (tweetProjectedTupleMap->capacity - 1);
	TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h];
	TweetProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->userIdTweetUserId == (userIdTweetUserId)
&& 				candidate->tweetId == (tweetId)
&& 				candidate->tweetDate .compare (tweetDate)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	TweetProjectedTupleEntry* p = new TweetProjectedTupleEntry;
	p->userIdTweetUserId = userIdTweetUserId;
	p->tweetId = tweetId;
	p->tweetDate = tweetDate;
	if(!lastP)
		tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	tweetProjectedTupleMap->size++;
	ensureSize(tweetProjectedTupleMap);
	return p;
}
int deleteTweetProjectedTupleIfZero(TweetProjectedTupleMap* tweetProjectedTupleMap
, int& userIdTweetUserId 
, int& tweetId 
, std::string& tweetDate 
) {
    int h = 31;
    hashCode(h
    , userIdTweetUserId
    , tweetId
    , tweetDate
    );
    h = h & (tweetProjectedTupleMap->capacity - 1);
    TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[h];
    TweetProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
              candidate->userIdTweetUserId == userIdTweetUserId
            && candidate->tweetId == tweetId
            && candidate->tweetDate == tweetDate
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


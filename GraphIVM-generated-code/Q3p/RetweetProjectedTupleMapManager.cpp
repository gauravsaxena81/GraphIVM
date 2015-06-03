#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>
inline static void hashCode(int&h, int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate ) __attribute__((always_inline));
static void increaseArraySize(RetweetProjectedTupleMap* retweetProjectedTupleEntryArray);
static void ensureSize(RetweetProjectedTupleMap* retweetProjectedTupleMap);


__inline static void ensureSize(RetweetProjectedTupleMap* retweetProjectedTupleMap) {
    if(retweetProjectedTupleMap->size >= retweetProjectedTupleMap->capacity * 0.8) {
        increaseArraySize(retweetProjectedTupleMap);
    }
}
static void increaseArraySize(RetweetProjectedTupleMap* retweetProjectedTupleMap) {
	int newcapacity = retweetProjectedTupleMap->capacity * 16;
	RetweetProjectedTupleEntry** newprojectedTupleEntryArray = new RetweetProjectedTupleEntry*[newcapacity]();
	for(int i = 0; i < retweetProjectedTupleMap->capacity; i++) {
		RetweetProjectedTupleEntry* p = retweetProjectedTupleMap->retweetProjectedTupleEntryArray[i];
		if(p) {
			do {
			    int h = 31; 
                hashCode(h
                 , p->retweetTweetId 
                 , p->tweetTweetIdRetweetRetweetTweetId 
                 , p->retweetTweetDate 
                );
                h = h & (newcapacity - 1);
                RetweetProjectedTupleEntry* nextP = p->next;
                p->next = newprojectedTupleEntryArray[h];
                newprojectedTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweetProjectedTupleMap->retweetProjectedTupleEntryArray);
	retweetProjectedTupleMap->retweetProjectedTupleEntryArray = newprojectedTupleEntryArray;
	retweetProjectedTupleMap->capacity = newcapacity;
}

__inline static void hashCode(int& h, int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate ) {
    h ^=  retweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  tweetTweetIdRetweetRetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  std::hash<std::string>()(retweetTweetDate) + 0x9e3779b9 + (h<<6) + (h>>2);
}
RetweetProjectedTupleEntry* putRetweetProjectedTuple(RetweetProjectedTupleMap* retweetProjectedTupleMap, int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate ) {
	int h = 31;
	hashCode(h
	, retweetTweetId
	, tweetTweetIdRetweetRetweetTweetId
	, retweetTweetDate
	); 
	h = h & (retweetProjectedTupleMap->capacity - 1);
	RetweetProjectedTupleEntry* candidate = retweetProjectedTupleMap->retweetProjectedTupleEntryArray[h];
	RetweetProjectedTupleEntry* lastP = NULL;
	while(candidate) {
		if(
				candidate->retweetTweetId == (retweetTweetId)
&& 				candidate->tweetTweetIdRetweetRetweetTweetId == (tweetTweetIdRetweetRetweetTweetId)
&& 				candidate->retweetTweetDate .compare (retweetTweetDate)
		) {
			candidate->count++;
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	RetweetProjectedTupleEntry* p = new RetweetProjectedTupleEntry;
	p->retweetTweetId = retweetTweetId;
	p->tweetTweetIdRetweetRetweetTweetId = tweetTweetIdRetweetRetweetTweetId;
	p->retweetTweetDate = retweetTweetDate;
	if(!lastP)
		retweetProjectedTupleMap->retweetProjectedTupleEntryArray[h] = p;
	else
		lastP->next = p;
	retweetProjectedTupleMap->size++;
	ensureSize(retweetProjectedTupleMap);
	return p;
}
int deleteRetweetProjectedTupleIfZero(RetweetProjectedTupleMap* retweetProjectedTupleMap
, int& retweetTweetId 
, int& tweetTweetIdRetweetRetweetTweetId 
, std::string& retweetTweetDate 
) {
    int h = 31;
    hashCode(h
    , retweetTweetId
    , tweetTweetIdRetweetRetweetTweetId
    , retweetTweetDate
    );
    h = h & (retweetProjectedTupleMap->capacity - 1);
    RetweetProjectedTupleEntry* candidate = retweetProjectedTupleMap->retweetProjectedTupleEntryArray[h];
    RetweetProjectedTupleEntry* lastP = NULL;
    while(candidate) {
        if(
              candidate->retweetTweetId == retweetTweetId
            && candidate->tweetTweetIdRetweetRetweetTweetId == tweetTweetIdRetweetRetweetTweetId
            && candidate->retweetTweetDate == retweetTweetDate
        ) {
            if(--candidate->count == 0) {
                if(lastP) {
                    lastP->next = candidate->next;
                } else
                    retweetProjectedTupleMap->retweetProjectedTupleEntryArray[h] = NULL;
                delete candidate;
            }
            return --(retweetProjectedTupleMap->size);
        } else {
            candidate = candidate->next;
        }
    }
    return -1;
}



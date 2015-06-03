#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r2TweetIdR3RetweetTweetId ) __attribute__((always_inline));
static void ensureSize(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap);
static void increaseArraySize(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap);

__inline static void ensureSize(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap) {
	if(retweet2Retweet3VertexValueMap->size >= retweet2Retweet3VertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet2Retweet3VertexValueMap);
	}
}
static void increaseArraySize(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap) {
	int newcapacity = retweet2Retweet3VertexValueMap->capacity * 2;
	Retweet2Retweet3VertexValueEntry** newVertexValueEntryArray = new Retweet2Retweet3VertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet2Retweet3VertexValueMap->capacity; i++) {
		Retweet2Retweet3VertexValueEntry* p = retweet2Retweet3VertexValueMap->retweet2Retweet3VertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r2TweetIdR3RetweetTweetId );
                h = h & (newcapacity - 1);
                Retweet2Retweet3VertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet2Retweet3VertexValueMap->retweet2Retweet3VertexValueEntryArray);
	retweet2Retweet3VertexValueMap->retweet2Retweet3VertexValueEntryArray = newVertexValueEntryArray;
	retweet2Retweet3VertexValueMap->capacity = newcapacity;
}
Retweet2Retweet3VertexValueEntry* putRetweet2Retweet3VertexValueIfAbsent(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap, int& r2TweetIdR3RetweetTweetId ) {
	int h = 31;
	hashCode(h, r2TweetIdR3RetweetTweetId ); 
	h = h & (retweet2Retweet3VertexValueMap->capacity - 1);
	Retweet2Retweet3VertexValueEntry* candidate = retweet2Retweet3VertexValueMap->retweet2Retweet3VertexValueEntryArray[h];
	Retweet2Retweet3VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r2TweetIdR3RetweetTweetId == (r2TweetIdR3RetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet2Retweet3VertexValueEntry* p = new Retweet2Retweet3VertexValueEntry;
	p->r2TweetIdR3RetweetTweetId = r2TweetIdR3RetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		retweet2Retweet3VertexValueMap->retweet2Retweet3VertexValueEntryArray[h] = p;
	retweet2Retweet3VertexValueMap->size++;
	ensureSize(retweet2Retweet3VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r2TweetIdR3RetweetTweetId ) {
	h ^=  r2TweetIdR3RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}

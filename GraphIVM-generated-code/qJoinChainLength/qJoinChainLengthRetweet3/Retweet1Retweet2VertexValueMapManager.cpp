#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r1TweetIdR2RetweetTweetId ) __attribute__((always_inline));
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
                hashCode(h, p->r1TweetIdR2RetweetTweetId );
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
Retweet1Retweet2VertexValueEntry* putRetweet1Retweet2VertexValueIfAbsent(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, int& r1TweetIdR2RetweetTweetId ) {
	int h = 31;
	hashCode(h, r1TweetIdR2RetweetTweetId ); 
	h = h & (retweet1Retweet2VertexValueMap->capacity - 1);
	Retweet1Retweet2VertexValueEntry* candidate = retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h];
	Retweet1Retweet2VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r1TweetIdR2RetweetTweetId == (r1TweetIdR2RetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet1Retweet2VertexValueEntry* p = new Retweet1Retweet2VertexValueEntry;
	p->r1TweetIdR2RetweetTweetId = r1TweetIdR2RetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		retweet1Retweet2VertexValueMap->retweet1Retweet2VertexValueEntryArray[h] = p;
	retweet1Retweet2VertexValueMap->size++;
	ensureSize(retweet1Retweet2VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r1TweetIdR2RetweetTweetId ) {
	h ^=  r1TweetIdR2RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}

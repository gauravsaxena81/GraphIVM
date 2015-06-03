#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r4TweetIdR5RetweetTweetId ) __attribute__((always_inline));
static void ensureSize(Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap);
static void increaseArraySize(Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap);

__inline static void ensureSize(Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap) {
	if(retweet4Retweet5VertexValueMap->size >= retweet4Retweet5VertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet4Retweet5VertexValueMap);
	}
}
static void increaseArraySize(Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap) {
	int newcapacity = retweet4Retweet5VertexValueMap->capacity * 2;
	Retweet4Retweet5VertexValueEntry** newVertexValueEntryArray = new Retweet4Retweet5VertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet4Retweet5VertexValueMap->capacity; i++) {
		Retweet4Retweet5VertexValueEntry* p = retweet4Retweet5VertexValueMap->retweet4Retweet5VertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r4TweetIdR5RetweetTweetId );
                h = h & (newcapacity - 1);
                Retweet4Retweet5VertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet4Retweet5VertexValueMap->retweet4Retweet5VertexValueEntryArray);
	retweet4Retweet5VertexValueMap->retweet4Retweet5VertexValueEntryArray = newVertexValueEntryArray;
	retweet4Retweet5VertexValueMap->capacity = newcapacity;
}
Retweet4Retweet5VertexValueEntry* putRetweet4Retweet5VertexValueIfAbsent(Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap, int& r4TweetIdR5RetweetTweetId ) {
	int h = 31;
	hashCode(h, r4TweetIdR5RetweetTweetId ); 
	h = h & (retweet4Retweet5VertexValueMap->capacity - 1);
	Retweet4Retweet5VertexValueEntry* candidate = retweet4Retweet5VertexValueMap->retweet4Retweet5VertexValueEntryArray[h];
	Retweet4Retweet5VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r4TweetIdR5RetweetTweetId == (r4TweetIdR5RetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet4Retweet5VertexValueEntry* p = new Retweet4Retweet5VertexValueEntry;
	p->r4TweetIdR5RetweetTweetId = r4TweetIdR5RetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		retweet4Retweet5VertexValueMap->retweet4Retweet5VertexValueEntryArray[h] = p;
	retweet4Retweet5VertexValueMap->size++;
	ensureSize(retweet4Retweet5VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r4TweetIdR5RetweetTweetId ) {
	h ^=  r4TweetIdR5RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}

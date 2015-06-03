#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r3TweetIdR4RetweetTweetId ) __attribute__((always_inline));
static void ensureSize(Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap);
static void increaseArraySize(Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap);

__inline static void ensureSize(Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap) {
	if(retweet3Retweet4VertexValueMap->size >= retweet3Retweet4VertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet3Retweet4VertexValueMap);
	}
}
static void increaseArraySize(Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap) {
	int newcapacity = retweet3Retweet4VertexValueMap->capacity * 2;
	Retweet3Retweet4VertexValueEntry** newVertexValueEntryArray = new Retweet3Retweet4VertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet3Retweet4VertexValueMap->capacity; i++) {
		Retweet3Retweet4VertexValueEntry* p = retweet3Retweet4VertexValueMap->retweet3Retweet4VertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r3TweetIdR4RetweetTweetId );
                h = h & (newcapacity - 1);
                Retweet3Retweet4VertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet3Retweet4VertexValueMap->retweet3Retweet4VertexValueEntryArray);
	retweet3Retweet4VertexValueMap->retweet3Retweet4VertexValueEntryArray = newVertexValueEntryArray;
	retweet3Retweet4VertexValueMap->capacity = newcapacity;
}
Retweet3Retweet4VertexValueEntry* putRetweet3Retweet4VertexValueIfAbsent(Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap, int& r3TweetIdR4RetweetTweetId ) {
	int h = 31;
	hashCode(h, r3TweetIdR4RetweetTweetId ); 
	h = h & (retweet3Retweet4VertexValueMap->capacity - 1);
	Retweet3Retweet4VertexValueEntry* candidate = retweet3Retweet4VertexValueMap->retweet3Retweet4VertexValueEntryArray[h];
	Retweet3Retweet4VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r3TweetIdR4RetweetTweetId == (r3TweetIdR4RetweetTweetId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet3Retweet4VertexValueEntry* p = new Retweet3Retweet4VertexValueEntry;
	p->r3TweetIdR4RetweetTweetId = r3TweetIdR4RetweetTweetId;
	if(lastP)
		lastP->next = p;
	else
		retweet3Retweet4VertexValueMap->retweet3Retweet4VertexValueEntryArray[h] = p;
	retweet3Retweet4VertexValueMap->size++;
	ensureSize(retweet3Retweet4VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r3TweetIdR4RetweetTweetId ) {
	h ^=  r3TweetIdR4RetweetTweetId + 0x9e3779b9 + (h<<6) + (h>>2);
}

#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r5UserIdFUserId ) __attribute__((always_inline));
static void ensureSize(Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap);
static void increaseArraySize(Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap);

__inline static void ensureSize(Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap) {
	if(retweet5FollowerVertexValueMap->size >= retweet5FollowerVertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet5FollowerVertexValueMap);
	}
}
static void increaseArraySize(Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap) {
	int newcapacity = retweet5FollowerVertexValueMap->capacity * 2;
	Retweet5FollowerVertexValueEntry** newVertexValueEntryArray = new Retweet5FollowerVertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet5FollowerVertexValueMap->capacity; i++) {
		Retweet5FollowerVertexValueEntry* p = retweet5FollowerVertexValueMap->retweet5FollowerVertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r5UserIdFUserId );
                h = h & (newcapacity - 1);
                Retweet5FollowerVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet5FollowerVertexValueMap->retweet5FollowerVertexValueEntryArray);
	retweet5FollowerVertexValueMap->retweet5FollowerVertexValueEntryArray = newVertexValueEntryArray;
	retweet5FollowerVertexValueMap->capacity = newcapacity;
}
Retweet5FollowerVertexValueEntry* putRetweet5FollowerVertexValueIfAbsent(Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap, int& r5UserIdFUserId ) {
	int h = 31;
	hashCode(h, r5UserIdFUserId ); 
	h = h & (retweet5FollowerVertexValueMap->capacity - 1);
	Retweet5FollowerVertexValueEntry* candidate = retweet5FollowerVertexValueMap->retweet5FollowerVertexValueEntryArray[h];
	Retweet5FollowerVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r5UserIdFUserId == (r5UserIdFUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet5FollowerVertexValueEntry* p = new Retweet5FollowerVertexValueEntry;
	p->r5UserIdFUserId = r5UserIdFUserId;
	if(lastP)
		lastP->next = p;
	else
		retweet5FollowerVertexValueMap->retweet5FollowerVertexValueEntryArray[h] = p;
	retweet5FollowerVertexValueMap->size++;
	ensureSize(retweet5FollowerVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r5UserIdFUserId ) {
	h ^=  r5UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

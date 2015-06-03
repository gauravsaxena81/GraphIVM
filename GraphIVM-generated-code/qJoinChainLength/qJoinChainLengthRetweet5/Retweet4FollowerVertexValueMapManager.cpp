#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r4UserIdFUserId ) __attribute__((always_inline));
static void ensureSize(Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap);
static void increaseArraySize(Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap);

__inline static void ensureSize(Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap) {
	if(retweet4FollowerVertexValueMap->size >= retweet4FollowerVertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet4FollowerVertexValueMap);
	}
}
static void increaseArraySize(Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap) {
	int newcapacity = retweet4FollowerVertexValueMap->capacity * 2;
	Retweet4FollowerVertexValueEntry** newVertexValueEntryArray = new Retweet4FollowerVertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet4FollowerVertexValueMap->capacity; i++) {
		Retweet4FollowerVertexValueEntry* p = retweet4FollowerVertexValueMap->retweet4FollowerVertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r4UserIdFUserId );
                h = h & (newcapacity - 1);
                Retweet4FollowerVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet4FollowerVertexValueMap->retweet4FollowerVertexValueEntryArray);
	retweet4FollowerVertexValueMap->retweet4FollowerVertexValueEntryArray = newVertexValueEntryArray;
	retweet4FollowerVertexValueMap->capacity = newcapacity;
}
Retweet4FollowerVertexValueEntry* putRetweet4FollowerVertexValueIfAbsent(Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap, int& r4UserIdFUserId ) {
	int h = 31;
	hashCode(h, r4UserIdFUserId ); 
	h = h & (retweet4FollowerVertexValueMap->capacity - 1);
	Retweet4FollowerVertexValueEntry* candidate = retweet4FollowerVertexValueMap->retweet4FollowerVertexValueEntryArray[h];
	Retweet4FollowerVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r4UserIdFUserId == (r4UserIdFUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet4FollowerVertexValueEntry* p = new Retweet4FollowerVertexValueEntry;
	p->r4UserIdFUserId = r4UserIdFUserId;
	if(lastP)
		lastP->next = p;
	else
		retweet4FollowerVertexValueMap->retweet4FollowerVertexValueEntryArray[h] = p;
	retweet4FollowerVertexValueMap->size++;
	ensureSize(retweet4FollowerVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r4UserIdFUserId ) {
	h ^=  r4UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

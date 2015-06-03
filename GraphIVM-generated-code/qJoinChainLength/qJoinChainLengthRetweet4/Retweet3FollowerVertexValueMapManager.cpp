#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r3UserIdFUserId ) __attribute__((always_inline));
static void ensureSize(Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap);
static void increaseArraySize(Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap);

__inline static void ensureSize(Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap) {
	if(retweet3FollowerVertexValueMap->size >= retweet3FollowerVertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet3FollowerVertexValueMap);
	}
}
static void increaseArraySize(Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap) {
	int newcapacity = retweet3FollowerVertexValueMap->capacity * 2;
	Retweet3FollowerVertexValueEntry** newVertexValueEntryArray = new Retweet3FollowerVertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet3FollowerVertexValueMap->capacity; i++) {
		Retweet3FollowerVertexValueEntry* p = retweet3FollowerVertexValueMap->retweet3FollowerVertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r3UserIdFUserId );
                h = h & (newcapacity - 1);
                Retweet3FollowerVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet3FollowerVertexValueMap->retweet3FollowerVertexValueEntryArray);
	retweet3FollowerVertexValueMap->retweet3FollowerVertexValueEntryArray = newVertexValueEntryArray;
	retweet3FollowerVertexValueMap->capacity = newcapacity;
}
Retweet3FollowerVertexValueEntry* putRetweet3FollowerVertexValueIfAbsent(Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap, int& r3UserIdFUserId ) {
	int h = 31;
	hashCode(h, r3UserIdFUserId ); 
	h = h & (retweet3FollowerVertexValueMap->capacity - 1);
	Retweet3FollowerVertexValueEntry* candidate = retweet3FollowerVertexValueMap->retweet3FollowerVertexValueEntryArray[h];
	Retweet3FollowerVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r3UserIdFUserId == (r3UserIdFUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet3FollowerVertexValueEntry* p = new Retweet3FollowerVertexValueEntry;
	p->r3UserIdFUserId = r3UserIdFUserId;
	if(lastP)
		lastP->next = p;
	else
		retweet3FollowerVertexValueMap->retweet3FollowerVertexValueEntryArray[h] = p;
	retweet3FollowerVertexValueMap->size++;
	ensureSize(retweet3FollowerVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r3UserIdFUserId ) {
	h ^=  r3UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

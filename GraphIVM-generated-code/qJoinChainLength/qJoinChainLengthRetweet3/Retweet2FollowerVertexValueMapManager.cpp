#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& r2UserIdFUserId ) __attribute__((always_inline));
static void ensureSize(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap);
static void increaseArraySize(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap);

__inline static void ensureSize(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap) {
	if(retweet2FollowerVertexValueMap->size >= retweet2FollowerVertexValueMap->capacity * 0.8) {
		increaseArraySize(retweet2FollowerVertexValueMap);
	}
}
static void increaseArraySize(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap) {
	int newcapacity = retweet2FollowerVertexValueMap->capacity * 2;
	Retweet2FollowerVertexValueEntry** newVertexValueEntryArray = new Retweet2FollowerVertexValueEntry*[newcapacity]();
	for(int i = 0; i < retweet2FollowerVertexValueMap->capacity; i++) {
		Retweet2FollowerVertexValueEntry* p = retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->r2UserIdFUserId );
                h = h & (newcapacity - 1);
                Retweet2FollowerVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray);
	retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray = newVertexValueEntryArray;
	retweet2FollowerVertexValueMap->capacity = newcapacity;
}
Retweet2FollowerVertexValueEntry* putRetweet2FollowerVertexValueIfAbsent(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, int& r2UserIdFUserId ) {
	int h = 31;
	hashCode(h, r2UserIdFUserId ); 
	h = h & (retweet2FollowerVertexValueMap->capacity - 1);
	Retweet2FollowerVertexValueEntry* candidate = retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h];
	Retweet2FollowerVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->r2UserIdFUserId == (r2UserIdFUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet2FollowerVertexValueEntry* p = new Retweet2FollowerVertexValueEntry;
	p->r2UserIdFUserId = r2UserIdFUserId;
	if(lastP)
		lastP->next = p;
	else
		retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h] = p;
	retweet2FollowerVertexValueMap->size++;
	ensureSize(retweet2FollowerVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& r2UserIdFUserId ) {
	h ^=  r2UserIdFUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

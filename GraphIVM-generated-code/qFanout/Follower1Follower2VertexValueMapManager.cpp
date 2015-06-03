#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& user1IdFollower2Id ) __attribute__((always_inline));
static void ensureSize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);
static void increaseArraySize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);

__inline static void ensureSize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
	if(follower1Follower2VertexValueMap->size >= follower1Follower2VertexValueMap->capacity * 0.8) {
		increaseArraySize(follower1Follower2VertexValueMap);
	}
}
static void increaseArraySize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
	int newcapacity = follower1Follower2VertexValueMap->capacity * 2;
	Follower1Follower2VertexValueEntry** newVertexValueEntryArray = new Follower1Follower2VertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower1Follower2VertexValueMap->capacity; i++) {
		Follower1Follower2VertexValueEntry* p = follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->user1IdFollower2Id );
                h = h & (newcapacity - 1);
                Follower1Follower2VertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray);
	follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray = newVertexValueEntryArray;
	follower1Follower2VertexValueMap->capacity = newcapacity;
}
Follower1Follower2VertexValueEntry* putFollower1Follower2VertexValueIfAbsent(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, int& user1IdFollower2Id ) {
	int h = 31;
	hashCode(h, user1IdFollower2Id ); 
	h = h & (follower1Follower2VertexValueMap->capacity - 1);
	Follower1Follower2VertexValueEntry* candidate = follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray[h];
	Follower1Follower2VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->user1IdFollower2Id == (user1IdFollower2Id)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Follower1Follower2VertexValueEntry* p = new Follower1Follower2VertexValueEntry;
	p->user1IdFollower2Id = user1IdFollower2Id;
	if(lastP)
		lastP->next = p;
	else
		follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray[h] = p;
	follower1Follower2VertexValueMap->size++;
	ensureSize(follower1Follower2VertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& user1IdFollower2Id ) {
	h ^=  user1IdFollower2Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

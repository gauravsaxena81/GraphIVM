#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower4IdTweetUserId ) __attribute__((always_inline));
static void ensureSize(Follower4TweetVertexValueMap* follower4TweetVertexValueMap);
static void increaseArraySize(Follower4TweetVertexValueMap* follower4TweetVertexValueMap);

__inline static void ensureSize(Follower4TweetVertexValueMap* follower4TweetVertexValueMap) {
	if(follower4TweetVertexValueMap->size >= follower4TweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(follower4TweetVertexValueMap);
	}
}
static void increaseArraySize(Follower4TweetVertexValueMap* follower4TweetVertexValueMap) {
	int newcapacity = follower4TweetVertexValueMap->capacity * 2;
	Follower4TweetVertexValueEntry** newVertexValueEntryArray = new Follower4TweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower4TweetVertexValueMap->capacity; i++) {
		Follower4TweetVertexValueEntry* p = follower4TweetVertexValueMap->follower4TweetVertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower4IdTweetUserId );
			h = h & (newcapacity - 1);
			do {
                Follower4TweetVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(follower4TweetVertexValueMap->follower4TweetVertexValueEntryArray);
	follower4TweetVertexValueMap->follower4TweetVertexValueEntryArray = newVertexValueEntryArray;
	follower4TweetVertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower4TweetVertexValueEntry* putFollower4TweetVertexValueIfAbsent(Follower4TweetVertexValueMap* follower4TweetVertexValueMap, int& follower4IdTweetUserId ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower4IdTweetUserId ); 
	h = h & (follower4TweetVertexValueMap->capacity - 1);
	Follower4TweetVertexValueEntry* candidate = follower4TweetVertexValueMap->follower4TweetVertexValueEntryArray[h];
	Follower4TweetVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower4IdTweetUserId == (follower4IdTweetUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Follower4TweetVertexValueEntry* p = new Follower4TweetVertexValueEntry;
	p->follower4IdTweetUserId = follower4IdTweetUserId;
	if(lastP)
		lastP->next = p;
	else
		follower4TweetVertexValueMap->follower4TweetVertexValueEntryArray[h] = p;
	follower4TweetVertexValueMap->size++;
	ensureSize(follower4TweetVertexValueMap);
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower4IdTweetUserId ) {
	h ^=  follower4IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

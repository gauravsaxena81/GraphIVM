#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower3IdTweetUserId ) __attribute__((always_inline));
static void ensureSize(Follower3TweetVertexValueMap* follower3TweetVertexValueMap);
static void increaseArraySize(Follower3TweetVertexValueMap* follower3TweetVertexValueMap);

__inline static void ensureSize(Follower3TweetVertexValueMap* follower3TweetVertexValueMap) {
	if(follower3TweetVertexValueMap->size >= follower3TweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(follower3TweetVertexValueMap);
	}
}
static void increaseArraySize(Follower3TweetVertexValueMap* follower3TweetVertexValueMap) {
	int newcapacity = follower3TweetVertexValueMap->capacity * 2;
	Follower3TweetVertexValueEntry** newVertexValueEntryArray = new Follower3TweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower3TweetVertexValueMap->capacity; i++) {
		Follower3TweetVertexValueEntry* p = follower3TweetVertexValueMap->follower3TweetVertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower3IdTweetUserId );
			h = h & (newcapacity - 1);
			do {
                Follower3TweetVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(follower3TweetVertexValueMap->follower3TweetVertexValueEntryArray);
	follower3TweetVertexValueMap->follower3TweetVertexValueEntryArray = newVertexValueEntryArray;
	follower3TweetVertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower3TweetVertexValueEntry* putFollower3TweetVertexValueIfAbsent(Follower3TweetVertexValueMap* follower3TweetVertexValueMap, int& follower3IdTweetUserId ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower3IdTweetUserId ); 
	h = h & (follower3TweetVertexValueMap->capacity - 1);
	Follower3TweetVertexValueEntry* candidate = follower3TweetVertexValueMap->follower3TweetVertexValueEntryArray[h];
	Follower3TweetVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower3IdTweetUserId == (follower3IdTweetUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Follower3TweetVertexValueEntry* p = new Follower3TweetVertexValueEntry;
	p->follower3IdTweetUserId = follower3IdTweetUserId;
	if(lastP)
		lastP->next = p;
	else
		follower3TweetVertexValueMap->follower3TweetVertexValueEntryArray[h] = p;
	follower3TweetVertexValueMap->size++;
	ensureSize(follower3TweetVertexValueMap);
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower3IdTweetUserId ) {
	h ^=  follower3IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

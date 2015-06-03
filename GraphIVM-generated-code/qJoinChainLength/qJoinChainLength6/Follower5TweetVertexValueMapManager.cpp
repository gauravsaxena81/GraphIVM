#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower5IdTweetUserId ) __attribute__((always_inline));
static void ensureSize(Follower5TweetVertexValueMap* follower5TweetVertexValueMap);
static void increaseArraySize(Follower5TweetVertexValueMap* follower5TweetVertexValueMap);

__inline static void ensureSize(Follower5TweetVertexValueMap* follower5TweetVertexValueMap) {
	if(follower5TweetVertexValueMap->size >= follower5TweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(follower5TweetVertexValueMap);
	}
}
static void increaseArraySize(Follower5TweetVertexValueMap* follower5TweetVertexValueMap) {
	int newcapacity = follower5TweetVertexValueMap->capacity * 2;
	Follower5TweetVertexValueEntry** newFollower5TweetVertexValueEntryArray = new Follower5TweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower5TweetVertexValueMap->capacity; i++) {
		Follower5TweetVertexValueEntry* p = follower5TweetVertexValueMap->follower5TweetVertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower5IdTweetUserId );
			h = h & (newcapacity - 1);
			newFollower5TweetVertexValueEntryArray[h] = p;
		}
	}
	delete(follower5TweetVertexValueMap->follower5TweetVertexValueEntryArray);
	follower5TweetVertexValueMap->follower5TweetVertexValueEntryArray = newFollower5TweetVertexValueEntryArray;
	follower5TweetVertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower5TweetVertexValueEntry* putFollower5TweetVertexValueIfAbsent(Follower5TweetVertexValueMap* follower5TweetVertexValueMap, int& follower5IdTweetUserId ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower5IdTweetUserId ); 
	h = h & (follower5TweetVertexValueMap->capacity - 1);
	Follower5TweetVertexValueEntry* candidate = follower5TweetVertexValueMap->follower5TweetVertexValueEntryArray[h];
	Follower5TweetVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower5IdTweetUserId == (follower5IdTweetUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	ensureSize(follower5TweetVertexValueMap);
	Follower5TweetVertexValueEntry* p = new Follower5TweetVertexValueEntry;
	p->follower5IdTweetUserId = follower5IdTweetUserId;
	if(lastP)
		lastP->next = p;
	else
		follower5TweetVertexValueMap->follower5TweetVertexValueEntryArray[h] = p;
	follower5TweetVertexValueMap->size++;
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower5IdTweetUserId ) {
	h ^=  follower5IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

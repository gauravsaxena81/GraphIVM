#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower2IdTweetUserId ) __attribute__((always_inline));
static void ensureSize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap);
static void increaseArraySize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap);

__inline static void ensureSize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
	if(follower2TweetVertexValueMap->size >= follower2TweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(follower2TweetVertexValueMap);
	}
}
static void increaseArraySize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
	int newcapacity = follower2TweetVertexValueMap->capacity * 2;
	Follower2TweetVertexValueEntry** newFollower2TweetVertexValueEntryArray = new Follower2TweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower2TweetVertexValueMap->capacity; i++) {
		Follower2TweetVertexValueEntry* p = follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower2IdTweetUserId );
			h = h & (newcapacity - 1);
			newFollower2TweetVertexValueEntryArray[h] = p;
		}
	}
	delete(follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray);
	follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray = newFollower2TweetVertexValueEntryArray;
	follower2TweetVertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower2TweetVertexValueEntry* putFollower2TweetVertexValueIfAbsent(Follower2TweetVertexValueMap* follower2TweetVertexValueMap, int& follower2IdTweetUserId ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower2IdTweetUserId ); 
	h = h & (follower2TweetVertexValueMap->capacity - 1);
	Follower2TweetVertexValueEntry* candidate = follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray[h];
	Follower2TweetVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower2IdTweetUserId == (follower2IdTweetUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	ensureSize(follower2TweetVertexValueMap);
	Follower2TweetVertexValueEntry* p = new Follower2TweetVertexValueEntry;
	p->follower2IdTweetUserId = follower2IdTweetUserId;
	if(lastP)
		lastP->next = p;
	else
		follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray[h] = p;
	follower2TweetVertexValueMap->size++;
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower2IdTweetUserId ) {
	h ^=  follower2IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

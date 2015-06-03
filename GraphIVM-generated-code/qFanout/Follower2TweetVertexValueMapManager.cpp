#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& user2IdTweetUserId ) __attribute__((always_inline));
static void ensureSize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap);
static void increaseArraySize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap);

__inline static void ensureSize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
	if(follower2TweetVertexValueMap->size >= follower2TweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(follower2TweetVertexValueMap);
	}
}
static void increaseArraySize(Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
	int newcapacity = follower2TweetVertexValueMap->capacity * 2;
	Follower2TweetVertexValueEntry** newVertexValueEntryArray = new Follower2TweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower2TweetVertexValueMap->capacity; i++) {
		Follower2TweetVertexValueEntry* p = follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray[i];
		if(p) {
			do {
    			int h = 31;
                hashCode(h, p->user2IdTweetUserId );
                h = h & (newcapacity - 1);
                Follower2TweetVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray);
	follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray = newVertexValueEntryArray;
	follower2TweetVertexValueMap->capacity = newcapacity;
}
Follower2TweetVertexValueEntry* putFollower2TweetVertexValueIfAbsent(Follower2TweetVertexValueMap* follower2TweetVertexValueMap, int& user2IdTweetUserId ) {
	int h = 31;
	hashCode(h, user2IdTweetUserId ); 
	h = h & (follower2TweetVertexValueMap->capacity - 1);
	Follower2TweetVertexValueEntry* candidate = follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray[h];
	Follower2TweetVertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->user2IdTweetUserId == (user2IdTweetUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Follower2TweetVertexValueEntry* p = new Follower2TweetVertexValueEntry;
	p->user2IdTweetUserId = user2IdTweetUserId;
	if(lastP)
		lastP->next = p;
	else
		follower2TweetVertexValueMap->follower2TweetVertexValueEntryArray[h] = p;
	follower2TweetVertexValueMap->size++;
	ensureSize(follower2TweetVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& user2IdTweetUserId ) {
	h ^=  user2IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}

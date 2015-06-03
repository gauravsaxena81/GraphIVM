#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>

inline static void hashCode(int& h, int& retweet2UserIdFollowerUserId ) __attribute__((always_inline));
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
                hashCode(h, p->retweet2UserIdFollowerUserId );
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
Retweet2FollowerVertexValueEntry* putRetweet2FollowerVertexValueIfAbsent(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, int& retweet2UserIdFollowerUserId ) {
	int h = 31;
	hashCode(h, retweet2UserIdFollowerUserId ); 
	h = h & (retweet2FollowerVertexValueMap->capacity - 1);
	Retweet2FollowerVertexValueEntry* candidate = retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h];
	Retweet2FollowerVertexValueEntry* lastP = NULL;
	while(candidate) {
		if(
		  candidate->retweet2UserIdFollowerUserId == (retweet2UserIdFollowerUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	Retweet2FollowerVertexValueEntry* p = new Retweet2FollowerVertexValueEntry;
	p->retweet2UserIdFollowerUserId = retweet2UserIdFollowerUserId;
	if(lastP)
		lastP->next = p;
	else
		retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h] = p;
	retweet2FollowerVertexValueMap->size++;
	ensureSize(retweet2FollowerVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& retweet2UserIdFollowerUserId ) {
	h ^=  retweet2UserIdFollowerUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}
void deleteRetweet2FollowerVertexValueIfZero(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, Retweet2JoinTupleEntry* retweet2JoinTuple
, int& retweet2UserIdFollowerUserId
) {
    std::vector<Retweet2JoinTupleEntry*> *vec = &(retweet2JoinTuple->retweet2FollowerVertexValue->retweet2JoinTupleList);
    Retweet2FollowerVertexValueEntry* entry = retweet2JoinTuple->retweet2FollowerVertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), retweet2JoinTuple));
    if(vec->size() == 0 && entry->followerJoinTupleList.size() == 0) {
        if(entry->next) {
            Retweet2FollowerVertexValueEntry* next = entry->next;
            entry->retweet2UserIdFollowerUserId = next->retweet2UserIdFollowerUserId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->retweet2UserIdFollowerUserId 
            );
            h = h & (retweet2FollowerVertexValueMap->capacity - 1);
            delete retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h];
            retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h] = NULL;
        }
        retweet2FollowerVertexValueMap->size--;
    }
}
void deleteRetweet2FollowerVertexValueIfZero(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, FollowerJoinTupleEntry* followerJoinTuple
, int& retweet2UserIdFollowerUserId
) {
    std::vector<FollowerJoinTupleEntry*> *vec = &(followerJoinTuple->retweet2FollowerVertexValue->followerJoinTupleList);
    Retweet2FollowerVertexValueEntry* entry = followerJoinTuple->retweet2FollowerVertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), followerJoinTuple));
    if(vec->size() == 0 && entry->retweet2JoinTupleList.size() == 0) {
        if(entry->next) {
            Retweet2FollowerVertexValueEntry* next = entry->next;
            entry->retweet2UserIdFollowerUserId = next->retweet2UserIdFollowerUserId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->retweet2UserIdFollowerUserId 
            );
            h = h & (retweet2FollowerVertexValueMap->capacity - 1);
            delete retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h];
            retweet2FollowerVertexValueMap->retweet2FollowerVertexValueEntryArray[h] = NULL;
        }
        retweet2FollowerVertexValueMap->size--;
    }
}

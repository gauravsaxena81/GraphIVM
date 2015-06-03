#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <algorithm>
inline static void hashCode(int& h, int& userIdTweetUserId ) __attribute__((always_inline));
inline static void ensureSize(UserTweetVertexValueMap* userTweetVertexValueMap) __attribute__((always_inline));
static void increaseArraySize(UserTweetVertexValueMap* userTweetVertexValueMap);

__inline static void ensureSize(UserTweetVertexValueMap* userTweetVertexValueMap) {
	if(userTweetVertexValueMap->size >= userTweetVertexValueMap->capacity * 0.8) {
		increaseArraySize(userTweetVertexValueMap);
	}
}
static void increaseArraySize(UserTweetVertexValueMap* userTweetVertexValueMap) {
	int newcapacity = userTweetVertexValueMap->capacity * 2;
	UserTweetVertexValueEntry** newVertexValueEntryArray = new UserTweetVertexValueEntry*[newcapacity]();
	for(int i = 0; i < userTweetVertexValueMap->capacity; i++) {
		UserTweetVertexValueEntry* p = userTweetVertexValueMap->userTweetVertexValueEntryArray[i];
		if(p) {
			do {
			int h = 31;
            hashCode(h, p->userIdTweetUserId );
            h = h & (newcapacity - 1);
                UserTweetVertexValueEntry* nextP = p->next;
                p->next = newVertexValueEntryArray[h];
                newVertexValueEntryArray[h] = p;
                p = nextP;
            } while (p);
		}
	}
	delete(userTweetVertexValueMap->userTweetVertexValueEntryArray);
	userTweetVertexValueMap->userTweetVertexValueEntryArray = newVertexValueEntryArray;
	userTweetVertexValueMap->capacity = newcapacity;
}
UserTweetVertexValueEntry* putUserTweetVertexValueIfAbsent(UserTweetVertexValueMap* userTweetVertexValueMap, int& userIdTweetUserId ) {
	int h = 31;
	hashCode(h, userIdTweetUserId ); 
	h = h & (userTweetVertexValueMap->capacity - 1);
	UserTweetVertexValueEntry* candidate = userTweetVertexValueMap->userTweetVertexValueEntryArray[h];
	UserTweetVertexValueEntry* lastP = NULL;
	while(candidate) {
		if(
		  candidate->userIdTweetUserId == (userIdTweetUserId)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	UserTweetVertexValueEntry* p = new UserTweetVertexValueEntry;
	p->userIdTweetUserId = userIdTweetUserId;
	if(lastP)
		lastP->next = p;
	else
		userTweetVertexValueMap->userTweetVertexValueEntryArray[h] = p;
	userTweetVertexValueMap->size++;
	ensureSize(userTweetVertexValueMap);
	return p;
}
__inline static void hashCode(int& h, int& userIdTweetUserId ) {
	//h ^=  userIdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
	h ^= userIdTweetUserId ^  (h >> 20) ^ (h >> 12) ^ (h >> 7) ^ (h >> 4);

}
void deleteUserTweetVertexValueIfZero(UserTweetVertexValueMap* userTweetVertexValueMap, UserJoinTupleEntry* userJoinTuple
, int& userIdTweetUserId
) {
    std::vector<UserJoinTupleEntry*> *vec = &(userJoinTuple->userTweetVertexValue->userJoinTupleList);
    UserTweetVertexValueEntry* entry = userJoinTuple->userTweetVertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), userJoinTuple));
    if(vec->size() == 0 && entry->tweetJoinTupleList.size() == 0) {
        if(entry->next) {
            UserTweetVertexValueEntry* next = entry->next;
            entry->userIdTweetUserId = next->userIdTweetUserId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->userIdTweetUserId 
            );
            h = h & (userTweetVertexValueMap->capacity - 1);
            delete userTweetVertexValueMap->userTweetVertexValueEntryArray[h];
            userTweetVertexValueMap->userTweetVertexValueEntryArray[h] = NULL;
        }
        userTweetVertexValueMap->size--;
    }
}
void deleteUserTweetVertexValueIfZero(UserTweetVertexValueMap* userTweetVertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& userIdTweetUserId
) {
    std::vector<TweetJoinTupleEntry*> *vec = &(tweetJoinTuple->userTweetVertexValue->tweetJoinTupleList);
    UserTweetVertexValueEntry* entry = tweetJoinTuple->userTweetVertexValue;
    vec->erase(std::find(vec->begin(), vec->end(), tweetJoinTuple));
    if(vec->size() == 0 && entry->userJoinTupleList.size() == 0) {
        if(entry->next) {
            UserTweetVertexValueEntry* next = entry->next;
            entry->userIdTweetUserId = next->userIdTweetUserId;
            entry->next = next->next;
            delete next;
        } else {
            int h = 31;
            hashCode(h
            , entry->userIdTweetUserId 
            );
            h = h & (userTweetVertexValueMap->capacity - 1);
            delete userTweetVertexValueMap->userTweetVertexValueEntryArray[h];
            userTweetVertexValueMap->userTweetVertexValueEntryArray[h] = NULL;
        }
        userTweetVertexValueMap->size--;
    }
}

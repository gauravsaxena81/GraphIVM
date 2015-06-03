#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& user2IdTweetUserId) __attribute__((always_inline));
static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap, int& user2IdTweetUserId);
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap, int& user2IdTweetUserId);
static void addToVertexValues(int& user2IdTweetUserId , TweetJoinTupleEntry* p, Follower2TweetVertexValueMap* follower2TweetVertexValueMap);



__inline static void ensureSize(TweetJoinTupleMap* tweetJoinTupleMap, int& user2IdTweetUserId) {
    if(tweetJoinTupleMap->size >= tweetJoinTupleMap->capacity * 0.8) {
        increaseArraySize(tweetJoinTupleMap, user2IdTweetUserId );
    }
}
static void increaseArraySize(TweetJoinTupleMap* tweetJoinTupleMap, int& user2IdTweetUserId) {
    int newcapacity = tweetJoinTupleMap->capacity * 2;
    TweetJoinTupleEntry** newJoinTupleEntryArray = new TweetJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < tweetJoinTupleMap->capacity; i++) {
        TweetJoinTupleEntry* p = tweetJoinTupleMap->tweetJoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, user2IdTweetUserId );
                h = h & (newcapacity - 1);
                TweetJoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete tweetJoinTupleMap->tweetJoinTupleEntryArray;
    tweetJoinTupleMap->tweetJoinTupleEntryArray = newJoinTupleEntryArray;
    tweetJoinTupleMap->capacity = newcapacity;
}
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& user2IdTweetUserId, Follower2TweetVertexValueMap* follower2TweetVertexValueMap ) {
    int h = 31;
    hashCode(h, user2IdTweetUserId ); 
    h = h & (tweetJoinTupleMap->capacity - 1);
    TweetJoinTupleEntry* candidate = tweetJoinTupleMap->tweetJoinTupleEntryArray[h];
    TweetJoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower2TweetVertexValue->user2IdTweetUserId == user2IdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    TweetJoinTupleEntry* p = new TweetJoinTupleEntry;
    if(!lastP)
        tweetJoinTupleMap->tweetJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    tweetJoinTupleMap->size++;
    ensureSize(tweetJoinTupleMap, user2IdTweetUserId );
    addToVertexValues(user2IdTweetUserId , p, follower2TweetVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& user2IdTweetUserId , TweetJoinTupleEntry* p, Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
    Follower2TweetVertexValueEntry* vertexValue = putFollower2TweetVertexValueIfAbsent(follower2TweetVertexValueMap, user2IdTweetUserId );
    p->follower2TweetVertexValue = vertexValue;
    vertexValue->tweetJoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& user2IdTweetUserId) {
    h ^=  user2IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



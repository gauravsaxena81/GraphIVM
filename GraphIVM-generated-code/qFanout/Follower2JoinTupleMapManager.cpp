#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& user1IdFollower2Id,int& user2IdTweetUserId) __attribute__((always_inline));
static void ensureSize(Follower2JoinTupleMap* follower2JoinTupleMap, int& user1IdFollower2Id,int& user2IdTweetUserId);
static void increaseArraySize(Follower2JoinTupleMap* follower2JoinTupleMap, int& user1IdFollower2Id,int& user2IdTweetUserId);
static void addToVertexValues(int& user1IdFollower2Id , Follower2JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);
static void addToVertexValues(int& user2IdTweetUserId , Follower2JoinTupleEntry* p, Follower2TweetVertexValueMap* follower2TweetVertexValueMap);



__inline static void ensureSize(Follower2JoinTupleMap* follower2JoinTupleMap, int& user1IdFollower2Id,int& user2IdTweetUserId) {
    if(follower2JoinTupleMap->size >= follower2JoinTupleMap->capacity * 0.8) {
        increaseArraySize(follower2JoinTupleMap, user1IdFollower2Id, user2IdTweetUserId );
    }
}
static void increaseArraySize(Follower2JoinTupleMap* follower2JoinTupleMap, int& user1IdFollower2Id,int& user2IdTweetUserId) {
    int newcapacity = follower2JoinTupleMap->capacity * 2;
    Follower2JoinTupleEntry** newJoinTupleEntryArray = new Follower2JoinTupleEntry*[newcapacity]();
    for(int i = 0; i < follower2JoinTupleMap->capacity; i++) {
        Follower2JoinTupleEntry* p = follower2JoinTupleMap->follower2JoinTupleEntryArray[i];
        if(p) {
            do {
                int h = 31;
                hashCode(h, user1IdFollower2Id, user2IdTweetUserId );
                h = h & (newcapacity - 1);
                Follower2JoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete follower2JoinTupleMap->follower2JoinTupleEntryArray;
    follower2JoinTupleMap->follower2JoinTupleEntryArray = newJoinTupleEntryArray;
    follower2JoinTupleMap->capacity = newcapacity;
}
Follower2JoinTupleEntry* putFollower2JoinTupleIfAbsent(Follower2JoinTupleMap* follower2JoinTupleMap, int& user1IdFollower2Id,int& user2IdTweetUserId, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, Follower2TweetVertexValueMap* follower2TweetVertexValueMap ) {
    int h = 31;
    hashCode(h, user1IdFollower2Id, user2IdTweetUserId ); 
    h = h & (follower2JoinTupleMap->capacity - 1);
    Follower2JoinTupleEntry* candidate = follower2JoinTupleMap->follower2JoinTupleEntryArray[h];
    Follower2JoinTupleEntry* lastP = candidate;
    while(candidate) {
        if(
                candidate->follower1Follower2VertexValue->user1IdFollower2Id == user1IdFollower2Id
        &&         candidate->follower2TweetVertexValue->user2IdTweetUserId == user2IdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    Follower2JoinTupleEntry* p = new Follower2JoinTupleEntry;
    if(!lastP)
        follower2JoinTupleMap->follower2JoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    follower2JoinTupleMap->size++;
    ensureSize(follower2JoinTupleMap, user1IdFollower2Id, user2IdTweetUserId );
    addToVertexValues(user1IdFollower2Id , p, follower1Follower2VertexValueMap);
    addToVertexValues(user2IdTweetUserId , p, follower2TweetVertexValueMap);
    return p;
}
__inline static void addToVertexValues(int& user1IdFollower2Id , Follower2JoinTupleEntry* p, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
    Follower1Follower2VertexValueEntry* vertexValue = putFollower1Follower2VertexValueIfAbsent(follower1Follower2VertexValueMap, user1IdFollower2Id );
    p->follower1Follower2VertexValue = vertexValue;
    vertexValue->follower2JoinTupleList.push_back(p);
}
__inline static void addToVertexValues(int& user2IdTweetUserId , Follower2JoinTupleEntry* p, Follower2TweetVertexValueMap* follower2TweetVertexValueMap) {
    Follower2TweetVertexValueEntry* vertexValue = putFollower2TweetVertexValueIfAbsent(follower2TweetVertexValueMap, user2IdTweetUserId );
    p->follower2TweetVertexValue = vertexValue;
    vertexValue->follower2JoinTupleList.push_back(p);
}
inline static void hashCode(int&h, int& user1IdFollower2Id,int& user2IdTweetUserId) {
    h ^=  user1IdFollower2Id + 0x9e3779b9 + (h<<6) + (h>>2);
    h ^=  user2IdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



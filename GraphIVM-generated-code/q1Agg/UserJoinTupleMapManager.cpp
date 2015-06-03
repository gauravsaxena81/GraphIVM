#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

inline static void hashCode(int& h, int& userIdTweetUserId) __attribute__((always_inline));
static void ensureSize(UserJoinTupleMap* userJoinTupleMap);
static void increaseArraySize(UserJoinTupleMap* userJoinTupleMap);
static void addToVertexValues(int& userIdTweetUserId , UserJoinTupleEntry* p, UserTweetVertexValueMap* userTweetVertexValueMap);



__inline static void ensureSize(UserJoinTupleMap* userJoinTupleMap) {
    if(userJoinTupleMap->size >= userJoinTupleMap->capacity * 0.8) {
        increaseArraySize(userJoinTupleMap);
    }
}
static void increaseArraySize(UserJoinTupleMap* userJoinTupleMap) {
    int newcapacity = userJoinTupleMap->capacity * 2;
    UserJoinTupleEntry** newJoinTupleEntryArray = new UserJoinTupleEntry*[newcapacity]();
    for(int i = 0; i < userJoinTupleMap->capacity; i++) {
        UserJoinTupleEntry* p = userJoinTupleMap->userJoinTupleEntryArray[i];
        if(p) {
            do {
            int h = 31;
            hashCode(h
            , p->userTweetVertexValue->userIdTweetUserId
            );
            h = h & (newcapacity - 1);
                UserJoinTupleEntry* nextP = p->next;
                p->next = newJoinTupleEntryArray[h];
                newJoinTupleEntryArray[h] = p;
                p = nextP;
            } while (p);
        }
    }
    delete userJoinTupleMap->userJoinTupleEntryArray;
    userJoinTupleMap->userJoinTupleEntryArray = newJoinTupleEntryArray;
    userJoinTupleMap->capacity = newcapacity;
}
UserJoinTupleEntry* putUserJoinTupleIfAbsent(UserJoinTupleMap* userJoinTupleMap, int& userIdTweetUserId, UserTweetVertexValueMap* userTweetVertexValueMap ) {
    int h = 31;
    hashCode(h, userIdTweetUserId ); 
    h = h & (userJoinTupleMap->capacity - 1);
    UserJoinTupleEntry* candidate = userJoinTupleMap->userJoinTupleEntryArray[h];
    UserJoinTupleEntry* lastP = NULL;
    while(candidate) {
        if(
                candidate->userTweetVertexValue->userIdTweetUserId == userIdTweetUserId
        ) {
            return candidate;
        } else {
            lastP = candidate;
            candidate = candidate->next;
        }
    }
    UserJoinTupleEntry* p = new UserJoinTupleEntry;
    addToVertexValues(userIdTweetUserId , p, userTweetVertexValueMap);
    if(!lastP)
        userJoinTupleMap->userJoinTupleEntryArray[h] = p;
    else
        lastP->next = p;
    userJoinTupleMap->size++;
    ensureSize(userJoinTupleMap);
    return p;
}
__inline static void addToVertexValues(int& userIdTweetUserId , UserJoinTupleEntry* p, UserTweetVertexValueMap* userTweetVertexValueMap) {
    UserTweetVertexValueEntry* vertexValue = putUserTweetVertexValueIfAbsent(userTweetVertexValueMap, userIdTweetUserId );
    p->userTweetVertexValue = vertexValue;
    vertexValue->userJoinTupleList.push_back(p);
}

inline static void hashCode(int&h, int& userIdTweetUserId) {
    h ^=  userIdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}



UserJoinTupleEntry** getUserJoinTuplePointer(UserJoinTupleMap* userJoinTupleMap
, int& userIdTweetUserId
)  {
    int h = 31;
    hashCode(h
    , userIdTweetUserId  
    );
    h = h & (userJoinTupleMap->capacity - 1);
    UserJoinTupleEntry* candidate = userJoinTupleMap->userJoinTupleEntryArray[h];
    UserJoinTupleEntry** address = &(userJoinTupleMap->userJoinTupleEntryArray[h]);
    while(candidate) {
        if(
              candidate->userTweetVertexValue->userIdTweetUserId == userIdTweetUserId
        ) {
            return address;
        } else {
            address = &(candidate->next);
            candidate = candidate->next;
        }
    }
    return NULL;
}
void removeJoinTupleEntry(UserJoinTupleMap* userJoinTupleMap, UserJoinTupleEntry** entryPointer) {
    if((*entryPointer)->next) {
        (*entryPointer)->next = (*entryPointer)->next->next;
    } else {
        delete *entryPointer;
        (*entryPointer) = NULL;
    }
    userJoinTupleMap->size--;
}

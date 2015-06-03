#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct UserTweetVertexValueMap;
struct UserTweetVertexValueEntry;
struct UserProjectedTupleMap;
struct TweetProjectedTupleMap;
struct UserProjectedTupleEntry;
struct TweetProjectedTupleEntry;
struct UserJoinTupleMap;
struct TweetJoinTupleMap;
struct UserJoinTupleEntry;
struct TweetJoinTupleEntry;



struct UserTweetVertexValueEntry {
    std::vector<UserJoinTupleEntry*> userJoinTupleList;
    std::vector<TweetJoinTupleEntry*> tweetJoinTupleList;
    UserTweetVertexValueEntry* next = NULL;
    int userIdTweetUserId;
};
struct UserTweetVertexValueMap {
    int capacity = 65536;
    int size = 0;
    UserTweetVertexValueEntry** userTweetVertexValueEntryArray = new UserTweetVertexValueEntry*[capacity]();
};

struct UserProjectedTupleMap {
    int capacity = 2;
    int size = 0;
    UserProjectedTupleEntry** userProjectedTupleEntryArray = new UserProjectedTupleEntry*[capacity]();
};
struct UserProjectedTupleEntry {
    UserProjectedTupleEntry* next = NULL;
    int userIdTweetUserId;
    int count = 1;
};

struct UserJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    UserJoinTupleEntry** userJoinTupleEntryArray = new UserJoinTupleEntry*[capacity]();
};

struct UserJoinTupleEntry {
    UserTweetVertexValueEntry* userTweetVertexValue = NULL;
    UserProjectedTupleMap* userProjectedTupleMap = new UserProjectedTupleMap;
    UserJoinTupleEntry* next = NULL;
};

struct TweetProjectedTupleMap {
    int capacity = 1;
    int size = 0;
    TweetProjectedTupleEntry** tweetProjectedTupleEntryArray = new TweetProjectedTupleEntry*[capacity]();
};
struct TweetProjectedTupleEntry {
    TweetProjectedTupleEntry* next = NULL;
    int count = 1;
};

struct TweetJoinTupleMap {
    int capacity = 2097152;
    int size = 0;
    TweetJoinTupleEntry** tweetJoinTupleEntryArray = new TweetJoinTupleEntry*[capacity]();
};

struct TweetJoinTupleEntry {
    UserTweetVertexValueEntry* userTweetVertexValue = NULL;
    TweetProjectedTupleMap* tweetProjectedTupleMap = new TweetProjectedTupleMap;
    TweetJoinTupleEntry* next = NULL;
};


#endif /* DATASTRUCTURES_HPP_ */


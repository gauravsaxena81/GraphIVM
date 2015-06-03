#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct UserTweetVertexValueMap;
struct TweetRetweetVertexValueMap;
struct UserTweetVertexValueEntry;
struct TweetRetweetVertexValueEntry;
struct UserProjectedTupleMap;
struct TweetProjectedTupleMap;
struct RetweetProjectedTupleMap;
struct UserProjectedTupleEntry;
struct TweetProjectedTupleEntry;
struct RetweetProjectedTupleEntry;
struct UserJoinTupleMap;
struct TweetJoinTupleMap;
struct RetweetJoinTupleMap;
struct UserJoinTupleEntry;
struct TweetJoinTupleEntry;
struct RetweetJoinTupleEntry;




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



struct TweetRetweetVertexValueEntry {
    std::vector<TweetJoinTupleEntry*> tweetJoinTupleList;
    std::vector<RetweetJoinTupleEntry*> retweetJoinTupleList;
    TweetRetweetVertexValueEntry* next = NULL;
    int tweetTweetIdRetweetRetweetTweetId;
};
struct TweetRetweetVertexValueMap {
    int capacity = 65536;
    int size = 0;
    TweetRetweetVertexValueEntry** tweetRetweetVertexValueEntryArray = new TweetRetweetVertexValueEntry*[capacity]();
};

struct UserProjectedTupleMap {
    int capacity = 4;
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
    int capacity = 4;
    int size = 0;
    TweetProjectedTupleEntry** tweetProjectedTupleEntryArray = new TweetProjectedTupleEntry*[capacity]();
};
struct TweetProjectedTupleEntry {
    TweetProjectedTupleEntry* next = NULL;
    int userIdTweetUserId;
    int tweetTweetIdRetweetRetweetTweetId;
    std::string tweetDate;
    int count = 1;
};

struct TweetJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    TweetJoinTupleEntry** tweetJoinTupleEntryArray = new TweetJoinTupleEntry*[capacity]();
};

struct TweetJoinTupleEntry {
    UserTweetVertexValueEntry* userTweetVertexValue = NULL;
    TweetRetweetVertexValueEntry* tweetRetweetVertexValue = NULL;
    TweetProjectedTupleMap* tweetProjectedTupleMap = new TweetProjectedTupleMap;
    TweetJoinTupleEntry* next = NULL;
};

struct RetweetProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    RetweetProjectedTupleEntry** retweetProjectedTupleEntryArray = new RetweetProjectedTupleEntry*[capacity]();
};
struct RetweetProjectedTupleEntry {
    RetweetProjectedTupleEntry* next = NULL;
    int retweetTweetId;
    int tweetTweetIdRetweetRetweetTweetId;
    std::string retweetTweetDate;
    int count = 1;
};

struct RetweetJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    RetweetJoinTupleEntry** retweetJoinTupleEntryArray = new RetweetJoinTupleEntry*[capacity]();
};

struct RetweetJoinTupleEntry {
    TweetRetweetVertexValueEntry* tweetRetweetVertexValue = NULL;
    RetweetProjectedTupleMap* retweetProjectedTupleMap = new RetweetProjectedTupleMap;
    RetweetJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

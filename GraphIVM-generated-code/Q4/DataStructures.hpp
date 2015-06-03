#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct TweetRetweet1VertexValueMap;
struct Retweet1Retweet2VertexValueMap;
struct Retweet2FollowerVertexValueMap;
struct TweetRetweet1VertexValueEntry;
struct Retweet1Retweet2VertexValueEntry;
struct Retweet2FollowerVertexValueEntry;
struct TweetProjectedTupleMap;
struct Retweet1ProjectedTupleMap;
struct Retweet2ProjectedTupleMap;
struct FollowerProjectedTupleMap;
struct TweetProjectedTupleEntry;
struct Retweet1ProjectedTupleEntry;
struct Retweet2ProjectedTupleEntry;
struct FollowerProjectedTupleEntry;
struct TweetJoinTupleMap;
struct Retweet1JoinTupleMap;
struct Retweet2JoinTupleMap;
struct FollowerJoinTupleMap;
struct TweetJoinTupleEntry;
struct Retweet1JoinTupleEntry;
struct Retweet2JoinTupleEntry;
struct FollowerJoinTupleEntry;




struct TweetRetweet1VertexValueEntry {
    std::vector<TweetJoinTupleEntry*> tweetJoinTupleList;
    std::vector<Retweet1JoinTupleEntry*> retweet1JoinTupleList;
    TweetRetweet1VertexValueEntry* next = NULL;
    int tweetTweetIdRetweet1RetweetTweetId;
};
struct TweetRetweet1VertexValueMap {
    int capacity = 65536;
    int size = 0;
    TweetRetweet1VertexValueEntry** tweetRetweet1VertexValueEntryArray = new TweetRetweet1VertexValueEntry*[capacity]();
};



struct Retweet1Retweet2VertexValueEntry {
    std::vector<Retweet1JoinTupleEntry*> retweet1JoinTupleList;
    std::vector<Retweet2JoinTupleEntry*> retweet2JoinTupleList;
    Retweet1Retweet2VertexValueEntry* next = NULL;
    int retweet1TweetIdRetweet2RetweetTweetId;
};
struct Retweet1Retweet2VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet1Retweet2VertexValueEntry** retweet1Retweet2VertexValueEntryArray = new Retweet1Retweet2VertexValueEntry*[capacity]();
};



struct Retweet2FollowerVertexValueEntry {
    std::vector<Retweet2JoinTupleEntry*> retweet2JoinTupleList;
    std::vector<FollowerJoinTupleEntry*> followerJoinTupleList;
    Retweet2FollowerVertexValueEntry* next = NULL;
    int retweet2UserIdFollowerUserId;
};
struct Retweet2FollowerVertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet2FollowerVertexValueEntry** retweet2FollowerVertexValueEntryArray = new Retweet2FollowerVertexValueEntry*[capacity]();
};

struct TweetProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    TweetProjectedTupleEntry** tweetProjectedTupleEntryArray = new TweetProjectedTupleEntry*[capacity]();
};
struct TweetProjectedTupleEntry {
    TweetProjectedTupleEntry* next = NULL;
    int userId;
    int tweetTweetIdRetweet1RetweetTweetId;
    int count = 1;
};

struct TweetJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    TweetJoinTupleEntry** tweetJoinTupleEntryArray = new TweetJoinTupleEntry*[capacity]();
};

struct TweetJoinTupleEntry {
    TweetRetweet1VertexValueEntry* tweetRetweet1VertexValue = NULL;
    TweetProjectedTupleMap* tweetProjectedTupleMap = new TweetProjectedTupleMap;
    TweetJoinTupleEntry* next = NULL;
};

struct Retweet1ProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    Retweet1ProjectedTupleEntry** retweet1ProjectedTupleEntryArray = new Retweet1ProjectedTupleEntry*[capacity]();
};
struct Retweet1ProjectedTupleEntry {
    Retweet1ProjectedTupleEntry* next = NULL;
    int tweetTweetIdRetweet1RetweetTweetId;
    int retweet1TweetIdRetweet2RetweetTweetId;
    int count = 1;
};

struct Retweet1JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet1JoinTupleEntry** retweet1JoinTupleEntryArray = new Retweet1JoinTupleEntry*[capacity]();
};

struct Retweet1JoinTupleEntry {
    TweetRetweet1VertexValueEntry* tweetRetweet1VertexValue = NULL;
    Retweet1Retweet2VertexValueEntry* retweet1Retweet2VertexValue = NULL;
    Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap = new Retweet1ProjectedTupleMap;
    Retweet1JoinTupleEntry* next = NULL;
};

struct Retweet2ProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    Retweet2ProjectedTupleEntry** retweet2ProjectedTupleEntryArray = new Retweet2ProjectedTupleEntry*[capacity]();
};
struct Retweet2ProjectedTupleEntry {
    Retweet2ProjectedTupleEntry* next = NULL;
    int retweet1TweetIdRetweet2RetweetTweetId;
    int retweet2TweetId;
    int retweet2UserIdFollowerUserId;
    int count = 1;
};

struct Retweet2JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet2JoinTupleEntry** retweet2JoinTupleEntryArray = new Retweet2JoinTupleEntry*[capacity]();
};

struct Retweet2JoinTupleEntry {
    Retweet1Retweet2VertexValueEntry* retweet1Retweet2VertexValue = NULL;
    Retweet2FollowerVertexValueEntry* retweet2FollowerVertexValue = NULL;
    Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap = new Retweet2ProjectedTupleMap;
    Retweet2JoinTupleEntry* next = NULL;
};

struct FollowerProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    FollowerProjectedTupleEntry** followerProjectedTupleEntryArray = new FollowerProjectedTupleEntry*[capacity]();
};
struct FollowerProjectedTupleEntry {
    FollowerProjectedTupleEntry* next = NULL;
    int followerId;
    int retweet2UserIdFollowerUserId;
    int count = 1;
};

struct FollowerJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    FollowerJoinTupleEntry** followerJoinTupleEntryArray = new FollowerJoinTupleEntry*[capacity]();
};

struct FollowerJoinTupleEntry {
    Retweet2FollowerVertexValueEntry* retweet2FollowerVertexValue = NULL;
    FollowerProjectedTupleMap* followerProjectedTupleMap = new FollowerProjectedTupleMap;
    FollowerJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

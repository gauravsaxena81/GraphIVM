#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct Retweet1Retweet2VertexValueMap;
struct Retweet2Retweet3VertexValueMap;
struct Retweet3Retweet4VertexValueMap;
struct Retweet4Retweet5VertexValueMap;
struct Retweet5FollowerVertexValueMap;
struct Retweet1Retweet2VertexValueEntry;
struct Retweet2Retweet3VertexValueEntry;
struct Retweet3Retweet4VertexValueEntry;
struct Retweet4Retweet5VertexValueEntry;
struct Retweet5FollowerVertexValueEntry;
struct Retweet1ProjectedTupleMap;
struct Retweet2ProjectedTupleMap;
struct Retweet3ProjectedTupleMap;
struct Retweet4ProjectedTupleMap;
struct Retweet5ProjectedTupleMap;
struct FollowerProjectedTupleMap;
struct Retweet1ProjectedTupleEntry;
struct Retweet2ProjectedTupleEntry;
struct Retweet3ProjectedTupleEntry;
struct Retweet4ProjectedTupleEntry;
struct Retweet5ProjectedTupleEntry;
struct FollowerProjectedTupleEntry;
struct Retweet1JoinTupleMap;
struct Retweet2JoinTupleMap;
struct Retweet3JoinTupleMap;
struct Retweet4JoinTupleMap;
struct Retweet5JoinTupleMap;
struct FollowerJoinTupleMap;
struct Retweet1JoinTupleEntry;
struct Retweet2JoinTupleEntry;
struct Retweet3JoinTupleEntry;
struct Retweet4JoinTupleEntry;
struct Retweet5JoinTupleEntry;
struct FollowerJoinTupleEntry;




struct Retweet1Retweet2VertexValueEntry {
    std::vector<Retweet1JoinTupleEntry*> retweet1JoinTupleList;
    std::vector<Retweet2JoinTupleEntry*> retweet2JoinTupleList;
    Retweet1Retweet2VertexValueEntry* next = NULL;
    int r1TweetIdR2RetweetTweetId;
};
struct Retweet1Retweet2VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet1Retweet2VertexValueEntry** retweet1Retweet2VertexValueEntryArray = new Retweet1Retweet2VertexValueEntry*[capacity]();
};



struct Retweet2Retweet3VertexValueEntry {
    std::vector<Retweet2JoinTupleEntry*> retweet2JoinTupleList;
    std::vector<Retweet3JoinTupleEntry*> retweet3JoinTupleList;
    Retweet2Retweet3VertexValueEntry* next = NULL;
    int r2TweetIdR3RetweetTweetId;
};
struct Retweet2Retweet3VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet2Retweet3VertexValueEntry** retweet2Retweet3VertexValueEntryArray = new Retweet2Retweet3VertexValueEntry*[capacity]();
};



struct Retweet3Retweet4VertexValueEntry {
    std::vector<Retweet3JoinTupleEntry*> retweet3JoinTupleList;
    std::vector<Retweet4JoinTupleEntry*> retweet4JoinTupleList;
    Retweet3Retweet4VertexValueEntry* next = NULL;
    int r3TweetIdR4RetweetTweetId;
};
struct Retweet3Retweet4VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet3Retweet4VertexValueEntry** retweet3Retweet4VertexValueEntryArray = new Retweet3Retweet4VertexValueEntry*[capacity]();
};



struct Retweet4Retweet5VertexValueEntry {
    std::vector<Retweet4JoinTupleEntry*> retweet4JoinTupleList;
    std::vector<Retweet5JoinTupleEntry*> retweet5JoinTupleList;
    Retweet4Retweet5VertexValueEntry* next = NULL;
    int r4TweetIdR5RetweetTweetId;
};
struct Retweet4Retweet5VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet4Retweet5VertexValueEntry** retweet4Retweet5VertexValueEntryArray = new Retweet4Retweet5VertexValueEntry*[capacity]();
};



struct Retweet5FollowerVertexValueEntry {
    std::vector<Retweet5JoinTupleEntry*> retweet5JoinTupleList;
    std::vector<FollowerJoinTupleEntry*> followerJoinTupleList;
    Retweet5FollowerVertexValueEntry* next = NULL;
    int r5UserIdFUserId;
};
struct Retweet5FollowerVertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet5FollowerVertexValueEntry** retweet5FollowerVertexValueEntryArray = new Retweet5FollowerVertexValueEntry*[capacity]();
};

struct Retweet1ProjectedTupleMap {
    int capacity = 8;
    int size = 0;
    Retweet1ProjectedTupleEntry** retweet1ProjectedTupleEntryArray = new Retweet1ProjectedTupleEntry*[capacity]();
};
struct Retweet1ProjectedTupleEntry {
    Retweet1ProjectedTupleEntry* next = NULL;
    int r1TweetId;
    int count = 1;
};

struct Retweet1JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet1JoinTupleEntry** retweet1JoinTupleEntryArray = new Retweet1JoinTupleEntry*[capacity]();
};

struct Retweet1JoinTupleEntry {
    Retweet1Retweet2VertexValueEntry* retweet1Retweet2VertexValue = NULL;
    Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap = new Retweet1ProjectedTupleMap;
    Retweet1JoinTupleEntry* next = NULL;
};

struct Retweet2ProjectedTupleMap {
    int capacity = 2;
    int size = 0;
    Retweet2ProjectedTupleEntry** retweet2ProjectedTupleEntryArray = new Retweet2ProjectedTupleEntry*[capacity]();
};
struct Retweet2ProjectedTupleEntry {
    Retweet2ProjectedTupleEntry* next = NULL;
    const int dummy = 0;
    int count = 1;
};

struct Retweet2JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet2JoinTupleEntry** retweet2JoinTupleEntryArray = new Retweet2JoinTupleEntry*[capacity]();
};

struct Retweet2JoinTupleEntry {
    Retweet1Retweet2VertexValueEntry* retweet1Retweet2VertexValue = NULL;
    Retweet2Retweet3VertexValueEntry* retweet2Retweet3VertexValue = NULL;
    Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap = new Retweet2ProjectedTupleMap;
    Retweet2JoinTupleEntry* next = NULL;
};

struct Retweet3ProjectedTupleMap {
    int capacity = 2;
    int size = 0;
    Retweet3ProjectedTupleEntry** retweet3ProjectedTupleEntryArray = new Retweet3ProjectedTupleEntry*[capacity]();
};
struct Retweet3ProjectedTupleEntry {
    Retweet3ProjectedTupleEntry* next = NULL;
    const int dummy = 0;
    int count = 1;
};

struct Retweet3JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet3JoinTupleEntry** retweet3JoinTupleEntryArray = new Retweet3JoinTupleEntry*[capacity]();
};

struct Retweet3JoinTupleEntry {
    Retweet2Retweet3VertexValueEntry* retweet2Retweet3VertexValue = NULL;
    Retweet3Retweet4VertexValueEntry* retweet3Retweet4VertexValue = NULL;
    Retweet3ProjectedTupleMap* retweet3ProjectedTupleMap = new Retweet3ProjectedTupleMap;
    Retweet3JoinTupleEntry* next = NULL;
};

struct Retweet4ProjectedTupleMap {
    int capacity = 2;
    int size = 0;
    Retweet4ProjectedTupleEntry** retweet4ProjectedTupleEntryArray = new Retweet4ProjectedTupleEntry*[capacity]();
};
struct Retweet4ProjectedTupleEntry {
    Retweet4ProjectedTupleEntry* next = NULL;
    const int dummy = 0;
    int count = 1;
};

struct Retweet4JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet4JoinTupleEntry** retweet4JoinTupleEntryArray = new Retweet4JoinTupleEntry*[capacity]();
};

struct Retweet4JoinTupleEntry {
    Retweet3Retweet4VertexValueEntry* retweet3Retweet4VertexValue = NULL;
    Retweet4Retweet5VertexValueEntry* retweet4Retweet5VertexValue = NULL;
    Retweet4ProjectedTupleMap* retweet4ProjectedTupleMap = new Retweet4ProjectedTupleMap;
    Retweet4JoinTupleEntry* next = NULL;
};

struct Retweet5ProjectedTupleMap {
    int capacity = 2;
    int size = 0;
    Retweet5ProjectedTupleEntry** retweet5ProjectedTupleEntryArray = new Retweet5ProjectedTupleEntry*[capacity]();
};
struct Retweet5ProjectedTupleEntry {
    Retweet5ProjectedTupleEntry* next = NULL;
    const int dummy = 0;
    int count = 1;
};

struct Retweet5JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Retweet5JoinTupleEntry** retweet5JoinTupleEntryArray = new Retweet5JoinTupleEntry*[capacity]();
};

struct Retweet5JoinTupleEntry {
    Retweet4Retweet5VertexValueEntry* retweet4Retweet5VertexValue = NULL;
    Retweet5FollowerVertexValueEntry* retweet5FollowerVertexValue = NULL;
    Retweet5ProjectedTupleMap* retweet5ProjectedTupleMap = new Retweet5ProjectedTupleMap;
    Retweet5JoinTupleEntry* next = NULL;
};

struct FollowerProjectedTupleMap {
    int capacity = 8;
    int size = 0;
    FollowerProjectedTupleEntry** followerProjectedTupleEntryArray = new FollowerProjectedTupleEntry*[capacity]();
};
struct FollowerProjectedTupleEntry {
    FollowerProjectedTupleEntry* next = NULL;
    int followerId;
    int count = 1;
};

struct FollowerJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    FollowerJoinTupleEntry** followerJoinTupleEntryArray = new FollowerJoinTupleEntry*[capacity]();
};

struct FollowerJoinTupleEntry {
    Retweet5FollowerVertexValueEntry* retweet5FollowerVertexValue = NULL;
    FollowerProjectedTupleMap* followerProjectedTupleMap = new FollowerProjectedTupleMap;
    FollowerJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

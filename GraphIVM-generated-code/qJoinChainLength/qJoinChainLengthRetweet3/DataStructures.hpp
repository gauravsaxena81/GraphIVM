#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct Retweet1Retweet2VertexValueMap;
struct Retweet2FollowerVertexValueMap;
struct Retweet1Retweet2VertexValueEntry;
struct Retweet2FollowerVertexValueEntry;
struct Retweet1ProjectedTupleMap;
struct Retweet2ProjectedTupleMap;
struct FollowerProjectedTupleMap;
struct Retweet1ProjectedTupleEntry;
struct Retweet2ProjectedTupleEntry;
struct FollowerProjectedTupleEntry;
struct Retweet1JoinTupleMap;
struct Retweet2JoinTupleMap;
struct FollowerJoinTupleMap;
struct Retweet1JoinTupleEntry;
struct Retweet2JoinTupleEntry;
struct FollowerJoinTupleEntry;


struct Retweet1Retweet2VertexValueEntry {
    std::vector<Retweet1JoinTupleEntry*> retweet1JoinTupleList;
    std::vector<Retweet2JoinTupleEntry*> retweet2JoinTupleList;
    Retweet1Retweet2VertexValueEntry* next = NULL;
    bool localLookAhead = false;
    bool globalLookAhead = true;
    int r1TweetIdR2RetweetTweetId;
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
    bool localLookAhead = false;
    bool globalLookAhead = true;
    int r2UserIdFUserId;
};
struct Retweet2FollowerVertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet2FollowerVertexValueEntry** retweet2FollowerVertexValueEntryArray = new Retweet2FollowerVertexValueEntry*[capacity]();
};

struct Retweet1ProjectedTupleMap {
    int capacity = 4;
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
    int capacity = 4;
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
    Retweet2FollowerVertexValueEntry* retweet2FollowerVertexValue = NULL;
    Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap = new Retweet2ProjectedTupleMap;
    Retweet2JoinTupleEntry* next = NULL;
};

struct FollowerProjectedTupleMap {
    int capacity = 2;
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
    Retweet2FollowerVertexValueEntry* retweet2FollowerVertexValue = NULL;
    FollowerProjectedTupleMap* followerProjectedTupleMap = new FollowerProjectedTupleMap;
    FollowerJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

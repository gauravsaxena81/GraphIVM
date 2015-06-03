#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct Retweet1Retweet2VertexValueMap;
struct Retweet2Retweet3VertexValueMap;
struct Retweet3FollowerVertexValueMap;
struct Retweet1Retweet2VertexValueEntry;
struct Retweet2Retweet3VertexValueEntry;
struct Retweet3FollowerVertexValueEntry;
struct Retweet1ProjectedTupleMap;
struct Retweet2ProjectedTupleMap;
struct Retweet3ProjectedTupleMap;
struct FollowerProjectedTupleMap;
struct Retweet1ProjectedTupleEntry;
struct Retweet2ProjectedTupleEntry;
struct Retweet3ProjectedTupleEntry;
struct FollowerProjectedTupleEntry;
struct Retweet1JoinTupleMap;
struct Retweet2JoinTupleMap;
struct Retweet3JoinTupleMap;
struct FollowerJoinTupleMap;
struct Retweet1JoinTupleEntry;
struct Retweet2JoinTupleEntry;
struct Retweet3JoinTupleEntry;
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



struct Retweet2Retweet3VertexValueEntry {
    std::vector<Retweet2JoinTupleEntry*> retweet2JoinTupleList;
    std::vector<Retweet3JoinTupleEntry*> retweet3JoinTupleList;
    Retweet2Retweet3VertexValueEntry* next = NULL;
    bool localLookAhead = false;
    bool globalLookAhead = true;
    int r2TweetIdR3RetweetTweetId;
};
struct Retweet2Retweet3VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet2Retweet3VertexValueEntry** retweet2Retweet3VertexValueEntryArray = new Retweet2Retweet3VertexValueEntry*[capacity]();
};



struct Retweet3FollowerVertexValueEntry {
    std::vector<Retweet3JoinTupleEntry*> retweet3JoinTupleList;
    std::vector<FollowerJoinTupleEntry*> followerJoinTupleList;
    Retweet3FollowerVertexValueEntry* next = NULL;
    bool localLookAhead = false;
    bool globalLookAhead = true;
    int r3UserIdFUserId;
};
struct Retweet3FollowerVertexValueMap {
    int capacity = 65536;
    int size = 0;
    Retweet3FollowerVertexValueEntry** retweet3FollowerVertexValueEntryArray = new Retweet3FollowerVertexValueEntry*[capacity]();
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
    Retweet3FollowerVertexValueEntry* retweet3FollowerVertexValue = NULL;
    Retweet3ProjectedTupleMap* retweet3ProjectedTupleMap = new Retweet3ProjectedTupleMap;
    Retweet3JoinTupleEntry* next = NULL;
};

struct FollowerProjectedTupleMap {
    int capacity = 4;
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
    Retweet3FollowerVertexValueEntry* retweet3FollowerVertexValue = NULL;
    FollowerProjectedTupleMap* followerProjectedTupleMap = new FollowerProjectedTupleMap;
    FollowerJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

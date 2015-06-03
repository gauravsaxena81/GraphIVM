#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct Follower1Follower2VertexValueMap;
struct Follower2Follower3VertexValueMap;
struct Follower3Follower4VertexValueMap;
struct Follower4Follower5VertexValueMap;
struct Follower5TweetVertexValueMap;
struct Follower1Follower2VertexValueEntry;
struct Follower2Follower3VertexValueEntry;
struct Follower3Follower4VertexValueEntry;
struct Follower4Follower5VertexValueEntry;
struct Follower5TweetVertexValueEntry;
//#Projected Tuple
struct Follower1ProjectedTupleMap;
struct Follower2ProjectedTupleMap;
struct Follower3ProjectedTupleMap;
struct Follower4ProjectedTupleMap;
struct Follower5ProjectedTupleMap;
struct TweetProjectedTupleMap;
struct Follower1ProjectedTupleEntry;
struct Follower2ProjectedTupleEntry;
struct Follower3ProjectedTupleEntry;
struct Follower4ProjectedTupleEntry;
struct Follower5ProjectedTupleEntry;
struct TweetProjectedTupleEntry;
//#Join Tuple
struct Follower1JoinTupleMap;
struct Follower2JoinTupleMap;
struct Follower3JoinTupleMap;
struct Follower4JoinTupleMap;
struct Follower5JoinTupleMap;
struct TweetJoinTupleMap;
struct Follower1JoinTupleEntry;
struct Follower2JoinTupleEntry;
struct Follower3JoinTupleEntry;
struct Follower4JoinTupleEntry;
struct Follower5JoinTupleEntry;
struct TweetJoinTupleEntry;

//#Implementations


//#Implementations

struct Follower1Follower2VertexValueEntry {
    std::vector<Follower1JoinTupleEntry*> follower1JoinTupleList;
    std::vector<Follower2JoinTupleEntry*> follower2JoinTupleList;
    Follower1Follower2VertexValueEntry* next = NULL;
    //%1 - vertex value attributes
    int follower1IdUser2Id;
};
struct Follower1Follower2VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower1Follower2VertexValueEntry** follower1Follower2VertexValueEntryArray = new Follower1Follower2VertexValueEntry*[capacity]();
};


//#Implementations

struct Follower2Follower3VertexValueEntry {
    std::vector<Follower2JoinTupleEntry*> follower2JoinTupleList;
    std::vector<Follower3JoinTupleEntry*> follower3JoinTupleList;
    Follower2Follower3VertexValueEntry* next = NULL;
    //%1 - vertex value attributes
    int follower2IdUser3Id;
};
struct Follower2Follower3VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower2Follower3VertexValueEntry** follower2Follower3VertexValueEntryArray = new Follower2Follower3VertexValueEntry*[capacity]();
};


//#Implementations

struct Follower3Follower4VertexValueEntry {
    std::vector<Follower3JoinTupleEntry*> follower3JoinTupleList;
    std::vector<Follower4JoinTupleEntry*> follower4JoinTupleList;
    Follower3Follower4VertexValueEntry* next = NULL;
    //%1 - vertex value attributes
    int follower3IdUser4Id;
};
struct Follower3Follower4VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower3Follower4VertexValueEntry** follower3Follower4VertexValueEntryArray = new Follower3Follower4VertexValueEntry*[capacity]();
};


//#Implementations

struct Follower4Follower5VertexValueEntry {
    std::vector<Follower4JoinTupleEntry*> follower4JoinTupleList;
    std::vector<Follower5JoinTupleEntry*> follower5JoinTupleList;
    Follower4Follower5VertexValueEntry* next = NULL;
    //%1 - vertex value attributes
    int follower4IdUser5Id;
};
struct Follower4Follower5VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower4Follower5VertexValueEntry** follower4Follower5VertexValueEntryArray = new Follower4Follower5VertexValueEntry*[capacity]();
};


//#Implementations

struct Follower5TweetVertexValueEntry {
    std::vector<Follower5JoinTupleEntry*> follower5JoinTupleList;
    std::vector<TweetJoinTupleEntry*> tweetJoinTupleList;
    Follower5TweetVertexValueEntry* next = NULL;
    //%1 - vertex value attributes
    int follower5IdTweetUserId;
};
struct Follower5TweetVertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower5TweetVertexValueEntry** follower5TweetVertexValueEntryArray = new Follower5TweetVertexValueEntry*[capacity]();
};

struct Follower1ProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    Follower1ProjectedTupleEntry** follower1ProjectedTupleEntryArray = new Follower1ProjectedTupleEntry*[capacity]();
};
struct Follower1ProjectedTupleEntry {
    Follower1ProjectedTupleEntry* next = NULL;
    int user1Id;
    int count = 1;
};

struct Follower1JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Follower1JoinTupleEntry** follower1JoinTupleEntryArray = new Follower1JoinTupleEntry*[capacity]();
};

struct Follower1JoinTupleEntry {
    Follower1Follower2VertexValueEntry* follower1Follower2VertexValue = NULL;
    Follower1ProjectedTupleMap* follower1ProjectedTupleMap = new Follower1ProjectedTupleMap;
    Follower1JoinTupleEntry* next = NULL;
};

struct Follower2JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Follower2JoinTupleEntry** follower2JoinTupleEntryArray = new Follower2JoinTupleEntry*[capacity]();
};

struct Follower2JoinTupleEntry {
    Follower1Follower2VertexValueEntry* follower1Follower2VertexValue = NULL;
    Follower2Follower3VertexValueEntry* follower2Follower3VertexValue = NULL;
    Follower2JoinTupleEntry* next = NULL;
};

struct Follower3JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Follower3JoinTupleEntry** follower3JoinTupleEntryArray = new Follower3JoinTupleEntry*[capacity]();
};

struct Follower3JoinTupleEntry {
    Follower2Follower3VertexValueEntry* follower2Follower3VertexValue = NULL;
    Follower3Follower4VertexValueEntry* follower3Follower4VertexValue = NULL;
    Follower3JoinTupleEntry* next = NULL;
};

struct Follower4JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Follower4JoinTupleEntry** follower4JoinTupleEntryArray = new Follower4JoinTupleEntry*[capacity]();
};

struct Follower4JoinTupleEntry {
    Follower3Follower4VertexValueEntry* follower3Follower4VertexValue = NULL;
    Follower4Follower5VertexValueEntry* follower4Follower5VertexValue = NULL;
    Follower4JoinTupleEntry* next = NULL;
};

struct Follower5JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Follower5JoinTupleEntry** follower5JoinTupleEntryArray = new Follower5JoinTupleEntry*[capacity]();
};

struct Follower5JoinTupleEntry {
    Follower4Follower5VertexValueEntry* follower4Follower5VertexValue = NULL;
    Follower5TweetVertexValueEntry* follower5TweetVertexValue = NULL;
    Follower5JoinTupleEntry* next = NULL;
};

struct TweetProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    TweetProjectedTupleEntry** tweetProjectedTupleEntryArray = new TweetProjectedTupleEntry*[capacity]();
};
struct TweetProjectedTupleEntry {
    TweetProjectedTupleEntry* next = NULL;
    int tweetId;
    int count = 1;
};

struct TweetJoinTupleMap {
    int capacity = 65536;
    int size = 0;
    TweetJoinTupleEntry** tweetJoinTupleEntryArray = new TweetJoinTupleEntry*[capacity]();
};

struct TweetJoinTupleEntry {
    Follower5TweetVertexValueEntry* follower5TweetVertexValue = NULL;
    TweetProjectedTupleMap* tweetProjectedTupleMap = new TweetProjectedTupleMap;
    TweetJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

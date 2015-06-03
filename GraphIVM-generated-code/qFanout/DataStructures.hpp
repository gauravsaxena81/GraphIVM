#include<string>
#include<vector>

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_

struct Follower1Follower2VertexValueMap;
struct Follower2TweetVertexValueMap;
struct Follower1Follower2VertexValueEntry;
struct Follower2TweetVertexValueEntry;
struct Follower1ProjectedTupleMap;
struct Follower2ProjectedTupleMap;
struct TweetProjectedTupleMap;
struct Follower1ProjectedTupleEntry;
struct Follower2ProjectedTupleEntry;
struct TweetProjectedTupleEntry;
struct Follower1JoinTupleMap;
struct Follower2JoinTupleMap;
struct TweetJoinTupleMap;
struct Follower1JoinTupleEntry;
struct Follower2JoinTupleEntry;
struct TweetJoinTupleEntry;




struct Follower1Follower2VertexValueEntry {
    std::vector<Follower1JoinTupleEntry*> follower1JoinTupleList;
    std::vector<Follower2JoinTupleEntry*> follower2JoinTupleList;
    Follower1Follower2VertexValueEntry* next = NULL;
    bool localLookAhead = false;
    bool globalLookAhead = true;
    int user1IdFollower2Id;
};
struct Follower1Follower2VertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower1Follower2VertexValueEntry** follower1Follower2VertexValueEntryArray = new Follower1Follower2VertexValueEntry*[capacity]();
};



struct Follower2TweetVertexValueEntry {
    std::vector<Follower2JoinTupleEntry*> follower2JoinTupleList;
    std::vector<TweetJoinTupleEntry*> tweetJoinTupleList;
    Follower2TweetVertexValueEntry* next = NULL;
    bool localLookAhead = false;
    bool globalLookAhead = true;
    int user2IdTweetUserId;
};
struct Follower2TweetVertexValueMap {
    int capacity = 65536;
    int size = 0;
    Follower2TweetVertexValueEntry** follower2TweetVertexValueEntryArray = new Follower2TweetVertexValueEntry*[capacity]();
};

struct Follower1ProjectedTupleMap {
    int capacity = 4;
    int size = 0;
    Follower1ProjectedTupleEntry** follower1ProjectedTupleEntryArray = new Follower1ProjectedTupleEntry*[capacity]();
};
struct Follower1ProjectedTupleEntry {
    Follower1ProjectedTupleEntry* next = NULL;
    int follower1Id;
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

struct Follower2ProjectedTupleMap {
    int capacity = 2;
    int size = 0;
    Follower2ProjectedTupleEntry** follower2ProjectedTupleEntryArray = new Follower2ProjectedTupleEntry*[capacity]();
};
struct Follower2ProjectedTupleEntry {
    Follower2ProjectedTupleEntry* next = NULL;
    const int dummy = 0;
    int count = 1;
};

struct Follower2JoinTupleMap {
    int capacity = 65536;
    int size = 0;
    Follower2JoinTupleEntry** follower2JoinTupleEntryArray = new Follower2JoinTupleEntry*[capacity]();
};

struct Follower2JoinTupleEntry {
    Follower1Follower2VertexValueEntry* follower1Follower2VertexValue = NULL;
    Follower2TweetVertexValueEntry* follower2TweetVertexValue = NULL;
    Follower2ProjectedTupleMap* follower2ProjectedTupleMap = new Follower2ProjectedTupleMap;
    Follower2JoinTupleEntry* next = NULL;
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
    Follower2TweetVertexValueEntry* follower2TweetVertexValue = NULL;
    TweetProjectedTupleMap* tweetProjectedTupleMap = new TweetProjectedTupleMap;
    TweetJoinTupleEntry* next = NULL;
};

#endif /* DATASTRUCTURES_HPP_ */

#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

//%1 table1 of vertex value, %2 table2 of vertex value, %3 table1 in camel case, %4 vertex values 
Follower1Follower2VertexValueEntry* putFollower1Follower2VertexValueIfAbsent(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, int& follower1IdUser2Id);
Follower2TweetVertexValueEntry* putFollower2TweetVertexValueIfAbsent(Follower2TweetVertexValueMap* follower2TweetVertexValueMap, int& follower2IdTweetUserId);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
Follower1JoinTupleEntry* putFollower1JoinTupleIfAbsent(Follower1JoinTupleMap* follower1JoinTupleMap, int& follower1IdUser2Id, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap );

typedef Follower1JoinTupleEntry* (*Follower1JoinTupleIteratorNext) ();
Follower1JoinTupleIteratorNext follower1JoinTupleIterator(Follower1JoinTupleMap* follower1JoinTupleMap);

//%1 = TableName, %2 = projected attributes, %3 - projected attributes
Follower1ProjectedTupleEntry* putFollower1ProjectedTuple(Follower1ProjectedTupleMap* follower1ProjectedTupleMap, int& user1Id );

//%1 = TableName, %2 = TableName in camel case
typedef Follower1ProjectedTupleEntry* (*Follower1ProjectedTupleIteratorNext) ();
Follower1ProjectedTupleIteratorNext follower1ProjectedTupleIterator(Follower1ProjectedTupleMap* follower1ProjectedTupleMap);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
Follower2JoinTupleEntry* putFollower2JoinTupleIfAbsent(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdTweetUserId, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, Follower2TweetVertexValueMap* follower2TweetVertexValueMap );

typedef Follower2JoinTupleEntry* (*Follower2JoinTupleIteratorNext) ();
Follower2JoinTupleIteratorNext follower2JoinTupleIterator(Follower2JoinTupleMap* follower2JoinTupleMap);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& follower2IdTweetUserId, Follower2TweetVertexValueMap* follower2TweetVertexValueMap );

typedef TweetJoinTupleEntry* (*TweetJoinTupleIteratorNext) ();
TweetJoinTupleIteratorNext tweetJoinTupleIterator(TweetJoinTupleMap* tweetJoinTupleMap);

//%1 = TableName, %2 = projected attributes, %3 - projected attributes
TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap, int& tweetId );

//%1 = TableName, %2 = TableName in camel case
typedef TweetProjectedTupleEntry* (*TweetProjectedTupleIteratorNext) ();
TweetProjectedTupleIteratorNext tweetProjectedTupleIterator(TweetProjectedTupleMap* tweetProjectedTupleMap);

#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

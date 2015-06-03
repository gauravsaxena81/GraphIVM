#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

//%1 table1 of vertex value, %2 table2 of vertex value, %3 table1 in camel case, %4 vertex values 
Follower1Follower2VertexValueEntry* putFollower1Follower2VertexValueIfAbsent(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, int& follower1IdUser2Id);
Follower2Follower3VertexValueEntry* putFollower2Follower3VertexValueIfAbsent(Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap, int& follower2IdUser3Id);
Follower3Follower4VertexValueEntry* putFollower3Follower4VertexValueIfAbsent(Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap, int& follower3IdUser4Id);
Follower4Follower5VertexValueEntry* putFollower4Follower5VertexValueIfAbsent(Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap, int& follower4IdUser5Id);
Follower5TweetVertexValueEntry* putFollower5TweetVertexValueIfAbsent(Follower5TweetVertexValueMap* follower5TweetVertexValueMap, int& follower5IdTweetUserId);

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
Follower2JoinTupleEntry* putFollower2JoinTupleIfAbsent(Follower2JoinTupleMap* follower2JoinTupleMap, int& follower1IdUser2Id,int& follower2IdUser3Id, Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap );

typedef Follower2JoinTupleEntry* (*Follower2JoinTupleIteratorNext) ();
Follower2JoinTupleIteratorNext follower2JoinTupleIterator(Follower2JoinTupleMap* follower2JoinTupleMap);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
Follower3JoinTupleEntry* putFollower3JoinTupleIfAbsent(Follower3JoinTupleMap* follower3JoinTupleMap, int& follower2IdUser3Id,int& follower3IdUser4Id, Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap );

typedef Follower3JoinTupleEntry* (*Follower3JoinTupleIteratorNext) ();
Follower3JoinTupleIteratorNext follower3JoinTupleIterator(Follower3JoinTupleMap* follower3JoinTupleMap);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
Follower4JoinTupleEntry* putFollower4JoinTupleIfAbsent(Follower4JoinTupleMap* follower4JoinTupleMap, int& follower3IdUser4Id,int& follower4IdUser5Id, Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap );

typedef Follower4JoinTupleEntry* (*Follower4JoinTupleIteratorNext) ();
Follower4JoinTupleIteratorNext follower4JoinTupleIterator(Follower4JoinTupleMap* follower4JoinTupleMap);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
Follower5JoinTupleEntry* putFollower5JoinTupleIfAbsent(Follower5JoinTupleMap* follower5JoinTupleMap, int& follower4IdUser5Id,int& follower5IdTweetUserId, Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap, Follower5TweetVertexValueMap* follower5TweetVertexValueMap );

typedef Follower5JoinTupleEntry* (*Follower5JoinTupleIteratorNext) ();
Follower5JoinTupleIteratorNext follower5JoinTupleIterator(Follower5JoinTupleMap* follower5JoinTupleMap);

//%1 = TableName, %2 = TableName in camel case, %3 join attribute values, %4 = %3$1s%4$1sVertexValueMap* %5$1s%4$1sVertexValueMap
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& follower5IdTweetUserId, Follower5TweetVertexValueMap* follower5TweetVertexValueMap );

typedef TweetJoinTupleEntry* (*TweetJoinTupleIteratorNext) ();
TweetJoinTupleIteratorNext tweetJoinTupleIterator(TweetJoinTupleMap* tweetJoinTupleMap);

//%1 = TableName, %2 = projected attributes, %3 - projected attributes
TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap, int& tweetId );

//%1 = TableName, %2 = TableName in camel case
typedef TweetProjectedTupleEntry* (*TweetProjectedTupleIteratorNext) ();
TweetProjectedTupleIteratorNext tweetProjectedTupleIterator(TweetProjectedTupleMap* tweetProjectedTupleMap);

#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

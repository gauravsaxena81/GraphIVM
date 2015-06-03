#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

Retweet1Retweet2VertexValueEntry* putRetweet1Retweet2VertexValueIfAbsent(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, int& r1TweetIdR2RetweetTweetId);
Retweet2Retweet3VertexValueEntry* putRetweet2Retweet3VertexValueIfAbsent(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap, int& r2TweetIdR3RetweetTweetId);
Retweet3FollowerVertexValueEntry* putRetweet3FollowerVertexValueIfAbsent(Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap, int& r3UserIdFUserId);

Retweet1JoinTupleEntry* putRetweet1JoinTupleIfAbsent(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap );
Retweet1ProjectedTupleEntry* putRetweet1ProjectedTuple(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap, int& r1TweetId );


Retweet2JoinTupleEntry* putRetweet2JoinTupleIfAbsent(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap );


Retweet2ProjectedTupleEntry* putRetweet2ProjectedTuple(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap);

Retweet3JoinTupleEntry* putRetweet3JoinTupleIfAbsent(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3UserIdFUserId, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap );


Retweet3ProjectedTupleEntry* putRetweet3ProjectedTuple(Retweet3ProjectedTupleMap* retweet3ProjectedTupleMap);

FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& r3UserIdFUserId, Retweet3FollowerVertexValueMap* retweet3FollowerVertexValueMap );
FollowerProjectedTupleEntry* putFollowerProjectedTuple(FollowerProjectedTupleMap* followerProjectedTupleMap, int& followerId );



#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

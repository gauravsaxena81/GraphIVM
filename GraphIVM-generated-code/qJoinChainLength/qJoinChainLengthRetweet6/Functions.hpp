#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

Retweet1Retweet2VertexValueEntry* putRetweet1Retweet2VertexValueIfAbsent(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, int& r1TweetIdR2RetweetTweetId);
Retweet2Retweet3VertexValueEntry* putRetweet2Retweet3VertexValueIfAbsent(Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap, int& r2TweetIdR3RetweetTweetId);
Retweet3Retweet4VertexValueEntry* putRetweet3Retweet4VertexValueIfAbsent(Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap, int& r3TweetIdR4RetweetTweetId);
Retweet4Retweet5VertexValueEntry* putRetweet4Retweet5VertexValueIfAbsent(Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap, int& r4TweetIdR5RetweetTweetId);
Retweet5FollowerVertexValueEntry* putRetweet5FollowerVertexValueIfAbsent(Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap, int& r5UserIdFUserId);

Retweet1JoinTupleEntry* putRetweet1JoinTupleIfAbsent(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& r1TweetIdR2RetweetTweetId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap );
Retweet1ProjectedTupleEntry* putRetweet1ProjectedTuple(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap, int& r1TweetId );


Retweet2JoinTupleEntry* putRetweet2JoinTupleIfAbsent(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& r1TweetIdR2RetweetTweetId,int& r2TweetIdR3RetweetTweetId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap );


Retweet2ProjectedTupleEntry* putRetweet2ProjectedTuple(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap);

Retweet3JoinTupleEntry* putRetweet3JoinTupleIfAbsent(Retweet3JoinTupleMap* retweet3JoinTupleMap, int& r2TweetIdR3RetweetTweetId,int& r3TweetIdR4RetweetTweetId, Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap, Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap );


Retweet3ProjectedTupleEntry* putRetweet3ProjectedTuple(Retweet3ProjectedTupleMap* retweet3ProjectedTupleMap);

Retweet4JoinTupleEntry* putRetweet4JoinTupleIfAbsent(Retweet4JoinTupleMap* retweet4JoinTupleMap, int& r3TweetIdR4RetweetTweetId,int& r4TweetIdR5RetweetTweetId, Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap );


Retweet4ProjectedTupleEntry* putRetweet4ProjectedTuple(Retweet4ProjectedTupleMap* retweet4ProjectedTupleMap);

Retweet5JoinTupleEntry* putRetweet5JoinTupleIfAbsent(Retweet5JoinTupleMap* retweet5JoinTupleMap, int& r4TweetIdR5RetweetTweetId,int& r5UserIdFUserId, Retweet4Retweet5VertexValueMap* retweet4Retweet5VertexValueMap, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap );


Retweet5ProjectedTupleEntry* putRetweet5ProjectedTuple(Retweet5ProjectedTupleMap* retweet5ProjectedTupleMap);

FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& r5UserIdFUserId, Retweet5FollowerVertexValueMap* retweet5FollowerVertexValueMap );
FollowerProjectedTupleEntry* putFollowerProjectedTuple(FollowerProjectedTupleMap* followerProjectedTupleMap, int& followerId );



#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

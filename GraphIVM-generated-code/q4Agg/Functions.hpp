#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

TweetRetweet1VertexValueEntry* putTweetRetweet1VertexValueIfAbsent(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, int& tweetTweetIdRetweet1RetweetTweetId);
Retweet1Retweet2VertexValueEntry* putRetweet1Retweet2VertexValueIfAbsent(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, int& retweet1TweetIdRetweet2RetweetTweetId);
Retweet2FollowerVertexValueEntry* putRetweet2FollowerVertexValueIfAbsent(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, int& retweet2UserIdFollowerUserId);
void deleteTweetRetweet1VertexValueIfZero(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& tweetTweetIdRetweet1RetweetTweetId
);

void deleteTweetRetweet1VertexValueIfZero(TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, Retweet1JoinTupleEntry* retweet1JoinTuple
, int& tweetTweetIdRetweet1RetweetTweetId
);

void deleteRetweet1Retweet2VertexValueIfZero(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet1JoinTupleEntry* retweet1JoinTuple
, int& retweet1TweetIdRetweet2RetweetTweetId
);

void deleteRetweet1Retweet2VertexValueIfZero(Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2JoinTupleEntry* retweet2JoinTuple
, int& retweet1TweetIdRetweet2RetweetTweetId
);

void deleteRetweet2FollowerVertexValueIfZero(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, Retweet2JoinTupleEntry* retweet2JoinTuple
, int& retweet2UserIdFollowerUserId
);

void deleteRetweet2FollowerVertexValueIfZero(Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap, FollowerJoinTupleEntry* followerJoinTuple
, int& retweet2UserIdFollowerUserId
);



TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& tweetTweetIdRetweet1RetweetTweetId, TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap );


TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap
, int& userId
, int& tweetTweetIdRetweet1RetweetTweetId
);



TweetJoinTupleEntry** getTweetJoinTuplePointer(TweetJoinTupleMap* tweetJoinTupleMap
, int& tweetTweetIdRetweet1RetweetTweetId
);
void removeJoinTupleEntry(TweetJoinTupleMap* tweetJoinTupleMap, TweetJoinTupleEntry** entryPointer);

int deleteTweetProjectedTupleIfZero(TweetProjectedTupleMap* tweetProjectedTupleMap
, int& userId 
, int& tweetTweetIdRetweet1RetweetTweetId 
);
Retweet1JoinTupleEntry* putRetweet1JoinTupleIfAbsent(Retweet1JoinTupleMap* retweet1JoinTupleMap, int& tweetTweetIdRetweet1RetweetTweetId,int& retweet1TweetIdRetweet2RetweetTweetId, TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap );


Retweet1ProjectedTupleEntry* putRetweet1ProjectedTuple(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap
);



Retweet1JoinTupleEntry** getRetweet1JoinTuplePointer(Retweet1JoinTupleMap* retweet1JoinTupleMap
, int& tweetTweetIdRetweet1RetweetTweetId
, int& retweet1TweetIdRetweet2RetweetTweetId
);
void removeJoinTupleEntry(Retweet1JoinTupleMap* retweet1JoinTupleMap, Retweet1JoinTupleEntry** entryPointer);

int deleteRetweet1ProjectedTupleIfZero(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap
);
Retweet2JoinTupleEntry* putRetweet2JoinTupleIfAbsent(Retweet2JoinTupleMap* retweet2JoinTupleMap, int& retweet1TweetIdRetweet2RetweetTweetId,int& retweet2UserIdFollowerUserId, Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap );


Retweet2ProjectedTupleEntry* putRetweet2ProjectedTuple(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap
);



Retweet2JoinTupleEntry** getRetweet2JoinTuplePointer(Retweet2JoinTupleMap* retweet2JoinTupleMap
, int& retweet1TweetIdRetweet2RetweetTweetId
, int& retweet2UserIdFollowerUserId
);
void removeJoinTupleEntry(Retweet2JoinTupleMap* retweet2JoinTupleMap, Retweet2JoinTupleEntry** entryPointer);

int deleteRetweet2ProjectedTupleIfZero(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap
);
FollowerJoinTupleEntry* putFollowerJoinTupleIfAbsent(FollowerJoinTupleMap* followerJoinTupleMap, int& retweet2UserIdFollowerUserId, Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap );


FollowerProjectedTupleEntry* putFollowerProjectedTuple(FollowerProjectedTupleMap* followerProjectedTupleMap
);



FollowerJoinTupleEntry** getFollowerJoinTuplePointer(FollowerJoinTupleMap* followerJoinTupleMap
, int& retweet2UserIdFollowerUserId
);
void removeJoinTupleEntry(FollowerJoinTupleMap* followerJoinTupleMap, FollowerJoinTupleEntry** entryPointer);

int deleteFollowerProjectedTupleIfZero(FollowerProjectedTupleMap* followerProjectedTupleMap
);
#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

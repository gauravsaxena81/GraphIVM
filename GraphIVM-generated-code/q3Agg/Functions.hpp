#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

UserTweetVertexValueEntry* putUserTweetVertexValueIfAbsent(UserTweetVertexValueMap* userTweetVertexValueMap, int& userIdTweetUserId);
TweetRetweetVertexValueEntry* putTweetRetweetVertexValueIfAbsent(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap, int& tweetTweetIdRetweetRetweetTweetId);
void deleteUserTweetVertexValueIfZero(UserTweetVertexValueMap* userTweetVertexValueMap, UserJoinTupleEntry* userJoinTuple
, int& userIdTweetUserId
);

void deleteUserTweetVertexValueIfZero(UserTweetVertexValueMap* userTweetVertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& userIdTweetUserId
);

void deleteTweetRetweetVertexValueIfZero(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& tweetTweetIdRetweetRetweetTweetId
);

void deleteTweetRetweetVertexValueIfZero(TweetRetweetVertexValueMap* tweetRetweetVertexValueMap, RetweetJoinTupleEntry* retweetJoinTuple
, int& tweetTweetIdRetweetRetweetTweetId
);



UserJoinTupleEntry* putUserJoinTupleIfAbsent(UserJoinTupleMap* userJoinTupleMap, int& userIdTweetUserId, UserTweetVertexValueMap* userTweetVertexValueMap );


UserProjectedTupleEntry* putUserProjectedTuple(UserProjectedTupleMap* userProjectedTupleMap
, int& userIdTweetUserId
);



UserJoinTupleEntry** getUserJoinTuplePointer(UserJoinTupleMap* userJoinTupleMap
, int& userIdTweetUserId
);
void removeJoinTupleEntry(UserJoinTupleMap* userJoinTupleMap, UserJoinTupleEntry** entryPointer);

int deleteUserProjectedTupleIfZero(UserProjectedTupleMap* userProjectedTupleMap
, int& userIdTweetUserId 
);
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& userIdTweetUserId,int& tweetTweetIdRetweetRetweetTweetId, UserTweetVertexValueMap* userTweetVertexValueMap, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap );


TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap
);



TweetJoinTupleEntry** getTweetJoinTuplePointer(TweetJoinTupleMap* tweetJoinTupleMap
, int& userIdTweetUserId
, int& tweetTweetIdRetweetRetweetTweetId
);
void removeJoinTupleEntry(TweetJoinTupleMap* tweetJoinTupleMap, TweetJoinTupleEntry** entryPointer);

int deleteTweetProjectedTupleIfZero(TweetProjectedTupleMap* tweetProjectedTupleMap
);
RetweetJoinTupleEntry* putRetweetJoinTupleIfAbsent(RetweetJoinTupleMap* retweetJoinTupleMap, int& tweetTweetIdRetweetRetweetTweetId, TweetRetweetVertexValueMap* tweetRetweetVertexValueMap );


RetweetProjectedTupleEntry* putRetweetProjectedTuple(RetweetProjectedTupleMap* retweetProjectedTupleMap
);



RetweetJoinTupleEntry** getRetweetJoinTuplePointer(RetweetJoinTupleMap* retweetJoinTupleMap
, int& tweetTweetIdRetweetRetweetTweetId
);
void removeJoinTupleEntry(RetweetJoinTupleMap* retweetJoinTupleMap, RetweetJoinTupleEntry** entryPointer);

int deleteRetweetProjectedTupleIfZero(RetweetProjectedTupleMap* retweetProjectedTupleMap
);
#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

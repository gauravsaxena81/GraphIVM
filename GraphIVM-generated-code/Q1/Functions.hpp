#include "DataStructures.hpp"

#ifndef DATASTRUCTURES_FUNCTIONS_HPP_
#define DATASTRUCTURES_FUNCTIONS_HPP_

UserTweetVertexValueEntry* putUserTweetVertexValueIfAbsent(UserTweetVertexValueMap* userTweetVertexValueMap, int& userIdTweetUserId);
void deleteUserTweetVertexValueIfZero(UserTweetVertexValueMap* userTweetVertexValueMap, UserJoinTupleEntry* userJoinTuple
, int& userIdTweetUserId
);

void deleteUserTweetVertexValueIfZero(UserTweetVertexValueMap* userTweetVertexValueMap, TweetJoinTupleEntry* tweetJoinTuple
, int& userIdTweetUserId
);



UserJoinTupleEntry* putUserJoinTupleIfAbsent(UserJoinTupleMap* userJoinTupleMap, int& userIdTweetUserId, UserTweetVertexValueMap* userTweetVertexValueMap );


UserProjectedTupleEntry* putUserProjectedTuple(UserProjectedTupleMap* userProjectedTupleMap
, int& userId
);



UserJoinTupleEntry** getUserJoinTuplePointer(UserJoinTupleMap* userJoinTupleMap
, int& userIdTweetUserId
);
void removeJoinTupleEntry(UserJoinTupleMap* userJoinTupleMap, UserJoinTupleEntry** entryPointer);

int deleteUserProjectedTupleIfZero(UserProjectedTupleMap* userProjectedTupleMap
, int& userId 
);
TweetJoinTupleEntry* putTweetJoinTupleIfAbsent(TweetJoinTupleMap* tweetJoinTupleMap, int& userIdTweetUserId, UserTweetVertexValueMap* userTweetVertexValueMap );


TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap
, int& userIdTweetUserId
, int& tweetId
, std::string& tweetDate
);



TweetJoinTupleEntry** getTweetJoinTuplePointer(TweetJoinTupleMap* tweetJoinTupleMap
, int& userIdTweetUserId
);
void removeJoinTupleEntry(TweetJoinTupleMap* tweetJoinTupleMap, TweetJoinTupleEntry** entryPointer);

int deleteTweetProjectedTupleIfZero(TweetProjectedTupleMap* tweetProjectedTupleMap
, int& userIdTweetUserId 
, int& tweetId 
, std::string& tweetDate 
);
#endif /* DATASTRUCTURES_FUNCTIONS_HPP_ */

#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


TweetProjectedTupleEntry* putTweetProjectedTuple(TweetProjectedTupleMap* tweetProjectedTupleMap) {
    TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new TweetProjectedTupleEntry;
       tweetProjectedTupleMap->tweetProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}
int deleteTweetProjectedTupleIfZero(TweetProjectedTupleMap* tweetProjectedTupleMap) {
    TweetProjectedTupleEntry* candidate = tweetProjectedTupleMap->tweetProjectedTupleEntryArray[0];
    if(--candidate->count == 0) {
        tweetProjectedTupleMap->tweetProjectedTupleEntryArray[0] = NULL;
        delete candidate;
    }
    return --(tweetProjectedTupleMap->size);
}



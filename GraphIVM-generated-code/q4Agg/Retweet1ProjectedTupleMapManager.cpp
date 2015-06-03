#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


Retweet1ProjectedTupleEntry* putRetweet1ProjectedTuple(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap) {
    Retweet1ProjectedTupleEntry* candidate = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new Retweet1ProjectedTupleEntry;
       retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}
int deleteRetweet1ProjectedTupleIfZero(Retweet1ProjectedTupleMap* retweet1ProjectedTupleMap) {
    Retweet1ProjectedTupleEntry* candidate = retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[0];
    if(--candidate->count == 0) {
        retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[0] = NULL;
        delete candidate;
    }
    return --(retweet1ProjectedTupleMap->size);
}



#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


Retweet2ProjectedTupleEntry* putRetweet2ProjectedTuple(Retweet2ProjectedTupleMap* retweet2ProjectedTupleMap) {
    Retweet2ProjectedTupleEntry* candidate = retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new Retweet2ProjectedTupleEntry;
       retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}



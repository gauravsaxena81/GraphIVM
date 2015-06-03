#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


Retweet4ProjectedTupleEntry* putRetweet4ProjectedTuple(Retweet4ProjectedTupleMap* retweet4ProjectedTupleMap) {
    Retweet4ProjectedTupleEntry* candidate = retweet4ProjectedTupleMap->retweet4ProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new Retweet4ProjectedTupleEntry;
       retweet4ProjectedTupleMap->retweet4ProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}



#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


Retweet3ProjectedTupleEntry* putRetweet3ProjectedTuple(Retweet3ProjectedTupleMap* retweet3ProjectedTupleMap) {
    Retweet3ProjectedTupleEntry* candidate = retweet3ProjectedTupleMap->retweet3ProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new Retweet3ProjectedTupleEntry;
       retweet3ProjectedTupleMap->retweet3ProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}



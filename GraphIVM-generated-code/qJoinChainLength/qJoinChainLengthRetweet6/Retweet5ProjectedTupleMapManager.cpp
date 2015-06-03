#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


Retweet5ProjectedTupleEntry* putRetweet5ProjectedTuple(Retweet5ProjectedTupleMap* retweet5ProjectedTupleMap) {
    Retweet5ProjectedTupleEntry* candidate = retweet5ProjectedTupleMap->retweet5ProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new Retweet5ProjectedTupleEntry;
       retweet5ProjectedTupleMap->retweet5ProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}



#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


Follower2ProjectedTupleEntry* putFollower2ProjectedTuple(Follower2ProjectedTupleMap* follower2ProjectedTupleMap) {
    Follower2ProjectedTupleEntry* candidate = follower2ProjectedTupleMap->follower2ProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new Follower2ProjectedTupleEntry;
       follower2ProjectedTupleMap->follower2ProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}



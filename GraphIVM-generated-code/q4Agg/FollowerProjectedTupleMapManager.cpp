#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


FollowerProjectedTupleEntry* putFollowerProjectedTuple(FollowerProjectedTupleMap* followerProjectedTupleMap) {
    FollowerProjectedTupleEntry* candidate = followerProjectedTupleMap->followerProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new FollowerProjectedTupleEntry;
       followerProjectedTupleMap->followerProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}
int deleteFollowerProjectedTupleIfZero(FollowerProjectedTupleMap* followerProjectedTupleMap) {
    FollowerProjectedTupleEntry* candidate = followerProjectedTupleMap->followerProjectedTupleEntryArray[0];
    if(--candidate->count == 0) {
        followerProjectedTupleMap->followerProjectedTupleEntryArray[0] = NULL;
        delete candidate;
    }
    return --(followerProjectedTupleMap->size);
}



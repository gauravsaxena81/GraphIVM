#include "DataStructures.hpp"
#include "Functions.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>


RetweetProjectedTupleEntry* putRetweetProjectedTuple(RetweetProjectedTupleMap* retweetProjectedTupleMap) {
    RetweetProjectedTupleEntry* candidate = retweetProjectedTupleMap->retweetProjectedTupleEntryArray[0];
    if(candidate)
       candidate->count++;
    else {
       candidate = new RetweetProjectedTupleEntry;
       retweetProjectedTupleMap->retweetProjectedTupleEntryArray[0] = candidate;
    }
    return candidate;
}
int deleteRetweetProjectedTupleIfZero(RetweetProjectedTupleMap* retweetProjectedTupleMap) {
    RetweetProjectedTupleEntry* candidate = retweetProjectedTupleMap->retweetProjectedTupleEntryArray[0];
    if(--candidate->count == 0) {
        retweetProjectedTupleMap->retweetProjectedTupleEntryArray[0] = NULL;
        delete candidate;
    }
    return --(retweetProjectedTupleMap->size);
}



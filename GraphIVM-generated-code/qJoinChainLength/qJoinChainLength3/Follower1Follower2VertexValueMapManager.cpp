#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower1IdUser2Id ) __attribute__((always_inline));
static void ensureSize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);
static void increaseArraySize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap);

__inline static void ensureSize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
	if(follower1Follower2VertexValueMap->size >= follower1Follower2VertexValueMap->capacity * 0.8) {
		increaseArraySize(follower1Follower2VertexValueMap);
	}
}
static void increaseArraySize(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap) {
	int newcapacity = follower1Follower2VertexValueMap->capacity * 2;
	Follower1Follower2VertexValueEntry** newFollower1Follower2VertexValueEntryArray = new Follower1Follower2VertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower1Follower2VertexValueMap->capacity; i++) {
		Follower1Follower2VertexValueEntry* p = follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower1IdUser2Id );
			h = h & (newcapacity - 1);
			newFollower1Follower2VertexValueEntryArray[h] = p;
		}
	}
	delete(follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray);
	follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray = newFollower1Follower2VertexValueEntryArray;
	follower1Follower2VertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower1Follower2VertexValueEntry* putFollower1Follower2VertexValueIfAbsent(Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap, int& follower1IdUser2Id ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower1IdUser2Id ); 
	h = h & (follower1Follower2VertexValueMap->capacity - 1);
	Follower1Follower2VertexValueEntry* candidate = follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray[h];
	Follower1Follower2VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower1IdUser2Id == (follower1IdUser2Id)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	ensureSize(follower1Follower2VertexValueMap);
	Follower1Follower2VertexValueEntry* p = new Follower1Follower2VertexValueEntry;
	p->follower1IdUser2Id = follower1IdUser2Id;
	if(lastP)
		lastP->next = p;
	else
		follower1Follower2VertexValueMap->follower1Follower2VertexValueEntryArray[h] = p;
	follower1Follower2VertexValueMap->size++;
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower1IdUser2Id ) {
	h ^=  follower1IdUser2Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower4IdUser5Id ) __attribute__((always_inline));
static void ensureSize(Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap);
static void increaseArraySize(Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap);

__inline static void ensureSize(Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap) {
	if(follower4Follower5VertexValueMap->size >= follower4Follower5VertexValueMap->capacity * 0.8) {
		increaseArraySize(follower4Follower5VertexValueMap);
	}
}
static void increaseArraySize(Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap) {
	int newcapacity = follower4Follower5VertexValueMap->capacity * 2;
	Follower4Follower5VertexValueEntry** newFollower4Follower5VertexValueEntryArray = new Follower4Follower5VertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower4Follower5VertexValueMap->capacity; i++) {
		Follower4Follower5VertexValueEntry* p = follower4Follower5VertexValueMap->follower4Follower5VertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower4IdUser5Id );
			h = h & (newcapacity - 1);
			newFollower4Follower5VertexValueEntryArray[h] = p;
		}
	}
	delete(follower4Follower5VertexValueMap->follower4Follower5VertexValueEntryArray);
	follower4Follower5VertexValueMap->follower4Follower5VertexValueEntryArray = newFollower4Follower5VertexValueEntryArray;
	follower4Follower5VertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower4Follower5VertexValueEntry* putFollower4Follower5VertexValueIfAbsent(Follower4Follower5VertexValueMap* follower4Follower5VertexValueMap, int& follower4IdUser5Id ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower4IdUser5Id ); 
	h = h & (follower4Follower5VertexValueMap->capacity - 1);
	Follower4Follower5VertexValueEntry* candidate = follower4Follower5VertexValueMap->follower4Follower5VertexValueEntryArray[h];
	Follower4Follower5VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower4IdUser5Id == (follower4IdUser5Id)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	ensureSize(follower4Follower5VertexValueMap);
	Follower4Follower5VertexValueEntry* p = new Follower4Follower5VertexValueEntry;
	p->follower4IdUser5Id = follower4IdUser5Id;
	if(lastP)
		lastP->next = p;
	else
		follower4Follower5VertexValueMap->follower4Follower5VertexValueEntryArray[h] = p;
	follower4Follower5VertexValueMap->size++;
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower4IdUser5Id ) {
	h ^=  follower4IdUser5Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

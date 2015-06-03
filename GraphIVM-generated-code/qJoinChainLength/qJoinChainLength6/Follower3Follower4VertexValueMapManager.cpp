#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower3IdUser4Id ) __attribute__((always_inline));
static void ensureSize(Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap);
static void increaseArraySize(Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap);

__inline static void ensureSize(Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap) {
	if(follower3Follower4VertexValueMap->size >= follower3Follower4VertexValueMap->capacity * 0.8) {
		increaseArraySize(follower3Follower4VertexValueMap);
	}
}
static void increaseArraySize(Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap) {
	int newcapacity = follower3Follower4VertexValueMap->capacity * 2;
	Follower3Follower4VertexValueEntry** newFollower3Follower4VertexValueEntryArray = new Follower3Follower4VertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower3Follower4VertexValueMap->capacity; i++) {
		Follower3Follower4VertexValueEntry* p = follower3Follower4VertexValueMap->follower3Follower4VertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower3IdUser4Id );
			h = h & (newcapacity - 1);
			newFollower3Follower4VertexValueEntryArray[h] = p;
		}
	}
	delete(follower3Follower4VertexValueMap->follower3Follower4VertexValueEntryArray);
	follower3Follower4VertexValueMap->follower3Follower4VertexValueEntryArray = newFollower3Follower4VertexValueEntryArray;
	follower3Follower4VertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower3Follower4VertexValueEntry* putFollower3Follower4VertexValueIfAbsent(Follower3Follower4VertexValueMap* follower3Follower4VertexValueMap, int& follower3IdUser4Id ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower3IdUser4Id ); 
	h = h & (follower3Follower4VertexValueMap->capacity - 1);
	Follower3Follower4VertexValueEntry* candidate = follower3Follower4VertexValueMap->follower3Follower4VertexValueEntryArray[h];
	Follower3Follower4VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower3IdUser4Id == (follower3IdUser4Id)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	ensureSize(follower3Follower4VertexValueMap);
	Follower3Follower4VertexValueEntry* p = new Follower3Follower4VertexValueEntry;
	p->follower3IdUser4Id = follower3IdUser4Id;
	if(lastP)
		lastP->next = p;
	else
		follower3Follower4VertexValueMap->follower3Follower4VertexValueEntryArray[h] = p;
	follower3Follower4VertexValueMap->size++;
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower3IdUser4Id ) {
	h ^=  follower3IdUser4Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

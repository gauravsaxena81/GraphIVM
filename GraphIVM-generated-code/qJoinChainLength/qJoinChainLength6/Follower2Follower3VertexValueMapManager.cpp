#include <string>
#include <stdlib.h>
#include "DataStructures.hpp"
#include "Functions.hpp"

//%1 - vertex value attributes
inline static void hashCode(int& h, int& follower2IdUser3Id ) __attribute__((always_inline));
static void ensureSize(Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap);
static void increaseArraySize(Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap);

__inline static void ensureSize(Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap) {
	if(follower2Follower3VertexValueMap->size >= follower2Follower3VertexValueMap->capacity * 0.8) {
		increaseArraySize(follower2Follower3VertexValueMap);
	}
}
static void increaseArraySize(Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap) {
	int newcapacity = follower2Follower3VertexValueMap->capacity * 2;
	Follower2Follower3VertexValueEntry** newFollower2Follower3VertexValueEntryArray = new Follower2Follower3VertexValueEntry*[newcapacity]();
	for(int i = 0; i < follower2Follower3VertexValueMap->capacity; i++) {
		Follower2Follower3VertexValueEntry* p = follower2Follower3VertexValueMap->follower2Follower3VertexValueEntryArray[i];
		if(p) {
			//Store the first element of p to the newprojectedTupleEntryArray array. Rest are chained to this
			int h = 31;
			//%1 - vertex value attribute
			hashCode(h, p->follower2IdUser3Id );
			h = h & (newcapacity - 1);
			newFollower2Follower3VertexValueEntryArray[h] = p;
		}
	}
	delete(follower2Follower3VertexValueMap->follower2Follower3VertexValueEntryArray);
	follower2Follower3VertexValueMap->follower2Follower3VertexValueEntryArray = newFollower2Follower3VertexValueEntryArray;
	follower2Follower3VertexValueMap->capacity = newcapacity;
}
//%4 - vertex value attributes
Follower2Follower3VertexValueEntry* putFollower2Follower3VertexValueIfAbsent(Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap, int& follower2IdUser3Id ) {
	int h = 31;
	//%1 - vertex value attribute values
	hashCode(h, follower2IdUser3Id ); 
	h = h & (follower2Follower3VertexValueMap->capacity - 1);
	Follower2Follower3VertexValueEntry* candidate = follower2Follower3VertexValueMap->follower2Follower3VertexValueEntryArray[h];
	Follower2Follower3VertexValueEntry* lastP = candidate;
	while(candidate) {
		if(
		  candidate->follower2IdUser3Id == (follower2IdUser3Id)
	      ) {
			return candidate;
		} else {
			lastP = candidate;
			candidate = candidate->next;
		}
	}
	ensureSize(follower2Follower3VertexValueMap);
	Follower2Follower3VertexValueEntry* p = new Follower2Follower3VertexValueEntry;
	p->follower2IdUser3Id = follower2IdUser3Id;
	if(lastP)
		lastP->next = p;
	else
		follower2Follower3VertexValueMap->follower2Follower3VertexValueEntryArray[h] = p;
	follower2Follower3VertexValueMap->size++;
	return p;
}
//arguments based on key
//void remove(%1$s%2$sVertexValueMap* %1$s%2$sVertexValueMap, int userId) {
	//implement later
	//if count = 0, delete auto
	//%1$s%2$sVertexValueMap->size--;
//}
//%1 - vertex value attributes
__inline static void hashCode(int& h, int& follower2IdUser3Id ) {
	h ^=  follower2IdUser3Id + 0x9e3779b9 + (h<<6) + (h>>2);
}

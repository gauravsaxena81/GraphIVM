#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

//%1 - tableName, %2 - attributes, both join and projected
void addFollower1Tuple(int& user1IdFollower2Id, int& follower1Id );
void addFollower2Tuple(int& user1IdFollower2Id, int& user2IdFollower3Id );
void addFollower3Tuple(int& user2IdFollower3Id, int& user3IdTweetUserId );
void addTweetTuple(int& user3IdTweetUserId, int& tweetId, bool doBfs );

void scan();

int count = 0;
Follower1JoinTupleMap* follower1JoinTupleMap = new Follower1JoinTupleMap;
Follower2JoinTupleMap* follower2JoinTupleMap = new Follower2JoinTupleMap;
Follower3JoinTupleMap* follower3JoinTupleMap = new Follower3JoinTupleMap;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap = new Follower1Follower2VertexValueMap;
Follower2Follower3VertexValueMap* follower2Follower3VertexValueMap = new Follower2Follower3VertexValueMap;
Follower3TweetVertexValueMap* follower3TweetVertexValueMap = new Follower3TweetVertexValueMap;

using namespace std;

int main() {
	try {//PT
	      pqxx::connection C("dbname=bsmagen100k user=gaurav password=gaurav \
	      hostaddr=127.0.0.1 port=5432");
	      if (C.is_open()) {
		 cout << "Opened database successfully: " << C.dbname() << endl;
	      } else {
		 cout << "Can't open database" << endl;
		 return -1;
	      }
	      std::string sql = "select * from follower";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NU(C);
	      
	      /* Execute SQL query */
	      pqxx::result RU( NU.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Follower Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RU.begin(); c != RU.end(); ++c) {
			int user = c[0].as<long>();
			int foll = c[1].as<long>();
			addFollower1Tuple(foll, user); 
			addFollower2Tuple(user, foll); 
			addFollower3Tuple(user, foll); 
	      }
		cout<<"End Follower Table Load"<<endl;
		NU.commit();
		
	      sql = "select user_id, tweet_id from tweet";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NT(C);
	      
	      /* Execute SQL query */
	      pqxx::result RT( NT.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Tweet Table Load"<<endl;
		int i = 0;
	      for (pqxx::result::const_iterator c = RT.begin(); c != RT.end(); ++c) {
			if(i++ % 1000 == 0)
				cout<<i<<endl;
			int c0 = c[0].as<int>();
			int c1 = c[1].as<int>();
			addTweetTuple(c0, c1, false);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
	      /* Create SQL statement */
	} catch (const std::exception &e){
	      cerr << e.what() << std::endl;
	}
	int MAX = 50000;
	int X = 10000000;
	auto start_time = std::chrono::steady_clock::now();	
	for(int i = 0; i < MAX; i++) {
		int c0 = 4571;
		int c1 = X + i;
		addTweetTuple(c0, c1, false);
	} 
	scan();
	double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
	std::cout << time_in_seconds<<"ms"<<std::endl;
	std::cout<<count<<endl;
	return 0;
}
//%1 - all projected attributes
void api(int& follower1Id, int& tweetId ) {}
void bfsFollower1Diff(Follower1JoinTupleEntry* follower1Jt , Follower1ProjectedTupleEntry* follower1Pt) {}

//%2 - both join and projected attributes
void bfsFollower2Diff(Follower2JoinTupleEntry* follower2Jt ) {}

//%2 - both join and projected attributes
void bfsFollower3Diff(Follower3JoinTupleEntry* follower3Jt ) {}

//%2 - both join and projected attributes
void bfsTweetDiff(TweetJoinTupleEntry* tweetJt, TweetProjectedTupleEntry* tweetPt) {
	for(Follower3JoinTupleEntry* j : tweetJt->follower3TweetVertexValue->follower3JoinTupleList) {
		for(Follower2JoinTupleEntry* k : j->follower2Follower3VertexValue->follower2JoinTupleList) {
			for(Follower1JoinTupleEntry* l : k->follower1Follower2VertexValue->follower1JoinTupleList) {
				//Follower1ProjectedTupleIteratorNext nextUserProjTuple = follower1ProjectedTupleIterator(l->follower1ProjectedTupleMap);
				for(int index1 = 0; index1 < l->follower1ProjectedTupleMap->capacity; index1++) {
					Follower1ProjectedTupleEntry* currentP = l->follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[index1];
						while(currentP) {
							for(int m = 0; m < currentP->count; m++) 
								api(currentP->user1Id, tweetPt->tweetId);
							currentP = currentP->next;
						}
				}
				//for(Follower1ProjectedTupleEntry* o = nextUserProjTuple(); o ; o = nextUserProjTuple() ) {
				//	for(int m = 0; m < o->count; m++) {
						//cout<<o->user1Id<<","<<tweetPt->tweetId<<endl;
				//		api(o->user1Id, tweetPt->tweetId);
				//	}
				//}
			}
		}
	}
}
void scan() {
	TweetJoinTupleIteratorNext nextTweetJoinTuple = tweetJoinTupleIterator(tweetJoinTupleMap);
	for(TweetJoinTupleEntry* tweetJt = nextTweetJoinTuple(); tweetJt ; tweetJt = nextTweetJoinTuple() ) {
	TweetProjectedTupleIteratorNext nextTweetProjectedTuple = tweetProjectedTupleIterator(tweetJt->tweetProjectedTupleMap);
	for(TweetProjectedTupleEntry* tweetPt = nextTweetProjectedTuple(); tweetPt ; tweetPt = nextTweetProjectedTuple() ) {
	for(Follower3JoinTupleEntry* j : tweetJt->follower3TweetVertexValue->follower3JoinTupleList) {
		for(Follower2JoinTupleEntry* k : j->follower2Follower3VertexValue->follower2JoinTupleList) {
			for(Follower1JoinTupleEntry* l : k->follower1Follower2VertexValue->follower1JoinTupleList) {
				Follower1ProjectedTupleIteratorNext nextUserProjTuple = follower1ProjectedTupleIterator(l->follower1ProjectedTupleMap);
				for(Follower1ProjectedTupleEntry* o = nextUserProjTuple(); o ; o = nextUserProjTuple() ) {
					for(int m = 0; m < o->count; m++) {
						//cout<<o->follower1Id<<","<<tweetPt->tweetId<<endl;
						api(o->user1Id, tweetPt->tweetId);
					}
				}
			}
		}
	}
	}
	}
}
//%2 - both join and projected attributes

//%2 - both join and projected attributes
void addFollower1Tuple(int& user1IdFollower2Id, int& follower1Id ) {
    //%3 - join attributes, %4 - vertex value maps
	Follower1JoinTupleEntry* jt = putFollower1JoinTupleIfAbsent(follower1JoinTupleMap, user1IdFollower2Id , follower1Follower2VertexValueMap );
	//%3 - projected attributes
	Follower1ProjectedTupleEntry* pt = putFollower1ProjectedTuple(jt->follower1ProjectedTupleMap, follower1Id );
	bfsFollower1Diff(jt , pt);
}

//%2 - both join and projected attributes
void addFollower2Tuple(int& user1IdFollower2Id, int& user2IdFollower3Id ) {
    //%3 - join attributes, %4 - vertex value maps
	Follower2JoinTupleEntry* jt = putFollower2JoinTupleIfAbsent(follower2JoinTupleMap, user1IdFollower2Id, user2IdFollower3Id , follower1Follower2VertexValueMap, follower2Follower3VertexValueMap );
	//%3 - projected attributes
	bfsFollower2Diff(jt );
}

//%2 - both join and projected attributes
void addFollower3Tuple(int& user2IdFollower3Id, int& user3IdTweetUserId ) {
    //%3 - join attributes, %4 - vertex value maps
	Follower3JoinTupleEntry* jt = putFollower3JoinTupleIfAbsent(follower3JoinTupleMap, user2IdFollower3Id, user3IdTweetUserId , follower2Follower3VertexValueMap, follower3TweetVertexValueMap );
	//%3 - projected attributes
	bfsFollower3Diff(jt );
}

//%2 - both join and projected attributes
void addTweetTuple(int& user3IdTweetUserId, int& tweetId, bool doBfs ) {
    //%3 - join attributes, %4 - vertex value maps
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, user3IdTweetUserId , follower3TweetVertexValueMap );
	//%3 - projected attributes
	TweetProjectedTupleEntry* pt = putTweetProjectedTuple(jt->tweetProjectedTupleMap, tweetId );
	if(doBfs)
	bfsTweetDiff(jt , pt);
}

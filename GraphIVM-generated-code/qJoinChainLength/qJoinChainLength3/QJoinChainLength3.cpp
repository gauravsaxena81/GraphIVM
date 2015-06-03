#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

//%1 - tableName, %2 - attributes, both join and projected
void addFollower1Tuple(int& follower1IdUser2Id, int& user1Id );
void addFollower2Tuple(int& follower1IdUser2Id, int& follower2IdTweetUserId );
void addTweetTuple(int& follower2IdTweetUserId, int& tweetId, bool doBfs );

void scan();
int count;
Follower1JoinTupleMap* follower1JoinTupleMap = new Follower1JoinTupleMap;
Follower2JoinTupleMap* follower2JoinTupleMap = new Follower2JoinTupleMap;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
Follower1Follower2VertexValueMap* follower1Follower2VertexValueMap = new Follower1Follower2VertexValueMap;
Follower2TweetVertexValueMap* follower2TweetVertexValueMap = new Follower2TweetVertexValueMap;

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
void api(int& user1Id, int& tweetId ) {count++;}
void bfsFollower1Diff(Follower1JoinTupleEntry* follower1Jt , Follower1ProjectedTupleEntry* follower1Pt) {}

//%2 - both join and projected attributes
void bfsFollower2Diff(Follower2JoinTupleEntry* follower2Jt ) {}

//%2 - both join and projected attributes
void bfsTweetDiff(TweetJoinTupleEntry* tweetJt, TweetProjectedTupleEntry* tweetPt) {
	for(Follower2JoinTupleEntry* j : tweetJt->follower2TweetVertexValue->follower2JoinTupleList) {
		for(Follower1JoinTupleEntry* l : j->follower1Follower2VertexValue->follower1JoinTupleList) {
			for(int index1 = 0; index1 < l->follower1ProjectedTupleMap->capacity; index1++) {
				Follower1ProjectedTupleEntry* currentP = l->follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[index1];
				while(currentP) {
					for(int m = 0; m < currentP->count; m++)
						api(currentP->user1Id, tweetPt->tweetId);
					currentP = currentP->next;
				}
			}
		}
	}
}

void scan() {
	TweetJoinTupleIteratorNext nextTweetJoinTuple = tweetJoinTupleIterator(tweetJoinTupleMap);
	for(TweetJoinTupleEntry* tweetJt = nextTweetJoinTuple(); tweetJt ; tweetJt = nextTweetJoinTuple() ) {
	TweetProjectedTupleIteratorNext nextTweetProjectedTuple = tweetProjectedTupleIterator(tweetJt->tweetProjectedTupleMap);
	for(TweetProjectedTupleEntry* tweetPt = nextTweetProjectedTuple(); tweetPt ; tweetPt = nextTweetProjectedTuple() ) {
	for(Follower2JoinTupleEntry* j : tweetJt->follower2TweetVertexValue->follower2JoinTupleList) {
		for(Follower1JoinTupleEntry* l : j->follower1Follower2VertexValue->follower1JoinTupleList) {
			Follower1ProjectedTupleIteratorNext nextUserProjTuple = follower1ProjectedTupleIterator(l->follower1ProjectedTupleMap);
			for(Follower1ProjectedTupleEntry* o = nextUserProjTuple(); o ; o = nextUserProjTuple() ) {
				for(int m = 0; m < o->count; m++) {
					//cout<<o->user1Id<<","<<tweetPt->tweetId<<endl;
					api(o->user1Id, tweetPt->tweetId);
				}
			}
		}
	}
	}
	}
}
//%2 - both join and projected attributes

//%2 - both join and projected attributes
void addFollower1Tuple(int& follower1IdUser2Id, int& user1Id ) {
    //%3 - join attributes, %4 - vertex value maps
	Follower1JoinTupleEntry* jt = putFollower1JoinTupleIfAbsent(follower1JoinTupleMap, follower1IdUser2Id , follower1Follower2VertexValueMap );
	//%3 - projected attributes
	Follower1ProjectedTupleEntry* pt = putFollower1ProjectedTuple(jt->follower1ProjectedTupleMap, user1Id );
	bfsFollower1Diff(jt , pt);
}

//%2 - both join and projected attributes
void addFollower2Tuple(int& follower1IdUser2Id, int& follower2IdTweetUserId ) {
    //%3 - join attributes, %4 - vertex value maps
	Follower2JoinTupleEntry* jt = putFollower2JoinTupleIfAbsent(follower2JoinTupleMap, follower1IdUser2Id, follower2IdTweetUserId , follower1Follower2VertexValueMap, follower2TweetVertexValueMap );
	//%3 - projected attributes
	bfsFollower2Diff(jt );
}

//%2 - both join and projected attributes
void addTweetTuple(int& follower2IdTweetUserId, int& tweetId, bool doBfs ) {
    //%3 - join attributes, %4 - vertex value maps
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, follower2IdTweetUserId , follower2TweetVertexValueMap );
	//%3 - projected attributes
	TweetProjectedTupleEntry* pt = putTweetProjectedTuple(jt->tweetProjectedTupleMap, tweetId );
	if(doBfs)
	bfsTweetDiff(jt , pt);
}

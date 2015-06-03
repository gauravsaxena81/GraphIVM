#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include <pqxx/pqxx>

//%1 - tableName, %2 - attributes, both join and projected
void addFollower1Tuple(int& user1IdFollower2Id, int& follower1Id );
void addFollower2Tuple(int& user1IdFollower2Id, int& user2IdTweetUserId );
void addTweetTuple(int& user2IdTweetUserId, int& tweetId, bool doBfs );

void scan();
int count = 0;
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
	      std::string sql = "select user_id, follower_id from follower";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NU(C);
	      
	      /* Execute SQL query */
	      pqxx::result RU( NU.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start User Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RU.begin(); c != RU.end(); ++c) {
			int user = c[0].as<int>();
			int follower = c[1].as<int>();
			addFollower1Tuple(user, follower); 
			addFollower2Tuple(follower, user); 
	      }
		cout<<"End User Table Load"<<endl;
		NU.commit();
		
	      sql = "select user_id, tweet_id from tweet";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NT(C);
	      
	      /* Execute SQL query */
	      pqxx::result RT( NT.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Tweet Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RT.begin(); c != RT.end(); ++c) {
			int c0 = c[0].as<int>();
			int c1 = c[1].as<int>();
			addTweetTuple(c0, c1, false);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
		const int MAX = 50000;
		int X = 100000;
		int fanout = 100;
		for(int j = 0; j < fanout; j++) {
			int u2 = 2 * X + j;
			int u0 = X + j;
			addFollower1Tuple(u2, u0);
		}
		for(int i = 0; i < MAX; i++) {
			for(int j = 0; j < fanout; j++) {
				int u1 = X + i;
				int u2 = 2 * X + j;
				addFollower2Tuple(u2, u1);
			}
		}
		cout<<"Start Inserting Tweet"<<endl;
		const auto start_time = std::chrono::steady_clock::now();
		for(int i = 0; i < MAX; i++) {
			int u1 = X + i;
			int t1 = X + i;
			addTweetTuple(u1, t1, true);
		}
		//scan();
		double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
		std::cout << time_in_seconds<<"ms"<<std::endl;
		std::cout << "count:"<<count<<std::endl;
	      C.disconnect();
	} catch (const std::exception &e){
		cout<<"Program Exited Abnotmally"<<endl;
	      cerr << e.what() << std::endl;
	}
	return 0;
}
//%1 - all projected attributes
void api(int& follower1Id, int& tweetId ) {count++;}
void bfsFollower1Diff(Follower1JoinTupleEntry* follower1Jt , Follower1ProjectedTupleEntry* follower1Pt) {}

//%2 - both join and projected attributes
void bfsFollower2Diff(Follower2JoinTupleEntry* follower2Jt ) {}

//%2 - both join and projected attributes
void bfsTweetDiff(TweetJoinTupleEntry* tweetJt , TweetProjectedTupleEntry* tweetPt) {
	for(Follower2JoinTupleEntry* i : tweetJt->follower2TweetVertexValue->follower2JoinTupleList) {
		for(Follower1JoinTupleEntry* j : i->follower1Follower2VertexValue->follower1JoinTupleList) {
			for(int index1 = 0; index1 < j->follower1ProjectedTupleMap->capacity; index1++) {
				int max = tweetPt->count * i->follower2ProjectedTupleMap->follower2ProjectedTupleEntryArray[0]->count;
				Follower1ProjectedTupleEntry* currentP = j->follower1ProjectedTupleMap->follower1ProjectedTupleEntryArray[index1];
				while(currentP) {
					for(int m = 0; m < max * currentP->count; m++)
						api(currentP->follower1Id, tweetPt->tweetId);
						//cout<<currentP->followerId<<","<<tweetPt->tweetId<<endl;
					currentP = currentP->next;
				}
			}
		}
	}	
}
void scan() {
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
void addFollower2Tuple(int& user1IdFollower2Id, int& user2IdTweetUserId ) {
    //%3 - join attributes, %4 - vertex value maps
	Follower2JoinTupleEntry* jt = putFollower2JoinTupleIfAbsent(follower2JoinTupleMap, user1IdFollower2Id, user2IdTweetUserId , follower1Follower2VertexValueMap, follower2TweetVertexValueMap );
	//%3 - projected attributes
	Follower2ProjectedTupleEntry* pt = putFollower2ProjectedTuple(jt->follower2ProjectedTupleMap);
	bfsFollower2Diff(jt );
}

//%2 - both join and projected attributes
void addTweetTuple(int& user2IdTweetUserId, int& tweetId, bool doBfs) {
    //%3 - join attributes, %4 - vertex value maps
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, user2IdTweetUserId , follower2TweetVertexValueMap );
	//%3 - projected attributes
	TweetProjectedTupleEntry* pt = putTweetProjectedTuple(jt->tweetProjectedTupleMap, tweetId );
	if(doBfs)
	bfsTweetDiff(jt , pt);
}

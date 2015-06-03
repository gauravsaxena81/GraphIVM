#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

//%1 - tableName, %2 - attributes, both join and projected
void addRetweet1Tuple(int& r1TweetIdR2RetweetTweetId, int& r1TweetId );
void addRetweet2Tuple(int& r1TweetIdR2RetweetTweetId, int& r2UserIdFUserId );
void addFollowerTuple(int& r2UserIdFUserId, int& followerId, bool doBfs );
void scan();

int count = 0;
Retweet1JoinTupleMap* retweet1JoinTupleMap = new Retweet1JoinTupleMap;
Retweet2JoinTupleMap* retweet2JoinTupleMap = new Retweet2JoinTupleMap;
FollowerJoinTupleMap* followerJoinTupleMap = new FollowerJoinTupleMap;
Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap = new Retweet1Retweet2VertexValueMap;
Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap = new Retweet2FollowerVertexValueMap;

using namespace std;

int main(int argc, char* argv[]) {
	try {//PT
	      pqxx::connection C("dbname=bsmagen100k user=gaurav password=gaurav \
	      hostaddr=127.0.0.1 port=5432");
	      if (C.is_open()) {
		 cout << "Opened database successfully: " << C.dbname() << endl;
	      } else {
		 cout << "Can't open database" << endl;
		 return -1;
	      }
	      std::string sql = "select tweet_id, retweet_tweet_id, user_id from retweet order by 1";
	      //std::string sql = "select tweet_id, retweet_tweet_id, user_id from retweet";

	      /* Create a non-transactional object. */
	      pqxx::nontransaction NU(C);
	      
	      /* Execute SQL query */
	      pqxx::result RU( NU.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Retweet Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RU.begin(); c != RU.end(); ++c) {
			int t = c[0].as<long>();
			int r = c[1].as<long>();
			int u = c[2].as<long>();
			addRetweet1Tuple(t, t); 
			addRetweet2Tuple(r, u); 
	      }
		cout<<"End Retweet Table Load"<<endl;
		NU.commit();
		
	      //sql = "select user_id, follower_id from follower;";
	      sql = "select user_id, follower_id from follower";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NT(C);
	      
	      /* Execute SQL query */
	      pqxx::result RT( NT.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Tweet Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RT.begin(); c != RT.end(); ++c) {
			int u = c[0].as<int>();
			int f = c[1].as<int>();
			addFollowerTuple(u, f, false);
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
		int u = 4831;
		int f = X + i;
		addFollowerTuple(u, f, true);
	}
	//scan(); 
	double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
	std::cout << time_in_seconds<<"ms"<<std::endl;
	std::cout<<count<<endl;
	return 0;
}
//%1 - all projected attributes
void api(int& r1TweetId, int& followerId ) {count++;}
void bfsRetweet1Diff(Retweet1JoinTupleEntry* retweet1Jt , Retweet1ProjectedTupleEntry* retweet1Pt) {}

//%2 - both join and projected attributes
void bfsRetweet2Diff(Retweet2JoinTupleEntry* retweet2Jt ) {}

//%2 - both join and projected attributes
void bfsFollowerDiff(FollowerJoinTupleEntry* followerJt , FollowerProjectedTupleEntry* followerPt) {
	for(int mm = 0; mm < 2; mm++) {
	if(true || followerJt->retweet2FollowerVertexValue->localLookAhead && followerJt->retweet2FollowerVertexValue->globalLookAhead) {
		for(Retweet2JoinTupleEntry* i : followerJt->retweet2FollowerVertexValue->retweet2JoinTupleList) {
			if(true || i->retweet1Retweet2VertexValue->localLookAhead && i->retweet1Retweet2VertexValue->globalLookAhead) {
				if(true || i->retweet1Retweet2VertexValue->retweet1JoinTupleList.size() == 0) {
					followerJt->retweet2FollowerVertexValue->globalLookAhead = false;
				}
				for(Retweet1JoinTupleEntry* j : i->retweet1Retweet2VertexValue->retweet1JoinTupleList) {
					for(int index1 = 0; index1 < j->retweet1ProjectedTupleMap->capacity; index1++) {
						int max = followerPt->count * i->retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[0]->count;
						Retweet1ProjectedTupleEntry* currentP = j->retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[index1];
						while(currentP) {
							for(int m = 0; m < max * currentP->count; m++)
								api(currentP->r1TweetId, followerPt->followerId);
								//cout<<currentP->r1TweetId<<","<<followerPt->followerId<<endl;
							currentP = currentP->next;
						}
					}
				}
			}
		}
	}
	}
}
void scan() {
	for(int index1 = 0; index1 < followerJoinTupleMap->capacity; index1++) {
		FollowerJoinTupleEntry* i = followerJoinTupleMap->followerJoinTupleEntryArray[index1];
		if(i)
		for(int index2 = 0; index2 < i->followerProjectedTupleMap->capacity; index2++) {
			FollowerProjectedTupleEntry* currentP =  i->followerProjectedTupleMap->followerProjectedTupleEntryArray[index2];
			while(currentP) {
				bfsFollowerDiff(i, currentP);
				currentP = currentP->next;
			}
		}
	}
}

//%2 - both join and projected attributes

//%2 - both join and projected attributes
void addRetweet1Tuple(int& r1TweetIdR2RetweetTweetId, int& r1TweetId ) {
    //%3 - join attributes, %4 - vertex value maps
	Retweet1JoinTupleEntry* jt = putRetweet1JoinTupleIfAbsent(retweet1JoinTupleMap, r1TweetIdR2RetweetTweetId , retweet1Retweet2VertexValueMap );
	//%3 - projected attributes
	Retweet1ProjectedTupleEntry* pt = putRetweet1ProjectedTuple(jt->retweet1ProjectedTupleMap, r1TweetId );
	bfsRetweet1Diff(jt , pt);
}

//%2 - both join and projected attributes
void addRetweet2Tuple(int& r1TweetIdR2RetweetTweetId, int& r2UserIdFUserId) {
    //%3 - join attributes, %4 - vertex value maps
	Retweet2JoinTupleEntry* jt = putRetweet2JoinTupleIfAbsent(retweet2JoinTupleMap, r1TweetIdR2RetweetTweetId, r2UserIdFUserId , retweet1Retweet2VertexValueMap, retweet2FollowerVertexValueMap );
	putRetweet2ProjectedTuple(jt->retweet2ProjectedTupleMap);
	//%3 - projected attributes
	bfsRetweet2Diff(jt );
}

//%2 - both join and projected attributes
void addFollowerTuple(int& r2UserIdFUserId, int& followerId, bool doBfs) {
    //%3 - join attributes, %4 - vertex value maps
	FollowerJoinTupleEntry* jt = putFollowerJoinTupleIfAbsent(followerJoinTupleMap, r2UserIdFUserId , retweet2FollowerVertexValueMap );
	//%3 - projected attributes
	FollowerProjectedTupleEntry* pt = putFollowerProjectedTuple(jt->followerProjectedTupleMap, followerId );
	if(doBfs)
	bfsFollowerDiff(jt , pt);
}

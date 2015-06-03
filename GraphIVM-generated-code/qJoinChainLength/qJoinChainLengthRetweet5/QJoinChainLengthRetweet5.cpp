#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

void addRetweet1Tuple(int& r1TweetIdR2RetweetTweetId, int& r1TweetId );
void addRetweet2Tuple(int& r1TweetIdR2RetweetTweetId, int& r2TweetIdR3RetweetTweetId );
void addRetweet3Tuple(int& r2TweetIdR3RetweetTweetId, int& r3TweetIdR4RetweetTweetId );
void addRetweet4Tuple(int& r3TweetIdR4RetweetTweetId, int& r4UserIdFUserId );
void addFollowerTuple(int& r4UserIdFUserId, int& followerId, bool doBfs );

void scan();
int count = 0;
Retweet1JoinTupleMap* retweet1JoinTupleMap = new Retweet1JoinTupleMap;
Retweet2JoinTupleMap* retweet2JoinTupleMap = new Retweet2JoinTupleMap;
Retweet3JoinTupleMap* retweet3JoinTupleMap = new Retweet3JoinTupleMap;
Retweet4JoinTupleMap* retweet4JoinTupleMap = new Retweet4JoinTupleMap;
FollowerJoinTupleMap* followerJoinTupleMap = new FollowerJoinTupleMap;
Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap = new Retweet1Retweet2VertexValueMap;
Retweet2Retweet3VertexValueMap* retweet2Retweet3VertexValueMap = new Retweet2Retweet3VertexValueMap;
Retweet3Retweet4VertexValueMap* retweet3Retweet4VertexValueMap = new Retweet3Retweet4VertexValueMap;
Retweet4FollowerVertexValueMap* retweet4FollowerVertexValueMap = new Retweet4FollowerVertexValueMap;

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
			addRetweet2Tuple(r, t); 
			addRetweet3Tuple(r, t); 
			addRetweet4Tuple(r, u); 
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
void api(int& r1TweetId, int& followerId ) {count++;}
void bfsRetweet1Diff(Retweet1JoinTupleEntry* retweet1Jt , Retweet1ProjectedTupleEntry* retweet1Pt) {}

void bfsRetweet2Diff(Retweet2JoinTupleEntry* retweet2Jt ) {}

void bfsRetweet3Diff(Retweet3JoinTupleEntry* retweet3Jt ) {}

void bfsRetweet4Diff(Retweet4JoinTupleEntry* retweet4Jt ) {}

void bfsFollowerDiff(FollowerJoinTupleEntry* followerJt , FollowerProjectedTupleEntry* followerPt) {
	if(true || followerJt->retweet4FollowerVertexValue->localLookAhead && followerJt->retweet4FollowerVertexValue->globalLookAhead) { 
	for(Retweet4JoinTupleEntry* i2 : followerJt->retweet4FollowerVertexValue->retweet4JoinTupleList) {
		if(true || i2->retweet3Retweet4VertexValue->localLookAhead && i2->retweet3Retweet4VertexValue->globalLookAhead) {
		if(i2->retweet3Retweet4VertexValue->retweet3JoinTupleList.size() == 0) {
			followerJt->retweet4FollowerVertexValue->globalLookAhead = false;
		}
		for(Retweet3JoinTupleEntry* i : i2->retweet3Retweet4VertexValue->retweet3JoinTupleList) {
			if(true || i->retweet2Retweet3VertexValue->localLookAhead && i->retweet2Retweet3VertexValue->globalLookAhead) {
			if(i->retweet2Retweet3VertexValue->retweet2JoinTupleList.size() == 0) {
				followerJt->retweet4FollowerVertexValue->globalLookAhead = false;
			}
			for(Retweet2JoinTupleEntry* i1 : i->retweet2Retweet3VertexValue->retweet2JoinTupleList) {
				if(true || i1->retweet1Retweet2VertexValue->localLookAhead && i1->retweet1Retweet2VertexValue->globalLookAhead) {
				if(i1->retweet1Retweet2VertexValue->retweet1JoinTupleList.size() == 0) {
					followerJt->retweet4FollowerVertexValue->globalLookAhead = false;
				}
				for(Retweet1JoinTupleEntry* j : i1->retweet1Retweet2VertexValue->retweet1JoinTupleList) {
					for(int index1 = 0; index1 < j->retweet1ProjectedTupleMap->capacity; index1++) {
						int max = followerPt->count
								* i->retweet3ProjectedTupleMap->retweet3ProjectedTupleEntryArray[0]->count
								* i1->retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[0]->count
								* i2->retweet4ProjectedTupleMap->retweet4ProjectedTupleEntryArray[0]->count;
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

void addRetweet1Tuple(int& r1TweetIdR2RetweetTweetId, int& r1TweetId ) {
	Retweet1JoinTupleEntry* jt = putRetweet1JoinTupleIfAbsent(retweet1JoinTupleMap, r1TweetIdR2RetweetTweetId , retweet1Retweet2VertexValueMap );
	Retweet1ProjectedTupleEntry* pt = putRetweet1ProjectedTuple(jt->retweet1ProjectedTupleMap, r1TweetId );
	
	bfsRetweet1Diff(jt , pt);
}

void addRetweet2Tuple(int& r1TweetIdR2RetweetTweetId, int& r2TweetIdR3RetweetTweetId ) {
	Retweet2JoinTupleEntry* jt = putRetweet2JoinTupleIfAbsent(retweet2JoinTupleMap, r1TweetIdR2RetweetTweetId, r2TweetIdR3RetweetTweetId , retweet1Retweet2VertexValueMap, retweet2Retweet3VertexValueMap );
	Retweet2ProjectedTupleEntry* pt = putRetweet2ProjectedTuple(jt->retweet2ProjectedTupleMap);
	bfsRetweet2Diff(jt );
}

void addRetweet3Tuple(int& r2TweetIdR3RetweetTweetId, int& r3TweetIdR4RetweetTweetId ) {
	Retweet3JoinTupleEntry* jt = putRetweet3JoinTupleIfAbsent(retweet3JoinTupleMap, r2TweetIdR3RetweetTweetId, r3TweetIdR4RetweetTweetId , retweet2Retweet3VertexValueMap, retweet3Retweet4VertexValueMap );
	Retweet3ProjectedTupleEntry* pt = putRetweet3ProjectedTuple(jt->retweet3ProjectedTupleMap);
	bfsRetweet3Diff(jt );
}

void addRetweet4Tuple(int& r3TweetIdR4RetweetTweetId, int& r4UserIdFUserId ) {
	Retweet4JoinTupleEntry* jt = putRetweet4JoinTupleIfAbsent(retweet4JoinTupleMap, r3TweetIdR4RetweetTweetId, r4UserIdFUserId , retweet3Retweet4VertexValueMap, retweet4FollowerVertexValueMap );
	Retweet4ProjectedTupleEntry* pt = putRetweet4ProjectedTuple(jt->retweet4ProjectedTupleMap);
	bfsRetweet4Diff(jt );
}

void addFollowerTuple(int& r4UserIdFUserId, int& followerId, bool doBfs ) {
	FollowerJoinTupleEntry* jt = putFollowerJoinTupleIfAbsent(followerJoinTupleMap, r4UserIdFUserId , retweet4FollowerVertexValueMap );
	FollowerProjectedTupleEntry* pt = putFollowerProjectedTuple(jt->followerProjectedTupleMap, followerId );
	
	if(doBfs)
	bfsFollowerDiff(jt , pt);
}

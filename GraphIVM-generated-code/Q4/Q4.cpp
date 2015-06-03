#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

void addTweetTuple(int& userId, int& tweetTweetIdRetweet1RetweetTweetId );
void addRetweet1Tuple(int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId );
void addRetweet2Tuple(int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId );
void addFollowerTuple(int& followerId, int& retweet2UserIdFollowerUserId, bool doBfs );
void deleteTweetTuple(int& userId, int& tweetTweetIdRetweet1RetweetTweetId );
void deleteRetweet1Tuple(int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId );
void deleteRetweet2Tuple(int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId );
void deleteFollowerTuple(int& followerId, int& retweet2UserIdFollowerUserId, bool doBfs );


void scan();
int count = 0;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
Retweet1JoinTupleMap* retweet1JoinTupleMap = new Retweet1JoinTupleMap;
Retweet2JoinTupleMap* retweet2JoinTupleMap = new Retweet2JoinTupleMap;
FollowerJoinTupleMap* followerJoinTupleMap = new FollowerJoinTupleMap;
TweetRetweet1VertexValueMap* tweetRetweet1VertexValueMap = new TweetRetweet1VertexValueMap;
Retweet1Retweet2VertexValueMap* retweet1Retweet2VertexValueMap = new Retweet1Retweet2VertexValueMap;
Retweet2FollowerVertexValueMap* retweet2FollowerVertexValueMap = new Retweet2FollowerVertexValueMap;

using namespace std;

int main() {
	try {//PT
	      pqxx::connection C("dbname=bsmagen1m user=gaurav password=gaurav \
	      hostaddr=127.0.0.1 port=5432");
	      if (C.is_open()) {
		 cout << "Opened database successfully: " << C.dbname() << endl;
	      } else {
		 cout << "Can't open database" << endl;
		 return -1;
	      }
	      std::string sql = "select user_id, tweet_id, tweet_date from tweet";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NT(C);
	      
	      /* Execute SQL query */
	      pqxx::result RT( NT.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Tweet Table Load"<<endl;
		int i = 0;
	      for (pqxx::result::const_iterator c = RT.begin(); c != RT.end(); ++c) {
			int u = c[0].as<int>();
			int t = c[1].as<int>();
			addTweetTuple(u, t);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
	      /* Create SQL statement */
	      sql = "select tweet_id, retweet_tweet_id, tweet_date from retweet";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NR(C);
	      
	      /* Execute SQL query */
	      pqxx::result RR( NR.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start ReTweet Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RR.begin(); c != RR.end(); ++c) {
			int t = c[0].as<int>();
			int r = c[1].as<int>();
			addRetweet1Tuple(r, t);
	      }
		cout<<"End Retweet Table Load"<<endl;
		NR.commit();
	      sql = "select tweet_id, retweet_tweet_id, user_id from retweet";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NR2(C);
	      
	      /* Execute SQL query */
	      pqxx::result RR2( NR2.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start ReTweet 2 Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RR2.begin(); c != RR2.end(); ++c) {
			int t = c[0].as<int>();
			int r = c[1].as<int>();
			int u = c[2].as<int>();
			addRetweet2Tuple(r, t, u);
	      }
		cout<<"End Retweet 2 Table Load"<<endl;
		NR2.commit();
	      sql = "select * from follower";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NU(C);
	      
	      /* Execute SQL query */
	      pqxx::result RU( NU.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start User Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RU.begin(); c != RU.end(); ++c) {
			int u = c[0].as<long>();
			int f = c[1].as<long>();
			addFollowerTuple(f, u, false); 
	      }
		cout<<"End User Table Load"<<endl;
		NU.commit();
		int MAX = 50000;
		int X = 100000000;
		int which = 1;
		if(which == 1) {
			std::string c2 = "1992-01-01";
			for (int i = 0; i < MAX; i++) {
				int u = 384827;
				int f = X + i;
				addFollowerTuple(f, u, false);
			}  
			auto start_time = std::chrono::steady_clock::now();
			for (int i = 0; i < MAX; i++) {
				int u = 384827;
				int f = X + i;
				//addFollowerTuple(f, u, true);
				deleteFollowerTuple(f, u, true);
			}  
			double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
			std::cout << time_in_seconds<<"ms"<<std::endl;
		}
		std::cout << count<<std::endl;
	      C.disconnect();
	} catch (const std::exception &e){
	      cerr << e.what() << std::endl;
	}
	return 0;
}
void api(int& userId, int& tweetTweetIdRetweet1RetweetTweetId
		, int& tweetTweetIdRetweet1RetweetTweetId1, int& retweet1TweetIdRetweet2RetweetTweetId
		, int& retweet1TweetIdRetweet2RetweetTweetId1, int& retweet2TweetId, int& retweet2UserIdFollowerUserId
		, int& followerId, int& retweet2UserIdFollowerUserId1)  {count++;}
void bfsTweetDiff(TweetJoinTupleEntry* tweetJt , TweetProjectedTupleEntry* tweetPt) {}

void bfsRetweet1Diff(Retweet1JoinTupleEntry* retweet1Jt , Retweet1ProjectedTupleEntry* retweet1Pt) {}

void bfsRetweet2Diff(Retweet2JoinTupleEntry* retweet2Jt , Retweet2ProjectedTupleEntry* retweet2Pt) {}

void bfsFollowerDiff(FollowerJoinTupleEntry* followerJt, int& followerId, int& retweet2UserIdFollowerUserId) {
	int max = 1;
	for(Retweet2JoinTupleEntry* i : followerJt->retweet2FollowerVertexValue->retweet2JoinTupleList) {
		for(int index1 = 0; index1 < i->retweet2ProjectedTupleMap->capacity; index1++) {
			Retweet2ProjectedTupleEntry* currentR2 = i->retweet2ProjectedTupleMap->retweet2ProjectedTupleEntryArray[index1];
			while(currentR2) {
				max *= currentR2->count;
				for(Retweet1JoinTupleEntry* j : i->retweet1Retweet2VertexValue->retweet1JoinTupleList) {
					for(int index2 = 0; index2 < j->retweet1ProjectedTupleMap->capacity; index2++) {
						Retweet1ProjectedTupleEntry* currentR1 = j->retweet1ProjectedTupleMap->retweet1ProjectedTupleEntryArray[index2];
						while(currentR1) {
							max *= currentR1->count;
							for(TweetJoinTupleEntry* k : j->tweetRetweet1VertexValue->tweetJoinTupleList) {
								for(int index3 = 0; index3 < k->tweetProjectedTupleMap->capacity; index3++) {
									TweetProjectedTupleEntry* currentT = k->tweetProjectedTupleMap->tweetProjectedTupleEntryArray[index3];
									while(currentT) {
										max = max * currentT->count;
										for(int m = 0; m < max; m++)
											api(currentT->userId, currentT->tweetTweetIdRetweet1RetweetTweetId
												, currentR1->tweetTweetIdRetweet1RetweetTweetId, currentR1->retweet1TweetIdRetweet2RetweetTweetId
												, currentR2->retweet1TweetIdRetweet2RetweetTweetId, currentR2->retweet2TweetId, currentR2->retweet2UserIdFollowerUserId
												,followerId, retweet2UserIdFollowerUserId);
										max /= currentT->count;
										currentT = currentT->next;
									}
								}
							}
							max /= currentR1->count;
							currentR1 = currentR1->next;
						}
					}
				}
				max /= currentR2->count;
				currentR2 = currentR2->next;
			}
		}
	}
}


void addTweetTuple(int& userId, int& tweetTweetIdRetweet1RetweetTweetId ) {
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, tweetTweetIdRetweet1RetweetTweetId , tweetRetweet1VertexValueMap );
	TweetProjectedTupleEntry* pt = putTweetProjectedTuple(jt->tweetProjectedTupleMap, userId, tweetTweetIdRetweet1RetweetTweetId );
	
	bfsTweetDiff(jt , pt);
}

void addRetweet1Tuple(int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId ) {
	Retweet1JoinTupleEntry* jt = putRetweet1JoinTupleIfAbsent(retweet1JoinTupleMap, tweetTweetIdRetweet1RetweetTweetId, retweet1TweetIdRetweet2RetweetTweetId , tweetRetweet1VertexValueMap, retweet1Retweet2VertexValueMap );
	Retweet1ProjectedTupleEntry* pt = putRetweet1ProjectedTuple(jt->retweet1ProjectedTupleMap, tweetTweetIdRetweet1RetweetTweetId, retweet1TweetIdRetweet2RetweetTweetId );
	
	bfsRetweet1Diff(jt , pt);
}

void addRetweet2Tuple(int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId ) {
	Retweet2JoinTupleEntry* jt = putRetweet2JoinTupleIfAbsent(retweet2JoinTupleMap, retweet1TweetIdRetweet2RetweetTweetId, retweet2UserIdFollowerUserId , retweet1Retweet2VertexValueMap, retweet2FollowerVertexValueMap );
	Retweet2ProjectedTupleEntry* pt = putRetweet2ProjectedTuple(jt->retweet2ProjectedTupleMap, retweet1TweetIdRetweet2RetweetTweetId, retweet2TweetId, retweet2UserIdFollowerUserId );
	
	bfsRetweet2Diff(jt , pt);
}

void addFollowerTuple(int& followerId, int& retweet2UserIdFollowerUserId , bool doBfs) {
	FollowerJoinTupleEntry* jt = putFollowerJoinTupleIfAbsent(followerJoinTupleMap, retweet2UserIdFollowerUserId , retweet2FollowerVertexValueMap );
	FollowerProjectedTupleEntry* pt = putFollowerProjectedTuple(jt->followerProjectedTupleMap, followerId, retweet2UserIdFollowerUserId );
	if(doBfs)	
	bfsFollowerDiff(jt
			, followerId
			, retweet2UserIdFollowerUserId);
}
void deleteTweetTuple(int& userId, int& tweetTweetIdRetweet1RetweetTweetId ) {
    TweetJoinTupleEntry** jtp = getTweetJoinTuplePointer(tweetJoinTupleMap, tweetTweetIdRetweet1RetweetTweetId );
    if(deleteTweetProjectedTupleIfZero((*jtp)->tweetProjectedTupleMap
    , userId
    , tweetTweetIdRetweet1RetweetTweetId
    ) == 0) {
        deleteTweetRetweet1VertexValueIfZero(tweetRetweet1VertexValueMap, *jtp
        , tweetTweetIdRetweet1RetweetTweetId
        );
        removeJoinTupleEntry(tweetJoinTupleMap, jtp);
    }
}
void deleteRetweet1Tuple(int& tweetTweetIdRetweet1RetweetTweetId, int& retweet1TweetIdRetweet2RetweetTweetId ) {
    Retweet1JoinTupleEntry** jtp = getRetweet1JoinTuplePointer(retweet1JoinTupleMap, tweetTweetIdRetweet1RetweetTweetId, retweet1TweetIdRetweet2RetweetTweetId );
    if(deleteRetweet1ProjectedTupleIfZero((*jtp)->retweet1ProjectedTupleMap
    , tweetTweetIdRetweet1RetweetTweetId
    , retweet1TweetIdRetweet2RetweetTweetId
    ) == 0) {
        deleteTweetRetweet1VertexValueIfZero(tweetRetweet1VertexValueMap, *jtp
        , tweetTweetIdRetweet1RetweetTweetId
        );
        deleteRetweet1Retweet2VertexValueIfZero(retweet1Retweet2VertexValueMap, *jtp
        , retweet1TweetIdRetweet2RetweetTweetId
        );
        removeJoinTupleEntry(retweet1JoinTupleMap, jtp);
    }
}
void deleteRetweet2Tuple(int& retweet1TweetIdRetweet2RetweetTweetId, int& retweet2TweetId, int& retweet2UserIdFollowerUserId ) {
    Retweet2JoinTupleEntry** jtp = getRetweet2JoinTuplePointer(retweet2JoinTupleMap, retweet1TweetIdRetweet2RetweetTweetId, retweet2UserIdFollowerUserId );
    if(deleteRetweet2ProjectedTupleIfZero((*jtp)->retweet2ProjectedTupleMap
    , retweet1TweetIdRetweet2RetweetTweetId
    , retweet2TweetId
    , retweet2UserIdFollowerUserId
    ) == 0) {
        deleteRetweet1Retweet2VertexValueIfZero(retweet1Retweet2VertexValueMap, *jtp
        , retweet1TweetIdRetweet2RetweetTweetId
        );
        deleteRetweet2FollowerVertexValueIfZero(retweet2FollowerVertexValueMap, *jtp
        , retweet2UserIdFollowerUserId
        );
        removeJoinTupleEntry(retweet2JoinTupleMap, jtp);
    }
}
void deleteFollowerTuple(int& followerId, int& retweet2UserIdFollowerUserId, bool doBfs ) {
    FollowerJoinTupleEntry** jtp = getFollowerJoinTuplePointer(followerJoinTupleMap, retweet2UserIdFollowerUserId );
    if(doBfs)
    	bfsFollowerDiff((*jtp)
		, followerId
		, retweet2UserIdFollowerUserId);
    if(deleteFollowerProjectedTupleIfZero((*jtp)->followerProjectedTupleMap
    , followerId
    , retweet2UserIdFollowerUserId
    ) == 0) {
        deleteRetweet2FollowerVertexValueIfZero(retweet2FollowerVertexValueMap, *jtp
        , retweet2UserIdFollowerUserId
        );
        removeJoinTupleEntry(followerJoinTupleMap, jtp);
    }
}

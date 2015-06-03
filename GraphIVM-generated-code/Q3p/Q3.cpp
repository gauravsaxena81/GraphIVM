#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

void addUserTuple(int& userIdTweetUserId );
void addTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId, std::string& tweetDate );
void addRetweetTuple(int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate, bool doBfs);
void deleteUserTuple(int& userIdTweetUserId );
void deleteTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId, std::string& tweetDate );
void deleteRetweetTuple(int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate, bool doBfs );

void scan();
int count = 0;
UserJoinTupleMap* userJoinTupleMap = new UserJoinTupleMap;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
RetweetJoinTupleMap* retweetJoinTupleMap = new RetweetJoinTupleMap;
UserTweetVertexValueMap* userTweetVertexValueMap = new UserTweetVertexValueMap;
TweetRetweetVertexValueMap* tweetRetweetVertexValueMap = new TweetRetweetVertexValueMap;

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
	      std::string sql = "select * from users";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NU(C);
	      
	      /* Execute SQL query */
	      pqxx::result RU( NU.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start User Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RU.begin(); c != RU.end(); ++c) {
			int user = c[0].as<long>();
			addUserTuple(user); 
	      }
		cout<<"End User Table Load"<<endl;
		NU.commit();
		
	      sql = "select user_id, tweet_id, tweet_date from tweet where tweet_id < 12404312";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NT(C);
	      
	      /* Execute SQL query */
	      pqxx::result RT( NT.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Tweet Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RT.begin(); c != RT.end(); ++c) {
			int c0 = c[0].as<int>();
			int c1 = c[1].as<int>();
			std::string c2 = c[2].as<string>();
			addTweetTuple(c0, c1, c2);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
	      sql = "select tweet_id, retweet_tweet_id, tweet_date from retweet where tweet_id < 12404312";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NR(C);
	      
	      /* Execute SQL query */
	      pqxx::result RR( NR.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start ReTweet Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RR.begin(); c != RR.end(); ++c) {
			int t = c[0].as<int>();
			int r = c[1].as<int>();
			std::string c2 = c[2].as<string>();
			addRetweetTuple(t, r, c2, false);
	      }
		cout<<"End Retweet Table Load"<<endl;
		NR.commit();
	      /* Create SQL statement */
	      sql = "select user_id, tweet_id, tweet_date, -1 as retweet_tweet_id from tweet T union select user_id, tweet_id, tweet_date, retweet_tweet_id from retweet order by 2 desc limit 50000";

	      /* Create a non-transactional object. */
	      pqxx::nontransaction N(C);
	      
	      /* Execute SQL query */
	      pqxx::result R( N.exec( sql ));
	      
	      /* List down all the records */
		std::string ts;
		long t0, t1;
		auto start_time = std::chrono::steady_clock::now();
	      for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
			//if(c[3].as<long>() == -1) 
			//{
			//	ts = c[2].as<string>();
			//	t0 = c[0].as<long>(); 
			//	t1 = c[1].as<long>();
			//}
		}
		double overhead = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
		start_time = std::chrono::steady_clock::now();
	      for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
			if(c[3].as<int>() == -1)
			{
				int c0 = c[0].as<int>();
				int c1 = c[1].as<int>();
				std::string c2 = c[2].as<string>();
				addTweetTuple(c0, c1, c2);
			} else {
				int t = c[1].as<int>();
				int r = c[3].as<int>();
				std::string c2 = c[2].as<string>();
				addRetweetTuple(t, r, c2, false);
			}
	      }
		double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
		std::cout << time_in_seconds - overhead<<"ms"<<std::endl;
		//std::cout<<i<<endl;
	      C.disconnect();
	} catch (const std::exception &e){
	      cerr << e.what() << std::endl;
	}
	return 0;
}
void api(int& userId, int& userIdTweetUserId, int& tweetId, std::string& tweetDate, int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate ) {count++;}
void bfsUserDiff(UserJoinTupleEntry* userJt , UserProjectedTupleEntry* userPt) {}

void bfsTweetDiff(TweetJoinTupleEntry* tweetJt , TweetProjectedTupleEntry* tweetPt) {}

void bfsRetweetDiff(RetweetJoinTupleEntry* retweetJt, int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate ) {
	for(TweetJoinTupleEntry* i1 : retweetJt->tweetRetweetVertexValue->tweetJoinTupleList) {
		for(int index2 = 0; index2 < i1->tweetProjectedTupleMap->capacity; index2++) {
			TweetProjectedTupleEntry* currentT = i1->tweetProjectedTupleMap->tweetProjectedTupleEntryArray[index2];
			while(currentT) {
				for(UserJoinTupleEntry* i : i1->userTweetVertexValue->userJoinTupleList) {
					for(int index1 = 0; index1 < i->userProjectedTupleMap->capacity; index1++) {
						UserProjectedTupleEntry* currentP = i->userProjectedTupleMap->userProjectedTupleEntryArray[index1];
						while(currentP) {
							for(int m = 0; m < currentP->count * currentT->count; m++)
								api(currentP->userIdTweetUserId, currentT->tweetTweetIdRetweetRetweetTweetId, currentT->userIdTweetUserId, currentT->tweetDate
										, retweetTweetId
										, tweetTweetIdRetweetRetweetTweetId
										, retweetTweetDate);
							currentP = currentP->next;
						}
					}
				}
				currentT = currentT->next;
			}
		}
	}
}


void addUserTuple(int& userIdTweetUserId ) {
	UserJoinTupleEntry* jt = putUserJoinTupleIfAbsent(userJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	UserProjectedTupleEntry* pt = putUserProjectedTuple(jt->userProjectedTupleMap, userIdTweetUserId );
	
	bfsUserDiff(jt , pt);
}

void addTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId, std::string& tweetDate ) {
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, userIdTweetUserId, tweetTweetIdRetweetRetweetTweetId , userTweetVertexValueMap, tweetRetweetVertexValueMap );
	TweetProjectedTupleEntry* pt = putTweetProjectedTuple(jt->tweetProjectedTupleMap, userIdTweetUserId, tweetTweetIdRetweetRetweetTweetId, tweetDate );
	
	bfsTweetDiff(jt , pt);
}

void addRetweetTuple(int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate, bool doBfs ) {
	RetweetJoinTupleEntry* jt = putRetweetJoinTupleIfAbsent(retweetJoinTupleMap, tweetTweetIdRetweetRetweetTweetId , tweetRetweetVertexValueMap );
	if(jt->retweetProjectedTupleMap->size == 0)
	putRetweetProjectedTuple(jt->retweetProjectedTupleMap, retweetTweetId, tweetTweetIdRetweetRetweetTweetId, retweetTweetDate );
	if(doBfs)	
	bfsRetweetDiff(jt, retweetTweetId
		    , tweetTweetIdRetweetRetweetTweetId
		    , retweetTweetDate);
}
void deleteUserTuple(int& userIdTweetUserId ) {
    UserJoinTupleEntry** jtp = getUserJoinTuplePointer(userJoinTupleMap, userIdTweetUserId );
    if(deleteUserProjectedTupleIfZero((*jtp)->userProjectedTupleMap
    , userIdTweetUserId
    ) == 0) {
        deleteUserTweetVertexValueIfZero(userTweetVertexValueMap, *jtp
        , userIdTweetUserId
        );
        removeJoinTupleEntry(userJoinTupleMap, jtp);
    }
}
void deleteTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId, std::string& tweetDate ) {
    TweetJoinTupleEntry** jtp = getTweetJoinTuplePointer(tweetJoinTupleMap, userIdTweetUserId, tweetTweetIdRetweetRetweetTweetId );
    if(deleteTweetProjectedTupleIfZero((*jtp)->tweetProjectedTupleMap
    , userIdTweetUserId
    , tweetTweetIdRetweetRetweetTweetId
    , tweetDate
    ) == 0) {
        deleteUserTweetVertexValueIfZero(userTweetVertexValueMap, *jtp
        , userIdTweetUserId
        );
        deleteTweetRetweetVertexValueIfZero(tweetRetweetVertexValueMap, *jtp
        , tweetTweetIdRetweetRetweetTweetId
        );
        removeJoinTupleEntry(tweetJoinTupleMap, jtp);
    }
}
void deleteRetweetTuple(int& retweetTweetId, int& tweetTweetIdRetweetRetweetTweetId, std::string& retweetTweetDate, bool doBfs ) {
    RetweetJoinTupleEntry** jtp = getRetweetJoinTuplePointer(retweetJoinTupleMap, tweetTweetIdRetweetRetweetTweetId );
    if(doBfs)
    	bfsRetweetDiff((*jtp), retweetTweetId
    		    , tweetTweetIdRetweetRetweetTweetId
    		    , retweetTweetDate);
    if(deleteRetweetProjectedTupleIfZero((*jtp)->retweetProjectedTupleMap
    , retweetTweetId
    , tweetTweetIdRetweetRetweetTweetId
    , retweetTweetDate
    ) == 0) {
        deleteTweetRetweetVertexValueIfZero(tweetRetweetVertexValueMap, *jtp
        , tweetTweetIdRetweetRetweetTweetId
        );
        removeJoinTupleEntry(retweetJoinTupleMap, jtp);
    }
}

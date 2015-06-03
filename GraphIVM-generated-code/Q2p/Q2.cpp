#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

void addUserTuple(int& userIdTweetUserId, int& followerId );
void addTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate, bool doBfs );
void deleteUserTuple(int& userIdTweetUserId, int& followerId );
void deleteTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate, bool doBfs );

void scan();
int count = 0;
UserJoinTupleMap* userJoinTupleMap = new UserJoinTupleMap;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
UserTweetVertexValueMap* userTweetVertexValueMap = new UserTweetVertexValueMap;

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
	      std::string sql = "select * from follower";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NU(C);
	      
	      /* Execute SQL query */
	      pqxx::result RU( NU.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start User Table Load"<<endl;
	      for (pqxx::result::const_iterator c = RU.begin(); c != RU.end(); ++c) {
			int user = c[0].as<long>();
			int f = c[1].as<long>();
			addUserTuple(user, f); 
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
			addTweetTuple(c0, c1, c2, true);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
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
				addTweetTuple(c0, c1, c2, false);
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
void api(int& followerId, int& userIdTweetUserId, int& tweetId, std::string& tweetDate ) {count++;}
void bfsUserDiff(UserJoinTupleEntry* userJt , UserProjectedTupleEntry* userPt) {}

void bfsTweetDiff(TweetJoinTupleEntry* tweetJt, int& userIdTweetUserId, int& tweetId, std::string& tweetDate) {
	for(UserJoinTupleEntry* i : tweetJt->userTweetVertexValue->userJoinTupleList) {
		for(int index1 = 0; index1 < i->userProjectedTupleMap->capacity; index1++) {
			UserProjectedTupleEntry* currentP = i->userProjectedTupleMap->userProjectedTupleEntryArray[index1];
			while(currentP) {
				for(int m = 0; m < currentP->count; m++)
					api(currentP->followerId, userIdTweetUserId, tweetId, tweetDate);
				currentP = currentP->next;
			}
		}
	}
}


void addUserTuple(int& userIdTweetUserId, int& followerId ) {
	UserJoinTupleEntry* jt = putUserJoinTupleIfAbsent(userJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	UserProjectedTupleEntry* pt = putUserProjectedTuple(jt->userProjectedTupleMap, followerId );
	
	bfsUserDiff(jt , pt);
}

void addTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate, bool doBfs ) {
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	if(jt->tweetProjectedTupleMap->size == 0)
		putTweetProjectedTuple(jt->tweetProjectedTupleMap, userIdTweetUserId, tweetId, tweetDate );
	if(doBfs)
		bfsTweetDiff(jt , userIdTweetUserId
		    , tweetId
		    , tweetDate);
}
void deleteUserTuple(int& userIdTweetUserId, int& followerId ) {
    UserJoinTupleEntry** jtp = getUserJoinTuplePointer(userJoinTupleMap, userIdTweetUserId );
    if(deleteUserProjectedTupleIfZero((*jtp)->userProjectedTupleMap
    , followerId
    ) == 0) {
        deleteUserTweetVertexValueIfZero(userTweetVertexValueMap, *jtp
        , userIdTweetUserId
        );
        removeJoinTupleEntry(userJoinTupleMap, jtp);
    }
}
void deleteTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate, bool doBfs ) {
    TweetJoinTupleEntry** jtp = getTweetJoinTuplePointer(tweetJoinTupleMap, userIdTweetUserId );
    if(doBfs)
    	bfsTweetDiff((*jtp), userIdTweetUserId
    		    , tweetId
    		    , tweetDate );
    if(deleteTweetProjectedTupleIfZero((*jtp)->tweetProjectedTupleMap
    , userIdTweetUserId
    , tweetId
    , tweetDate
    ) == 0) {
        deleteUserTweetVertexValueIfZero(userTweetVertexValueMap, *jtp
        , userIdTweetUserId
        );
        removeJoinTupleEntry(tweetJoinTupleMap, jtp);
    }
}

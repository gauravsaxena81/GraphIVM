#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>
bool bb;
struct op{
	int userId;
	op* next = NULL;
	int count = 1;
};
void addUserTuple(int& userIdTweetUserId );
void addTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId );
void addRetweetTuple(int& tweetTweetIdRetweetRetweetTweetId, bool doBfs);
void deleteUserTuple(int& userIdTweetUserId );
void deleteTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId );
void deleteRetweetTuple(int& tweetTweetIdRetweetRetweetTweetId , bool doBfs);
inline static void hashCode(int& h, int& userIdTweetUserId ) __attribute__((always_inline));
inline static void api(int& userId, int& count1, op** arr) __attribute__((always_inline));

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
			addTweetTuple(c0, c1);
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
			int r = c[1].as<int>();
			addRetweetTuple(r, false);
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
		}
		double overhead = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
		start_time = std::chrono::steady_clock::now();
	      for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
			if(c[3].as<int>() == -1)
			{
				int c0 = c[0].as<int>();
				int c1 = c[1].as<int>();
				addTweetTuple(c0, c1);
			} else {
				int r = c[3].as<int>();
				addRetweetTuple(r, true);
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
void bfsUserDiff(UserJoinTupleEntry* userJt , UserProjectedTupleEntry* userPt) {}

void bfsTweetDiff(TweetJoinTupleEntry* tweetJt ) {}

__inline static void api(int& userId, int& count1, op** arr) {
	bb = true;
	count++;
	int h = 31;
	hashCode(h, userId);
	h = h  & (65535);
	op *c = arr[h];
	op *l = NULL;
	while(c) {
		if(c->userId == userId){
			c->count++;
			return;
		}
		l = c;
		c = c->next;
	}
	op *x = new op;
	x->userId = userId;
	if(l)
		l->next = x;
	else
		arr[h] = x;
	
}
__inline static void hashCode(int& h, int& userIdTweetUserId ) {
    h ^=  userIdTweetUserId + 0x9e3779b9 + (h<<6) + (h>>2);
}
op** arr = new op*[65536]();
void bfsRetweetDiff(RetweetJoinTupleEntry* retweetJt) {
	int one = 1;
	for(TweetJoinTupleEntry* i : retweetJt->tweetRetweetVertexValue->tweetJoinTupleList) {
		for(UserJoinTupleEntry* i1 : i->userTweetVertexValue->userJoinTupleList) {
			for(int index1 = 0; index1 < i1->userProjectedTupleMap->capacity; index1++) {
				UserProjectedTupleEntry* currentP = i1->userProjectedTupleMap->userProjectedTupleEntryArray[index1];
				while(currentP) {
					//count++;
					api(currentP->userIdTweetUserId, one, arr);
					currentP = currentP->next;
				}
			}
		}
	}
}


void addUserTuple(int& userIdTweetUserId ) {
	UserJoinTupleEntry* jt = putUserJoinTupleIfAbsent(userJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	UserProjectedTupleEntry* pt = putUserProjectedTuple(jt->userProjectedTupleMap, userIdTweetUserId );
	
	bfsUserDiff(jt , pt);
}

void addTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId ) {
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, userIdTweetUserId, tweetTweetIdRetweetRetweetTweetId , userTweetVertexValueMap, tweetRetweetVertexValueMap );
	TweetProjectedTupleEntry* pt = putTweetProjectedTuple(jt->tweetProjectedTupleMap);
	bfsTweetDiff(jt );
}

void addRetweetTuple(int& tweetTweetIdRetweetRetweetTweetId, bool doBfs ) {
	RetweetJoinTupleEntry* jt = putRetweetJoinTupleIfAbsent(retweetJoinTupleMap, tweetTweetIdRetweetRetweetTweetId , tweetRetweetVertexValueMap );
	RetweetProjectedTupleEntry* pt = putRetweetProjectedTuple(jt->retweetProjectedTupleMap);
	if(doBfs)
	bfsRetweetDiff(jt);
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
void deleteTweetTuple(int& userIdTweetUserId, int& tweetTweetIdRetweetRetweetTweetId ) {
    TweetJoinTupleEntry** jtp = getTweetJoinTuplePointer(tweetJoinTupleMap, userIdTweetUserId, tweetTweetIdRetweetRetweetTweetId );
    if(deleteTweetProjectedTupleIfZero((*jtp)->tweetProjectedTupleMap
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
void deleteRetweetTuple(int& tweetTweetIdRetweetRetweetTweetId, bool doBfs ) {
    RetweetJoinTupleEntry** jtp = getRetweetJoinTuplePointer(retweetJoinTupleMap, tweetTweetIdRetweetRetweetTweetId );
	if(doBfs)
		bfsRetweetDiff((*jtp));
    if(deleteRetweetProjectedTupleIfZero((*jtp)->retweetProjectedTupleMap
    ) == 0) {
        deleteTweetRetweetVertexValueIfZero(tweetRetweetVertexValueMap, *jtp
        , tweetTweetIdRetweetRetweetTweetId
        );
        removeJoinTupleEntry(retweetJoinTupleMap, jtp);
    }
}

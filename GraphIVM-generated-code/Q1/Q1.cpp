#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>

void addUserTuple(int& userIdTweetUserId, int& userId );
void addTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate, bool doBfs );
void deleteUserTuple(int& userIdTweetUserId, int& userId );
void deleteTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate , bool doBfs);
inline static void bfsTweetDiff(TweetJoinTupleEntry* tweetJt, int& userIdTweetUserId, int& tweetId, std::string& tweetDate) __attribute__((always_inline));


void scan();
int count = 0;
UserJoinTupleMap* userJoinTupleMap = new UserJoinTupleMap;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
UserTweetVertexValueMap* userTweetVertexValueMap = new UserTweetVertexValueMap;

std::vector<TweetProjectedTupleEntry*> tVector;
using namespace std;
int main() {
	for(int i = 0; i < 10000000; i++)
		tVector.push_back(new TweetProjectedTupleEntry);
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
			addUserTuple(user, user); 
	      }
		cout<<"End User Table Load"<<endl;
		NU.commit();
		
	      sql = "select user_id, tweet_id, tweet_date from tweet";
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
			std::string c2 = c[2].as<string>();
			addTweetTuple(c0, c1, c2, false);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
	      /* Create SQL statement */
		int which = 2;
		if(which == 1) {
			int MAX = 50000;
			int X = 100000000;
			std::string c2 = "1992-01-01";
			for (int i = 0; i < MAX; i++) {
				int u = 100;
				int t = X + i;
				addTweetTuple(u, t, c2, false);
			}  
			auto start_time = std::chrono::steady_clock::now();
			for (int i = 0; i < MAX; i++) {
				int u = 100;
				int t = X + i;
				//addTweetTuple(u, t, c2, true);
				deleteTweetTuple(u, t, c2, true);
			}  
			double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
			std::cout << time_in_seconds<<"ms"<<std::endl;
		} else {
			int MAX = 50000;
			int X = 1;
			//int X = 100000000;
			for (int i = 0; i < MAX; i++) {
				int u = X + i;
			//	addUserTuple(u, u);
			}  
			std::string c2 = "1992-01-01";
			for (int i = 0; i < MAX; i++) {
				int u = X + i;
				int t = X + i;
				//addTweetTuple(u, t, c2, false);
			}  
			auto start_time = std::chrono::steady_clock::now();
			for (int i = 0; i < MAX; i++) {
				int u = X + i;
				int t = X + i;
				addTweetTuple(u, t, c2, true);
				//deleteTweetTuple(u, t, c2, true);
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
void api(int& userId, int& userIdTweetUserId, int& tweetId, std::string& tweetDate ) {count++;}
void bfsUserDiff(UserJoinTupleEntry* userJt , UserProjectedTupleEntry* userPt) {}

__inline static void bfsTweetDiff(TweetJoinTupleEntry* tweetJt, int& userIdTweetUserId, int& tweetId, std::string& tweetDate) {
	for(UserJoinTupleEntry* i : tweetJt->userTweetVertexValue->userJoinTupleList) {
		for(int index1 = 0; index1 < i->userProjectedTupleMap->capacity; index1++) {
			UserProjectedTupleEntry* currentP = i->userProjectedTupleMap->userProjectedTupleEntryArray[index1];
			while(currentP) {
				for(int m = 0; m < currentP->count; m++)
					api(currentP->userId
						, userIdTweetUserId
						, tweetId
						, tweetDate);
				currentP = currentP->next;
			}
		}
	}
}


void addUserTuple(int& userIdTweetUserId, int& userId ) {
	UserJoinTupleEntry* jt = putUserJoinTupleIfAbsent(userJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	UserProjectedTupleEntry* pt = putUserProjectedTuple(jt->userProjectedTupleMap, userId );

	bfsUserDiff(jt , pt);
}

void addTweetTuple(int& userIdTweetUserId, int& tweetId, std::string& tweetDate , bool doBfs) {
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	putTweetProjectedTuple( jt->tweetProjectedTupleMap, userIdTweetUserId, tweetId, tweetDate );
	if(doBfs) {
		bfsTweetDiff(jt , userIdTweetUserId, tweetId, tweetDate);
	}
}
void deleteUserTuple(int& userIdTweetUserId, int& userId) {
    UserJoinTupleEntry** jtp = getUserJoinTuplePointer(userJoinTupleMap, userIdTweetUserId );
    if(deleteUserProjectedTupleIfZero((*jtp)->userProjectedTupleMap
    , userId
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
		bfsTweetDiff((*jtp) , userIdTweetUserId
		    , tweetId
		    , tweetDate);
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

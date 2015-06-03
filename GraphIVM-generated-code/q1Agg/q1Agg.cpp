#include <stdlib.h>
#include <string>
#include <chrono>
#include <iostream>
#include "DataStructures.hpp"
#include "Functions.hpp"
#include<pqxx/pqxx>


struct op{
	int userId;
	op* next = NULL;
	int count = 1;
};
std::vector<op*> v;
void addUserTuple(int& userIdTweetUserId );
void addTweetTuple(int& userIdTweetUserId, bool doBfs );
void deleteUserTuple(int& userIdTweetUserId );
void deleteTweetTuple(int& userIdTweetUserId , bool doBfs );
inline static void hashCode(int& h, int& userIdTweetUserId ) __attribute__((always_inline));
inline static void api(int& userId, int& count1, op** arr) __attribute__((always_inline));
inline static void bfsTweetDiff(TweetJoinTupleEntry* tweetJt) __attribute__((always_inline));
void scan();
int count = 0;
UserJoinTupleMap* userJoinTupleMap = new UserJoinTupleMap;
TweetJoinTupleMap* tweetJoinTupleMap = new TweetJoinTupleMap;
UserTweetVertexValueMap* userTweetVertexValueMap = new UserTweetVertexValueMap;
using namespace std;

int main() {
	for(int i = 0; i < 50000; i++)
		v.push_back(new op);
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
		
	      sql = "select user_id  from tweet";
	      /* Create a non-transactional object. */
	      pqxx::nontransaction NT(C);
	      
	      /* Execute SQL query */
	      pqxx::result RT( NT.exec( sql ));
	      
	      /* List down all the records */
		cout<<"Start Tweet Table Load"<<endl;
		int i = 0;
	      for (pqxx::result::const_iterator c = RT.begin(); c != RT.end(); ++c) {
			int c0 = c[0].as<int>();
			addTweetTuple(c0, false);
	      }
		cout<<"End Tweet Table Load"<<endl;
		NT.commit();
	      /* Create SQL statement */
		int which = 1;
		if(which == 1) {
			int MAX = 50000;
			int X = 30000;
			for (int i = 0; i < MAX; i++) {
				//int u = 2;
				int u = X + i % 20000;
				addTweetTuple(u, false);
			} 
			int eSize = tweetJoinTupleMap->size;
			auto start_time = std::chrono::steady_clock::now();
			for (int i = 0; i < MAX; i++) {
				int u = X + i % 20000;
				//deleteTweetTuple(u, true);
				addTweetTuple(u, true);
			}  
			double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
			std::cout << time_in_seconds<<"ms"<<std::endl;
			std::cout << tweetJoinTupleMap->size - eSize<<" new jts"<<std::endl;
		} else {
			int MAX = 50000;
			int X = 100000000;
			for (int i = 0; i < MAX; i++) {
				int u = X + i;
				addUserTuple(u);
			}  
			for (int i = 0; i < MAX; i++) {
				int u = X + i;
				//addTweetTuple(u, false);
			}  
			int eSize = tweetJoinTupleMap->size;
			auto start_time = std::chrono::steady_clock::now();
			for (int i = 0; i < MAX; i++) {
				int u = X + i;
				//deleteTweetTuple(u, true);
				addTweetTuple(u, true);
			}  
			double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
			std::cout << time_in_seconds<<"ms"<<std::endl;
			std::cout << tweetJoinTupleMap->size - eSize<<" new jts"<<std::endl;
		}
		std::cout << count<<std::endl;
	      C.disconnect();
	} catch (const std::exception &e){
	      cerr << e.what() << std::endl;
	}
	return 0;
}

__inline static void api(int& userId, int& count1, op** arr) {
	int h = 31;
	count++;
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
void bfsUserDiff(UserJoinTupleEntry* userJt, UserProjectedTupleEntry* userPt) {}

op** arr = new op*[65536]();
__inline static void bfsTweetDiff(TweetJoinTupleEntry* tweetJt) {
	int one = 1;
	for(UserJoinTupleEntry* i : tweetJt->userTweetVertexValue->userJoinTupleList) {
		for(int index1 = 0; index1 < i->userProjectedTupleMap->capacity; index1++) {
			UserProjectedTupleEntry* currentP = i->userProjectedTupleMap->userProjectedTupleEntryArray[index1];
			while(currentP) {
				//count++;
				api(currentP->userIdTweetUserId, one, arr);
				currentP = currentP->next;
			}
		}
	}
}

void addUserTuple(int& userIdTweetUserId ) {
	UserJoinTupleEntry* jt = putUserJoinTupleIfAbsent(userJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap );
	UserProjectedTupleEntry* pt = putUserProjectedTuple(jt->userProjectedTupleMap
	, userIdTweetUserId
	);
	bfsUserDiff(jt, pt);
}

void addTweetTuple(int& userIdTweetUserId, bool doBfs ) {
	TweetJoinTupleEntry* jt = putTweetJoinTupleIfAbsent(tweetJoinTupleMap, userIdTweetUserId , userTweetVertexValueMap);
	//putTweetProjectedTuple(jt->tweetProjectedTupleMap);
	if(doBfs) {
		bfsTweetDiff(jt);
	}
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
void deleteTweetTuple(int& userIdTweetUserId , bool doBfs ) {
    TweetJoinTupleEntry** jtp = getTweetJoinTuplePointer(tweetJoinTupleMap, userIdTweetUserId );
	if(doBfs)
		bfsTweetDiff((*jtp));
    if(deleteTweetProjectedTupleIfZero((*jtp)->tweetProjectedTupleMap
    ) == 0) {
        deleteUserTweetVertexValueIfZero(userTweetVertexValueMap, *jtp
        , userIdTweetUserId
        );
        removeJoinTupleEntry(tweetJoinTupleMap, jtp);
    }
}

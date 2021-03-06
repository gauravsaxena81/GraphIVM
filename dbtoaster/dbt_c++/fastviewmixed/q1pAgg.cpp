#include "q1pAgg.hpp"
#include <chrono>
#include<pqxx/pqxx>
namespace dbtoaster{
    class CustomProgram_1 : public Program
    {
    public:
        void process_stream_event(const event_t& ev) {
            //cout << "on_" << dbtoaster::event_name[ev.type] << "_";
            // cout << get_relation_name(ev.id) << "(" << ev.data << ")" << endl;

            Program::process_stream_event(ev);
        }
    };

    class CustomProgram_2 : public Program
    {
    public:
//		CustomProgram_2(int argc = 0, char* argv[] = 0) : Program(argc,argv) {}
        void process_streams() {
		Program::process_streams();
		try {//PT
			pqxx::connection C("dbname=bsmagen1m user=gaurav password=gaurav \
		      hostaddr=127.0.0.1 port=5432");
		      if (C.is_open()) {
			 cout << "Opened database successfully: " << C.dbname() << endl;
		      } else {
			 cout << "Can't open database" << endl;
			 return;
		      }
		      /* Create SQL statement */
		      std::string sql = "select user_id, tweet_id, tweet_date, -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit 50000";

		      /* Create a non-transactional object. */
		      pqxx::nontransaction N(C);
		      
		      /* Execute SQL query */
		      pqxx::result R( N.exec( sql ));
		      
		        STRING_TYPE *ts;
			long t0, t1;
			auto start_time = std::chrono::steady_clock::now();
		      for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
				if(c[3].as<long>() > -1) {
					ts = new STRING_TYPE(c[2].as<string>().c_str());
					t0 = c[0].as<long>(); 
					t1 = c[1].as<long>();
				}
			}
			double overhead = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
			start_time = std::chrono::steady_clock::now();
		      for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
				if(c[3].as<long>() == -1) {
					STRING_TYPE *s = new STRING_TYPE(c[2].as<string>().c_str());
					data.on_insert_TWEET(c[0].as<long>(), c[1].as<long>(),*s);
					s = ts;
				}
		      }
			double time_in_seconds = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
			std::cout << time_in_seconds-overhead<<"ms"<<std::endl;
			std::cout << t0<<","<<t1<<endl;
		      C.disconnect();
		} catch (const std::exception &e){
		      cerr << e.what() << std::endl;
		}
        }
    };
}


/**
 * In order to compile this file one would have to include a header containing
 * the definition of "dbtoaster::Program" and "dbtoaster::Program::snapshot_t"
 * as generated by the dbtoaster C++ code generator. Using the "-c" dbtoaster
 * command line option the generated header file is automatically included and
 * compiled against this file.
 */

/**
 * Determines whether "--async" was specified as a command line argument.
 *
 * @param argc
 * @param argv
 * @return true if "--async" is one of the command line arguments
 */
bool async_mode(int argc, char* argv[])
{
    for(int i = 1; i < argc; ++i)
        if( !strcmp(argv[i],"--async") )
            return true;
    return false;
}

/**
 * Main function that executes the sql program corresponding to the header file
 * included. If "-async" is among the command line arguments then the execution
 * is performed asynchronous in a seperate thread, otherwise it is performed in
 * the same thread. If performed in a separate thread and while not finished it
 * continuously gets snapshots of the results and prints them in xml format. At
 * the end of the execution the final results are also printed.
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    bool async = async_mode(argc,argv);

    //dbtoaster::Program p;//(argc,argv);
    //dbtoaster::CustomProgram_1 p;
    dbtoaster::CustomProgram_2 p;
    //dbtoaster::Program::snapshot_t snap;
	dbtoaster::CustomProgram_2::snapshot_t snap;

    cout << "Initializing program:" << endl;
    p.init();
	
    p.run( async );

    cout << "Running program:" << endl;
    while( !p.is_finished() )
    {
       snap = p.get_snapshot();
       DBT_SERIALIZATION_NVP_OF_PTR(cout, snap);
    }

    cout << "Printing final result:" << endl;
    snap = p.get_snapshot();
    //DBT_SERIALIZATION_NVP_OF_PTR(cout, snap);
    cout << std::endl;
    return 0;
}


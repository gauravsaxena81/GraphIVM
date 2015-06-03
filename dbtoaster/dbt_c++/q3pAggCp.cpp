#include "q3pAgg.hpp"
#include <chrono>
#include <pqxx/pqxx>
#include "Api.cpp"
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

    std::vector<void*> r;
    class CustomProgram_2 : public Program
    {
    public:
//		CustomProgram_2(int argc = 0, char* argv[] = 0) : Program(argc,argv) {}
        void process_streams() {
		Program::process_streams();
		__SQL_COUNT_AGGREGATE_1_map m = data.get___SQL_COUNT_AGGREGATE_1();
		int x = 0;
		auto start_time = std::chrono::steady_clock::now();
		m.foreach([x] (const __SQL_COUNT_AGGREGATE_1_entry& e) {
			//void* rr[2];
			//r.push_back(rr);
			//rr[0] = new long(e.U_USER_ID); 
			//rr[1] = new long(e.__av);
			api.useDiff(e.U_USER_ID, e.__av);
		});
		double overhead = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time).count();
		cout<< overhead <<endl;
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

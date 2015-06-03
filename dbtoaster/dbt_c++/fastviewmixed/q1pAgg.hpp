#include "program_base.cpp"
#include "hpds/KDouble.cpp"
#include "hash.hpp"
#include "mmap/mmap.hpp"
#include "hpds/pstring.hpp"
#include "hpds/pstringops.hpp"
#include "event.cpp"
#include "runtime.cpp"
#include "standard_adaptors.cpp"
#include "standard_functions.cpp"
#include "statistics_split.cpp"
#include "smhasher/MurmurHash2.cpp"
#include "streams.cpp"
#include "iprogram.cpp"
#include "Api.cpp"
Api api;
#define ELEM_SEPARATOR "\n\t\t\t"


namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct __SQL_COUNT_AGGREGATE_1_entry {
    long A_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_entry* nxt; __SQL_COUNT_AGGREGATE_1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_entry(const long c0, const long c1) { A_USER_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_entry(const __SQL_COUNT_AGGREGATE_1_entry& other) : A_USER_ID( other.A_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_entry& modify(const long c0) { A_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_entry& x, const __SQL_COUNT_AGGREGATE_1_entry& y) {
      return x.A_USER_ID == y.A_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_entry,long,__SQL_COUNT_AGGREGATE_1_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_entry,long,__SQL_COUNT_AGGREGATE_1_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mTWEET1_entry {
    long __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mTWEET1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mTWEET1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mTWEET1_entry(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry(const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& other) : __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& modify(const long c0) { __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mTWEET1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mTWEET1_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_mTWEET1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mTWEET1_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mTWEET1_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mUSERS1_entry {
    long __SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mUSERS1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mUSERS1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mUSERS1_entry(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry(const __SQL_COUNT_AGGREGATE_1_mUSERS1_entry& other) : __SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mUSERS1_entry& modify(const long c0) { __SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mUSERS1_entry& x, const __SQL_COUNT_AGGREGATE_1_mUSERS1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mUSERS1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mUSERS1_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_mUSERS1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mUSERS1_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mUSERS1_map_0;
  
  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t{
    struct timeval t0,t; long tT,tN,tS;
    tlq_t(): tN(0), tS(0) { gettimeofday(&t0,NULL); }
  
  /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
  
      ar << "\n";
      const __SQL_COUNT_AGGREGATE_1_map& ___SQL_COUNT_AGGREGATE_1 = get___SQL_COUNT_AGGREGATE_1();
      dbtoaster::serialize_nvp_tabbed(ar, STRING(__SQL_COUNT_AGGREGATE_1), ___SQL_COUNT_AGGREGATE_1, "\t");
  
    }
  
    /* Functions returning / computing the results of top level queries */
    const __SQL_COUNT_AGGREGATE_1_map& get___SQL_COUNT_AGGREGATE_1() const {
      return __SQL_COUNT_AGGREGATE_1;
    
    }
  
  protected:
  
    /* Data structures used for storing / computing top level queries */
    __SQL_COUNT_AGGREGATE_1_map __SQL_COUNT_AGGREGATE_1;
  
  };
  
  /* Type definition providing a way to incrementally maintain the results of the sql program */
  struct data_t : tlq_t{
    data_t(): tlq_t() {
      
    }
  
    #ifdef DBT_PROFILE
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> exec_stats;
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> ivc_stats;
    #endif
  
    /* Registering relations and trigger functions */
    void register_data(ProgramBase& pb) {
  
      pb.add_map<__SQL_COUNT_AGGREGATE_1_map>( "__SQL_COUNT_AGGREGATE_1", __SQL_COUNT_AGGREGATE_1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mTWEET1_map>( "__SQL_COUNT_AGGREGATE_1_mTWEET1", __SQL_COUNT_AGGREGATE_1_mTWEET1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mUSERS1_map>( "__SQL_COUNT_AGGREGATE_1_mUSERS1", __SQL_COUNT_AGGREGATE_1_mUSERS1 );
  
      pb.add_relation("TWEET");
      pb.add_relation("USERS");
  
      
  
      pb.add_trigger("TWEET", insert_tuple, std::bind(&data_t::unwrap_insert_TWEET, this, std::placeholders::_1));
      pb.add_trigger("TWEET", delete_tuple, std::bind(&data_t::unwrap_delete_TWEET, this, std::placeholders::_1));
      pb.add_trigger("USERS", insert_tuple, std::bind(&data_t::unwrap_insert_USERS, this, std::placeholders::_1));
      pb.add_trigger("USERS", delete_tuple, std::bind(&data_t::unwrap_delete_USERS, this, std::placeholders::_1));
  
      #ifdef DBT_PROFILE
      exec_stats = pb.exec_stats;
      ivc_stats = pb.ivc_stats;
      exec_stats->register_probe(0, "insert_TWEET_s0");
      exec_stats->register_probe(1, "insert_TWEET_s1");
      exec_stats->register_probe(2, "delete_TWEET_s0");
      exec_stats->register_probe(3, "delete_TWEET_s1");
      exec_stats->register_probe(4, "insert_USERS_s0");
      exec_stats->register_probe(5, "insert_USERS_s1");
      exec_stats->register_probe(6, "delete_USERS_s0");
      exec_stats->register_probe(7, "delete_USERS_s1");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_TWEET(const long tweet_user_id, const long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"insert_TWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_TWEET")
      {  //++tN;
	
	//api.useDiff(tweet_user_id, __SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se2.modify(tweet_user_id)));
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se1.modify(tweet_user_id),__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se2.modify(tweet_user_id)));
        __SQL_COUNT_AGGREGATE_1_mUSERS1.addOrDelOnZero(se3.modify(tweet_user_id),1L);
      }
      END_TRIGGER(exec_stats,"insert_TWEET")
      END_TRIGGER(ivc_stats,"insert_TWEET")
    }
    void unwrap_insert_TWEET(const event_args_t& ea) {
      on_insert_TWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])));
    }
    
    void on_delete_TWEET(const long tweet_user_id, const long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"delete_TWEET")
      BEGIN_TRIGGER(ivc_stats,"delete_TWEET")
      {  //++tN;
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se4.modify(tweet_user_id),(__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se5.modify(tweet_user_id)) * -1L));
        __SQL_COUNT_AGGREGATE_1_mUSERS1.addOrDelOnZero(se6.modify(tweet_user_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_TWEET")
      END_TRIGGER(ivc_stats,"delete_TWEET")
    }
    void unwrap_delete_TWEET(const event_args_t& ea) {
      on_delete_TWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])));
    }
    
    void on_insert_USERS(const long users_user_id) {
      BEGIN_TRIGGER(exec_stats,"insert_USERS")
      BEGIN_TRIGGER(ivc_stats,"insert_USERS")
      {  //++tN;
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se7.modify(users_user_id),__SQL_COUNT_AGGREGATE_1_mUSERS1.getValueOrDefault(se8.modify(users_user_id)));
        __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se9.modify(users_user_id),1L);
      }
      END_TRIGGER(exec_stats,"insert_USERS")
      END_TRIGGER(ivc_stats,"insert_USERS")
    }
    void unwrap_insert_USERS(const event_args_t& ea) {
      on_insert_USERS(*(reinterpret_cast<long*>(ea[0])));
    }
    
    void on_delete_USERS(const long users_user_id) {
      BEGIN_TRIGGER(exec_stats,"delete_USERS")
      BEGIN_TRIGGER(ivc_stats,"delete_USERS")
      {  //++tN;
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se10.modify(users_user_id),(__SQL_COUNT_AGGREGATE_1_mUSERS1.getValueOrDefault(se11.modify(users_user_id)) * -1L));
        __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se12.modify(users_user_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_USERS")
      END_TRIGGER(ivc_stats,"delete_USERS")
    }
    void unwrap_delete_USERS(const event_args_t& ea) {
      on_delete_USERS(*(reinterpret_cast<long*>(ea[0])));
    }
    
    void on_system_ready_event() {
      BEGIN_TRIGGER(exec_stats,"system_ready_event")
      BEGIN_TRIGGER(ivc_stats,"system_ready_event")
      {  //
        
      }
      END_TRIGGER(exec_stats,"system_ready_event")
      END_TRIGGER(ivc_stats,"system_ready_event")
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    __SQL_COUNT_AGGREGATE_1_entry se1;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se2;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se3;
    __SQL_COUNT_AGGREGATE_1_entry se4;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se5;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se6;
    __SQL_COUNT_AGGREGATE_1_entry se7;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se8;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se9;
    __SQL_COUNT_AGGREGATE_1_entry se10;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se11;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se12;
  
    /* Data structures used for storing materialized views */
    __SQL_COUNT_AGGREGATE_1_mTWEET1_map __SQL_COUNT_AGGREGATE_1_mTWEET1;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_map __SQL_COUNT_AGGREGATE_1_mUSERS1;
    
    
  
  };
  
  /* Type definition providing a way to execute the sql program */
  class Program : public ProgramBase
  {
    public:
      Program(int argc = 0, char* argv[] = 0) : ProgramBase(argc,argv) {
        data.register_data(*this);
        
        /* Specifying data sources */
        
        
        pair<string,string> source1_adaptor_params[] = { make_pair("schema","long,long,string") };
        std::shared_ptr<csv_adaptor> source1_adaptor(new csv_adaptor(get_relation_id("TWEET"),1,source1_adaptor_params));
        frame_descriptor source1_fd("\n");
        std::shared_ptr<dbt_file_source> source1_file(new dbt_file_source("examples/data/fastview/tweet.csv",source1_fd,source1_adaptor));
        add_source(source1_file);
        
        pair<string,string> source2_adaptor_params[] = { make_pair("schema","long") };
        std::shared_ptr<csv_adaptor> source2_adaptor(new csv_adaptor(get_relation_id("USERS"),1,source2_adaptor_params));
        frame_descriptor source2_fd("\n");
        std::shared_ptr<dbt_file_source> source2_file(new dbt_file_source("examples/data/fastview/users.csv",source2_fd,source2_adaptor));
        add_source(source2_file);
  
      }
  
      /* Imports data for static tables and performs view initialization based on it. */
      void init() {
          //P0_PLACE_HOLDER
          table_multiplexer.init_source(run_opts->batch_size, run_opts->parallel, true);
          stream_multiplexer.init_source(run_opts->batch_size, run_opts->parallel, false);
          process_tables();
          data.on_system_ready_event();
          //P2_PLACE_HOLDER
      }
  
      /* Saves a snapshot of the data required to obtain the results of top level queries. */
      snapshot_t take_snapshot(){
          tlq_t* d = new tlq_t((tlq_t&)data);
          //if (d->tS==0) { gettimeofday(&(d->t),NULL); d->tT=((d->t).tv_sec-(d->t0).tv_sec)*1000000L+((d->t).tv_usec-(d->t0).tv_usec); } printf("SAMPLE=standard,%ld,%ld,%ld\n",d->tT,d->tN,d->tS);
          return snapshot_t( d );
      }
  
    protected:
      data_t data;
  };
  class Q1pAgg : public Program
  {
    public:
      Q1pAgg(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

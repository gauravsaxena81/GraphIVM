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
  struct COUNT_entry {
    long A_USER_ID; long B_USER_ID; long B_TWEET_ID; STRING_TYPE B_TWEET_DATE; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; B_USER_ID = 0L; B_TWEET_ID = 0L; B_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4) { A_USER_ID = c0; B_USER_ID = c1; B_TWEET_ID = c2; B_TWEET_DATE = c3; __av = c4; }
    COUNT_entry(const COUNT_entry& other) : A_USER_ID( other.A_USER_ID ), B_USER_ID( other.B_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), B_TWEET_DATE( other.B_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3) { A_USER_ID = c0; B_USER_ID = c1; B_TWEET_ID = c2; B_TWEET_DATE = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      hash_combine(h, e.B_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.B_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.A_USER_ID == y.A_USER_ID && x.B_USER_ID == y.B_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.B_TWEET_DATE == y.B_TWEET_DATE;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey0123_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey0123_idxfn,true> HashIndex_COUNT_map_0123;
  
  struct COUNT_mTWEET1_entry {
    long COUNT_mTWEETTWEET_USER_ID; long __av;  COUNT_mTWEET1_entry* nxt; COUNT_mTWEET1_entry* prv;
    explicit COUNT_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mTWEETTWEET_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET1_entry(const long c0, const long c1) { COUNT_mTWEETTWEET_USER_ID = c0; __av = c1; }
    COUNT_mTWEET1_entry(const COUNT_mTWEET1_entry& other) : COUNT_mTWEETTWEET_USER_ID( other.COUNT_mTWEETTWEET_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET1_entry& modify(const long c0) { COUNT_mTWEETTWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET1_entry& x, const COUNT_mTWEET1_entry& y) {
      return x.COUNT_mTWEETTWEET_USER_ID == y.COUNT_mTWEETTWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mTWEET1_entry,long,
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,true>
  > COUNT_mTWEET1_map;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,true> HashIndex_COUNT_mTWEET1_map_0;
  
  struct COUNT_mUSERS1_entry {
    long COUNT_mUSERSUSERS_USER_ID; long B_TWEET_ID; STRING_TYPE B_TWEET_DATE; long __av;  COUNT_mUSERS1_entry* nxt; COUNT_mUSERS1_entry* prv;
    explicit COUNT_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERSUSERS_USER_ID = 0L; B_TWEET_ID = 0L; B_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS1_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; B_TWEET_DATE = c2; __av = c3; }
    COUNT_mUSERS1_entry(const COUNT_mUSERS1_entry& other) : COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), B_TWEET_DATE( other.B_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS1_entry& modify(const long c0, const long c1, const STRING_TYPE& c2) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; B_TWEET_DATE = c2;  return *this; }
    FORCE_INLINE COUNT_mUSERS1_entry& modify0(const long c0) { COUNT_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS1_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.B_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS1_entry& x, const COUNT_mUSERS1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.B_TWEET_DATE == y.B_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS1_entry& x, const COUNT_mUSERS1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS1_entry,long,
    HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey012_idxfn,true>,
    HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey0_idxfn,false>
  > COUNT_mUSERS1_map;
  typedef HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey012_idxfn,true> HashIndex_COUNT_mUSERS1_map_012;
  typedef HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey0_idxfn,false> HashIndex_COUNT_mUSERS1_map_0;
  
  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t{
    struct timeval t0,t; long tT,tN,tS;
    tlq_t(): tN(0), tS(0) { gettimeofday(&t0,NULL); }
  
  /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
  
      ar << "\n";
      const COUNT_map& _COUNT = get_COUNT();
      dbtoaster::serialize_nvp_tabbed(ar, STRING(COUNT), _COUNT, "\t");
  
    }
  
    /* Functions returning / computing the results of top level queries */
    const COUNT_map& get_COUNT() const {
      return COUNT;
    
    }
  
  protected:
  
    /* Data structures used for storing / computing top level queries */
    COUNT_map COUNT;
  
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
  
      pb.add_map<COUNT_map>( "COUNT", COUNT );
      pb.add_map<COUNT_mTWEET1_map>( "COUNT_mTWEET1", COUNT_mTWEET1 );
      pb.add_map<COUNT_mUSERS1_map>( "COUNT_mUSERS1", COUNT_mUSERS1 );
  
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
	long x = COUNT_mTWEET1.getValueOrDefault(se2.modify(tweet_user_id));
	//for(int i = 0; i < x; i++)
		api.useDiffQ1(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date);
        //COUNT.addOrDelOnZero(se1.modify(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date),COUNT_mTWEET1.getValueOrDefault(se2.modify(tweet_user_id)));
        COUNT_mUSERS1.addOrDelOnZero(se3.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),1L);
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
        COUNT.addOrDelOnZero(se4.modify(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se5.modify(tweet_user_id)) * -1L));
        COUNT_mUSERS1.addOrDelOnZero(se6.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),-1L);
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
        { //slice
          const HASH_RES_t h1 = COUNT_mUSERS1_mapkey0_idxfn::hash(se8.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS1_map_0* i1 = static_cast<HashIndex_COUNT_mUSERS1_map_0*>(COUNT_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS1_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mUSERS1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mUSERS1_mapkey0_idxfn::equals(se8, *e1)) {
            long b_tweet_id = e1->B_TWEET_ID;
            STRING_TYPE b_tweet_date = e1->B_TWEET_DATE;
            long v1 = e1->__av;
            COUNT.addOrDelOnZero(se7.modify(users_user_id,users_user_id,b_tweet_id,b_tweet_date),v1);
          } while ((n1=n1->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se9.modify(users_user_id),1L);
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
        { //slice
          const HASH_RES_t h2 = COUNT_mUSERS1_mapkey0_idxfn::hash(se11.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS1_map_0* i2 = static_cast<HashIndex_COUNT_mUSERS1_map_0*>(COUNT_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS1_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mUSERS1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mUSERS1_mapkey0_idxfn::equals(se11, *e2)) {
            long b_tweet_id = e2->B_TWEET_ID;
            STRING_TYPE b_tweet_date = e2->B_TWEET_DATE;
            long v2 = e2->__av;
            COUNT.addOrDelOnZero(se10.modify(users_user_id,users_user_id,b_tweet_id,b_tweet_date),(v2 * -1L));
          } while ((n2=n2->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se12.modify(users_user_id),-1L);
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
    COUNT_entry se1;
    COUNT_mTWEET1_entry se2;
    COUNT_mUSERS1_entry se3;
    COUNT_entry se4;
    COUNT_mTWEET1_entry se5;
    COUNT_mUSERS1_entry se6;
    COUNT_entry se7;
    COUNT_mUSERS1_entry se8;
    COUNT_mTWEET1_entry se9;
    COUNT_entry se10;
    COUNT_mUSERS1_entry se11;
    COUNT_mTWEET1_entry se12;
  
    /* Data structures used for storing materialized views */
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mUSERS1_map COUNT_mUSERS1;
    
    
  
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
  class Q1 : public Program
  {
    public:
      Q1(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

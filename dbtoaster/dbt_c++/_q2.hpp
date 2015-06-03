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
#define ELEM_SEPARATOR "\n\t\t\t"

namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct COUNT_entry {
    long F_USER_ID; long A_FOLLOWER_ID; long T_USER_ID; long B_TWEET_ID; STRING_TYPE B_TWEET_DATE; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*F_USER_ID = 0L; A_FOLLOWER_ID = 0L; T_USER_ID = 0L; B_TWEET_ID = 0L; B_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5) { F_USER_ID = c0; A_FOLLOWER_ID = c1; T_USER_ID = c2; B_TWEET_ID = c3; B_TWEET_DATE = c4; __av = c5; }
    COUNT_entry(const COUNT_entry& other) : F_USER_ID( other.F_USER_ID ), A_FOLLOWER_ID( other.A_FOLLOWER_ID ), T_USER_ID( other.T_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), B_TWEET_DATE( other.B_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4) { F_USER_ID = c0; A_FOLLOWER_ID = c1; T_USER_ID = c2; B_TWEET_ID = c3; B_TWEET_DATE = c4;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.A_FOLLOWER_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.B_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.F_USER_ID == y.F_USER_ID && x.A_FOLLOWER_ID == y.A_FOLLOWER_ID && x.T_USER_ID == y.T_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.B_TWEET_DATE == y.B_TWEET_DATE;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey01234_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey01234_idxfn,true> HashIndex_COUNT_map_01234;
  
  struct COUNT_mTWEET1_entry {
    long COUNT_mTWEETTWEET_USER_ID; long A_FOLLOWER_ID; long __av;  COUNT_mTWEET1_entry* nxt; COUNT_mTWEET1_entry* prv;
    explicit COUNT_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mTWEETTWEET_USER_ID = 0L; A_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET1_entry(const long c0, const long c1, const long c2) { COUNT_mTWEETTWEET_USER_ID = c0; A_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mTWEET1_entry(const COUNT_mTWEET1_entry& other) : COUNT_mTWEETTWEET_USER_ID( other.COUNT_mTWEETTWEET_USER_ID ), A_FOLLOWER_ID( other.A_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET1_entry& modify(const long c0, const long c1) { COUNT_mTWEETTWEET_USER_ID = c0; A_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mTWEET1_entry& modify0(const long c0) { COUNT_mTWEETTWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mTWEET1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_USER_ID);
      hash_combine(h, e.A_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET1_entry& x, const COUNT_mTWEET1_entry& y) {
      return x.COUNT_mTWEETTWEET_USER_ID == y.COUNT_mTWEETTWEET_USER_ID && x.A_FOLLOWER_ID == y.A_FOLLOWER_ID;
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
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey01_idxfn,true>,
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,false>
  > COUNT_mTWEET1_map;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey01_idxfn,true> HashIndex_COUNT_mTWEET1_map_01;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,false> HashIndex_COUNT_mTWEET1_map_0;
  
  struct COUNT_mFOLLOWER1_entry {
    long COUNT_mFOLLOWERFOLLOWER_USER_ID; long B_TWEET_ID; STRING_TYPE B_TWEET_DATE; long __av;  COUNT_mFOLLOWER1_entry* nxt; COUNT_mFOLLOWER1_entry* prv;
    explicit COUNT_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; B_TWEET_ID = 0L; B_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; B_TWEET_ID = c1; B_TWEET_DATE = c2; __av = c3; }
    COUNT_mFOLLOWER1_entry(const COUNT_mFOLLOWER1_entry& other) : COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), B_TWEET_DATE( other.B_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify(const long c0, const long c1, const STRING_TYPE& c2) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; B_TWEET_ID = c1; B_TWEET_DATE = c2;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.B_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_entry& x, const COUNT_mFOLLOWER1_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.B_TWEET_DATE == y.B_TWEET_DATE;
    }
  };
  
  struct COUNT_mFOLLOWER1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_entry& x, const COUNT_mFOLLOWER1_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER1_entry,long,
    HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey012_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER1_map;
  typedef HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey012_idxfn,true> HashIndex_COUNT_mFOLLOWER1_map_012;
  typedef HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER1_map_0;
  
  
  
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
      pb.add_map<COUNT_mFOLLOWER1_map>( "COUNT_mFOLLOWER1", COUNT_mFOLLOWER1 );
  
      pb.add_relation("FOLLOWER");
      pb.add_relation("TWEET");
  
      
  
      pb.add_trigger("FOLLOWER", insert_tuple, std::bind(&data_t::unwrap_insert_FOLLOWER, this, std::placeholders::_1));
      pb.add_trigger("FOLLOWER", delete_tuple, std::bind(&data_t::unwrap_delete_FOLLOWER, this, std::placeholders::_1));
      pb.add_trigger("TWEET", insert_tuple, std::bind(&data_t::unwrap_insert_TWEET, this, std::placeholders::_1));
      pb.add_trigger("TWEET", delete_tuple, std::bind(&data_t::unwrap_delete_TWEET, this, std::placeholders::_1));
  
      #ifdef DBT_PROFILE
      exec_stats = pb.exec_stats;
      ivc_stats = pb.ivc_stats;
      exec_stats->register_probe(0, "insert_FOLLOWER_s0");
      exec_stats->register_probe(1, "insert_FOLLOWER_s1");
      exec_stats->register_probe(2, "delete_FOLLOWER_s0");
      exec_stats->register_probe(3, "delete_FOLLOWER_s1");
      exec_stats->register_probe(4, "insert_TWEET_s0");
      exec_stats->register_probe(5, "insert_TWEET_s1");
      exec_stats->register_probe(6, "delete_TWEET_s0");
      exec_stats->register_probe(7, "delete_TWEET_s1");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_FOLLOWER(const long follower_user_id, const long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"insert_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"insert_FOLLOWER")
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = COUNT_mFOLLOWER1_mapkey0_idxfn::hash(se2.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_0* i1 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_0*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mFOLLOWER1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mFOLLOWER1_mapkey0_idxfn::equals(se2, *e1)) {
            long b_tweet_id = e1->B_TWEET_ID;
            STRING_TYPE b_tweet_date = e1->B_TWEET_DATE;
            long v1 = e1->__av;
            COUNT.addOrDelOnZero(se1.modify(follower_user_id,follower_follower_id,follower_user_id,b_tweet_id,b_tweet_date),v1);
          } while ((n1=n1->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se3.modify(follower_user_id,follower_follower_id),1L);
      }
      END_TRIGGER(exec_stats,"insert_FOLLOWER")
      END_TRIGGER(ivc_stats,"insert_FOLLOWER")
    }
    void unwrap_insert_FOLLOWER(const event_args_t& ea) {
      on_insert_FOLLOWER(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])));
    }
    
    void on_delete_FOLLOWER(const long follower_user_id, const long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"delete_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"delete_FOLLOWER")
      {  //++tN;
        { //slice
          const HASH_RES_t h2 = COUNT_mFOLLOWER1_mapkey0_idxfn::hash(se5.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_0* i2 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_0*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mFOLLOWER1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mFOLLOWER1_mapkey0_idxfn::equals(se5, *e2)) {
            long b_tweet_id = e2->B_TWEET_ID;
            STRING_TYPE b_tweet_date = e2->B_TWEET_DATE;
            long v2 = e2->__av;
            COUNT.addOrDelOnZero(se4.modify(follower_user_id,follower_follower_id,follower_user_id,b_tweet_id,b_tweet_date),(v2 * -1L));
          } while ((n2=n2->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se6.modify(follower_user_id,follower_follower_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_FOLLOWER")
      END_TRIGGER(ivc_stats,"delete_FOLLOWER")
    }
    void unwrap_delete_FOLLOWER(const event_args_t& ea) {
      on_delete_FOLLOWER(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])));
    }
    
    std::vector<void*> on_insert_TWEET(const long tweet_user_id, const long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"insert_TWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_TWEET")
      std::vector<void*> r;
      {  //++tN;
        { //slice
          const HASH_RES_t h3 = COUNT_mTWEET1_mapkey0_idxfn::hash(se8.modify0(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_0* i3 = static_cast<HashIndex_COUNT_mTWEET1_map_0*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mTWEET1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mTWEET1_mapkey0_idxfn::equals(se8, *e3)) {
 	    void *rr[5];
	    r.push_back(rr);
            const long a_follower_id = e3->A_FOLLOWER_ID;
	    rr[0] = new long(tweet_user_id);
	    rr[1] = new long(a_follower_id);
	    rr[2] = new long(tweet_user_id);
	    rr[3] = new long(tweet_tweet_id);
	    rr[4] = new STRING_TYPE(tweet_tweet_date);
            long v3 = e3->__av;
            //COUNT.addOrDelOnZero(se7.modify(tweet_user_id,a_follower_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date),v3);
                COUNT_entry e = se7.modify(tweet_user_id,a_follower_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date);
          } while ((n3=n3->nxt));
        }
        COUNT_mFOLLOWER1.addOrDelOnZero(se9.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),1L);
      }
      END_TRIGGER(exec_stats,"insert_TWEET")
      END_TRIGGER(ivc_stats,"insert_TWEET")
	return r;
    }
    void unwrap_insert_TWEET(const event_args_t& ea) {
      on_insert_TWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])));
    }
    
    void on_delete_TWEET(const long tweet_user_id, const long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"delete_TWEET")
      BEGIN_TRIGGER(ivc_stats,"delete_TWEET")
      {  //++tN;
        { //slice
          const HASH_RES_t h4 = COUNT_mTWEET1_mapkey0_idxfn::hash(se11.modify0(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_0* i4 = static_cast<HashIndex_COUNT_mTWEET1_map_0*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_0::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mTWEET1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mTWEET1_mapkey0_idxfn::equals(se11, *e4)) {
            long a_follower_id = e4->A_FOLLOWER_ID;
            long v4 = e4->__av;
            COUNT.addOrDelOnZero(se10.modify(tweet_user_id,a_follower_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
        COUNT_mFOLLOWER1.addOrDelOnZero(se12.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),-1L);
      }
      END_TRIGGER(exec_stats,"delete_TWEET")
      END_TRIGGER(ivc_stats,"delete_TWEET")
    }
    void unwrap_delete_TWEET(const event_args_t& ea) {
      on_delete_TWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])));
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
    COUNT_mFOLLOWER1_entry se2;
    COUNT_mTWEET1_entry se3;
    COUNT_entry se4;
    COUNT_mFOLLOWER1_entry se5;
    COUNT_mTWEET1_entry se6;
    COUNT_entry se7;
    COUNT_mTWEET1_entry se8;
    COUNT_mFOLLOWER1_entry se9;
    COUNT_entry se10;
    COUNT_mTWEET1_entry se11;
    COUNT_mFOLLOWER1_entry se12;
  
    /* Data structures used for storing materialized views */
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mFOLLOWER1_map COUNT_mFOLLOWER1;
    
    
  
  };
  
  /* Type definition providing a way to execute the sql program */
  class Program : public ProgramBase
  {
    public:
      Program(int argc = 0, char* argv[] = 0) : ProgramBase(argc,argv) {
        data.register_data(*this);
        
        /* Specifying data sources */
        
        
        pair<string,string> source1_adaptor_params[] = { make_pair("schema","long,long") };
        std::shared_ptr<csv_adaptor> source1_adaptor(new csv_adaptor(get_relation_id("FOLLOWER"),1,source1_adaptor_params));
        frame_descriptor source1_fd("\n");
        std::shared_ptr<dbt_file_source> source1_file(new dbt_file_source("examples/data/fastview/follower.csv",source1_fd,source1_adaptor));
        add_source(source1_file);
        
        pair<string,string> source2_adaptor_params[] = { make_pair("schema","long,long,string") };
        std::shared_ptr<csv_adaptor> source2_adaptor(new csv_adaptor(get_relation_id("TWEET"),1,source2_adaptor_params));
        frame_descriptor source2_fd("\n");
        std::shared_ptr<dbt_file_source> source2_file(new dbt_file_source("examples/data/fastview/tweet.csv",source2_fd,source2_adaptor));
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
  class Q2 : public Program
  {
    public:
      Q2(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

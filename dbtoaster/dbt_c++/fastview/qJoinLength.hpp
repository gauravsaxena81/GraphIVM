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
    long A_USER_ID; long Z_TWEET_ID; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; Z_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2) { A_USER_ID = c0; Z_TWEET_ID = c1; __av = c2; }
    COUNT_entry(const COUNT_entry& other) : A_USER_ID( other.A_USER_ID ), Z_TWEET_ID( other.Z_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1) { A_USER_ID = c0; Z_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      hash_combine(h, e.Z_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.A_USER_ID == y.A_USER_ID && x.Z_TWEET_ID == y.Z_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey01_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey01_idxfn,true> HashIndex_COUNT_map_01;
  
  struct COUNT_mTWEET1_entry {
    long A_USER_ID; long COUNT_mTWEETTWEET_USER_ID; long __av;  COUNT_mTWEET1_entry* nxt; COUNT_mTWEET1_entry* prv;
    explicit COUNT_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; COUNT_mTWEETTWEET_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET1_entry(const long c0, const long c1, const long c2) { A_USER_ID = c0; COUNT_mTWEETTWEET_USER_ID = c1; __av = c2; }
    COUNT_mTWEET1_entry(const COUNT_mTWEET1_entry& other) : A_USER_ID( other.A_USER_ID ), COUNT_mTWEETTWEET_USER_ID( other.COUNT_mTWEETTWEET_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET1_entry& modify(const long c0, const long c1) { A_USER_ID = c0; COUNT_mTWEETTWEET_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mTWEET1_entry& modify1(const long c1) { COUNT_mTWEETTWEET_USER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mTWEET1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      hash_combine(h, e.COUNT_mTWEETTWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET1_entry& x, const COUNT_mTWEET1_entry& y) {
      return x.A_USER_ID == y.A_USER_ID && x.COUNT_mTWEETTWEET_USER_ID == y.COUNT_mTWEETTWEET_USER_ID;
    }
  };
  
  struct COUNT_mTWEET1_mapkey1_idxfn {
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
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey1_idxfn,false>
  > COUNT_mTWEET1_map;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey01_idxfn,true> HashIndex_COUNT_mTWEET1_map_01;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey1_idxfn,false> HashIndex_COUNT_mTWEET1_map_1;
  
  struct COUNT_mFOLLOWER2_entry {
    long COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID; long Z_TWEET_ID; long __av;  COUNT_mFOLLOWER2_entry* nxt; COUNT_mFOLLOWER2_entry* prv;
    explicit COUNT_mFOLLOWER2_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = 0L; Z_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER2_entry(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1; __av = c2; }
    COUNT_mFOLLOWER2_entry(const COUNT_mFOLLOWER2_entry& other) : COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID( other.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID ), Z_TWEET_ID( other.Z_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER2_entry& modify(const long c0, const long c1) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER2_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      hash_combine(h, e.Z_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER2_entry& x, const COUNT_mFOLLOWER2_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID && x.Z_TWEET_ID == y.Z_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER2_entry& x, const COUNT_mFOLLOWER2_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER2_entry,long,
    HashIndex<COUNT_mFOLLOWER2_entry,long,COUNT_mFOLLOWER2_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER2_entry,long,COUNT_mFOLLOWER2_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER2_map;
  typedef HashIndex<COUNT_mFOLLOWER2_entry,long,COUNT_mFOLLOWER2_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER2_map_01;
  typedef HashIndex<COUNT_mFOLLOWER2_entry,long,COUNT_mFOLLOWER2_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER2_map_0;
  
  struct COUNT_mFOLLOWER4_entry {
    long A_USER_ID; long COUNT_mFOLLOWERFOLLOWER_USER_ID; long __av;  COUNT_mFOLLOWER4_entry* nxt; COUNT_mFOLLOWER4_entry* prv;
    explicit COUNT_mFOLLOWER4_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER4_entry(const long c0, const long c1, const long c2) { A_USER_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; __av = c2; }
    COUNT_mFOLLOWER4_entry(const COUNT_mFOLLOWER4_entry& other) : A_USER_ID( other.A_USER_ID ), COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER4_entry& modify(const long c0, const long c1) { A_USER_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER4_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER4_entry& modify0(const long c0) { A_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER4_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER4_entry& x, const COUNT_mFOLLOWER4_entry& y) {
      return x.A_USER_ID == y.A_USER_ID && x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER4_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER4_entry& x, const COUNT_mFOLLOWER4_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER4_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER4_entry& x, const COUNT_mFOLLOWER4_entry& y) {
      return x.A_USER_ID == y.A_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER4_entry,long,
    HashIndex<COUNT_mFOLLOWER4_entry,long,COUNT_mFOLLOWER4_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER4_entry,long,COUNT_mFOLLOWER4_mapkey1_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER4_entry,long,COUNT_mFOLLOWER4_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER4_map;
  typedef HashIndex<COUNT_mFOLLOWER4_entry,long,COUNT_mFOLLOWER4_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER4_map_01;
  typedef HashIndex<COUNT_mFOLLOWER4_entry,long,COUNT_mFOLLOWER4_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER4_map_1;
  typedef HashIndex<COUNT_mFOLLOWER4_entry,long,COUNT_mFOLLOWER4_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER4_map_0;
  
  struct COUNT_mFOLLOWER5_entry {
    long COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID; long Z_TWEET_ID; long __av;  COUNT_mFOLLOWER5_entry* nxt; COUNT_mFOLLOWER5_entry* prv;
    explicit COUNT_mFOLLOWER5_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = 0L; Z_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER5_entry(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1; __av = c2; }
    COUNT_mFOLLOWER5_entry(const COUNT_mFOLLOWER5_entry& other) : COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID( other.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID ), Z_TWEET_ID( other.Z_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER5_entry& modify(const long c0, const long c1) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER5_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER5_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      hash_combine(h, e.Z_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER5_entry& x, const COUNT_mFOLLOWER5_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID && x.Z_TWEET_ID == y.Z_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER5_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER5_entry& x, const COUNT_mFOLLOWER5_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER5_entry,long,
    HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER5_map;
  typedef HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER5_map_01;
  typedef HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER5_map_0;
  
  struct tuple2_L_L {
    long _1; long __av; tuple2_L_L* nxt; tuple2_L_L* prv;
    explicit tuple2_L_L() : nxt(nullptr), prv(nullptr) { }
    explicit tuple2_L_L(const long c1, long c__av=0L) : nxt(nullptr), prv(nullptr) { _1 = c1; __av = c__av;}
    int operator<(const tuple2_L_L &rhs) const { 
      if(this->_1!=rhs._1) return (this->_1<rhs._1);
      return 0;
    }
    int operator==(const tuple2_L_L &rhs) const { return ((this->_1==rhs._1)); }
    FORCE_INLINE tuple2_L_L& modify(const long c0, long c__av) { _1 = c0; __av = c__av; return *this; }
    static bool equals(const tuple2_L_L &x, const tuple2_L_L &y) { return ((x._1==y._1)); }
    static long hash(const tuple2_L_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      return h;
    }
  };
  
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
    data_t(): tlq_t(), _c1(16U), _c4(16U), _c3(16U), _c2(16U) {
      
    }
  
    #ifdef DBT_PROFILE
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> exec_stats;
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> ivc_stats;
    #endif
  
    /* Registering relations and trigger functions */
    void register_data(ProgramBase& pb) {
  
      pb.add_map<COUNT_map>( "COUNT", COUNT );
      pb.add_map<COUNT_mTWEET1_map>( "COUNT_mTWEET1", COUNT_mTWEET1 );
      pb.add_map<COUNT_mFOLLOWER2_map>( "COUNT_mFOLLOWER2", COUNT_mFOLLOWER2 );
      pb.add_map<COUNT_mFOLLOWER4_map>( "COUNT_mFOLLOWER4", COUNT_mFOLLOWER4 );
      pb.add_map<COUNT_mFOLLOWER5_map>( "COUNT_mFOLLOWER5", COUNT_mFOLLOWER5 );
  
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
      exec_stats->register_probe(2, "insert_FOLLOWER_s2");
      exec_stats->register_probe(3, "insert_FOLLOWER_s3");
      exec_stats->register_probe(4, "delete_FOLLOWER_s0");
      exec_stats->register_probe(5, "delete_FOLLOWER_s1");
      exec_stats->register_probe(6, "delete_FOLLOWER_s2");
      exec_stats->register_probe(7, "delete_FOLLOWER_s3");
      exec_stats->register_probe(8, "insert_TWEET_s0");
      exec_stats->register_probe(9, "insert_TWEET_s1");
      exec_stats->register_probe(10, "insert_TWEET_s2");
      exec_stats->register_probe(11, "delete_TWEET_s0");
      exec_stats->register_probe(12, "delete_TWEET_s1");
      exec_stats->register_probe(13, "delete_TWEET_s2");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_FOLLOWER(const long follower_user_id, const long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"insert_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"insert_FOLLOWER")
      {  //++tN;
        _c1.clear();
        { //slice
          const HASH_RES_t h1 = COUNT_mFOLLOWER2_mapkey0_idxfn::hash(se2.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER2_map_0* i1 = static_cast<HashIndex_COUNT_mFOLLOWER2_map_0*>(COUNT_mFOLLOWER2.index[1]);
          HashIndex_COUNT_mFOLLOWER2_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mFOLLOWER2_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mFOLLOWER2_mapkey0_idxfn::equals(se2, *e1)) {
            long z_tweet_id = e1->Z_TWEET_ID;
            long v2 = e1->__av;
            add_to_temp_map(_c1, st1.modify(z_tweet_id,v2));
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se3.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i2 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mFOLLOWER5_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se3, *e2)) {
            long z_tweet_id = e2->Z_TWEET_ID;
            long v3 = e2->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c1, st2.modify(z_tweet_id,v3)) : voidFunc());
          } while ((n2=n2->nxt));
        }
        { //temp foreach
          tuple2_L_L* e3 = _c1.head;
          while(e3){
            long z_tweet_id = e3->_1;
            long v4 = e3->__av;
            COUNT.addOrDelOnZero(se1.modify(follower_user_id,z_tweet_id),v4);
            e3 = e3->nxt;
          }
        }
        { //slice
          const HASH_RES_t h4 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se5.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i3 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n3 = &(i3->buckets_[h4 % i3->size_]);
          COUNT_mFOLLOWER4_entry* e4;
          do if ((e4=n3->obj) && h4 == n3->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se5, *e4)) {
            long a_user_id = e4->A_USER_ID;
            long v5 = e4->__av;
            { //slice
              const HASH_RES_t h3 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se4.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER5_map_0* i4 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
              HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n4 = &(i4->buckets_[h3 % i4->size_]);
              COUNT_mFOLLOWER5_entry* e5;
              do if ((e5=n4->obj) && h3 == n4->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se4, *e5)) {
                long z_tweet_id = e5->Z_TWEET_ID;
                long v6 = e5->__av;
                COUNT.addOrDelOnZero(se1.modify(a_user_id,z_tweet_id),(v5 * v6));
              } while ((n4=n4->nxt));
            }
          } while ((n3=n3->nxt));
        }
        _c2.clear();
        { //slice
          const HASH_RES_t h5 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se7.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i5 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mFOLLOWER4_entry* e6;
          do if ((e6=n5->obj) && h5 == n5->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se7, *e6)) {
            long count_mtweettweet_user_id = e6->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v8 = e6->__av;
            add_to_temp_map(_c2, st3.modify(count_mtweettweet_user_id,v8));
          } while ((n5=n5->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c2, st4.modify(follower_follower_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e7 = _c2.head;
          while(e7){
            long count_mtweettweet_user_id = e7->_1;
            long v9 = e7->__av;
            COUNT_mTWEET1.addOrDelOnZero(se6.modify(follower_user_id,count_mtweettweet_user_id),v9);
            e7 = e7->nxt;
          }
        }
        { //slice
          const HASH_RES_t h6 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se8.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i6 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mFOLLOWER4_entry* e8;
          do if ((e8=n6->obj) && h6 == n6->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se8, *e8)) {
            long a_user_id = e8->A_USER_ID;
            long v10 = e8->__av;
            COUNT_mTWEET1.addOrDelOnZero(se6.modify(a_user_id,follower_follower_id),v10);
          } while ((n6=n6->nxt));
        }
        { //slice
          const HASH_RES_t h7 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se10.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i7 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          COUNT_mFOLLOWER5_entry* e9;
          do if ((e9=n7->obj) && h7 == n7->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se10, *e9)) {
            long z_tweet_id = e9->Z_TWEET_ID;
            long v11 = e9->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se9.modify(follower_user_id,z_tweet_id),v11);
          } while ((n7=n7->nxt));
        }
        COUNT_mFOLLOWER4.addOrDelOnZero(se11.modify(follower_user_id,follower_follower_id),1L);
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
        _c3.clear();
        { //slice
          const HASH_RES_t h8 = COUNT_mFOLLOWER2_mapkey0_idxfn::hash(se13.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER2_map_0* i8 = static_cast<HashIndex_COUNT_mFOLLOWER2_map_0*>(COUNT_mFOLLOWER2.index[1]);
          HashIndex_COUNT_mFOLLOWER2_map_0::IdxNode* n8 = &(i8->buckets_[h8 % i8->size_]);
          COUNT_mFOLLOWER2_entry* e10;
          do if ((e10=n8->obj) && h8 == n8->hash && COUNT_mFOLLOWER2_mapkey0_idxfn::equals(se13, *e10)) {
            long z_tweet_id = e10->Z_TWEET_ID;
            long v13 = e10->__av;
            add_to_temp_map(_c3, st5.modify(z_tweet_id,(v13 * -1L)));
          } while ((n8=n8->nxt));
        }
        { //slice
          const HASH_RES_t h9 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se14.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i9 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n9 = &(i9->buckets_[h9 % i9->size_]);
          COUNT_mFOLLOWER5_entry* e11;
          do if ((e11=n9->obj) && h9 == n9->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se14, *e11)) {
            long z_tweet_id = e11->Z_TWEET_ID;
            long v14 = e11->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c3, st6.modify(z_tweet_id,v14)) : voidFunc());
          } while ((n9=n9->nxt));
        }
        { //temp foreach
          tuple2_L_L* e12 = _c3.head;
          while(e12){
            long z_tweet_id = e12->_1;
            long v15 = e12->__av;
            COUNT.addOrDelOnZero(se12.modify(follower_user_id,z_tweet_id),v15);
            e12 = e12->nxt;
          }
        }
        { //slice
          const HASH_RES_t h11 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se16.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i10 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n10 = &(i10->buckets_[h11 % i10->size_]);
          COUNT_mFOLLOWER4_entry* e13;
          do if ((e13=n10->obj) && h11 == n10->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se16, *e13)) {
            long a_user_id = e13->A_USER_ID;
            long v16 = e13->__av;
            { //slice
              const HASH_RES_t h10 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se15.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER5_map_0* i11 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
              HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n11 = &(i11->buckets_[h10 % i11->size_]);
              COUNT_mFOLLOWER5_entry* e14;
              do if ((e14=n11->obj) && h10 == n11->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se15, *e14)) {
                long z_tweet_id = e14->Z_TWEET_ID;
                long v17 = e14->__av;
                COUNT.addOrDelOnZero(se12.modify(a_user_id,z_tweet_id),(v16 * (v17 * -1L)));
              } while ((n11=n11->nxt));
            }
          } while ((n10=n10->nxt));
        }
        _c4.clear();
        { //slice
          const HASH_RES_t h12 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se18.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i12 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n12 = &(i12->buckets_[h12 % i12->size_]);
          COUNT_mFOLLOWER4_entry* e15;
          do if ((e15=n12->obj) && h12 == n12->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se18, *e15)) {
            long count_mtweettweet_user_id = e15->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v19 = e15->__av;
            add_to_temp_map(_c4, st7.modify(count_mtweettweet_user_id,(v19 * -1L)));
          } while ((n12=n12->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c4, st8.modify(follower_follower_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e16 = _c4.head;
          while(e16){
            long count_mtweettweet_user_id = e16->_1;
            long v20 = e16->__av;
            COUNT_mTWEET1.addOrDelOnZero(se17.modify(follower_user_id,count_mtweettweet_user_id),v20);
            e16 = e16->nxt;
          }
        }
        { //slice
          const HASH_RES_t h13 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se19.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i13 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n13 = &(i13->buckets_[h13 % i13->size_]);
          COUNT_mFOLLOWER4_entry* e17;
          do if ((e17=n13->obj) && h13 == n13->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se19, *e17)) {
            long a_user_id = e17->A_USER_ID;
            long v21 = e17->__av;
            COUNT_mTWEET1.addOrDelOnZero(se17.modify(a_user_id,follower_follower_id),(v21 * -1L));
          } while ((n13=n13->nxt));
        }
        { //slice
          const HASH_RES_t h14 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se21.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i14 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n14 = &(i14->buckets_[h14 % i14->size_]);
          COUNT_mFOLLOWER5_entry* e18;
          do if ((e18=n14->obj) && h14 == n14->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se21, *e18)) {
            long z_tweet_id = e18->Z_TWEET_ID;
            long v22 = e18->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se20.modify(follower_user_id,z_tweet_id),(v22 * -1L));
          } while ((n14=n14->nxt));
        }
        COUNT_mFOLLOWER4.addOrDelOnZero(se22.modify(follower_user_id,follower_follower_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_FOLLOWER")
      END_TRIGGER(ivc_stats,"delete_FOLLOWER")
    }
    void unwrap_delete_FOLLOWER(const event_args_t& ea) {
      on_delete_FOLLOWER(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])));
    }
    
    void on_insert_TWEET(const long tweet_user_id, const long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"insert_TWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_TWEET")
      {  //++tN;
        { //slice
          const HASH_RES_t h15 = COUNT_mTWEET1_mapkey1_idxfn::hash(se24.modify1(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_1* i15 = static_cast<HashIndex_COUNT_mTWEET1_map_1*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_1::IdxNode* n15 = &(i15->buckets_[h15 % i15->size_]);
          COUNT_mTWEET1_entry* e19;
          do if ((e19=n15->obj) && h15 == n15->hash && COUNT_mTWEET1_mapkey1_idxfn::equals(se24, *e19)) {
            long a_user_id = e19->A_USER_ID;
            long v23 = e19->__av;
		//for(int i = 0;i < v23; i++) {
			api.useDiff(tweet_user_id,tweet_tweet_id,a_user_id);
		//}
            //COUNT.addOrDelOnZero(se23.modify(a_user_id,tweet_tweet_id),v23);
          } while ((n15=n15->nxt));
        }
        { //slice
          const HASH_RES_t h16 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se26.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i16 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          COUNT_mFOLLOWER4_entry* e20;
          do if ((e20=n16->obj) && h16 == n16->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se26, *e20)) {
            long count_mfollowerfollower_follower_id = e20->A_USER_ID;
            long v24 = e20->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se25.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),v24);
          } while ((n16=n16->nxt));
        }
        COUNT_mFOLLOWER5.addOrDelOnZero(se27.modify(tweet_user_id,tweet_tweet_id),1L);
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
        { //slice
          const HASH_RES_t h17 = COUNT_mTWEET1_mapkey1_idxfn::hash(se29.modify1(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_1* i17 = static_cast<HashIndex_COUNT_mTWEET1_map_1*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_1::IdxNode* n17 = &(i17->buckets_[h17 % i17->size_]);
          COUNT_mTWEET1_entry* e21;
          do if ((e21=n17->obj) && h17 == n17->hash && COUNT_mTWEET1_mapkey1_idxfn::equals(se29, *e21)) {
            long a_user_id = e21->A_USER_ID;
            long v25 = e21->__av;
            COUNT.addOrDelOnZero(se28.modify(a_user_id,tweet_tweet_id),(v25 * -1L));
          } while ((n17=n17->nxt));
        }
        { //slice
          const HASH_RES_t h18 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se31.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i18 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n18 = &(i18->buckets_[h18 % i18->size_]);
          COUNT_mFOLLOWER4_entry* e22;
          do if ((e22=n18->obj) && h18 == n18->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se31, *e22)) {
            long count_mfollowerfollower_follower_id = e22->A_USER_ID;
            long v26 = e22->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se30.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),(v26 * -1L));
          } while ((n18=n18->nxt));
        }
        COUNT_mFOLLOWER5.addOrDelOnZero(se32.modify(tweet_user_id,tweet_tweet_id),-1L);
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
    tuple2_L_L st1;
    COUNT_mFOLLOWER2_entry se2;
    tuple2_L_L st2;
    COUNT_mFOLLOWER5_entry se3;
    COUNT_mFOLLOWER5_entry se4;
    COUNT_mFOLLOWER4_entry se5;
    COUNT_mTWEET1_entry se6;
    tuple2_L_L st3;
    COUNT_mFOLLOWER4_entry se7;
    tuple2_L_L st4;
    COUNT_mFOLLOWER4_entry se8;
    COUNT_mFOLLOWER2_entry se9;
    COUNT_mFOLLOWER5_entry se10;
    COUNT_mFOLLOWER4_entry se11;
    COUNT_entry se12;
    tuple2_L_L st5;
    COUNT_mFOLLOWER2_entry se13;
    tuple2_L_L st6;
    COUNT_mFOLLOWER5_entry se14;
    COUNT_mFOLLOWER5_entry se15;
    COUNT_mFOLLOWER4_entry se16;
    COUNT_mTWEET1_entry se17;
    tuple2_L_L st7;
    COUNT_mFOLLOWER4_entry se18;
    tuple2_L_L st8;
    COUNT_mFOLLOWER4_entry se19;
    COUNT_mFOLLOWER2_entry se20;
    COUNT_mFOLLOWER5_entry se21;
    COUNT_mFOLLOWER4_entry se22;
    COUNT_entry se23;
    COUNT_mTWEET1_entry se24;
    COUNT_mFOLLOWER2_entry se25;
    COUNT_mFOLLOWER4_entry se26;
    COUNT_mFOLLOWER5_entry se27;
    COUNT_entry se28;
    COUNT_mTWEET1_entry se29;
    COUNT_mFOLLOWER2_entry se30;
    COUNT_mFOLLOWER4_entry se31;
    COUNT_mFOLLOWER5_entry se32;
  
    /* Data structures used for storing materialized views */
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mFOLLOWER2_map COUNT_mFOLLOWER2;
    COUNT_mFOLLOWER4_map COUNT_mFOLLOWER4;
    COUNT_mFOLLOWER5_map COUNT_mFOLLOWER5;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c1;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c4;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c3;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c2;
    
  
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
        std::shared_ptr<dbt_file_source> source1_file(new dbt_file_source("examples/data/fastviewsmall/follower.csv",source1_fd,source1_adaptor));
        add_source(source1_file);
        
        pair<string,string> source2_adaptor_params[] = { make_pair("schema","long,long,string") };
        std::shared_ptr<csv_adaptor> source2_adaptor(new csv_adaptor(get_relation_id("TWEET"),1,source2_adaptor_params));
        frame_descriptor source2_fd("\n");
        std::shared_ptr<dbt_file_source> source2_file(new dbt_file_source("examples/data/fastviewsmall/tweet.csv",source2_fd,source2_adaptor));
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
  class QJoinLength : public Program
  {
    public:
      QJoinLength(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

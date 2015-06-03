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
    long A_TWEET_ID; long Z_FOLLOWER_ID; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; Z_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2) { A_TWEET_ID = c0; Z_FOLLOWER_ID = c1; __av = c2; }
    COUNT_entry(const COUNT_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1) { A_TWEET_ID = c0; Z_FOLLOWER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey01_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey01_idxfn,true> HashIndex_COUNT_map_01;
  
  struct COUNT_mFOLLOWER1_entry {
    long A_TWEET_ID; long COUNT_mFOLLOWERFOLLOWER_USER_ID; long __av;  COUNT_mFOLLOWER1_entry* nxt; COUNT_mFOLLOWER1_entry* prv;
    explicit COUNT_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_entry(const long c0, const long c1, const long c2) { A_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; __av = c2; }
    COUNT_mFOLLOWER1_entry(const COUNT_mFOLLOWER1_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify(const long c0, const long c1) { A_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_entry& x, const COUNT_mFOLLOWER1_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mapkey1_idxfn {
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
    HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey1_idxfn,false>
  > COUNT_mFOLLOWER1_map;
  typedef HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER1_map_01;
  typedef HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER1_map_1;
  
  struct COUNT_mFOLLOWER1_mRETWEET2_entry {
    long COUNT_mFOLLOWERFOLLOWER_USER_ID; long COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID; long __av;  COUNT_mFOLLOWER1_mRETWEET2_entry* nxt; COUNT_mFOLLOWER1_mRETWEET2_entry* prv;
    explicit COUNT_mFOLLOWER1_mRETWEET2_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_mRETWEET2_entry(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1; __av = c2; }
    COUNT_mFOLLOWER1_mRETWEET2_entry(const COUNT_mFOLLOWER1_mRETWEET2_entry& other) : COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID( other.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_mRETWEET2_entry& modify(const long c0, const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mRETWEET2_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mRETWEET2_entry& modify1(const long c1) { COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mRETWEET2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mRETWEET2_entry& x, const COUNT_mFOLLOWER1_mRETWEET2_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID && x.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mRETWEET2_entry& x, const COUNT_mFOLLOWER1_mRETWEET2_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mRETWEET2_entry& x, const COUNT_mFOLLOWER1_mRETWEET2_entry& y) {
      return x.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER1_mRETWEET2_entry,long,
    HashIndex<COUNT_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mFOLLOWER1_mRETWEET2_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn,false>
  > COUNT_mFOLLOWER1_mRETWEET2_map;
  typedef HashIndex<COUNT_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mFOLLOWER1_mRETWEET2_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_01;
  typedef HashIndex<COUNT_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0;
  typedef HashIndex<COUNT_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1;
  
  struct COUNT_mRETWEET2_entry {
    long COUNT_mRETWEETRETWEET_TWEET_ID; long Z_FOLLOWER_ID; long __av;  COUNT_mRETWEET2_entry* nxt; COUNT_mRETWEET2_entry* prv;
    explicit COUNT_mRETWEET2_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_TWEET_ID = 0L; Z_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET2_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_TWEET_ID = c0; Z_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mRETWEET2_entry(const COUNT_mRETWEET2_entry& other) : COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET2_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_TWEET_ID = c0; Z_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET2_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET2_entry& x, const COUNT_mRETWEET2_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID;
    }
  };
  
  struct COUNT_mRETWEET2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET2_entry& x, const COUNT_mRETWEET2_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET2_entry,long,
    HashIndex<COUNT_mRETWEET2_entry,long,COUNT_mRETWEET2_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET2_entry,long,COUNT_mRETWEET2_mapkey0_idxfn,false>
  > COUNT_mRETWEET2_map;
  typedef HashIndex<COUNT_mRETWEET2_entry,long,COUNT_mRETWEET2_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET2_map_01;
  typedef HashIndex<COUNT_mRETWEET2_entry,long,COUNT_mRETWEET2_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET2_map_0;
  
  struct COUNT_mRETWEET5_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long __av;  COUNT_mRETWEET5_entry* nxt; COUNT_mRETWEET5_entry* prv;
    explicit COUNT_mRETWEET5_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET5_entry(const long c0, const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; __av = c1; }
    COUNT_mRETWEET5_entry(const COUNT_mRETWEET5_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET5_entry& modify(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET5_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET5_entry& x, const COUNT_mRETWEET5_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET5_entry,long,
    HashIndex<COUNT_mRETWEET5_entry,long,COUNT_mRETWEET5_mapkey0_idxfn,true>
  > COUNT_mRETWEET5_map;
  typedef HashIndex<COUNT_mRETWEET5_entry,long,COUNT_mRETWEET5_mapkey0_idxfn,true> HashIndex_COUNT_mRETWEET5_map_0;
  
  struct COUNT_mRETWEET6_entry {
    long COUNT_mRETWEETRETWEET_USER_ID; long Z_FOLLOWER_ID; long __av;  COUNT_mRETWEET6_entry* nxt; COUNT_mRETWEET6_entry* prv;
    explicit COUNT_mRETWEET6_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_USER_ID = 0L; Z_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET6_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_USER_ID = c0; Z_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mRETWEET6_entry(const COUNT_mRETWEET6_entry& other) : COUNT_mRETWEETRETWEET_USER_ID( other.COUNT_mRETWEETRETWEET_USER_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET6_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_USER_ID = c0; Z_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET6_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET6_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET6_entry& x, const COUNT_mRETWEET6_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID;
    }
  };
  
  struct COUNT_mRETWEET6_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET6_entry& x, const COUNT_mRETWEET6_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET6_entry,long,
    HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey0_idxfn,false>
  > COUNT_mRETWEET6_map;
  typedef HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET6_map_01;
  typedef HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET6_map_0;
  
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
      pb.add_map<COUNT_mFOLLOWER1_map>( "COUNT_mFOLLOWER1", COUNT_mFOLLOWER1 );
      pb.add_map<COUNT_mFOLLOWER1_mRETWEET2_map>( "COUNT_mFOLLOWER1_mRETWEET2", COUNT_mFOLLOWER1_mRETWEET2 );
      pb.add_map<COUNT_mRETWEET2_map>( "COUNT_mRETWEET2", COUNT_mRETWEET2 );
      pb.add_map<COUNT_mRETWEET5_map>( "COUNT_mRETWEET5", COUNT_mRETWEET5 );
      pb.add_map<COUNT_mRETWEET6_map>( "COUNT_mRETWEET6", COUNT_mRETWEET6 );
  
      pb.add_relation("FOLLOWER");
      pb.add_relation("RETWEET");
  
      
  
      pb.add_trigger("FOLLOWER", insert_tuple, std::bind(&data_t::unwrap_insert_FOLLOWER, this, std::placeholders::_1));
      pb.add_trigger("FOLLOWER", delete_tuple, std::bind(&data_t::unwrap_delete_FOLLOWER, this, std::placeholders::_1));
      pb.add_trigger("RETWEET", insert_tuple, std::bind(&data_t::unwrap_insert_RETWEET, this, std::placeholders::_1));
      pb.add_trigger("RETWEET", delete_tuple, std::bind(&data_t::unwrap_delete_RETWEET, this, std::placeholders::_1));
  
      #ifdef DBT_PROFILE
      exec_stats = pb.exec_stats;
      ivc_stats = pb.ivc_stats;
      exec_stats->register_probe(0, "insert_FOLLOWER_s0");
      exec_stats->register_probe(1, "insert_FOLLOWER_s1");
      exec_stats->register_probe(2, "insert_FOLLOWER_s2");
      exec_stats->register_probe(3, "delete_FOLLOWER_s0");
      exec_stats->register_probe(4, "delete_FOLLOWER_s1");
      exec_stats->register_probe(5, "delete_FOLLOWER_s2");
      exec_stats->register_probe(6, "insert_RETWEET_s0");
      exec_stats->register_probe(7, "insert_RETWEET_s1");
      exec_stats->register_probe(8, "insert_RETWEET_s2");
      exec_stats->register_probe(9, "insert_RETWEET_s3");
      exec_stats->register_probe(10, "insert_RETWEET_s4");
      exec_stats->register_probe(11, "delete_RETWEET_s0");
      exec_stats->register_probe(12, "delete_RETWEET_s1");
      exec_stats->register_probe(13, "delete_RETWEET_s2");
      exec_stats->register_probe(14, "delete_RETWEET_s3");
      exec_stats->register_probe(15, "delete_RETWEET_s4");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_FOLLOWER(const long follower_user_id, const long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"insert_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"insert_FOLLOWER")
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = COUNT_mFOLLOWER1_mapkey1_idxfn::hash(se2.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_1* i1 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_1*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_1::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mFOLLOWER1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mFOLLOWER1_mapkey1_idxfn::equals(se2, *e1)) {
            long a_tweet_id = e1->A_TWEET_ID;
            long v1 = e1->__av;
		api.useDiff(a_tweet_id, follower_follower_id);
            //COUNT.addOrDelOnZero(se1.modify(a_tweet_id,follower_follower_id),v1);
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::hash(se4.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0* i2 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0*>(COUNT_mFOLLOWER1_mRETWEET2.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mFOLLOWER1_mRETWEET2_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::equals(se4, *e2)) {
            long count_mretweetretweet_tweet_id = e2->COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long v2 = e2->__av;
            COUNT_mRETWEET2.addOrDelOnZero(se3.modify(count_mretweetretweet_tweet_id,follower_follower_id),v2);
          } while ((n2=n2->nxt));
        }
        COUNT_mRETWEET6.addOrDelOnZero(se5.modify(follower_user_id,follower_follower_id),1L);
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
          const HASH_RES_t h3 = COUNT_mFOLLOWER1_mapkey1_idxfn::hash(se7.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_1* i3 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_1*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_1::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mFOLLOWER1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mFOLLOWER1_mapkey1_idxfn::equals(se7, *e3)) {
            long a_tweet_id = e3->A_TWEET_ID;
            long v3 = e3->__av;
            COUNT.addOrDelOnZero(se6.modify(a_tweet_id,follower_follower_id),(v3 * -1L));
          } while ((n3=n3->nxt));
        }
        { //slice
          const HASH_RES_t h4 = COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::hash(se9.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0* i4 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0*>(COUNT_mFOLLOWER1_mRETWEET2.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_0::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mFOLLOWER1_mRETWEET2_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::equals(se9, *e4)) {
            long count_mretweetretweet_tweet_id = e4->COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long v4 = e4->__av;
            COUNT_mRETWEET2.addOrDelOnZero(se8.modify(count_mretweetretweet_tweet_id,follower_follower_id),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
        COUNT_mRETWEET6.addOrDelOnZero(se10.modify(follower_user_id,follower_follower_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_FOLLOWER")
      END_TRIGGER(ivc_stats,"delete_FOLLOWER")
    }
    void unwrap_delete_FOLLOWER(const event_args_t& ea) {
      on_delete_FOLLOWER(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])));
    }
    
    void on_insert_RETWEET(const long retweet_user_id, const long retweet_tweet_id, const STRING_TYPE& retweet_tweet_date, const long retweet_retweet_tweet_id) {
      BEGIN_TRIGGER(exec_stats,"insert_RETWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_RETWEET")
      {  //++tN;
        _c1.clear();
        { //slice
          const HASH_RES_t h5 = COUNT_mRETWEET2_mapkey0_idxfn::hash(se12.modify0(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET2_map_0* i5 = static_cast<HashIndex_COUNT_mRETWEET2_map_0*>(COUNT_mRETWEET2.index[1]);
          HashIndex_COUNT_mRETWEET2_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mRETWEET2_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mRETWEET2_mapkey0_idxfn::equals(se12, *e5)) {
            long z_follower_id = e5->Z_FOLLOWER_ID;
            long v6 = e5->__av;
            add_to_temp_map(_c1, st1.modify(z_follower_id,v6));
          } while ((n5=n5->nxt));
        }
        { //slice
          const HASH_RES_t h6 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se13.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i6 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mRETWEET6_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se13, *e6)) {
            long z_follower_id = e6->Z_FOLLOWER_ID;
            long v7 = e6->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c1, st2.modify(z_follower_id,v7)) : voidFunc());
          } while ((n6=n6->nxt));
        }
        { //temp foreach
          tuple2_L_L* e7 = _c1.head;
          while(e7){
            long z_follower_id = e7->_1;
            long v8 = e7->__av;
            COUNT.addOrDelOnZero(se11.modify(retweet_tweet_id,z_follower_id),v8);
            e7 = e7->nxt;
          }
        }
        { //slice
          const HASH_RES_t h7 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se15.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i7 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          COUNT_mRETWEET6_entry* e8;
          do if ((e8=n7->obj) && h7 == n7->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se15, *e8)) {
            long z_follower_id = e8->Z_FOLLOWER_ID;
            long v9 = e8->__av;
            COUNT.addOrDelOnZero(se11.modify(retweet_retweet_tweet_id,z_follower_id),(COUNT_mRETWEET5.getValueOrDefault(se14.modify(retweet_retweet_tweet_id)) * v9));
          } while ((n7=n7->nxt));
        }
        _c2.clear();
        { //slice
          const HASH_RES_t h8 = COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se17.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1* i8 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mFOLLOWER1_mRETWEET2.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n8 = &(i8->buckets_[h8 % i8->size_]);
          COUNT_mFOLLOWER1_mRETWEET2_entry* e9;
          do if ((e9=n8->obj) && h8 == n8->hash && COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se17, *e9)) {
            long count_mfollowerfollower_user_id = e9->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v11 = e9->__av;
            add_to_temp_map(_c2, st3.modify(count_mfollowerfollower_user_id,v11));
          } while ((n8=n8->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c2, st4.modify(retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e10 = _c2.head;
          while(e10){
            long count_mfollowerfollower_user_id = e10->_1;
            long v12 = e10->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se16.modify(retweet_tweet_id,count_mfollowerfollower_user_id),v12);
            e10 = e10->nxt;
          }
        }
        COUNT_mFOLLOWER1.addOrDelOnZero(se16.modify(retweet_retweet_tweet_id,retweet_user_id),COUNT_mRETWEET5.getValueOrDefault(se18.modify(retweet_retweet_tweet_id)));
        COUNT_mFOLLOWER1_mRETWEET2.addOrDelOnZero(se19.modify(retweet_user_id,retweet_retweet_tweet_id),1L);
        { //slice
          const HASH_RES_t h9 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se21.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i9 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n9 = &(i9->buckets_[h9 % i9->size_]);
          COUNT_mRETWEET6_entry* e11;
          do if ((e11=n9->obj) && h9 == n9->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se21, *e11)) {
            long z_follower_id = e11->Z_FOLLOWER_ID;
            long v13 = e11->__av;
            COUNT_mRETWEET2.addOrDelOnZero(se20.modify(retweet_retweet_tweet_id,z_follower_id),v13);
          } while ((n9=n9->nxt));
        }
        COUNT_mRETWEET5.addOrDelOnZero(se22.modify(retweet_tweet_id),1L);
      }
      END_TRIGGER(exec_stats,"insert_RETWEET")
      END_TRIGGER(ivc_stats,"insert_RETWEET")
    }
    void unwrap_insert_RETWEET(const event_args_t& ea) {
      on_insert_RETWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])), *(reinterpret_cast<long*>(ea[3])));
    }
    
    void on_delete_RETWEET(const long retweet_user_id, const long retweet_tweet_id, const STRING_TYPE& retweet_tweet_date, const long retweet_retweet_tweet_id) {
      BEGIN_TRIGGER(exec_stats,"delete_RETWEET")
      BEGIN_TRIGGER(ivc_stats,"delete_RETWEET")
      {  //++tN;
        _c3.clear();
        { //slice
          const HASH_RES_t h10 = COUNT_mRETWEET2_mapkey0_idxfn::hash(se24.modify0(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET2_map_0* i10 = static_cast<HashIndex_COUNT_mRETWEET2_map_0*>(COUNT_mRETWEET2.index[1]);
          HashIndex_COUNT_mRETWEET2_map_0::IdxNode* n10 = &(i10->buckets_[h10 % i10->size_]);
          COUNT_mRETWEET2_entry* e12;
          do if ((e12=n10->obj) && h10 == n10->hash && COUNT_mRETWEET2_mapkey0_idxfn::equals(se24, *e12)) {
            long z_follower_id = e12->Z_FOLLOWER_ID;
            long v15 = e12->__av;
            add_to_temp_map(_c3, st5.modify(z_follower_id,(v15 * -1L)));
          } while ((n10=n10->nxt));
        }
        { //slice
          const HASH_RES_t h11 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se25.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i11 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n11 = &(i11->buckets_[h11 % i11->size_]);
          COUNT_mRETWEET6_entry* e13;
          do if ((e13=n11->obj) && h11 == n11->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se25, *e13)) {
            long z_follower_id = e13->Z_FOLLOWER_ID;
            long v16 = e13->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c3, st6.modify(z_follower_id,v16)) : voidFunc());
          } while ((n11=n11->nxt));
        }
        { //temp foreach
          tuple2_L_L* e14 = _c3.head;
          while(e14){
            long z_follower_id = e14->_1;
            long v17 = e14->__av;
            COUNT.addOrDelOnZero(se23.modify(retweet_tweet_id,z_follower_id),v17);
            e14 = e14->nxt;
          }
        }
        { //slice
          const HASH_RES_t h12 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se27.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i12 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n12 = &(i12->buckets_[h12 % i12->size_]);
          COUNT_mRETWEET6_entry* e15;
          do if ((e15=n12->obj) && h12 == n12->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se27, *e15)) {
            long z_follower_id = e15->Z_FOLLOWER_ID;
            long v18 = e15->__av;
            COUNT.addOrDelOnZero(se23.modify(retweet_retweet_tweet_id,z_follower_id),(COUNT_mRETWEET5.getValueOrDefault(se26.modify(retweet_retweet_tweet_id)) * (v18 * -1L)));
          } while ((n12=n12->nxt));
        }
        _c4.clear();
        { //slice
          const HASH_RES_t h13 = COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se29.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1* i13 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mFOLLOWER1_mRETWEET2.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n13 = &(i13->buckets_[h13 % i13->size_]);
          COUNT_mFOLLOWER1_mRETWEET2_entry* e16;
          do if ((e16=n13->obj) && h13 == n13->hash && COUNT_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se29, *e16)) {
            long count_mfollowerfollower_user_id = e16->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v20 = e16->__av;
            add_to_temp_map(_c4, st7.modify(count_mfollowerfollower_user_id,(v20 * -1L)));
          } while ((n13=n13->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c4, st8.modify(retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e17 = _c4.head;
          while(e17){
            long count_mfollowerfollower_user_id = e17->_1;
            long v21 = e17->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se28.modify(retweet_tweet_id,count_mfollowerfollower_user_id),v21);
            e17 = e17->nxt;
          }
        }
        COUNT_mFOLLOWER1.addOrDelOnZero(se28.modify(retweet_retweet_tweet_id,retweet_user_id),(COUNT_mRETWEET5.getValueOrDefault(se30.modify(retweet_retweet_tweet_id)) * -1L));
        COUNT_mFOLLOWER1_mRETWEET2.addOrDelOnZero(se31.modify(retweet_user_id,retweet_retweet_tweet_id),-1L);
        { //slice
          const HASH_RES_t h14 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se33.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i14 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n14 = &(i14->buckets_[h14 % i14->size_]);
          COUNT_mRETWEET6_entry* e18;
          do if ((e18=n14->obj) && h14 == n14->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se33, *e18)) {
            long z_follower_id = e18->Z_FOLLOWER_ID;
            long v22 = e18->__av;
            COUNT_mRETWEET2.addOrDelOnZero(se32.modify(retweet_retweet_tweet_id,z_follower_id),(v22 * -1L));
          } while ((n14=n14->nxt));
        }
        COUNT_mRETWEET5.addOrDelOnZero(se34.modify(retweet_tweet_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_RETWEET")
      END_TRIGGER(ivc_stats,"delete_RETWEET")
    }
    void unwrap_delete_RETWEET(const event_args_t& ea) {
      on_delete_RETWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])), *(reinterpret_cast<long*>(ea[3])));
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
    COUNT_mRETWEET2_entry se3;
    COUNT_mFOLLOWER1_mRETWEET2_entry se4;
    COUNT_mRETWEET6_entry se5;
    COUNT_entry se6;
    COUNT_mFOLLOWER1_entry se7;
    COUNT_mRETWEET2_entry se8;
    COUNT_mFOLLOWER1_mRETWEET2_entry se9;
    COUNT_mRETWEET6_entry se10;
    COUNT_entry se11;
    tuple2_L_L st1;
    COUNT_mRETWEET2_entry se12;
    tuple2_L_L st2;
    COUNT_mRETWEET6_entry se13;
    COUNT_mRETWEET5_entry se14;
    COUNT_mRETWEET6_entry se15;
    COUNT_mFOLLOWER1_entry se16;
    tuple2_L_L st3;
    COUNT_mFOLLOWER1_mRETWEET2_entry se17;
    tuple2_L_L st4;
    COUNT_mRETWEET5_entry se18;
    COUNT_mFOLLOWER1_mRETWEET2_entry se19;
    COUNT_mRETWEET2_entry se20;
    COUNT_mRETWEET6_entry se21;
    COUNT_mRETWEET5_entry se22;
    COUNT_entry se23;
    tuple2_L_L st5;
    COUNT_mRETWEET2_entry se24;
    tuple2_L_L st6;
    COUNT_mRETWEET6_entry se25;
    COUNT_mRETWEET5_entry se26;
    COUNT_mRETWEET6_entry se27;
    COUNT_mFOLLOWER1_entry se28;
    tuple2_L_L st7;
    COUNT_mFOLLOWER1_mRETWEET2_entry se29;
    tuple2_L_L st8;
    COUNT_mRETWEET5_entry se30;
    COUNT_mFOLLOWER1_mRETWEET2_entry se31;
    COUNT_mRETWEET2_entry se32;
    COUNT_mRETWEET6_entry se33;
    COUNT_mRETWEET5_entry se34;
  
    /* Data structures used for storing materialized views */
    COUNT_mFOLLOWER1_map COUNT_mFOLLOWER1;
    COUNT_mFOLLOWER1_mRETWEET2_map COUNT_mFOLLOWER1_mRETWEET2;
    COUNT_mRETWEET2_map COUNT_mRETWEET2;
    COUNT_mRETWEET5_map COUNT_mRETWEET5;
    COUNT_mRETWEET6_map COUNT_mRETWEET6;
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
        
        pair<string,string> source2_adaptor_params[] = { make_pair("schema","long,long,string,long") };
        std::shared_ptr<csv_adaptor> source2_adaptor(new csv_adaptor(get_relation_id("RETWEET"),1,source2_adaptor_params));
        frame_descriptor source2_fd("\n");
        std::shared_ptr<dbt_file_source> source2_file(new dbt_file_source("examples/data/fastviewsmall/retweet.csv",source2_fd,source2_adaptor));
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
  class QJoinLengthRetweet : public Program
  {
    public:
      QJoinLengthRetweet(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

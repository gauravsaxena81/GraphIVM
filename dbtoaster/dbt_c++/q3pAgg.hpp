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
    long U_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_entry* nxt; __SQL_COUNT_AGGREGATE_1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_entry() : nxt(nullptr), prv(nullptr) { /*U_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_entry(const long c0, const long c1) { U_USER_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_entry(const __SQL_COUNT_AGGREGATE_1_entry& other) : U_USER_ID( other.U_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_entry& modify(const long c0) { U_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, U_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.U_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_entry& x, const __SQL_COUNT_AGGREGATE_1_entry& y) {
      return x.U_USER_ID == y.U_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_entry,long,__SQL_COUNT_AGGREGATE_1_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_entry,long,__SQL_COUNT_AGGREGATE_1_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry {
    long __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID; long U_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; U_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry(const long c0, const long c1, const long c2) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; U_USER_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& other) : __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID ), U_USER_ID( other.U_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& modify(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; U_USER_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& modify0(const long c0) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, U_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.U_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID && x.U_USER_ID == y.U_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mRETWEET1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey01_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mRETWEET1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_01;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry {
    long __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID; long __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID = 0L; __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry(const long c0, const long c1, const long c2) { __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID = c0; __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& other) : __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID ), __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& modify(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID = c0; __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& modify1(const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& modify0(const long c0) { __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID);
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID && x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey01_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn,false>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_01;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0;
  
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
  
  struct __SQL_COUNT_AGGREGATE_1_mTWEET2_entry {
    long __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mTWEET2_entry* nxt; __SQL_COUNT_AGGREGATE_1_mTWEET2_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mTWEET2_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mTWEET2_entry(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry(const __SQL_COUNT_AGGREGATE_1_mTWEET2_entry& other) : __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mTWEET2_entry& modify(const long c0) { __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mTWEET2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mTWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mTWEET2_entry& x, const __SQL_COUNT_AGGREGATE_1_mTWEET2_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mTWEET2_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mTWEET2_entry,long,__SQL_COUNT_AGGREGATE_1_mTWEET2_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_mTWEET2_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mTWEET2_entry,long,__SQL_COUNT_AGGREGATE_1_mTWEET2_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mTWEET2_map_0;
  
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
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mRETWEET1_map>( "__SQL_COUNT_AGGREGATE_1_mRETWEET1", __SQL_COUNT_AGGREGATE_1_mRETWEET1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map>( "__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1", __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mTWEET1_map>( "__SQL_COUNT_AGGREGATE_1_mTWEET1", __SQL_COUNT_AGGREGATE_1_mTWEET1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mTWEET2_map>( "__SQL_COUNT_AGGREGATE_1_mTWEET2", __SQL_COUNT_AGGREGATE_1_mTWEET2 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mUSERS1_map>( "__SQL_COUNT_AGGREGATE_1_mUSERS1", __SQL_COUNT_AGGREGATE_1_mUSERS1 );
  
      pb.add_relation("RETWEET");
      pb.add_relation("TWEET");
      pb.add_relation("USERS");
  
      
  
      pb.add_trigger("RETWEET", insert_tuple, std::bind(&data_t::unwrap_insert_RETWEET, this, std::placeholders::_1));
      pb.add_trigger("RETWEET", delete_tuple, std::bind(&data_t::unwrap_delete_RETWEET, this, std::placeholders::_1));
      pb.add_trigger("TWEET", insert_tuple, std::bind(&data_t::unwrap_insert_TWEET, this, std::placeholders::_1));
      pb.add_trigger("TWEET", delete_tuple, std::bind(&data_t::unwrap_delete_TWEET, this, std::placeholders::_1));
      pb.add_trigger("USERS", insert_tuple, std::bind(&data_t::unwrap_insert_USERS, this, std::placeholders::_1));
      pb.add_trigger("USERS", delete_tuple, std::bind(&data_t::unwrap_delete_USERS, this, std::placeholders::_1));
  
      #ifdef DBT_PROFILE
      exec_stats = pb.exec_stats;
      ivc_stats = pb.ivc_stats;
      exec_stats->register_probe(0, "insert_RETWEET_s0");
      exec_stats->register_probe(1, "insert_RETWEET_s1");
      exec_stats->register_probe(2, "insert_RETWEET_s2");
      exec_stats->register_probe(3, "delete_RETWEET_s0");
      exec_stats->register_probe(4, "delete_RETWEET_s1");
      exec_stats->register_probe(5, "delete_RETWEET_s2");
      exec_stats->register_probe(6, "insert_TWEET_s0");
      exec_stats->register_probe(7, "insert_TWEET_s1");
      exec_stats->register_probe(8, "insert_TWEET_s2");
      exec_stats->register_probe(9, "insert_TWEET_s3");
      exec_stats->register_probe(10, "delete_TWEET_s0");
      exec_stats->register_probe(11, "delete_TWEET_s1");
      exec_stats->register_probe(12, "delete_TWEET_s2");
      exec_stats->register_probe(13, "delete_TWEET_s3");
      exec_stats->register_probe(14, "insert_USERS_s0");
      exec_stats->register_probe(15, "insert_USERS_s1");
      exec_stats->register_probe(16, "insert_USERS_s2");
      exec_stats->register_probe(17, "delete_USERS_s0");
      exec_stats->register_probe(18, "delete_USERS_s1");
      exec_stats->register_probe(19, "delete_USERS_s2");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    int i = 0;
    /* Trigger functions for stream relations */
    void on_insert_RETWEET(const long retweet_user_id, const long retweet_tweet_id, const STRING_TYPE& retweet_tweet_date, const long retweet_retweet_tweet_id) {
      BEGIN_TRIGGER(exec_stats,"insert_RETWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_RETWEET")
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = __SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn::hash(se2.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0* i1 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn::equals(se2, *e1)) {
            long u_user_id = e1->U_USER_ID;
            long v1 = e1->__av;
		//api.useDiff(u_user_id, v1);
            __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se1.modify(u_user_id),v1);
          } while ((n1=n1->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mTWEET2.addOrDelOnZero(se3.modify(retweet_retweet_tweet_id),1L);
        { //slice
          const HASH_RES_t h2 = __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se5.modify1(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1* i2 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1*>(__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se5, *e2)) {
            long __sql_count_aggregate_1_musersusers_user_id = e2->__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID;
            long v2 = e2->__av;
            __SQL_COUNT_AGGREGATE_1_mUSERS1.addOrDelOnZero(se4.modify(__sql_count_aggregate_1_musersusers_user_id),v2);
          } while ((n2=n2->nxt));
        }
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
        { //slice
          const HASH_RES_t h3 = __SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn::hash(se7.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0* i3 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET1_mapkey0_idxfn::equals(se7, *e3)) {
            long u_user_id = e3->U_USER_ID;
            long v3 = e3->__av;
            __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se6.modify(u_user_id),(v3 * -1L));
          } while ((n3=n3->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mTWEET2.addOrDelOnZero(se8.modify(retweet_retweet_tweet_id),-1L);
        { //slice
          const HASH_RES_t h4 = __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se10.modify1(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1* i4 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1*>(__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_1::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se10, *e4)) {
            long __sql_count_aggregate_1_musersusers_user_id = e4->__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERSUSERS_USER_ID;
            long v4 = e4->__av;
            __SQL_COUNT_AGGREGATE_1_mUSERS1.addOrDelOnZero(se9.modify(__sql_count_aggregate_1_musersusers_user_id),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
      }
      END_TRIGGER(exec_stats,"delete_RETWEET")
      END_TRIGGER(ivc_stats,"delete_RETWEET")
    }
    void unwrap_delete_RETWEET(const event_args_t& ea) {
      on_delete_RETWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])), *(reinterpret_cast<long*>(ea[3])));
    }
    
    void on_insert_TWEET(const long tweet_user_id, const long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"insert_TWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_TWEET")
      {  //++tN;
	//api.useDiff(tweet_user_id, (__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se12.modify(tweet_user_id)) * __SQL_COUNT_AGGREGATE_1_mTWEET2.getValueOrDefault(se13.modify(tweet_tweet_id))));
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se11.modify(tweet_user_id),(__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se12.modify(tweet_user_id)) * __SQL_COUNT_AGGREGATE_1_mTWEET2.getValueOrDefault(se13.modify(tweet_tweet_id))));
        __SQL_COUNT_AGGREGATE_1_mRETWEET1.addOrDelOnZero(se14.modify(tweet_tweet_id,tweet_user_id),__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se15.modify(tweet_user_id)));
        __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1.addOrDelOnZero(se16.modify(tweet_user_id,tweet_tweet_id),1L);
        __SQL_COUNT_AGGREGATE_1_mUSERS1.addOrDelOnZero(se17.modify(tweet_user_id),__SQL_COUNT_AGGREGATE_1_mTWEET2.getValueOrDefault(se18.modify(tweet_tweet_id)));
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
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se19.modify(tweet_user_id),(__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se20.modify(tweet_user_id)) * (__SQL_COUNT_AGGREGATE_1_mTWEET2.getValueOrDefault(se21.modify(tweet_tweet_id)) * -1L)));
        __SQL_COUNT_AGGREGATE_1_mRETWEET1.addOrDelOnZero(se22.modify(tweet_tweet_id,tweet_user_id),(__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se23.modify(tweet_user_id)) * -1L));
        __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1.addOrDelOnZero(se24.modify(tweet_user_id,tweet_tweet_id),-1L);
        __SQL_COUNT_AGGREGATE_1_mUSERS1.addOrDelOnZero(se25.modify(tweet_user_id),(__SQL_COUNT_AGGREGATE_1_mTWEET2.getValueOrDefault(se26.modify(tweet_tweet_id)) * -1L));
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
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se27.modify(users_user_id),__SQL_COUNT_AGGREGATE_1_mUSERS1.getValueOrDefault(se28.modify(users_user_id)));
        { //slice
          const HASH_RES_t h5 = __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn::hash(se30.modify0(users_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0* i5 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn::equals(se30, *e5)) {
            long __sql_count_aggregate_1_mretweetretweet_retweet_tweet_id = e5->__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v5 = e5->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET1.addOrDelOnZero(se29.modify(__sql_count_aggregate_1_mretweetretweet_retweet_tweet_id,users_user_id),v5);
          } while ((n5=n5->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se31.modify(users_user_id),1L);
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
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se32.modify(users_user_id),(__SQL_COUNT_AGGREGATE_1_mUSERS1.getValueOrDefault(se33.modify(users_user_id)) * -1L));
        { //slice
          const HASH_RES_t h6 = __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn::hash(se35.modify0(users_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0* i6 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_mapkey0_idxfn::equals(se35, *e6)) {
            long __sql_count_aggregate_1_mretweetretweet_retweet_tweet_id = e6->__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v6 = e6->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET1.addOrDelOnZero(se34.modify(__sql_count_aggregate_1_mretweetretweet_retweet_tweet_id,users_user_id),(v6 * -1L));
          } while ((n6=n6->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se36.modify(users_user_id),-1L);
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
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry se2;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry se3;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se4;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry se5;
    __SQL_COUNT_AGGREGATE_1_entry se6;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry se7;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry se8;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se9;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry se10;
    __SQL_COUNT_AGGREGATE_1_entry se11;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se12;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry se13;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry se14;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se15;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry se16;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se17;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry se18;
    __SQL_COUNT_AGGREGATE_1_entry se19;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se20;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry se21;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry se22;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se23;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry se24;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se25;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_entry se26;
    __SQL_COUNT_AGGREGATE_1_entry se27;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se28;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry se29;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry se30;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se31;
    __SQL_COUNT_AGGREGATE_1_entry se32;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_entry se33;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_entry se34;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_entry se35;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se36;
  
    /* Data structures used for storing materialized views */
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_map __SQL_COUNT_AGGREGATE_1_mRETWEET1;
    __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1_map __SQL_COUNT_AGGREGATE_1_mRETWEET1_mUSERS1;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_map __SQL_COUNT_AGGREGATE_1_mTWEET1;
    __SQL_COUNT_AGGREGATE_1_mTWEET2_map __SQL_COUNT_AGGREGATE_1_mTWEET2;
    __SQL_COUNT_AGGREGATE_1_mUSERS1_map __SQL_COUNT_AGGREGATE_1_mUSERS1;
    
    
  
  };
  
  /* Type definition providing a way to execute the sql program */
  class Program : public ProgramBase
  {
    public:
      Program(int argc = 0, char* argv[] = 0) : ProgramBase(argc,argv) {
        data.register_data(*this);
        
        /* Specifying data sources */
        
        
        pair<string,string> source1_adaptor_params[] = { make_pair("schema","long,long,string,long") };
        std::shared_ptr<csv_adaptor> source1_adaptor(new csv_adaptor(get_relation_id("RETWEET"),1,source1_adaptor_params));
        frame_descriptor source1_fd("\n");
        std::shared_ptr<dbt_file_source> source1_file(new dbt_file_source("examples/data/fastview/retweet.csv",source1_fd,source1_adaptor));
        add_source(source1_file);
        
        pair<string,string> source2_adaptor_params[] = { make_pair("schema","long,long,string") };
        std::shared_ptr<csv_adaptor> source2_adaptor(new csv_adaptor(get_relation_id("TWEET"),1,source2_adaptor_params));
        frame_descriptor source2_fd("\n");
        std::shared_ptr<dbt_file_source> source2_file(new dbt_file_source("examples/data/fastview/tweet.csv",source2_fd,source2_adaptor));
        add_source(source2_file);
        
        pair<string,string> source3_adaptor_params[] = { make_pair("schema","long") };
        std::shared_ptr<csv_adaptor> source3_adaptor(new csv_adaptor(get_relation_id("USERS"),1,source3_adaptor_params));
        frame_descriptor source3_fd("\n");
        std::shared_ptr<dbt_file_source> source3_file(new dbt_file_source("examples/data/fastview/users.csv",source3_fd,source3_adaptor));
        add_source(source3_file);
  
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
  class Q3Agg : public Program
  {
    public:
      Q3Agg(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

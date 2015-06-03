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
    long T_USER_ID; long A_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_entry* nxt; __SQL_COUNT_AGGREGATE_1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_entry() : nxt(nullptr), prv(nullptr) { /*T_USER_ID = 0L; A_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_entry(const long c0, const long c1, const long c2) { T_USER_ID = c0; A_TWEET_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_entry(const __SQL_COUNT_AGGREGATE_1_entry& other) : T_USER_ID( other.T_USER_ID ), A_TWEET_ID( other.A_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_entry& modify(const long c0, const long c1) { T_USER_ID = c0; A_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.A_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_entry& x, const __SQL_COUNT_AGGREGATE_1_entry& y) {
      return x.T_USER_ID == y.T_USER_ID && x.A_TWEET_ID == y.A_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_entry,long,__SQL_COUNT_AGGREGATE_1_mapkey01_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_entry,long,__SQL_COUNT_AGGREGATE_1_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_map_01;
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry {
    long A_TWEET_ID; long __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID; long T_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = 0L; T_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry(const long c0, const long c1, const long c2, const long c3) { A_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c1; T_USER_ID = c2; __av = c3; }
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID ), T_USER_ID( other.T_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& modify(const long c0, const long c1, const long c2) { A_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c1; T_USER_ID = c2;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& modify1(const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.T_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID && x.T_USER_ID == y.T_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey012_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey012_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_012;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1;
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry {
    long __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID; long __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* nxt; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = 0L; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry(const long c0, const long c1, const long c2) { __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& other) : __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID ), __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& modify(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& modify0(const long c0) { __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& modify1(const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID && x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey01_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn,false>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_01;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1;
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry {
    long __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID; long __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = 0L; __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry(const long c0, const long c1, const long c2) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& other) : __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID ), __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& modify(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& modify1(const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& modify0(const long c0) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID && x.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey01_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn,false>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_01;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry {
    long __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID; long __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* nxt; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry(const long c0, const long c1, const long c2) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& other) : __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID ), __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& modify(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& modify0(const long c0) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& modify1(const long c1) { __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID && x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& x, const __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey01_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn,false>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_01;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1;
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry {
    long __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID; long T_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* nxt; __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; T_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry(const long c0, const long c1, const long c2) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; T_USER_ID = c1; __av = c2; }
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry(const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& other) : __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID ), T_USER_ID( other.T_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& modify(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; T_USER_ID = c1;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& modify0(const long c0) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.T_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID && x.T_USER_ID == y.T_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mRETWEET2_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET2_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey01_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET2_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mRETWEET2_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET2_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey01_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_01;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET2_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry {
    long __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry* nxt; __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry(const __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry& other) : __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry& modify(const long c0) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET3_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mRETWEET3_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET3_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_mRETWEET3_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET3_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET3_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET3_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry {
    long A_TWEET_ID; long __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID; long T_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry* nxt; __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; T_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry(const long c0, const long c1, const long c2, const long c3) { A_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; T_USER_ID = c2; __av = c3; }
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry(const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID ), T_USER_ID( other.T_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& modify(const long c0, const long c1, const long c2) { A_TWEET_ID = c0; __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; T_USER_ID = c2;  return *this; }
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& modify1(const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.T_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID && x.T_USER_ID == y.T_USER_ID;
    }
  };
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mRETWEET5_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey012_idxfn,true>,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn,false>
  > __SQL_COUNT_AGGREGATE_1_mRETWEET5_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey012_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_012;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET5_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn,false> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1;
  
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry {
    long __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry* nxt; __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry(const __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry& other) : __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID( other.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry& modify(const long c0) { __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mRETWEET6_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry& x, const __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID == y.__SQL_COUNT_AGGREGATE_1_mRETWEETRETWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mRETWEET6_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET6_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET6_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_mRETWEET6_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mRETWEET6_entry,long,__SQL_COUNT_AGGREGATE_1_mRETWEET6_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET6_map_0;
  
  struct __SQL_COUNT_AGGREGATE_1_mTWEET1_entry {
    long __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID; long __av;  __SQL_COUNT_AGGREGATE_1_mTWEET1_entry* nxt; __SQL_COUNT_AGGREGATE_1_mTWEET1_entry* prv;
    explicit __SQL_COUNT_AGGREGATE_1_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit __SQL_COUNT_AGGREGATE_1_mTWEET1_entry(const long c0, const long c1) { __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID = c0; __av = c1; }
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry(const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& other) : __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID( other.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& modify(const long c0) { __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct __SQL_COUNT_AGGREGATE_1_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& x, const __SQL_COUNT_AGGREGATE_1_mTWEET1_entry& y) {
      return x.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID == y.__SQL_COUNT_AGGREGATE_1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<__SQL_COUNT_AGGREGATE_1_mTWEET1_entry,long,
    HashIndex<__SQL_COUNT_AGGREGATE_1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mTWEET1_mapkey0_idxfn,true>
  > __SQL_COUNT_AGGREGATE_1_mTWEET1_map;
  typedef HashIndex<__SQL_COUNT_AGGREGATE_1_mTWEET1_entry,long,__SQL_COUNT_AGGREGATE_1_mTWEET1_mapkey0_idxfn,true> HashIndex___SQL_COUNT_AGGREGATE_1_mTWEET1_map_0;
  
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
    data_t(): tlq_t(), _c1(16U), _c4(16U), _c3(16U), _c2(16U) {
      
    }
  
    #ifdef DBT_PROFILE
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> exec_stats;
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> ivc_stats;
    #endif
  
    /* Registering relations and trigger functions */
    void register_data(ProgramBase& pb) {
  
      pb.add_map<__SQL_COUNT_AGGREGATE_1_map>( "__SQL_COUNT_AGGREGATE_1", __SQL_COUNT_AGGREGATE_1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map>( "__SQL_COUNT_AGGREGATE_1_mFOLLOWER1", __SQL_COUNT_AGGREGATE_1_mFOLLOWER1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map>( "__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3", __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map>( "__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1", __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map>( "__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5", __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mRETWEET2_map>( "__SQL_COUNT_AGGREGATE_1_mRETWEET2", __SQL_COUNT_AGGREGATE_1_mRETWEET2 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mRETWEET3_map>( "__SQL_COUNT_AGGREGATE_1_mRETWEET3", __SQL_COUNT_AGGREGATE_1_mRETWEET3 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mRETWEET5_map>( "__SQL_COUNT_AGGREGATE_1_mRETWEET5", __SQL_COUNT_AGGREGATE_1_mRETWEET5 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mRETWEET6_map>( "__SQL_COUNT_AGGREGATE_1_mRETWEET6", __SQL_COUNT_AGGREGATE_1_mRETWEET6 );
      pb.add_map<__SQL_COUNT_AGGREGATE_1_mTWEET1_map>( "__SQL_COUNT_AGGREGATE_1_mTWEET1", __SQL_COUNT_AGGREGATE_1_mTWEET1 );
  
      pb.add_relation("FOLLOWER");
      pb.add_relation("RETWEET");
      pb.add_relation("TWEET");
  
      
  
      pb.add_trigger("FOLLOWER", insert_tuple, std::bind(&data_t::unwrap_insert_FOLLOWER, this, std::placeholders::_1));
      pb.add_trigger("FOLLOWER", delete_tuple, std::bind(&data_t::unwrap_delete_FOLLOWER, this, std::placeholders::_1));
      pb.add_trigger("RETWEET", insert_tuple, std::bind(&data_t::unwrap_insert_RETWEET, this, std::placeholders::_1));
      pb.add_trigger("RETWEET", delete_tuple, std::bind(&data_t::unwrap_delete_RETWEET, this, std::placeholders::_1));
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
      exec_stats->register_probe(8, "insert_RETWEET_s0");
      exec_stats->register_probe(9, "insert_RETWEET_s1");
      exec_stats->register_probe(10, "insert_RETWEET_s2");
      exec_stats->register_probe(11, "insert_RETWEET_s3");
      exec_stats->register_probe(12, "insert_RETWEET_s4");
      exec_stats->register_probe(13, "insert_RETWEET_s5");
      exec_stats->register_probe(14, "insert_RETWEET_s6");
      exec_stats->register_probe(15, "insert_RETWEET_s7");
      exec_stats->register_probe(16, "delete_RETWEET_s0");
      exec_stats->register_probe(17, "delete_RETWEET_s1");
      exec_stats->register_probe(18, "delete_RETWEET_s2");
      exec_stats->register_probe(19, "delete_RETWEET_s3");
      exec_stats->register_probe(20, "delete_RETWEET_s4");
      exec_stats->register_probe(21, "delete_RETWEET_s5");
      exec_stats->register_probe(22, "delete_RETWEET_s6");
      exec_stats->register_probe(23, "delete_RETWEET_s7");
      exec_stats->register_probe(24, "insert_TWEET_s0");
      exec_stats->register_probe(25, "insert_TWEET_s1");
      exec_stats->register_probe(26, "insert_TWEET_s2");
      exec_stats->register_probe(27, "insert_TWEET_s3");
      exec_stats->register_probe(28, "delete_TWEET_s0");
      exec_stats->register_probe(29, "delete_TWEET_s1");
      exec_stats->register_probe(30, "delete_TWEET_s2");
      exec_stats->register_probe(31, "delete_TWEET_s3");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_FOLLOWER(const long follower_user_id, const long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"insert_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"insert_FOLLOWER")
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn::hash(se2.modify1(follower_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1* i1 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn::equals(se2, *e1)) {
            long a_tweet_id = e1->A_TWEET_ID;
            long t_user_id = e1->T_USER_ID;
            long v1 = e1->__av;
		api.useDiff(t_user_id,a_tweet_id,v1);
            //__SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se1.modify(t_user_id,a_tweet_id),v1);
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn::hash(se4.modify0(follower_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0* i2 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn::equals(se4, *e2)) {
            long __sql_count_aggregate_1_mretweetretweet_tweet_id = e2->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long v2 = e2->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET3.addOrDelOnZero(se3.modify(__sql_count_aggregate_1_mretweetretweet_tweet_id),v2);
          } while ((n2=n2->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mRETWEET6.addOrDelOnZero(se5.modify(follower_user_id),1L);
        { //slice
          const HASH_RES_t h3 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn::hash(se7.modify1(follower_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1* i3 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn::equals(se7, *e3)) {
            long __sql_count_aggregate_1_mtweettweet_tweet_id = e3->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long v3 = e3->__av;
            __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se6.modify(__sql_count_aggregate_1_mtweettweet_tweet_id),v3);
          } while ((n3=n3->nxt));
        }
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
          const HASH_RES_t h4 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn::hash(se9.modify1(follower_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1* i4 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map_1::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mapkey1_idxfn::equals(se9, *e4)) {
            long a_tweet_id = e4->A_TWEET_ID;
            long t_user_id = e4->T_USER_ID;
            long v4 = e4->__av;
		api.useDiff(t_user_id,a_tweet_id, v4);
            //__SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se8.modify(t_user_id,a_tweet_id),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
        { //slice
          const HASH_RES_t h5 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn::hash(se11.modify0(follower_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0* i5 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey0_idxfn::equals(se11, *e5)) {
            long __sql_count_aggregate_1_mretweetretweet_tweet_id = e5->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long v5 = e5->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET3.addOrDelOnZero(se10.modify(__sql_count_aggregate_1_mretweetretweet_tweet_id),(v5 * -1L));
          } while ((n5=n5->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mRETWEET6.addOrDelOnZero(se12.modify(follower_user_id),-1L);
        { //slice
          const HASH_RES_t h6 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn::hash(se14.modify1(follower_user_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1* i6 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_1::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey1_idxfn::equals(se14, *e6)) {
            long __sql_count_aggregate_1_mtweettweet_tweet_id = e6->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long v6 = e6->__av;
            __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se13.modify(__sql_count_aggregate_1_mtweettweet_tweet_id),(v6 * -1L));
          } while ((n6=n6->nxt));
        }
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
        { //slice
          const HASH_RES_t h7 = __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::hash(se18.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0* i7 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET2.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* e7;
          do if ((e7=n7->obj) && h7 == n7->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::equals(se18, *e7)) {
            long t_user_id = e7->T_USER_ID;
            long v7 = e7->__av;
		api.useDiff(t_user_id,retweet_retweet_tweet_id,v7);
            //__SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se15.modify(t_user_id,retweet_retweet_tweet_id),(v7 * (__SQL_COUNT_AGGREGATE_1_mRETWEET3.getValueOrDefault(se16.modify(retweet_tweet_id)) + (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? __SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se17.modify(retweet_user_id)) : 0L))));
          } while ((n7=n7->nxt));
        }
        { //slice
          const HASH_RES_t h8 = __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::hash(se20.modify1(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1* i8 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1*>(__SQL_COUNT_AGGREGATE_1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1::IdxNode* n8 = &(i8->buckets_[h8 % i8->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry* e8;
          do if ((e8=n8->obj) && h8 == n8->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::equals(se20, *e8)) {
            long a_tweet_id = e8->A_TWEET_ID;
            long t_user_id = e8->T_USER_ID;
            long v8 = e8->__av;
            __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se15.modify(t_user_id,a_tweet_id),(v8 * __SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se19.modify(retweet_user_id))));
          } while ((n8=n8->nxt));
        }
        { //slice
          const HASH_RES_t h10 = __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::hash(se23.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0* i9 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET2.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0::IdxNode* n9 = &(i9->buckets_[h10 % i9->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* e9;
          do if ((e9=n9->obj) && h10 == n9->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::equals(se23, *e9)) {
            long t_user_id = e9->T_USER_ID;
            long v9 = e9->__av;
            _c1.clear();
            { //slice
              const HASH_RES_t h9 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::hash(se22.modify1(retweet_tweet_id));
              const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1* i10 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.index[2]);
              HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1::IdxNode* n10 = &(i10->buckets_[h9 % i10->size_]);
              __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* e10;
              do if ((e10=n10->obj) && h9 == n10->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::equals(se22, *e10)) {
                long __sql_count_aggregate_1_mfollowerfollower_user_id = e10->__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
                long v11 = e10->__av;
                add_to_temp_map(_c1, st1.modify(__sql_count_aggregate_1_mfollowerfollower_user_id,v11));
              } while ((n10=n10->nxt));
            }
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c1, st2.modify(retweet_user_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e11 = _c1.head;
              while(e11){
                long __sql_count_aggregate_1_mfollowerfollower_user_id = e11->_1;
                long v12 = e11->__av;
                __SQL_COUNT_AGGREGATE_1_mFOLLOWER1.addOrDelOnZero(se21.modify(retweet_retweet_tweet_id,__sql_count_aggregate_1_mfollowerfollower_user_id,t_user_id),(v9 * v12));
                e11 = e11->nxt;
              }
            }
          } while ((n9=n9->nxt));
        }
        { //slice
          const HASH_RES_t h11 = __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::hash(se24.modify1(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1* i11 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1*>(__SQL_COUNT_AGGREGATE_1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1::IdxNode* n11 = &(i11->buckets_[h11 % i11->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry* e12;
          do if ((e12=n11->obj) && h11 == n11->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::equals(se24, *e12)) {
            long a_tweet_id = e12->A_TWEET_ID;
            long t_user_id = e12->T_USER_ID;
            long v13 = e12->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1.addOrDelOnZero(se21.modify(a_tweet_id,retweet_user_id,t_user_id),v13);
          } while ((n11=n11->nxt));
        }
        _c2.clear();
        { //slice
          const HASH_RES_t h12 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::hash(se26.modify1(retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1* i12 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1::IdxNode* n12 = &(i12->buckets_[h12 % i12->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* e13;
          do if ((e13=n12->obj) && h12 == n12->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::equals(se26, *e13)) {
            long __sql_count_aggregate_1_mfollowerfollower_user_id = e13->__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
            long v15 = e13->__av;
            add_to_temp_map(_c2, st3.modify(__sql_count_aggregate_1_mfollowerfollower_user_id,v15));
          } while ((n12=n12->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c2, st4.modify(retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e14 = _c2.head;
          while(e14){
            long __sql_count_aggregate_1_mfollowerfollower_user_id = e14->_1;
            long v16 = e14->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.addOrDelOnZero(se25.modify(retweet_retweet_tweet_id,__sql_count_aggregate_1_mfollowerfollower_user_id),v16);
            e14 = e14->nxt;
          }
        }
        { //slice
          const HASH_RES_t h13 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::hash(se27.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0* i13 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0::IdxNode* n13 = &(i13->buckets_[h13 % i13->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* e15;
          do if ((e15=n13->obj) && h13 == n13->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::equals(se27, *e15)) {
            long __sql_count_aggregate_1_mfollower1_mtweettweet_tweet_id = e15->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long v17 = e15->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.addOrDelOnZero(se25.modify(__sql_count_aggregate_1_mfollower1_mtweettweet_tweet_id,retweet_user_id),v17);
          } while ((n13=n13->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.addOrDelOnZero(se28.modify(retweet_user_id,retweet_retweet_tweet_id),1L);
        { //slice
          const HASH_RES_t h14 = __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::hash(se30.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0* i14 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET2.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0::IdxNode* n14 = &(i14->buckets_[h14 % i14->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* e16;
          do if ((e16=n14->obj) && h14 == n14->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::equals(se30, *e16)) {
            long t_user_id = e16->T_USER_ID;
            long v18 = e16->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET5.addOrDelOnZero(se29.modify(retweet_retweet_tweet_id,retweet_tweet_id,t_user_id),v18);
          } while ((n14=n14->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se31.modify(retweet_retweet_tweet_id),(__SQL_COUNT_AGGREGATE_1_mRETWEET3.getValueOrDefault(se32.modify(retweet_tweet_id)) + (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? __SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se33.modify(retweet_user_id)) : 0L)));
        { //slice
          const HASH_RES_t h15 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::hash(se35.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0* i15 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0::IdxNode* n15 = &(i15->buckets_[h15 % i15->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* e17;
          do if ((e17=n15->obj) && h15 == n15->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::equals(se35, *e17)) {
            long __sql_count_aggregate_1_mtweettweet_tweet_id = e17->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long v19 = e17->__av;
            __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se31.modify(__sql_count_aggregate_1_mtweettweet_tweet_id),(v19 * __SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se34.modify(retweet_user_id))));
          } while ((n15=n15->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mRETWEET3.addOrDelOnZero(se36.modify(retweet_retweet_tweet_id),__SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se37.modify(retweet_user_id)));
        __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.addOrDelOnZero(se38.modify(retweet_tweet_id,retweet_retweet_tweet_id),1L);
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
          const HASH_RES_t h16 = __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::hash(se42.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0* i16 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET2.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* e18;
          do if ((e18=n16->obj) && h16 == n16->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::equals(se42, *e18)) {
            long t_user_id = e18->T_USER_ID;
            long v20 = e18->__av;
		api.useDiff(t_user_id, retweet_retweet_tweet_id, v20);
            //__SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se39.modify(t_user_id,retweet_retweet_tweet_id),(v20 * ((__SQL_COUNT_AGGREGATE_1_mRETWEET3.getValueOrDefault(se40.modify(retweet_tweet_id)) * -1L) + (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? __SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se41.modify(retweet_user_id)) : 0L))));
          } while ((n16=n16->nxt));
        }
        { //slice
          const HASH_RES_t h17 = __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::hash(se44.modify1(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1* i17 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1*>(__SQL_COUNT_AGGREGATE_1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1::IdxNode* n17 = &(i17->buckets_[h17 % i17->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry* e19;
          do if ((e19=n17->obj) && h17 == n17->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::equals(se44, *e19)) {
            long a_tweet_id = e19->A_TWEET_ID;
            long t_user_id = e19->T_USER_ID;
            long v21 = e19->__av;
            __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se39.modify(t_user_id,a_tweet_id),(v21 * (__SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se43.modify(retweet_user_id)) * -1L)));
          } while ((n17=n17->nxt));
        }
        { //slice
          const HASH_RES_t h19 = __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::hash(se47.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0* i18 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET2.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0::IdxNode* n18 = &(i18->buckets_[h19 % i18->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* e20;
          do if ((e20=n18->obj) && h19 == n18->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::equals(se47, *e20)) {
            long t_user_id = e20->T_USER_ID;
            long v22 = e20->__av;
            _c3.clear();
            { //slice
              const HASH_RES_t h18 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::hash(se46.modify1(retweet_tweet_id));
              const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1* i19 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.index[2]);
              HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1::IdxNode* n19 = &(i19->buckets_[h18 % i19->size_]);
              __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* e21;
              do if ((e21=n19->obj) && h18 == n19->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::equals(se46, *e21)) {
                long __sql_count_aggregate_1_mfollowerfollower_user_id = e21->__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
                long v24 = e21->__av;
                add_to_temp_map(_c3, st5.modify(__sql_count_aggregate_1_mfollowerfollower_user_id,(v24 * -1L)));
              } while ((n19=n19->nxt));
            }
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c3, st6.modify(retweet_user_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e22 = _c3.head;
              while(e22){
                long __sql_count_aggregate_1_mfollowerfollower_user_id = e22->_1;
                long v25 = e22->__av;
                __SQL_COUNT_AGGREGATE_1_mFOLLOWER1.addOrDelOnZero(se45.modify(retweet_retweet_tweet_id,__sql_count_aggregate_1_mfollowerfollower_user_id,t_user_id),(v22 * v25));
                e22 = e22->nxt;
              }
            }
          } while ((n18=n18->nxt));
        }
        { //slice
          const HASH_RES_t h20 = __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::hash(se48.modify1(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1* i20 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1*>(__SQL_COUNT_AGGREGATE_1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET5_map_1::IdxNode* n20 = &(i20->buckets_[h20 % i20->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry* e23;
          do if ((e23=n20->obj) && h20 == n20->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET5_mapkey1_idxfn::equals(se48, *e23)) {
            long a_tweet_id = e23->A_TWEET_ID;
            long t_user_id = e23->T_USER_ID;
            long v26 = e23->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1.addOrDelOnZero(se45.modify(a_tweet_id,retweet_user_id,t_user_id),(v26 * -1L));
          } while ((n20=n20->nxt));
        }
        _c4.clear();
        { //slice
          const HASH_RES_t h21 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::hash(se50.modify1(retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1* i21 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map_1::IdxNode* n21 = &(i21->buckets_[h21 % i21->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry* e24;
          do if ((e24=n21->obj) && h21 == n21->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_mapkey1_idxfn::equals(se50, *e24)) {
            long __sql_count_aggregate_1_mfollowerfollower_user_id = e24->__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
            long v28 = e24->__av;
            add_to_temp_map(_c4, st7.modify(__sql_count_aggregate_1_mfollowerfollower_user_id,(v28 * -1L)));
          } while ((n21=n21->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c4, st8.modify(retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e25 = _c4.head;
          while(e25){
            long __sql_count_aggregate_1_mfollowerfollower_user_id = e25->_1;
            long v29 = e25->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.addOrDelOnZero(se49.modify(retweet_retweet_tweet_id,__sql_count_aggregate_1_mfollowerfollower_user_id),v29);
            e25 = e25->nxt;
          }
        }
        { //slice
          const HASH_RES_t h22 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::hash(se51.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0* i22 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0::IdxNode* n22 = &(i22->buckets_[h22 % i22->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* e26;
          do if ((e26=n22->obj) && h22 == n22->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::equals(se51, *e26)) {
            long __sql_count_aggregate_1_mfollower1_mtweettweet_tweet_id = e26->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long v30 = e26->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.addOrDelOnZero(se49.modify(__sql_count_aggregate_1_mfollower1_mtweettweet_tweet_id,retweet_user_id),(v30 * -1L));
          } while ((n22=n22->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3.addOrDelOnZero(se52.modify(retweet_user_id,retweet_retweet_tweet_id),-1L);
        { //slice
          const HASH_RES_t h23 = __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::hash(se54.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0* i23 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0*>(__SQL_COUNT_AGGREGATE_1_mRETWEET2.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mRETWEET2_map_0::IdxNode* n23 = &(i23->buckets_[h23 % i23->size_]);
          __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry* e27;
          do if ((e27=n23->obj) && h23 == n23->hash && __SQL_COUNT_AGGREGATE_1_mRETWEET2_mapkey0_idxfn::equals(se54, *e27)) {
            long t_user_id = e27->T_USER_ID;
            long v31 = e27->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET5.addOrDelOnZero(se53.modify(retweet_retweet_tweet_id,retweet_tweet_id,t_user_id),(v31 * -1L));
          } while ((n23=n23->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se55.modify(retweet_retweet_tweet_id),((__SQL_COUNT_AGGREGATE_1_mRETWEET3.getValueOrDefault(se56.modify(retweet_tweet_id)) * -1L) + (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? __SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se57.modify(retweet_user_id)) : 0L)));
        { //slice
          const HASH_RES_t h24 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::hash(se59.modify0(retweet_retweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0* i24 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.index[1]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_0::IdxNode* n24 = &(i24->buckets_[h24 % i24->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* e28;
          do if ((e28=n24->obj) && h24 == n24->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey0_idxfn::equals(se59, *e28)) {
            long __sql_count_aggregate_1_mtweettweet_tweet_id = e28->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long v32 = e28->__av;
            __SQL_COUNT_AGGREGATE_1_mTWEET1.addOrDelOnZero(se55.modify(__sql_count_aggregate_1_mtweettweet_tweet_id),(v32 * (__SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se58.modify(retweet_user_id)) * -1L)));
          } while ((n24=n24->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mRETWEET3.addOrDelOnZero(se60.modify(retweet_retweet_tweet_id),(__SQL_COUNT_AGGREGATE_1_mRETWEET6.getValueOrDefault(se61.modify(retweet_user_id)) * -1L));
        __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.addOrDelOnZero(se62.modify(retweet_tweet_id,retweet_retweet_tweet_id),-1L);
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
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se63.modify(tweet_user_id,tweet_tweet_id),__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se64.modify(tweet_tweet_id)));
        { //slice
          const HASH_RES_t h25 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn::hash(se66.modify0(tweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0* i25 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0::IdxNode* n25 = &(i25->buckets_[h25 % i25->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry* e29;
          do if ((e29=n25->obj) && h25 == n25->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn::equals(se66, *e29)) {
            long __sql_count_aggregate_1_mfollowerfollower_user_id = e29->__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
            long v33 = e29->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1.addOrDelOnZero(se65.modify(tweet_tweet_id,__sql_count_aggregate_1_mfollowerfollower_user_id,tweet_user_id),v33);
          } while ((n25=n25->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mRETWEET2.addOrDelOnZero(se67.modify(tweet_tweet_id,tweet_user_id),1L);
        { //slice
          const HASH_RES_t h26 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn::hash(se69.modify1(tweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1* i26 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1::IdxNode* n26 = &(i26->buckets_[h26 % i26->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* e30;
          do if ((e30=n26->obj) && h26 == n26->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn::equals(se69, *e30)) {
            long __sql_count_aggregate_1_mretweetretweet_retweet_tweet_id = e30->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v34 = e30->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET5.addOrDelOnZero(se68.modify(tweet_tweet_id,__sql_count_aggregate_1_mretweetretweet_retweet_tweet_id,tweet_user_id),v34);
          } while ((n26=n26->nxt));
        }
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
        __SQL_COUNT_AGGREGATE_1.addOrDelOnZero(se70.modify(tweet_user_id,tweet_tweet_id),(__SQL_COUNT_AGGREGATE_1_mTWEET1.getValueOrDefault(se71.modify(tweet_tweet_id)) * -1L));
        { //slice
          const HASH_RES_t h27 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn::hash(se73.modify0(tweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0* i27 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map_0::IdxNode* n27 = &(i27->buckets_[h27 % i27->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry* e31;
          do if ((e31=n27->obj) && h27 == n27->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mapkey0_idxfn::equals(se73, *e31)) {
            long __sql_count_aggregate_1_mfollowerfollower_user_id = e31->__SQL_COUNT_AGGREGATE_1_mFOLLOWERFOLLOWER_USER_ID;
            long v35 = e31->__av;
            __SQL_COUNT_AGGREGATE_1_mFOLLOWER1.addOrDelOnZero(se72.modify(tweet_tweet_id,__sql_count_aggregate_1_mfollowerfollower_user_id,tweet_user_id),(v35 * -1L));
          } while ((n27=n27->nxt));
        }
        __SQL_COUNT_AGGREGATE_1_mRETWEET2.addOrDelOnZero(se74.modify(tweet_tweet_id,tweet_user_id),-1L);
        { //slice
          const HASH_RES_t h28 = __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn::hash(se76.modify1(tweet_tweet_id));
          const HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1* i28 = static_cast<HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1*>(__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5.index[2]);
          HashIndex___SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map_1::IdxNode* n28 = &(i28->buckets_[h28 % i28->size_]);
          __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry* e32;
          do if ((e32=n28->obj) && h28 == n28->hash && __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_mapkey1_idxfn::equals(se76, *e32)) {
            long __sql_count_aggregate_1_mretweetretweet_retweet_tweet_id = e32->__SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v36 = e32->__av;
            __SQL_COUNT_AGGREGATE_1_mRETWEET5.addOrDelOnZero(se75.modify(tweet_tweet_id,__sql_count_aggregate_1_mretweetretweet_retweet_tweet_id,tweet_user_id),(v36 * -1L));
          } while ((n28=n28->nxt));
        }
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
    __SQL_COUNT_AGGREGATE_1_entry se1;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry se2;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se3;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se4;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se5;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se6;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry se7;
    __SQL_COUNT_AGGREGATE_1_entry se8;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry se9;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se10;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se11;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se12;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se13;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry se14;
    __SQL_COUNT_AGGREGATE_1_entry se15;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se16;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se17;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se18;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se19;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se20;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry se21;
    tuple2_L_L st1;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se22;
    tuple2_L_L st2;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se23;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se24;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry se25;
    tuple2_L_L st3;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se26;
    tuple2_L_L st4;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se27;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se28;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se29;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se30;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se31;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se32;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se33;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se34;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se35;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se36;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se37;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se38;
    __SQL_COUNT_AGGREGATE_1_entry se39;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se40;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se41;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se42;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se43;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se44;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry se45;
    tuple2_L_L st5;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se46;
    tuple2_L_L st6;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se47;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se48;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry se49;
    tuple2_L_L st7;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se50;
    tuple2_L_L st8;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se51;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_entry se52;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se53;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se54;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se55;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se56;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se57;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se58;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se59;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_entry se60;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_entry se61;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se62;
    __SQL_COUNT_AGGREGATE_1_entry se63;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se64;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry se65;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry se66;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se67;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se68;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se69;
    __SQL_COUNT_AGGREGATE_1_entry se70;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_entry se71;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_entry se72;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_entry se73;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_entry se74;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_entry se75;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_entry se76;
  
    /* Data structures used for storing materialized views */
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_map __SQL_COUNT_AGGREGATE_1_mFOLLOWER1;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3_map __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mRETWEET3;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_map __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1;
    __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5_map __SQL_COUNT_AGGREGATE_1_mFOLLOWER1_mTWEET1_mRETWEET5;
    __SQL_COUNT_AGGREGATE_1_mRETWEET2_map __SQL_COUNT_AGGREGATE_1_mRETWEET2;
    __SQL_COUNT_AGGREGATE_1_mRETWEET3_map __SQL_COUNT_AGGREGATE_1_mRETWEET3;
    __SQL_COUNT_AGGREGATE_1_mRETWEET5_map __SQL_COUNT_AGGREGATE_1_mRETWEET5;
    __SQL_COUNT_AGGREGATE_1_mRETWEET6_map __SQL_COUNT_AGGREGATE_1_mRETWEET6;
    __SQL_COUNT_AGGREGATE_1_mTWEET1_map __SQL_COUNT_AGGREGATE_1_mTWEET1;
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
        std::shared_ptr<dbt_file_source> source1_file(new dbt_file_source("examples/data/fastview/follower.csv",source1_fd,source1_adaptor));
        add_source(source1_file);
        
        pair<string,string> source2_adaptor_params[] = { make_pair("schema","long,long,string,long") };
        std::shared_ptr<csv_adaptor> source2_adaptor(new csv_adaptor(get_relation_id("RETWEET"),1,source2_adaptor_params));
        frame_descriptor source2_fd("\n");
        std::shared_ptr<dbt_file_source> source2_file(new dbt_file_source("examples/data/fastview/retweet.csv",source2_fd,source2_adaptor));
        add_source(source2_file);
        
        pair<string,string> source3_adaptor_params[] = { make_pair("schema","long,long,string") };
        std::shared_ptr<csv_adaptor> source3_adaptor(new csv_adaptor(get_relation_id("TWEET"),1,source3_adaptor_params));
        frame_descriptor source3_fd("\n");
        std::shared_ptr<dbt_file_source> source3_file(new dbt_file_source("examples/data/fastview/tweet.csv",source3_fd,source3_adaptor));
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
  class Q4Agg : public Program
  {
    public:
      Q4Agg(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

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
    long T_USER_ID; long A_TWEET_ID; long R1_TWEET_ID; long R1_RETWEET_TWEET_ID; STRING_TYPE R1_TWEET_DATE; long R2_RETWEET_TWEET_ID; long R2_USER_ID; long R2_TWEET_ID; long F_USER_ID; long D_FOLLOWER_ID; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*T_USER_ID = 0L; A_TWEET_ID = 0L; R1_TWEET_ID = 0L; R1_RETWEET_TWEET_ID = 0L; R1_TWEET_DATE = ""; R2_RETWEET_TWEET_ID = 0L; R2_USER_ID = 0L; R2_TWEET_ID = 0L; F_USER_ID = 0L; D_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const long c6, const long c7, const long c8, const long c9, const long c10) { T_USER_ID = c0; A_TWEET_ID = c1; R1_TWEET_ID = c2; R1_RETWEET_TWEET_ID = c3; R1_TWEET_DATE = c4; R2_RETWEET_TWEET_ID = c5; R2_USER_ID = c6; R2_TWEET_ID = c7; F_USER_ID = c8; D_FOLLOWER_ID = c9; __av = c10; }
    COUNT_entry(const COUNT_entry& other) : T_USER_ID( other.T_USER_ID ), A_TWEET_ID( other.A_TWEET_ID ), R1_TWEET_ID( other.R1_TWEET_ID ), R1_RETWEET_TWEET_ID( other.R1_RETWEET_TWEET_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), R2_RETWEET_TWEET_ID( other.R2_RETWEET_TWEET_ID ), R2_USER_ID( other.R2_USER_ID ), R2_TWEET_ID( other.R2_TWEET_ID ), F_USER_ID( other.F_USER_ID ), D_FOLLOWER_ID( other.D_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const long c6, const long c7, const long c8, const long c9) { T_USER_ID = c0; A_TWEET_ID = c1; R1_TWEET_ID = c2; R1_RETWEET_TWEET_ID = c3; R1_TWEET_DATE = c4; R2_RETWEET_TWEET_ID = c5; R2_USER_ID = c6; R2_TWEET_ID = c7; F_USER_ID = c8; D_FOLLOWER_ID = c9;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey0123456789_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.R1_TWEET_ID);
      hash_combine(h, e.R1_RETWEET_TWEET_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      hash_combine(h, e.R2_RETWEET_TWEET_ID);
      hash_combine(h, e.R2_USER_ID);
      hash_combine(h, e.R2_TWEET_ID);
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.D_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.T_USER_ID == y.T_USER_ID && x.A_TWEET_ID == y.A_TWEET_ID && x.R1_TWEET_ID == y.R1_TWEET_ID && x.R1_RETWEET_TWEET_ID == y.R1_RETWEET_TWEET_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE && x.R2_RETWEET_TWEET_ID == y.R2_RETWEET_TWEET_ID && x.R2_USER_ID == y.R2_USER_ID && x.R2_TWEET_ID == y.R2_TWEET_ID && x.F_USER_ID == y.F_USER_ID && x.D_FOLLOWER_ID == y.D_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey0123456789_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey0123456789_idxfn,true> HashIndex_COUNT_map_0123456789;
  
  struct COUNT_mFOLLOWER1_entry {
    long A_TWEET_ID; long COUNT_mFOLLOWERFOLLOWER_USER_ID; long R1_RETWEET_TWEET_ID; long T_USER_ID; long R2_RETWEET_TWEET_ID; STRING_TYPE R1_TWEET_DATE; long R1_TWEET_ID; long R2_TWEET_ID; long __av;  COUNT_mFOLLOWER1_entry* nxt; COUNT_mFOLLOWER1_entry* prv;
    explicit COUNT_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; R1_RETWEET_TWEET_ID = 0L; T_USER_ID = 0L; R2_RETWEET_TWEET_ID = 0L; R1_TWEET_DATE = ""; R1_TWEET_ID = 0L; R2_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_entry(const long c0, const long c1, const long c2, const long c3, const long c4, const STRING_TYPE& c5, const long c6, const long c7, const long c8) { A_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; R1_RETWEET_TWEET_ID = c2; T_USER_ID = c3; R2_RETWEET_TWEET_ID = c4; R1_TWEET_DATE = c5; R1_TWEET_ID = c6; R2_TWEET_ID = c7; __av = c8; }
    COUNT_mFOLLOWER1_entry(const COUNT_mFOLLOWER1_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), R1_RETWEET_TWEET_ID( other.R1_RETWEET_TWEET_ID ), T_USER_ID( other.T_USER_ID ), R2_RETWEET_TWEET_ID( other.R2_RETWEET_TWEET_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), R1_TWEET_ID( other.R1_TWEET_ID ), R2_TWEET_ID( other.R2_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify(const long c0, const long c1, const long c2, const long c3, const long c4, const STRING_TYPE& c5, const long c6, const long c7) { A_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; R1_RETWEET_TWEET_ID = c2; T_USER_ID = c3; R2_RETWEET_TWEET_ID = c4; R1_TWEET_DATE = c5; R1_TWEET_ID = c6; R2_TWEET_ID = c7;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.R1_RETWEET_TWEET_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.R2_RETWEET_TWEET_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      hash_combine(h, e.R1_TWEET_ID);
      hash_combine(h, e.R2_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_entry& x, const COUNT_mFOLLOWER1_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID && x.R1_RETWEET_TWEET_ID == y.R1_RETWEET_TWEET_ID && x.T_USER_ID == y.T_USER_ID && x.R2_RETWEET_TWEET_ID == y.R2_RETWEET_TWEET_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE && x.R1_TWEET_ID == y.R1_TWEET_ID && x.R2_TWEET_ID == y.R2_TWEET_ID;
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
    HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey01234567_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey1_idxfn,false>
  > COUNT_mFOLLOWER1_map;
  typedef HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey01234567_idxfn,true> HashIndex_COUNT_mFOLLOWER1_map_01234567;
  typedef HashIndex<COUNT_mFOLLOWER1_entry,long,COUNT_mFOLLOWER1_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER1_map_1;
  
  struct COUNT_mFOLLOWER1_mRETWEET7_entry {
    long COUNT_mFOLLOWERFOLLOWER_USER_ID; long COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID; long R2_TWEET_ID; long __av;  COUNT_mFOLLOWER1_mRETWEET7_entry* nxt; COUNT_mFOLLOWER1_mRETWEET7_entry* prv;
    explicit COUNT_mFOLLOWER1_mRETWEET7_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = 0L; R2_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_mRETWEET7_entry(const long c0, const long c1, const long c2, const long c3) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1; R2_TWEET_ID = c2; __av = c3; }
    COUNT_mFOLLOWER1_mRETWEET7_entry(const COUNT_mFOLLOWER1_mRETWEET7_entry& other) : COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID( other.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID ), R2_TWEET_ID( other.R2_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_mRETWEET7_entry& modify(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1; R2_TWEET_ID = c2;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mRETWEET7_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mRETWEET7_entry& modify1(const long c1) { COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mRETWEET7_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      hash_combine(h, e.R2_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mRETWEET7_entry& x, const COUNT_mFOLLOWER1_mRETWEET7_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID && x.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID && x.R2_TWEET_ID == y.R2_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mRETWEET7_entry& x, const COUNT_mFOLLOWER1_mRETWEET7_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mRETWEET7_entry& x, const COUNT_mFOLLOWER1_mRETWEET7_entry& y) {
      return x.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER1_mRETWEET7_entry,long,
    HashIndex<COUNT_mFOLLOWER1_mRETWEET7_entry,long,COUNT_mFOLLOWER1_mRETWEET7_mapkey012_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_mRETWEET7_entry,long,COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER1_mRETWEET7_entry,long,COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn,false>
  > COUNT_mFOLLOWER1_mRETWEET7_map;
  typedef HashIndex<COUNT_mFOLLOWER1_mRETWEET7_entry,long,COUNT_mFOLLOWER1_mRETWEET7_mapkey012_idxfn,true> HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_012;
  typedef HashIndex<COUNT_mFOLLOWER1_mRETWEET7_entry,long,COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0;
  typedef HashIndex<COUNT_mFOLLOWER1_mRETWEET7_entry,long,COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1;
  
  struct COUNT_mFOLLOWER1_mTWEET1_entry {
    long COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID; long COUNT_mFOLLOWERFOLLOWER_USER_ID; long R2_RETWEET_TWEET_ID; STRING_TYPE R1_TWEET_DATE; long R1_TWEET_ID; long R2_TWEET_ID; long __av;  COUNT_mFOLLOWER1_mTWEET1_entry* nxt; COUNT_mFOLLOWER1_mTWEET1_entry* prv;
    explicit COUNT_mFOLLOWER1_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = 0L; COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; R2_RETWEET_TWEET_ID = 0L; R1_TWEET_DATE = ""; R1_TWEET_ID = 0L; R2_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_mTWEET1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const long c6) { COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; R2_RETWEET_TWEET_ID = c2; R1_TWEET_DATE = c3; R1_TWEET_ID = c4; R2_TWEET_ID = c5; __av = c6; }
    COUNT_mFOLLOWER1_mTWEET1_entry(const COUNT_mFOLLOWER1_mTWEET1_entry& other) : COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID( other.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID ), COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), R2_RETWEET_TWEET_ID( other.R2_RETWEET_TWEET_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), R1_TWEET_ID( other.R1_TWEET_ID ), R2_TWEET_ID( other.R2_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_mTWEET1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5) { COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; R2_RETWEET_TWEET_ID = c2; R1_TWEET_DATE = c3; R1_TWEET_ID = c4; R2_TWEET_ID = c5;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mTWEET1_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mTWEET1_entry& modify0(const long c0) { COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mTWEET1_mapkey012345_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.R2_RETWEET_TWEET_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      hash_combine(h, e.R1_TWEET_ID);
      hash_combine(h, e.R2_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mTWEET1_entry& x, const COUNT_mFOLLOWER1_mTWEET1_entry& y) {
      return x.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID && x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID && x.R2_RETWEET_TWEET_ID == y.R2_RETWEET_TWEET_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE && x.R1_TWEET_ID == y.R1_TWEET_ID && x.R2_TWEET_ID == y.R2_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mTWEET1_entry& x, const COUNT_mFOLLOWER1_mTWEET1_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mTWEET1_entry& x, const COUNT_mFOLLOWER1_mTWEET1_entry& y) {
      return x.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER1_mTWEET1_entry,long,
    HashIndex<COUNT_mFOLLOWER1_mTWEET1_entry,long,COUNT_mFOLLOWER1_mTWEET1_mapkey012345_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_mTWEET1_entry,long,COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER1_mTWEET1_entry,long,COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER1_mTWEET1_map;
  typedef HashIndex<COUNT_mFOLLOWER1_mTWEET1_entry,long,COUNT_mFOLLOWER1_mTWEET1_mapkey012345_idxfn,true> HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_012345;
  typedef HashIndex<COUNT_mFOLLOWER1_mTWEET1_entry,long,COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1;
  typedef HashIndex<COUNT_mFOLLOWER1_mTWEET1_entry,long,COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0;
  
  struct COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry {
    long COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID; long COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID; STRING_TYPE R1_TWEET_DATE; long __av;  COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* nxt; COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* prv;
    explicit COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = 0L; R1_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3) { COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c1; R1_TWEET_DATE = c2; __av = c3; }
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& other) : COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID ), COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID( other.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& modify(const long c0, const long c1, const STRING_TYPE& c2) { COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c1; R1_TWEET_DATE = c2;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& modify0(const long c0) { COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& modify1(const long c1) { COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& x, const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& y) {
      return x.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID && x.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE;
    }
  };
  
  struct COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& x, const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& y) {
      return x.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& x, const COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry& y) {
      return x.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID == y.COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,
    HashIndex<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey012_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn,false>
  > COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map;
  typedef HashIndex<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey012_idxfn,true> HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_012;
  typedef HashIndex<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0;
  typedef HashIndex<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry,long,COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1;
  
  struct COUNT_pRETWEET13_entry {
    long A_TWEET_ID; long COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID; long T_USER_ID; long R1_RETWEET_TWEET_ID; STRING_TYPE R1_TWEET_DATE; long __av;  COUNT_pRETWEET13_entry* nxt; COUNT_pRETWEET13_entry* prv;
    explicit COUNT_pRETWEET13_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = 0L; T_USER_ID = 0L; R1_RETWEET_TWEET_ID = 0L; R1_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_pRETWEET13_entry(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5) { A_TWEET_ID = c0; COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = c1; T_USER_ID = c2; R1_RETWEET_TWEET_ID = c3; R1_TWEET_DATE = c4; __av = c5; }
    COUNT_pRETWEET13_entry(const COUNT_pRETWEET13_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID ), T_USER_ID( other.T_USER_ID ), R1_RETWEET_TWEET_ID( other.R1_RETWEET_TWEET_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_pRETWEET13_entry& modify(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4) { A_TWEET_ID = c0; COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = c1; T_USER_ID = c2; R1_RETWEET_TWEET_ID = c3; R1_TWEET_DATE = c4;  return *this; }
    FORCE_INLINE COUNT_pRETWEET13_entry& modify1(const long c1) { COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_pRETWEET13_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_pRETWEET13_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.R1_RETWEET_TWEET_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_pRETWEET13_entry& x, const COUNT_pRETWEET13_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID && x.T_USER_ID == y.T_USER_ID && x.R1_RETWEET_TWEET_ID == y.R1_RETWEET_TWEET_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE;
    }
  };
  
  struct COUNT_pRETWEET13_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_pRETWEET13_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_pRETWEET13_entry& x, const COUNT_pRETWEET13_entry& y) {
      return x.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_pRETWEET13_entry,long,
    HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey01234_idxfn,true>,
    HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey1_idxfn,false>
  > COUNT_pRETWEET13_map;
  typedef HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey01234_idxfn,true> HashIndex_COUNT_pRETWEET13_map_01234;
  typedef HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey1_idxfn,false> HashIndex_COUNT_pRETWEET13_map_1;
  
  struct COUNT_mRETWEET6_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long T_USER_ID; long __av;  COUNT_mRETWEET6_entry* nxt; COUNT_mRETWEET6_entry* prv;
    explicit COUNT_mRETWEET6_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; T_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET6_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; T_USER_ID = c1; __av = c2; }
    COUNT_mRETWEET6_entry(const COUNT_mRETWEET6_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), T_USER_ID( other.T_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET6_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; T_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET6_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET6_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.T_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET6_entry& x, const COUNT_mRETWEET6_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.T_USER_ID == y.T_USER_ID;
    }
  };
  
  struct COUNT_mRETWEET6_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET6_entry& x, const COUNT_mRETWEET6_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET6_entry,long,
    HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey0_idxfn,false>
  > COUNT_mRETWEET6_map;
  typedef HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET6_map_01;
  typedef HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET6_map_0;
  
  struct COUNT_mRETWEET7_entry {
    long COUNT_mRETWEETRETWEET_TWEET_ID; long D_FOLLOWER_ID; long F_USER_ID; long R2_TWEET_ID; long R2_USER_ID; long __av;  COUNT_mRETWEET7_entry* nxt; COUNT_mRETWEET7_entry* prv;
    explicit COUNT_mRETWEET7_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_TWEET_ID = 0L; D_FOLLOWER_ID = 0L; F_USER_ID = 0L; R2_TWEET_ID = 0L; R2_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET7_entry(const long c0, const long c1, const long c2, const long c3, const long c4, const long c5) { COUNT_mRETWEETRETWEET_TWEET_ID = c0; D_FOLLOWER_ID = c1; F_USER_ID = c2; R2_TWEET_ID = c3; R2_USER_ID = c4; __av = c5; }
    COUNT_mRETWEET7_entry(const COUNT_mRETWEET7_entry& other) : COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), D_FOLLOWER_ID( other.D_FOLLOWER_ID ), F_USER_ID( other.F_USER_ID ), R2_TWEET_ID( other.R2_TWEET_ID ), R2_USER_ID( other.R2_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET7_entry& modify(const long c0, const long c1, const long c2, const long c3, const long c4) { COUNT_mRETWEETRETWEET_TWEET_ID = c0; D_FOLLOWER_ID = c1; F_USER_ID = c2; R2_TWEET_ID = c3; R2_USER_ID = c4;  return *this; }
    FORCE_INLINE COUNT_mRETWEET7_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET7_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      hash_combine(h, e.D_FOLLOWER_ID);
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.R2_TWEET_ID);
      hash_combine(h, e.R2_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET7_entry& x, const COUNT_mRETWEET7_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID && x.D_FOLLOWER_ID == y.D_FOLLOWER_ID && x.F_USER_ID == y.F_USER_ID && x.R2_TWEET_ID == y.R2_TWEET_ID && x.R2_USER_ID == y.R2_USER_ID;
    }
  };
  
  struct COUNT_mRETWEET7_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET7_entry& x, const COUNT_mRETWEET7_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET7_entry,long,
    HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey01234_idxfn,true>,
    HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey0_idxfn,false>
  > COUNT_mRETWEET7_map;
  typedef HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey01234_idxfn,true> HashIndex_COUNT_mRETWEET7_map_01234;
  typedef HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET7_map_0;
  
  struct COUNT_mRETWEET14_entry {
    long A_TWEET_ID; long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long R1_RETWEET_TWEET_ID; long T_USER_ID; STRING_TYPE R1_TWEET_DATE; long __av;  COUNT_mRETWEET14_entry* nxt; COUNT_mRETWEET14_entry* prv;
    explicit COUNT_mRETWEET14_entry() : nxt(nullptr), prv(nullptr) { /*A_TWEET_ID = 0L; COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; R1_RETWEET_TWEET_ID = 0L; T_USER_ID = 0L; R1_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mRETWEET14_entry(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5) { A_TWEET_ID = c0; COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; R1_RETWEET_TWEET_ID = c2; T_USER_ID = c3; R1_TWEET_DATE = c4; __av = c5; }
    COUNT_mRETWEET14_entry(const COUNT_mRETWEET14_entry& other) : A_TWEET_ID( other.A_TWEET_ID ), COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), R1_RETWEET_TWEET_ID( other.R1_RETWEET_TWEET_ID ), T_USER_ID( other.T_USER_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET14_entry& modify(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4) { A_TWEET_ID = c0; COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; R1_RETWEET_TWEET_ID = c2; T_USER_ID = c3; R1_TWEET_DATE = c4;  return *this; }
    FORCE_INLINE COUNT_mRETWEET14_entry& modify1(const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET14_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET14_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_TWEET_ID);
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.R1_RETWEET_TWEET_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET14_entry& x, const COUNT_mRETWEET14_entry& y) {
      return x.A_TWEET_ID == y.A_TWEET_ID && x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.R1_RETWEET_TWEET_ID == y.R1_RETWEET_TWEET_ID && x.T_USER_ID == y.T_USER_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE;
    }
  };
  
  struct COUNT_mRETWEET14_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET14_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET14_entry& x, const COUNT_mRETWEET14_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET14_entry,long,
    HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey01234_idxfn,true>,
    HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey1_idxfn,false>
  > COUNT_mRETWEET14_map;
  typedef HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey01234_idxfn,true> HashIndex_COUNT_mRETWEET14_map_01234;
  typedef HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey1_idxfn,false> HashIndex_COUNT_mRETWEET14_map_1;
  
  struct COUNT_mRETWEET15_entry {
    long COUNT_mRETWEETRETWEET_USER_ID; long D_FOLLOWER_ID; long __av;  COUNT_mRETWEET15_entry* nxt; COUNT_mRETWEET15_entry* prv;
    explicit COUNT_mRETWEET15_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_USER_ID = 0L; D_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET15_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_USER_ID = c0; D_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mRETWEET15_entry(const COUNT_mRETWEET15_entry& other) : COUNT_mRETWEETRETWEET_USER_ID( other.COUNT_mRETWEETRETWEET_USER_ID ), D_FOLLOWER_ID( other.D_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET15_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_USER_ID = c0; D_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET15_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET15_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET15_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      hash_combine(h, e.D_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET15_entry& x, const COUNT_mRETWEET15_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID && x.D_FOLLOWER_ID == y.D_FOLLOWER_ID;
    }
  };
  
  struct COUNT_mRETWEET15_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET15_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET15_entry& x, const COUNT_mRETWEET15_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET15_entry,long,
    HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey0_idxfn,false>
  > COUNT_mRETWEET15_map;
  typedef HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET15_map_01;
  typedef HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET15_map_0;
  
  struct COUNT_mTWEET1_entry {
    long COUNT_mTWEETTWEET_TWEET_ID; long D_FOLLOWER_ID; long R1_TWEET_ID; STRING_TYPE R1_TWEET_DATE; long R2_RETWEET_TWEET_ID; long R2_USER_ID; long R2_TWEET_ID; long F_USER_ID; long __av;  COUNT_mTWEET1_entry* nxt; COUNT_mTWEET1_entry* prv;
    explicit COUNT_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mTWEETTWEET_TWEET_ID = 0L; D_FOLLOWER_ID = 0L; R1_TWEET_ID = 0L; R1_TWEET_DATE = ""; R2_RETWEET_TWEET_ID = 0L; R2_USER_ID = 0L; R2_TWEET_ID = 0L; F_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const long c6, const long c7, const long c8) { COUNT_mTWEETTWEET_TWEET_ID = c0; D_FOLLOWER_ID = c1; R1_TWEET_ID = c2; R1_TWEET_DATE = c3; R2_RETWEET_TWEET_ID = c4; R2_USER_ID = c5; R2_TWEET_ID = c6; F_USER_ID = c7; __av = c8; }
    COUNT_mTWEET1_entry(const COUNT_mTWEET1_entry& other) : COUNT_mTWEETTWEET_TWEET_ID( other.COUNT_mTWEETTWEET_TWEET_ID ), D_FOLLOWER_ID( other.D_FOLLOWER_ID ), R1_TWEET_ID( other.R1_TWEET_ID ), R1_TWEET_DATE( other.R1_TWEET_DATE ), R2_RETWEET_TWEET_ID( other.R2_RETWEET_TWEET_ID ), R2_USER_ID( other.R2_USER_ID ), R2_TWEET_ID( other.R2_TWEET_ID ), F_USER_ID( other.F_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const long c6, const long c7) { COUNT_mTWEETTWEET_TWEET_ID = c0; D_FOLLOWER_ID = c1; R1_TWEET_ID = c2; R1_TWEET_DATE = c3; R2_RETWEET_TWEET_ID = c4; R2_USER_ID = c5; R2_TWEET_ID = c6; F_USER_ID = c7;  return *this; }
    FORCE_INLINE COUNT_mTWEET1_entry& modify0(const long c0) { COUNT_mTWEETTWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R1_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R2_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mTWEET1_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.D_FOLLOWER_ID);
      hash_combine(h, e.R1_TWEET_ID);
      hash_combine(h, e.R1_TWEET_DATE);
      hash_combine(h, e.R2_RETWEET_TWEET_ID);
      hash_combine(h, e.R2_USER_ID);
      hash_combine(h, e.R2_TWEET_ID);
      hash_combine(h, e.F_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET1_entry& x, const COUNT_mTWEET1_entry& y) {
      return x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID && x.D_FOLLOWER_ID == y.D_FOLLOWER_ID && x.R1_TWEET_ID == y.R1_TWEET_ID && x.R1_TWEET_DATE == y.R1_TWEET_DATE && x.R2_RETWEET_TWEET_ID == y.R2_RETWEET_TWEET_ID && x.R2_USER_ID == y.R2_USER_ID && x.R2_TWEET_ID == y.R2_TWEET_ID && x.F_USER_ID == y.F_USER_ID;
    }
  };
  
  struct COUNT_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET1_entry& x, const COUNT_mTWEET1_entry& y) {
      return x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mTWEET1_entry,long,
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey01234567_idxfn,true>,
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,false>
  > COUNT_mTWEET1_map;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey01234567_idxfn,true> HashIndex_COUNT_mTWEET1_map_01234567;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,false> HashIndex_COUNT_mTWEET1_map_0;
  
  struct tuple5_LLLL_L {
    long _1; long _2; long _3; long _4; long __av; tuple5_LLLL_L* nxt; tuple5_LLLL_L* prv;
    explicit tuple5_LLLL_L() : nxt(nullptr), prv(nullptr) { }
    explicit tuple5_LLLL_L(const long c1, const long c2, const long c3, const long c4, long c__av=0L) : nxt(nullptr), prv(nullptr) { _1 = c1; _2 = c2; _3 = c3; _4 = c4; __av = c__av;}
    int operator<(const tuple5_LLLL_L &rhs) const { 
      if(this->_1!=rhs._1) return (this->_1<rhs._1);
      if(this->_2!=rhs._2) return (this->_2<rhs._2);
      if(this->_3!=rhs._3) return (this->_3<rhs._3);
      if(this->_4!=rhs._4) return (this->_4<rhs._4);
      return 0;
    }
    int operator==(const tuple5_LLLL_L &rhs) const { return ((this->_1==rhs._1) && (this->_2==rhs._2) && (this->_3==rhs._3) && (this->_4==rhs._4)); }
    FORCE_INLINE tuple5_LLLL_L& modify(const long c0, const long c1, const long c2, const long c3, long c__av) { _1 = c0; _2 = c1; _3 = c2; _4 = c3; __av = c__av; return *this; }
    static bool equals(const tuple5_LLLL_L &x, const tuple5_LLLL_L &y) { return ((x._1==y._1) && (x._2==y._2) && (x._3==y._3) && (x._4==y._4)); }
    static long hash(const tuple5_LLLL_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      hash_combine(h, e._2);
      hash_combine(h, e._3);
      hash_combine(h, e._4);
      return h;
    }
  };
  struct tuple7_LLLLLL_L {
    long _1; long _2; long _3; long _4; long _5; long _6; long __av; tuple7_LLLLLL_L* nxt; tuple7_LLLLLL_L* prv;
    explicit tuple7_LLLLLL_L() : nxt(nullptr), prv(nullptr) { }
    explicit tuple7_LLLLLL_L(const long c1, const long c2, const long c3, const long c4, const long c5, const long c6, long c__av=0L) : nxt(nullptr), prv(nullptr) { _1 = c1; _2 = c2; _3 = c3; _4 = c4; _5 = c5; _6 = c6; __av = c__av;}
    int operator<(const tuple7_LLLLLL_L &rhs) const { 
      if(this->_1!=rhs._1) return (this->_1<rhs._1);
      if(this->_2!=rhs._2) return (this->_2<rhs._2);
      if(this->_3!=rhs._3) return (this->_3<rhs._3);
      if(this->_4!=rhs._4) return (this->_4<rhs._4);
      if(this->_5!=rhs._5) return (this->_5<rhs._5);
      if(this->_6!=rhs._6) return (this->_6<rhs._6);
      return 0;
    }
    int operator==(const tuple7_LLLLLL_L &rhs) const { return ((this->_1==rhs._1) && (this->_2==rhs._2) && (this->_3==rhs._3) && (this->_4==rhs._4) && (this->_5==rhs._5) && (this->_6==rhs._6)); }
    FORCE_INLINE tuple7_LLLLLL_L& modify(const long c0, const long c1, const long c2, const long c3, const long c4, const long c5, long c__av) { _1 = c0; _2 = c1; _3 = c2; _4 = c3; _5 = c4; _6 = c5; __av = c__av; return *this; }
    static bool equals(const tuple7_LLLLLL_L &x, const tuple7_LLLLLL_L &y) { return ((x._1==y._1) && (x._2==y._2) && (x._3==y._3) && (x._4==y._4) && (x._5==y._5) && (x._6==y._6)); }
    static long hash(const tuple7_LLLLLL_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      hash_combine(h, e._2);
      hash_combine(h, e._3);
      hash_combine(h, e._4);
      hash_combine(h, e._5);
      hash_combine(h, e._6);
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
    data_t(): tlq_t(), _c1(16U), _c4(16U), _c7(16U), _c6(16U), _c3(16U), _c2(16U), _c5(16U), _c8(16U) {
      
    }
  
    #ifdef DBT_PROFILE
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> exec_stats;
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> ivc_stats;
    #endif
  
    /* Registering relations and trigger functions */
    void register_data(ProgramBase& pb) {
  
      pb.add_map<COUNT_map>( "COUNT", COUNT );
      pb.add_map<COUNT_mFOLLOWER1_map>( "COUNT_mFOLLOWER1", COUNT_mFOLLOWER1 );
      pb.add_map<COUNT_mFOLLOWER1_mRETWEET7_map>( "COUNT_mFOLLOWER1_mRETWEET7", COUNT_mFOLLOWER1_mRETWEET7 );
      pb.add_map<COUNT_mFOLLOWER1_mTWEET1_map>( "COUNT_mFOLLOWER1_mTWEET1", COUNT_mFOLLOWER1_mTWEET1 );
      pb.add_map<COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map>( "COUNT_mFOLLOWER1_mTWEET1_mRETWEET11", COUNT_mFOLLOWER1_mTWEET1_mRETWEET11 );
      pb.add_map<COUNT_pRETWEET13_map>( "COUNT_pRETWEET13", COUNT_pRETWEET13 );
      pb.add_map<COUNT_mRETWEET6_map>( "COUNT_mRETWEET6", COUNT_mRETWEET6 );
      pb.add_map<COUNT_mRETWEET7_map>( "COUNT_mRETWEET7", COUNT_mRETWEET7 );
      pb.add_map<COUNT_mRETWEET14_map>( "COUNT_mRETWEET14", COUNT_mRETWEET14 );
      pb.add_map<COUNT_mRETWEET15_map>( "COUNT_mRETWEET15", COUNT_mRETWEET15 );
      pb.add_map<COUNT_mTWEET1_map>( "COUNT_mTWEET1", COUNT_mTWEET1 );
  
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
      exec_stats->register_probe(16, "insert_RETWEET_s8");
      exec_stats->register_probe(17, "delete_RETWEET_s0");
      exec_stats->register_probe(18, "delete_RETWEET_s1");
      exec_stats->register_probe(19, "delete_RETWEET_s2");
      exec_stats->register_probe(20, "delete_RETWEET_s3");
      exec_stats->register_probe(21, "delete_RETWEET_s4");
      exec_stats->register_probe(22, "delete_RETWEET_s5");
      exec_stats->register_probe(23, "delete_RETWEET_s6");
      exec_stats->register_probe(24, "delete_RETWEET_s7");
      exec_stats->register_probe(25, "delete_RETWEET_s8");
      exec_stats->register_probe(26, "insert_TWEET_s0");
      exec_stats->register_probe(27, "insert_TWEET_s1");
      exec_stats->register_probe(28, "insert_TWEET_s2");
      exec_stats->register_probe(29, "insert_TWEET_s3");
      exec_stats->register_probe(30, "insert_TWEET_s4");
      exec_stats->register_probe(31, "delete_TWEET_s0");
      exec_stats->register_probe(32, "delete_TWEET_s1");
      exec_stats->register_probe(33, "delete_TWEET_s2");
      exec_stats->register_probe(34, "delete_TWEET_s3");
      exec_stats->register_probe(35, "delete_TWEET_s4");
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
            long r1_retweet_tweet_id = e1->R1_RETWEET_TWEET_ID;
            long t_user_id = e1->T_USER_ID;
            long r2_retweet_tweet_id = e1->R2_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e1->R1_TWEET_DATE;
            long r1_tweet_id = e1->R1_TWEET_ID;
            long r2_tweet_id = e1->R2_TWEET_ID;
            long v1 = e1->__av;
            COUNT.addOrDelOnZero(se1.modify(t_user_id,a_tweet_id,r1_tweet_id,r1_retweet_tweet_id,r1_tweet_date,r2_retweet_tweet_id,follower_user_id,r2_tweet_id,follower_user_id,follower_follower_id),v1);
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn::hash(se4.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0* i2 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0*>(COUNT_mFOLLOWER1_mRETWEET7.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mFOLLOWER1_mRETWEET7_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn::equals(se4, *e2)) {
            long count_mretweetretweet_tweet_id = e2->COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long r2_tweet_id = e2->R2_TWEET_ID;
            long v2 = e2->__av;
            COUNT_mRETWEET7.addOrDelOnZero(se3.modify(count_mretweetretweet_tweet_id,follower_follower_id,follower_user_id,r2_tweet_id,follower_user_id),v2);
          } while ((n2=n2->nxt));
        }
        COUNT_mRETWEET15.addOrDelOnZero(se5.modify(follower_user_id,follower_follower_id),1L);
        { //slice
          const HASH_RES_t h3 = COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn::hash(se7.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1* i3 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1*>(COUNT_mFOLLOWER1_mTWEET1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mFOLLOWER1_mTWEET1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn::equals(se7, *e3)) {
            long count_mtweettweet_tweet_id = e3->COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long r2_retweet_tweet_id = e3->R2_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e3->R1_TWEET_DATE;
            long r1_tweet_id = e3->R1_TWEET_ID;
            long r2_tweet_id = e3->R2_TWEET_ID;
            long v3 = e3->__av;
            COUNT_mTWEET1.addOrDelOnZero(se6.modify(count_mtweettweet_tweet_id,follower_follower_id,r1_tweet_id,r1_tweet_date,r2_retweet_tweet_id,follower_user_id,r2_tweet_id,follower_user_id),v3);
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
          const HASH_RES_t h4 = COUNT_mFOLLOWER1_mapkey1_idxfn::hash(se9.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_1* i4 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_1*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_1::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mFOLLOWER1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mFOLLOWER1_mapkey1_idxfn::equals(se9, *e4)) {
            long a_tweet_id = e4->A_TWEET_ID;
            long r1_retweet_tweet_id = e4->R1_RETWEET_TWEET_ID;
            long t_user_id = e4->T_USER_ID;
            long r2_retweet_tweet_id = e4->R2_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e4->R1_TWEET_DATE;
            long r1_tweet_id = e4->R1_TWEET_ID;
            long r2_tweet_id = e4->R2_TWEET_ID;
            long v4 = e4->__av;
            COUNT.addOrDelOnZero(se8.modify(t_user_id,a_tweet_id,r1_tweet_id,r1_retweet_tweet_id,r1_tweet_date,r2_retweet_tweet_id,follower_user_id,r2_tweet_id,follower_user_id,follower_follower_id),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
        { //slice
          const HASH_RES_t h5 = COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn::hash(se11.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0* i5 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0*>(COUNT_mFOLLOWER1_mRETWEET7.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mFOLLOWER1_mRETWEET7_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mFOLLOWER1_mRETWEET7_mapkey0_idxfn::equals(se11, *e5)) {
            long count_mretweetretweet_tweet_id = e5->COUNT_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long r2_tweet_id = e5->R2_TWEET_ID;
            long v5 = e5->__av;
            COUNT_mRETWEET7.addOrDelOnZero(se10.modify(count_mretweetretweet_tweet_id,follower_follower_id,follower_user_id,r2_tweet_id,follower_user_id),(v5 * -1L));
          } while ((n5=n5->nxt));
        }
        COUNT_mRETWEET15.addOrDelOnZero(se12.modify(follower_user_id,follower_follower_id),-1L);
        { //slice
          const HASH_RES_t h6 = COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn::hash(se14.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1* i6 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1*>(COUNT_mFOLLOWER1_mTWEET1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_1::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mFOLLOWER1_mTWEET1_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mFOLLOWER1_mTWEET1_mapkey1_idxfn::equals(se14, *e6)) {
            long count_mtweettweet_tweet_id = e6->COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            long r2_retweet_tweet_id = e6->R2_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e6->R1_TWEET_DATE;
            long r1_tweet_id = e6->R1_TWEET_ID;
            long r2_tweet_id = e6->R2_TWEET_ID;
            long v6 = e6->__av;
            COUNT_mTWEET1.addOrDelOnZero(se13.modify(count_mtweettweet_tweet_id,follower_follower_id,r1_tweet_id,r1_tweet_date,r2_retweet_tweet_id,follower_user_id,r2_tweet_id,follower_user_id),(v6 * -1L));
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
          const HASH_RES_t h9 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se18.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i7 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n7 = &(i7->buckets_[h9 % i7->size_]);
          COUNT_mRETWEET6_entry* e7;
          do if ((e7=n7->obj) && h9 == n7->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se18, *e7)) {
            long t_user_id = e7->T_USER_ID;
            long v7 = e7->__av;
            _c1.clear();
            { //slice
              const HASH_RES_t h7 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se16.modify0(retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET7_map_0* i8 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
              HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n8 = &(i8->buckets_[h7 % i8->size_]);
              COUNT_mRETWEET7_entry* e8;
              do if ((e8=n8->obj) && h7 == n8->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se16, *e8)) {
                long d_follower_id = e8->D_FOLLOWER_ID;
                long r2_user_id = e8->F_USER_ID;
                long r2_tweet_id = e8->R2_TWEET_ID;
                long f_user_id = e8->R2_USER_ID;
                long v9 = e8->__av;
                add_to_temp_map(_c1, st1.modify(r2_tweet_id,r2_user_id,f_user_id,retweet_tweet_id,retweet_tweet_id,d_follower_id,v9));
              } while ((n8=n8->nxt));
            }
            { //slice
              const HASH_RES_t h8 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se17.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i9 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n9 = &(i9->buckets_[h8 % i9->size_]);
              COUNT_mRETWEET15_entry* e9;
              do if ((e9=n9->obj) && h8 == n9->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se17, *e9)) {
                long d_follower_id = e9->D_FOLLOWER_ID;
                long v10 = e9->__av;
                (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c1, st2.modify(retweet_tweet_id,retweet_user_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,d_follower_id,v10)) : voidFunc());
              } while ((n9=n9->nxt));
            }
            { //temp foreach
              tuple7_LLLLLL_L* e10 = _c1.head;
              while(e10){
                long r2_tweet_id = e10->_1;
                long r2_user_id = e10->_2;
                long f_user_id = e10->_3;
                long r2_retweet_tweet_id = e10->_4;
                long r1_tweet_id = e10->_5;
                long d_follower_id = e10->_6;
                long v11 = e10->__av;
                COUNT.addOrDelOnZero(se15.modify(t_user_id,retweet_retweet_tweet_id,r1_tweet_id,retweet_retweet_tweet_id,retweet_tweet_date,r2_retweet_tweet_id,r2_user_id,r2_tweet_id,f_user_id,d_follower_id),(v7 * v11));
                e10 = e10->nxt;
              }
            }
          } while ((n7=n7->nxt));
        }
        { //slice
          const HASH_RES_t h11 = COUNT_pRETWEET13_mapkey1_idxfn::hash(se20.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_pRETWEET13_map_1* i10 = static_cast<HashIndex_COUNT_pRETWEET13_map_1*>(COUNT_pRETWEET13.index[1]);
          HashIndex_COUNT_pRETWEET13_map_1::IdxNode* n10 = &(i10->buckets_[h11 % i10->size_]);
          COUNT_pRETWEET13_entry* e11;
          do if ((e11=n10->obj) && h11 == n10->hash && COUNT_pRETWEET13_mapkey1_idxfn::equals(se20, *e11)) {
            long a_tweet_id = e11->A_TWEET_ID;
            long t_user_id = e11->T_USER_ID;
            long r1_retweet_tweet_id = e11->R1_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e11->R1_TWEET_DATE;
            long v12 = e11->__av;
            { //slice
              const HASH_RES_t h10 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se19.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i11 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n11 = &(i11->buckets_[h10 % i11->size_]);
              COUNT_mRETWEET15_entry* e12;
              do if ((e12=n11->obj) && h10 == n11->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se19, *e12)) {
                long d_follower_id = e12->D_FOLLOWER_ID;
                long v13 = e12->__av;
                COUNT.addOrDelOnZero(se15.modify(t_user_id,a_tweet_id,retweet_retweet_tweet_id,r1_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_user_id,retweet_tweet_id,retweet_user_id,d_follower_id),(v12 * v13));
              } while ((n11=n11->nxt));
            }
          } while ((n10=n10->nxt));
        }
        { //slice
          const HASH_RES_t h13 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se23.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i12 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n12 = &(i12->buckets_[h13 % i12->size_]);
          COUNT_mRETWEET6_entry* e13;
          do if ((e13=n12->obj) && h13 == n12->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se23, *e13)) {
            long t_user_id = e13->T_USER_ID;
            long v14 = e13->__av;
            _c2.clear();
            { //slice
              const HASH_RES_t h12 = COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::hash(se22.modify1(retweet_tweet_id));
              const HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1* i13 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1*>(COUNT_mFOLLOWER1_mRETWEET7.index[2]);
              HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1::IdxNode* n13 = &(i13->buckets_[h12 % i13->size_]);
              COUNT_mFOLLOWER1_mRETWEET7_entry* e14;
              do if ((e14=n13->obj) && h12 == n13->hash && COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::equals(se22, *e14)) {
                long count_mfollowerfollower_user_id = e14->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long r2_tweet_id = e14->R2_TWEET_ID;
                long v16 = e14->__av;
                add_to_temp_map(_c2, st3.modify(r2_tweet_id,count_mfollowerfollower_user_id,retweet_tweet_id,retweet_tweet_id,v16));
              } while ((n13=n13->nxt));
            }
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c2, st4.modify(retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
            { //temp foreach
              tuple5_LLLL_L* e15 = _c2.head;
              while(e15){
                long r2_tweet_id = e15->_1;
                long count_mfollowerfollower_user_id = e15->_2;
                long r2_retweet_tweet_id = e15->_3;
                long r1_tweet_id = e15->_4;
                long v17 = e15->__av;
                COUNT_mFOLLOWER1.addOrDelOnZero(se21.modify(retweet_retweet_tweet_id,count_mfollowerfollower_user_id,retweet_retweet_tweet_id,t_user_id,r2_retweet_tweet_id,retweet_tweet_date,r1_tweet_id,r2_tweet_id),(v14 * v17));
                e15 = e15->nxt;
              }
            }
          } while ((n12=n12->nxt));
        }
        { //slice
          const HASH_RES_t h14 = COUNT_mRETWEET14_mapkey1_idxfn::hash(se24.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET14_map_1* i14 = static_cast<HashIndex_COUNT_mRETWEET14_map_1*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_1::IdxNode* n14 = &(i14->buckets_[h14 % i14->size_]);
          COUNT_mRETWEET14_entry* e16;
          do if ((e16=n14->obj) && h14 == n14->hash && COUNT_mRETWEET14_mapkey1_idxfn::equals(se24, *e16)) {
            long a_tweet_id = e16->A_TWEET_ID;
            long r1_retweet_tweet_id = e16->R1_RETWEET_TWEET_ID;
            long t_user_id = e16->T_USER_ID;
            STRING_TYPE r1_tweet_date = e16->R1_TWEET_DATE;
            long v18 = e16->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se21.modify(a_tweet_id,retweet_user_id,r1_retweet_tweet_id,t_user_id,retweet_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id),v18);
          } while ((n14=n14->nxt));
        }
        _c3.clear();
        { //slice
          const HASH_RES_t h15 = COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::hash(se26.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1* i15 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1*>(COUNT_mFOLLOWER1_mRETWEET7.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1::IdxNode* n15 = &(i15->buckets_[h15 % i15->size_]);
          COUNT_mFOLLOWER1_mRETWEET7_entry* e17;
          do if ((e17=n15->obj) && h15 == n15->hash && COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::equals(se26, *e17)) {
            long count_mfollowerfollower_user_id = e17->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long r2_tweet_id = e17->R2_TWEET_ID;
            long v20 = e17->__av;
            add_to_temp_map(_c3, st5.modify(r2_tweet_id,count_mfollowerfollower_user_id,retweet_tweet_id,retweet_tweet_id,v20));
          } while ((n15=n15->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c3, st6.modify(retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
        { //temp foreach
          tuple5_LLLL_L* e18 = _c3.head;
          while(e18){
            long r2_tweet_id = e18->_1;
            long count_mfollowerfollower_user_id = e18->_2;
            long r2_retweet_tweet_id = e18->_3;
            long r1_tweet_id = e18->_4;
            long v21 = e18->__av;
            COUNT_mFOLLOWER1_mTWEET1.addOrDelOnZero(se25.modify(retweet_retweet_tweet_id,count_mfollowerfollower_user_id,r2_retweet_tweet_id,retweet_tweet_date,r1_tweet_id,r2_tweet_id),v21);
            e18 = e18->nxt;
          }
        }
        { //slice
          const HASH_RES_t h16 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::hash(se27.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0* i16 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e19;
          do if ((e19=n16->obj) && h16 == n16->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::equals(se27, *e19)) {
            long count_mfollower1_mtweettweet_tweet_id = e19->COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e19->R1_TWEET_DATE;
            long v22 = e19->__av;
            COUNT_mFOLLOWER1_mTWEET1.addOrDelOnZero(se25.modify(count_mfollower1_mtweettweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id),v22);
          } while ((n16=n16->nxt));
        }
        COUNT_mFOLLOWER1_mRETWEET7.addOrDelOnZero(se28.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id),1L);
        { //slice
          const HASH_RES_t h17 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se30.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i17 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n17 = &(i17->buckets_[h17 % i17->size_]);
          COUNT_mRETWEET6_entry* e20;
          do if ((e20=n17->obj) && h17 == n17->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se30, *e20)) {
            long t_user_id = e20->T_USER_ID;
            long v23 = e20->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se29.modify(retweet_retweet_tweet_id,retweet_tweet_id,t_user_id,retweet_retweet_tweet_id,retweet_tweet_date),v23);
          } while ((n17=n17->nxt));
        }
        { //slice
          const HASH_RES_t h18 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se32.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i18 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n18 = &(i18->buckets_[h18 % i18->size_]);
          COUNT_mRETWEET6_entry* e21;
          do if ((e21=n18->obj) && h18 == n18->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se32, *e21)) {
            long t_user_id = e21->T_USER_ID;
            long v24 = e21->__av;
            COUNT_mRETWEET14.addOrDelOnZero(se31.modify(retweet_retweet_tweet_id,retweet_tweet_id,retweet_retweet_tweet_id,t_user_id,retweet_tweet_date),v24);
          } while ((n18=n18->nxt));
        }
        _c4.clear();
        { //slice
          const HASH_RES_t h19 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se34.modify0(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i19 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n19 = &(i19->buckets_[h19 % i19->size_]);
          COUNT_mRETWEET7_entry* e22;
          do if ((e22=n19->obj) && h19 == n19->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se34, *e22)) {
            long d_follower_id = e22->D_FOLLOWER_ID;
            long r2_user_id = e22->F_USER_ID;
            long r2_tweet_id = e22->R2_TWEET_ID;
            long f_user_id = e22->R2_USER_ID;
            long v26 = e22->__av;
            add_to_temp_map(_c4, st7.modify(r2_tweet_id,r2_user_id,f_user_id,retweet_tweet_id,retweet_tweet_id,d_follower_id,v26));
          } while ((n19=n19->nxt));
        }
        { //slice
          const HASH_RES_t h20 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se35.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET15_map_0* i20 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
          HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n20 = &(i20->buckets_[h20 % i20->size_]);
          COUNT_mRETWEET15_entry* e23;
          do if ((e23=n20->obj) && h20 == n20->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se35, *e23)) {
            long d_follower_id = e23->D_FOLLOWER_ID;
            long v27 = e23->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c4, st8.modify(retweet_tweet_id,retweet_user_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,d_follower_id,v27)) : voidFunc());
          } while ((n20=n20->nxt));
        }
        { //temp foreach
          tuple7_LLLLLL_L* e24 = _c4.head;
          while(e24){
            long r2_tweet_id = e24->_1;
            long r2_user_id = e24->_2;
            long f_user_id = e24->_3;
            long r2_retweet_tweet_id = e24->_4;
            long r1_tweet_id = e24->_5;
            long d_follower_id = e24->_6;
            long v28 = e24->__av;
            COUNT_mTWEET1.addOrDelOnZero(se33.modify(retweet_retweet_tweet_id,d_follower_id,r1_tweet_id,retweet_tweet_date,r2_retweet_tweet_id,r2_user_id,r2_tweet_id,f_user_id),v28);
            e24 = e24->nxt;
          }
        }
        { //slice
          const HASH_RES_t h22 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::hash(se37.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0* i21 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0::IdxNode* n21 = &(i21->buckets_[h22 % i21->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e25;
          do if ((e25=n21->obj) && h22 == n21->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::equals(se37, *e25)) {
            long count_mtweettweet_tweet_id = e25->COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e25->R1_TWEET_DATE;
            long v29 = e25->__av;
            { //slice
              const HASH_RES_t h21 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se36.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i22 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n22 = &(i22->buckets_[h21 % i22->size_]);
              COUNT_mRETWEET15_entry* e26;
              do if ((e26=n22->obj) && h21 == n22->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se36, *e26)) {
                long d_follower_id = e26->D_FOLLOWER_ID;
                long v30 = e26->__av;
                COUNT_mTWEET1.addOrDelOnZero(se33.modify(count_mtweettweet_tweet_id,d_follower_id,retweet_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_user_id,retweet_tweet_id,retweet_user_id),(v29 * v30));
              } while ((n22=n22->nxt));
            }
          } while ((n21=n21->nxt));
        }
        { //slice
          const HASH_RES_t h23 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se39.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET15_map_0* i23 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
          HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n23 = &(i23->buckets_[h23 % i23->size_]);
          COUNT_mRETWEET15_entry* e27;
          do if ((e27=n23->obj) && h23 == n23->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se39, *e27)) {
            long d_follower_id = e27->D_FOLLOWER_ID;
            long v31 = e27->__av;
            COUNT_mRETWEET7.addOrDelOnZero(se38.modify(retweet_retweet_tweet_id,d_follower_id,retweet_user_id,retweet_tweet_id,retweet_user_id),v31);
          } while ((n23=n23->nxt));
        }
        COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.addOrDelOnZero(se40.modify(retweet_tweet_id,retweet_retweet_tweet_id,retweet_tweet_date),1L);
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
          const HASH_RES_t h26 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se44.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i24 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n24 = &(i24->buckets_[h26 % i24->size_]);
          COUNT_mRETWEET6_entry* e28;
          do if ((e28=n24->obj) && h26 == n24->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se44, *e28)) {
            long t_user_id = e28->T_USER_ID;
            long v32 = e28->__av;
            _c5.clear();
            { //slice
              const HASH_RES_t h24 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se42.modify0(retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET7_map_0* i25 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
              HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n25 = &(i25->buckets_[h24 % i25->size_]);
              COUNT_mRETWEET7_entry* e29;
              do if ((e29=n25->obj) && h24 == n25->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se42, *e29)) {
                long d_follower_id = e29->D_FOLLOWER_ID;
                long f_user_id = e29->F_USER_ID;
                long r2_tweet_id = e29->R2_TWEET_ID;
                long r2_user_id = e29->R2_USER_ID;
                long v34 = e29->__av;
                add_to_temp_map(_c5, st9.modify(r2_tweet_id,r2_user_id,f_user_id,retweet_tweet_id,retweet_tweet_id,d_follower_id,(v34 * -1L)));
              } while ((n25=n25->nxt));
            }
            { //slice
              const HASH_RES_t h25 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se43.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i26 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n26 = &(i26->buckets_[h25 % i26->size_]);
              COUNT_mRETWEET15_entry* e30;
              do if ((e30=n26->obj) && h25 == n26->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se43, *e30)) {
                long d_follower_id = e30->D_FOLLOWER_ID;
                long v35 = e30->__av;
                (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c5, st10.modify(retweet_tweet_id,retweet_user_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,d_follower_id,v35)) : voidFunc());
              } while ((n26=n26->nxt));
            }
            { //temp foreach
              tuple7_LLLLLL_L* e31 = _c5.head;
              while(e31){
                long r2_tweet_id = e31->_1;
                long r2_user_id = e31->_2;
                long f_user_id = e31->_3;
                long r2_retweet_tweet_id = e31->_4;
                long r1_tweet_id = e31->_5;
                long d_follower_id = e31->_6;
                long v36 = e31->__av;
                COUNT.addOrDelOnZero(se41.modify(t_user_id,retweet_retweet_tweet_id,r1_tweet_id,retweet_retweet_tweet_id,retweet_tweet_date,r2_retweet_tweet_id,r2_user_id,r2_tweet_id,f_user_id,d_follower_id),(v32 * v36));
                e31 = e31->nxt;
              }
            }
          } while ((n24=n24->nxt));
        }
        { //slice
          const HASH_RES_t h28 = COUNT_mRETWEET14_mapkey1_idxfn::hash(se46.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET14_map_1* i27 = static_cast<HashIndex_COUNT_mRETWEET14_map_1*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_1::IdxNode* n27 = &(i27->buckets_[h28 % i27->size_]);
          COUNT_mRETWEET14_entry* e32;
          do if ((e32=n27->obj) && h28 == n27->hash && COUNT_mRETWEET14_mapkey1_idxfn::equals(se46, *e32)) {
            long a_tweet_id = e32->A_TWEET_ID;
            long r1_retweet_tweet_id = e32->R1_RETWEET_TWEET_ID;
            long t_user_id = e32->T_USER_ID;
            STRING_TYPE r1_tweet_date = e32->R1_TWEET_DATE;
            long v37 = e32->__av;
            { //slice
              const HASH_RES_t h27 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se45.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i28 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n28 = &(i28->buckets_[h27 % i28->size_]);
              COUNT_mRETWEET15_entry* e33;
              do if ((e33=n28->obj) && h27 == n28->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se45, *e33)) {
                long d_follower_id = e33->D_FOLLOWER_ID;
                long v38 = e33->__av;
                COUNT.addOrDelOnZero(se41.modify(t_user_id,a_tweet_id,retweet_retweet_tweet_id,r1_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_user_id,retweet_tweet_id,retweet_user_id,d_follower_id),(v37 * (v38 * -1L)));
              } while ((n28=n28->nxt));
            }
          } while ((n27=n27->nxt));
        }
        { //slice
          const HASH_RES_t h30 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se49.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i29 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n29 = &(i29->buckets_[h30 % i29->size_]);
          COUNT_mRETWEET6_entry* e34;
          do if ((e34=n29->obj) && h30 == n29->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se49, *e34)) {
            long t_user_id = e34->T_USER_ID;
            long v39 = e34->__av;
            _c6.clear();
            { //slice
              const HASH_RES_t h29 = COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::hash(se48.modify1(retweet_tweet_id));
              const HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1* i30 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1*>(COUNT_mFOLLOWER1_mRETWEET7.index[2]);
              HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1::IdxNode* n30 = &(i30->buckets_[h29 % i30->size_]);
              COUNT_mFOLLOWER1_mRETWEET7_entry* e35;
              do if ((e35=n30->obj) && h29 == n30->hash && COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::equals(se48, *e35)) {
                long count_mfollowerfollower_user_id = e35->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long r2_tweet_id = e35->R2_TWEET_ID;
                long v41 = e35->__av;
                add_to_temp_map(_c6, st11.modify(r2_tweet_id,count_mfollowerfollower_user_id,retweet_tweet_id,retweet_tweet_id,(v41 * -1L)));
              } while ((n30=n30->nxt));
            }
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c6, st12.modify(retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
            { //temp foreach
              tuple5_LLLL_L* e36 = _c6.head;
              while(e36){
                long r2_tweet_id = e36->_1;
                long count_mfollowerfollower_user_id = e36->_2;
                long r2_retweet_tweet_id = e36->_3;
                long r1_tweet_id = e36->_4;
                long v42 = e36->__av;
                COUNT_mFOLLOWER1.addOrDelOnZero(se47.modify(retweet_retweet_tweet_id,count_mfollowerfollower_user_id,retweet_retweet_tweet_id,t_user_id,r2_retweet_tweet_id,retweet_tweet_date,r1_tweet_id,r2_tweet_id),(v39 * v42));
                e36 = e36->nxt;
              }
            }
          } while ((n29=n29->nxt));
        }
        { //slice
          const HASH_RES_t h31 = COUNT_pRETWEET13_mapkey1_idxfn::hash(se50.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_pRETWEET13_map_1* i31 = static_cast<HashIndex_COUNT_pRETWEET13_map_1*>(COUNT_pRETWEET13.index[1]);
          HashIndex_COUNT_pRETWEET13_map_1::IdxNode* n31 = &(i31->buckets_[h31 % i31->size_]);
          COUNT_pRETWEET13_entry* e37;
          do if ((e37=n31->obj) && h31 == n31->hash && COUNT_pRETWEET13_mapkey1_idxfn::equals(se50, *e37)) {
            long a_tweet_id = e37->A_TWEET_ID;
            long t_user_id = e37->T_USER_ID;
            long r1_retweet_tweet_id = e37->R1_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e37->R1_TWEET_DATE;
            long v43 = e37->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se47.modify(a_tweet_id,retweet_user_id,r1_retweet_tweet_id,t_user_id,retweet_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id),(v43 * -1L));
          } while ((n31=n31->nxt));
        }
        _c7.clear();
        { //slice
          const HASH_RES_t h32 = COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::hash(se52.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1* i32 = static_cast<HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1*>(COUNT_mFOLLOWER1_mRETWEET7.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mRETWEET7_map_1::IdxNode* n32 = &(i32->buckets_[h32 % i32->size_]);
          COUNT_mFOLLOWER1_mRETWEET7_entry* e38;
          do if ((e38=n32->obj) && h32 == n32->hash && COUNT_mFOLLOWER1_mRETWEET7_mapkey1_idxfn::equals(se52, *e38)) {
            long count_mfollowerfollower_user_id = e38->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long r2_tweet_id = e38->R2_TWEET_ID;
            long v45 = e38->__av;
            add_to_temp_map(_c7, st13.modify(r2_tweet_id,count_mfollowerfollower_user_id,retweet_tweet_id,retweet_tweet_id,(v45 * -1L)));
          } while ((n32=n32->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c7, st14.modify(retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
        { //temp foreach
          tuple5_LLLL_L* e39 = _c7.head;
          while(e39){
            long r2_tweet_id = e39->_1;
            long count_mfollowerfollower_user_id = e39->_2;
            long r2_retweet_tweet_id = e39->_3;
            long r1_tweet_id = e39->_4;
            long v46 = e39->__av;
            COUNT_mFOLLOWER1_mTWEET1.addOrDelOnZero(se51.modify(retweet_retweet_tweet_id,count_mfollowerfollower_user_id,r2_retweet_tweet_id,retweet_tweet_date,r1_tweet_id,r2_tweet_id),v46);
            e39 = e39->nxt;
          }
        }
        { //slice
          const HASH_RES_t h33 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::hash(se53.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0* i33 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0::IdxNode* n33 = &(i33->buckets_[h33 % i33->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e40;
          do if ((e40=n33->obj) && h33 == n33->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::equals(se53, *e40)) {
            long count_mfollower1_mtweettweet_tweet_id = e40->COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e40->R1_TWEET_DATE;
            long v47 = e40->__av;
            COUNT_mFOLLOWER1_mTWEET1.addOrDelOnZero(se51.modify(count_mfollower1_mtweettweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id),(v47 * -1L));
          } while ((n33=n33->nxt));
        }
        COUNT_mFOLLOWER1_mRETWEET7.addOrDelOnZero(se54.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id),-1L);
        { //slice
          const HASH_RES_t h34 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se56.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i34 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n34 = &(i34->buckets_[h34 % i34->size_]);
          COUNT_mRETWEET6_entry* e41;
          do if ((e41=n34->obj) && h34 == n34->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se56, *e41)) {
            long t_user_id = e41->T_USER_ID;
            long v48 = e41->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se55.modify(retweet_retweet_tweet_id,retweet_tweet_id,t_user_id,retweet_retweet_tweet_id,retweet_tweet_date),(v48 * -1L));
          } while ((n34=n34->nxt));
        }
        { //slice
          const HASH_RES_t h35 = COUNT_mRETWEET6_mapkey0_idxfn::hash(se58.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_0* i35 = static_cast<HashIndex_COUNT_mRETWEET6_map_0*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_0::IdxNode* n35 = &(i35->buckets_[h35 % i35->size_]);
          COUNT_mRETWEET6_entry* e42;
          do if ((e42=n35->obj) && h35 == n35->hash && COUNT_mRETWEET6_mapkey0_idxfn::equals(se58, *e42)) {
            long t_user_id = e42->T_USER_ID;
            long v49 = e42->__av;
            COUNT_mRETWEET14.addOrDelOnZero(se57.modify(retweet_retweet_tweet_id,retweet_tweet_id,retweet_retweet_tweet_id,t_user_id,retweet_tweet_date),(v49 * -1L));
          } while ((n35=n35->nxt));
        }
        _c8.clear();
        { //slice
          const HASH_RES_t h36 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se60.modify0(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i36 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n36 = &(i36->buckets_[h36 % i36->size_]);
          COUNT_mRETWEET7_entry* e43;
          do if ((e43=n36->obj) && h36 == n36->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se60, *e43)) {
            long d_follower_id = e43->D_FOLLOWER_ID;
            long f_user_id = e43->F_USER_ID;
            long r2_tweet_id = e43->R2_TWEET_ID;
            long r2_user_id = e43->R2_USER_ID;
            long v51 = e43->__av;
            add_to_temp_map(_c8, st15.modify(r2_tweet_id,r2_user_id,f_user_id,retweet_tweet_id,retweet_tweet_id,d_follower_id,(v51 * -1L)));
          } while ((n36=n36->nxt));
        }
        { //slice
          const HASH_RES_t h37 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se61.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET15_map_0* i37 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
          HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n37 = &(i37->buckets_[h37 % i37->size_]);
          COUNT_mRETWEET15_entry* e44;
          do if ((e44=n37->obj) && h37 == n37->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se61, *e44)) {
            long d_follower_id = e44->D_FOLLOWER_ID;
            long v52 = e44->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c8, st16.modify(retweet_tweet_id,retweet_user_id,retweet_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,d_follower_id,v52)) : voidFunc());
          } while ((n37=n37->nxt));
        }
        { //temp foreach
          tuple7_LLLLLL_L* e45 = _c8.head;
          while(e45){
            long r2_tweet_id = e45->_1;
            long r2_user_id = e45->_2;
            long f_user_id = e45->_3;
            long r2_retweet_tweet_id = e45->_4;
            long r1_tweet_id = e45->_5;
            long d_follower_id = e45->_6;
            long v53 = e45->__av;
            COUNT_mTWEET1.addOrDelOnZero(se59.modify(retweet_retweet_tweet_id,d_follower_id,r1_tweet_id,retweet_tweet_date,r2_retweet_tweet_id,r2_user_id,r2_tweet_id,f_user_id),v53);
            e45 = e45->nxt;
          }
        }
        { //slice
          const HASH_RES_t h39 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::hash(se63.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0* i38 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[1]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_0::IdxNode* n38 = &(i38->buckets_[h39 % i38->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e46;
          do if ((e46=n38->obj) && h39 == n38->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey0_idxfn::equals(se63, *e46)) {
            long count_mtweettweet_tweet_id = e46->COUNT_mFOLLOWER1_mTWEETTWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e46->R1_TWEET_DATE;
            long v54 = e46->__av;
            { //slice
              const HASH_RES_t h38 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se62.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i39 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n39 = &(i39->buckets_[h38 % i39->size_]);
              COUNT_mRETWEET15_entry* e47;
              do if ((e47=n39->obj) && h38 == n39->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se62, *e47)) {
                long d_follower_id = e47->D_FOLLOWER_ID;
                long v55 = e47->__av;
                COUNT_mTWEET1.addOrDelOnZero(se59.modify(count_mtweettweet_tweet_id,d_follower_id,retweet_retweet_tweet_id,r1_tweet_date,retweet_retweet_tweet_id,retweet_user_id,retweet_tweet_id,retweet_user_id),(v54 * (v55 * -1L)));
              } while ((n39=n39->nxt));
            }
          } while ((n38=n38->nxt));
        }
        { //slice
          const HASH_RES_t h40 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se65.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET15_map_0* i40 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
          HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n40 = &(i40->buckets_[h40 % i40->size_]);
          COUNT_mRETWEET15_entry* e48;
          do if ((e48=n40->obj) && h40 == n40->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se65, *e48)) {
            long d_follower_id = e48->D_FOLLOWER_ID;
            long v56 = e48->__av;
            COUNT_mRETWEET7.addOrDelOnZero(se64.modify(retweet_retweet_tweet_id,d_follower_id,retweet_user_id,retweet_tweet_id,retweet_user_id),(v56 * -1L));
          } while ((n40=n40->nxt));
        }
        COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.addOrDelOnZero(se66.modify(retweet_tweet_id,retweet_retweet_tweet_id,retweet_tweet_date),-1L);
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
        { //slice
          const HASH_RES_t h41 = COUNT_mTWEET1_mapkey0_idxfn::hash(se68.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET1_map_0* i41 = static_cast<HashIndex_COUNT_mTWEET1_map_0*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_0::IdxNode* n41 = &(i41->buckets_[h41 % i41->size_]);
          COUNT_mTWEET1_entry* e49;
          do if ((e49=n41->obj) && h41 == n41->hash && COUNT_mTWEET1_mapkey0_idxfn::equals(se68, *e49)) {
            long d_follower_id = e49->D_FOLLOWER_ID;
            long r1_tweet_id = e49->R1_TWEET_ID;
            STRING_TYPE r1_tweet_date = e49->R1_TWEET_DATE;
            long r2_retweet_tweet_id = e49->R2_RETWEET_TWEET_ID;
            long r2_user_id = e49->R2_USER_ID;
            long r2_tweet_id = e49->R2_TWEET_ID;
            long f_user_id = e49->F_USER_ID;
            long v57 = e49->__av;
            COUNT.addOrDelOnZero(se67.modify(tweet_user_id,tweet_tweet_id,r1_tweet_id,tweet_tweet_id,r1_tweet_date,r2_retweet_tweet_id,r2_user_id,r2_tweet_id,f_user_id,d_follower_id),v57);
          } while ((n41=n41->nxt));
        }
        { //slice
          const HASH_RES_t h42 = COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn::hash(se70.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0* i42 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0*>(COUNT_mFOLLOWER1_mTWEET1.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0::IdxNode* n42 = &(i42->buckets_[h42 % i42->size_]);
          COUNT_mFOLLOWER1_mTWEET1_entry* e50;
          do if ((e50=n42->obj) && h42 == n42->hash && COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn::equals(se70, *e50)) {
            long count_mfollowerfollower_user_id = e50->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long r2_retweet_tweet_id = e50->R2_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e50->R1_TWEET_DATE;
            long r1_tweet_id = e50->R1_TWEET_ID;
            long r2_tweet_id = e50->R2_TWEET_ID;
            long v58 = e50->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se69.modify(tweet_tweet_id,count_mfollowerfollower_user_id,tweet_tweet_id,tweet_user_id,r2_retweet_tweet_id,r1_tweet_date,r1_tweet_id,r2_tweet_id),v58);
          } while ((n42=n42->nxt));
        }
        { //slice
          const HASH_RES_t h43 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::hash(se72.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1* i43 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1::IdxNode* n43 = &(i43->buckets_[h43 % i43->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e51;
          do if ((e51=n43->obj) && h43 == n43->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::equals(se72, *e51)) {
            long count_pretweetretweet_retweet_tweet_id = e51->COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e51->R1_TWEET_DATE;
            long v59 = e51->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se71.modify(tweet_tweet_id,count_pretweetretweet_retweet_tweet_id,tweet_user_id,tweet_tweet_id,r1_tweet_date),v59);
          } while ((n43=n43->nxt));
        }
        COUNT_mRETWEET6.addOrDelOnZero(se73.modify(tweet_tweet_id,tweet_user_id),1L);
        { //slice
          const HASH_RES_t h44 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::hash(se75.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1* i44 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1::IdxNode* n44 = &(i44->buckets_[h44 % i44->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e52;
          do if ((e52=n44->obj) && h44 == n44->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::equals(se75, *e52)) {
            long count_mretweetretweet_retweet_tweet_id = e52->COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e52->R1_TWEET_DATE;
            long v60 = e52->__av;
            COUNT_mRETWEET14.addOrDelOnZero(se74.modify(tweet_tweet_id,count_mretweetretweet_retweet_tweet_id,tweet_tweet_id,tweet_user_id,r1_tweet_date),v60);
          } while ((n44=n44->nxt));
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
        { //slice
          const HASH_RES_t h45 = COUNT_mTWEET1_mapkey0_idxfn::hash(se77.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET1_map_0* i45 = static_cast<HashIndex_COUNT_mTWEET1_map_0*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_0::IdxNode* n45 = &(i45->buckets_[h45 % i45->size_]);
          COUNT_mTWEET1_entry* e53;
          do if ((e53=n45->obj) && h45 == n45->hash && COUNT_mTWEET1_mapkey0_idxfn::equals(se77, *e53)) {
            long d_follower_id = e53->D_FOLLOWER_ID;
            long r1_tweet_id = e53->R1_TWEET_ID;
            STRING_TYPE r1_tweet_date = e53->R1_TWEET_DATE;
            long r2_retweet_tweet_id = e53->R2_RETWEET_TWEET_ID;
            long r2_user_id = e53->R2_USER_ID;
            long r2_tweet_id = e53->R2_TWEET_ID;
            long f_user_id = e53->F_USER_ID;
            long v61 = e53->__av;
            COUNT.addOrDelOnZero(se76.modify(tweet_user_id,tweet_tweet_id,r1_tweet_id,tweet_tweet_id,r1_tweet_date,r2_retweet_tweet_id,r2_user_id,r2_tweet_id,f_user_id,d_follower_id),(v61 * -1L));
          } while ((n45=n45->nxt));
        }
        { //slice
          const HASH_RES_t h46 = COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn::hash(se79.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0* i46 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0*>(COUNT_mFOLLOWER1_mTWEET1.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_map_0::IdxNode* n46 = &(i46->buckets_[h46 % i46->size_]);
          COUNT_mFOLLOWER1_mTWEET1_entry* e54;
          do if ((e54=n46->obj) && h46 == n46->hash && COUNT_mFOLLOWER1_mTWEET1_mapkey0_idxfn::equals(se79, *e54)) {
            long count_mfollowerfollower_user_id = e54->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long r2_retweet_tweet_id = e54->R2_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e54->R1_TWEET_DATE;
            long r1_tweet_id = e54->R1_TWEET_ID;
            long r2_tweet_id = e54->R2_TWEET_ID;
            long v62 = e54->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se78.modify(tweet_tweet_id,count_mfollowerfollower_user_id,tweet_tweet_id,tweet_user_id,r2_retweet_tweet_id,r1_tweet_date,r1_tweet_id,r2_tweet_id),(v62 * -1L));
          } while ((n46=n46->nxt));
        }
        { //slice
          const HASH_RES_t h47 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::hash(se81.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1* i47 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1::IdxNode* n47 = &(i47->buckets_[h47 % i47->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e55;
          do if ((e55=n47->obj) && h47 == n47->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::equals(se81, *e55)) {
            long count_pretweetretweet_retweet_tweet_id = e55->COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e55->R1_TWEET_DATE;
            long v63 = e55->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se80.modify(tweet_tweet_id,count_pretweetretweet_retweet_tweet_id,tweet_user_id,tweet_tweet_id,r1_tweet_date),(v63 * -1L));
          } while ((n47=n47->nxt));
        }
        COUNT_mRETWEET6.addOrDelOnZero(se82.modify(tweet_tweet_id,tweet_user_id),-1L);
        { //slice
          const HASH_RES_t h48 = COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::hash(se84.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1* i48 = static_cast<HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1*>(COUNT_mFOLLOWER1_mTWEET1_mRETWEET11.index[2]);
          HashIndex_COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map_1::IdxNode* n48 = &(i48->buckets_[h48 % i48->size_]);
          COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry* e56;
          do if ((e56=n48->obj) && h48 == n48->hash && COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_mapkey1_idxfn::equals(se84, *e56)) {
            long count_mretweetretweet_retweet_tweet_id = e56->COUNT_mFOLLOWER1_mTWEET1_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE r1_tweet_date = e56->R1_TWEET_DATE;
            long v64 = e56->__av;
            COUNT_mRETWEET14.addOrDelOnZero(se83.modify(tweet_tweet_id,count_mretweetretweet_retweet_tweet_id,tweet_tweet_id,tweet_user_id,r1_tweet_date),(v64 * -1L));
          } while ((n48=n48->nxt));
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
    COUNT_entry se1;
    COUNT_mFOLLOWER1_entry se2;
    COUNT_mRETWEET7_entry se3;
    COUNT_mFOLLOWER1_mRETWEET7_entry se4;
    COUNT_mRETWEET15_entry se5;
    COUNT_mTWEET1_entry se6;
    COUNT_mFOLLOWER1_mTWEET1_entry se7;
    COUNT_entry se8;
    COUNT_mFOLLOWER1_entry se9;
    COUNT_mRETWEET7_entry se10;
    COUNT_mFOLLOWER1_mRETWEET7_entry se11;
    COUNT_mRETWEET15_entry se12;
    COUNT_mTWEET1_entry se13;
    COUNT_mFOLLOWER1_mTWEET1_entry se14;
    COUNT_entry se15;
    tuple7_LLLLLL_L st1;
    COUNT_mRETWEET7_entry se16;
    tuple7_LLLLLL_L st2;
    COUNT_mRETWEET15_entry se17;
    COUNT_mRETWEET6_entry se18;
    COUNT_mRETWEET15_entry se19;
    COUNT_pRETWEET13_entry se20;
    COUNT_mFOLLOWER1_entry se21;
    tuple5_LLLL_L st3;
    COUNT_mFOLLOWER1_mRETWEET7_entry se22;
    tuple5_LLLL_L st4;
    COUNT_mRETWEET6_entry se23;
    COUNT_mRETWEET14_entry se24;
    COUNT_mFOLLOWER1_mTWEET1_entry se25;
    tuple5_LLLL_L st5;
    COUNT_mFOLLOWER1_mRETWEET7_entry se26;
    tuple5_LLLL_L st6;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se27;
    COUNT_mFOLLOWER1_mRETWEET7_entry se28;
    COUNT_pRETWEET13_entry se29;
    COUNT_mRETWEET6_entry se30;
    COUNT_mRETWEET14_entry se31;
    COUNT_mRETWEET6_entry se32;
    COUNT_mTWEET1_entry se33;
    tuple7_LLLLLL_L st7;
    COUNT_mRETWEET7_entry se34;
    tuple7_LLLLLL_L st8;
    COUNT_mRETWEET15_entry se35;
    COUNT_mRETWEET15_entry se36;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se37;
    COUNT_mRETWEET7_entry se38;
    COUNT_mRETWEET15_entry se39;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se40;
    COUNT_entry se41;
    tuple7_LLLLLL_L st9;
    COUNT_mRETWEET7_entry se42;
    tuple7_LLLLLL_L st10;
    COUNT_mRETWEET15_entry se43;
    COUNT_mRETWEET6_entry se44;
    COUNT_mRETWEET15_entry se45;
    COUNT_mRETWEET14_entry se46;
    COUNT_mFOLLOWER1_entry se47;
    tuple5_LLLL_L st11;
    COUNT_mFOLLOWER1_mRETWEET7_entry se48;
    tuple5_LLLL_L st12;
    COUNT_mRETWEET6_entry se49;
    COUNT_pRETWEET13_entry se50;
    COUNT_mFOLLOWER1_mTWEET1_entry se51;
    tuple5_LLLL_L st13;
    COUNT_mFOLLOWER1_mRETWEET7_entry se52;
    tuple5_LLLL_L st14;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se53;
    COUNT_mFOLLOWER1_mRETWEET7_entry se54;
    COUNT_pRETWEET13_entry se55;
    COUNT_mRETWEET6_entry se56;
    COUNT_mRETWEET14_entry se57;
    COUNT_mRETWEET6_entry se58;
    COUNT_mTWEET1_entry se59;
    tuple7_LLLLLL_L st15;
    COUNT_mRETWEET7_entry se60;
    tuple7_LLLLLL_L st16;
    COUNT_mRETWEET15_entry se61;
    COUNT_mRETWEET15_entry se62;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se63;
    COUNT_mRETWEET7_entry se64;
    COUNT_mRETWEET15_entry se65;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se66;
    COUNT_entry se67;
    COUNT_mTWEET1_entry se68;
    COUNT_mFOLLOWER1_entry se69;
    COUNT_mFOLLOWER1_mTWEET1_entry se70;
    COUNT_pRETWEET13_entry se71;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se72;
    COUNT_mRETWEET6_entry se73;
    COUNT_mRETWEET14_entry se74;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se75;
    COUNT_entry se76;
    COUNT_mTWEET1_entry se77;
    COUNT_mFOLLOWER1_entry se78;
    COUNT_mFOLLOWER1_mTWEET1_entry se79;
    COUNT_pRETWEET13_entry se80;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se81;
    COUNT_mRETWEET6_entry se82;
    COUNT_mRETWEET14_entry se83;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_entry se84;
  
    /* Data structures used for storing materialized views */
    COUNT_mFOLLOWER1_map COUNT_mFOLLOWER1;
    COUNT_mFOLLOWER1_mRETWEET7_map COUNT_mFOLLOWER1_mRETWEET7;
    COUNT_mFOLLOWER1_mTWEET1_map COUNT_mFOLLOWER1_mTWEET1;
    COUNT_mFOLLOWER1_mTWEET1_mRETWEET11_map COUNT_mFOLLOWER1_mTWEET1_mRETWEET11;
    COUNT_pRETWEET13_map COUNT_pRETWEET13;
    COUNT_mRETWEET6_map COUNT_mRETWEET6;
    COUNT_mRETWEET7_map COUNT_mRETWEET7;
    COUNT_mRETWEET14_map COUNT_mRETWEET14;
    COUNT_mRETWEET15_map COUNT_mRETWEET15;
    COUNT_mTWEET1_map COUNT_mTWEET1;
    MultiHashMap<tuple7_LLLLLL_L,long,HashIndex<tuple7_LLLLLL_L,long> > _c1;
    MultiHashMap<tuple7_LLLLLL_L,long,HashIndex<tuple7_LLLLLL_L,long> > _c4;
    MultiHashMap<tuple5_LLLL_L,long,HashIndex<tuple5_LLLL_L,long> > _c7;
    MultiHashMap<tuple5_LLLL_L,long,HashIndex<tuple5_LLLL_L,long> > _c6;
    MultiHashMap<tuple5_LLLL_L,long,HashIndex<tuple5_LLLL_L,long> > _c3;
    MultiHashMap<tuple5_LLLL_L,long,HashIndex<tuple5_LLLL_L,long> > _c2;
    MultiHashMap<tuple7_LLLLLL_L,long,HashIndex<tuple7_LLLLLL_L,long> > _c5;
    MultiHashMap<tuple7_LLLLLL_L,long,HashIndex<tuple7_LLLLLL_L,long> > _c8;
    
  
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
  class Q4 : public Program
  {
    public:
      Q4(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

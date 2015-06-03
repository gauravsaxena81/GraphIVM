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
    long TWEET_ID_1; long RETWEET_TWEET_ID_1; long Z_USER_ID; long Z_FOLLOWER_ID; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*TWEET_ID_1 = 0L; RETWEET_TWEET_ID_1 = 0L; Z_USER_ID = 0L; Z_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2, const long c3, const long c4) { TWEET_ID_1 = c0; RETWEET_TWEET_ID_1 = c1; Z_USER_ID = c2; Z_FOLLOWER_ID = c3; __av = c4; }
    COUNT_entry(const COUNT_entry& other) : TWEET_ID_1( other.TWEET_ID_1 ), RETWEET_TWEET_ID_1( other.RETWEET_TWEET_ID_1 ), Z_USER_ID( other.Z_USER_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1, const long c2, const long c3) { TWEET_ID_1 = c0; RETWEET_TWEET_ID_1 = c1; Z_USER_ID = c2; Z_FOLLOWER_ID = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, RETWEET_TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.TWEET_ID_1);
      hash_combine(h, e.RETWEET_TWEET_ID_1);
      hash_combine(h, e.Z_USER_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.TWEET_ID_1 == y.TWEET_ID_1 && x.RETWEET_TWEET_ID_1 == y.RETWEET_TWEET_ID_1 && x.Z_USER_ID == y.Z_USER_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey0123_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey0123_idxfn,true> HashIndex_COUNT_map_0123;
  
  struct COUNT_pRETWEET13_entry {
    long COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID; long TWEET_ID_1; long RETWEET_TWEET_ID_1; long __av;  COUNT_pRETWEET13_entry* nxt; COUNT_pRETWEET13_entry* prv;
    explicit COUNT_pRETWEET13_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = 0L; TWEET_ID_1 = 0L; RETWEET_TWEET_ID_1 = 0L; __av = 0L; */ }
    explicit COUNT_pRETWEET13_entry(const long c0, const long c1, const long c2, const long c3) { COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = c0; TWEET_ID_1 = c1; RETWEET_TWEET_ID_1 = c2; __av = c3; }
    COUNT_pRETWEET13_entry(const COUNT_pRETWEET13_entry& other) : COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID ), TWEET_ID_1( other.TWEET_ID_1 ), RETWEET_TWEET_ID_1( other.RETWEET_TWEET_ID_1 ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_pRETWEET13_entry& modify(const long c0, const long c1, const long c2) { COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = c0; TWEET_ID_1 = c1; RETWEET_TWEET_ID_1 = c2;  return *this; }
    FORCE_INLINE COUNT_pRETWEET13_entry& modify0(const long c0) { COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, RETWEET_TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_pRETWEET13_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_pRETWEET13_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.TWEET_ID_1);
      hash_combine(h, e.RETWEET_TWEET_ID_1);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_pRETWEET13_entry& x, const COUNT_pRETWEET13_entry& y) {
      return x.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_pRETWEETRETWEET_RETWEET_TWEET_ID && x.TWEET_ID_1 == y.TWEET_ID_1 && x.RETWEET_TWEET_ID_1 == y.RETWEET_TWEET_ID_1;
    }
  };
  
  struct COUNT_pRETWEET13_mapkey0_idxfn {
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
    HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey012_idxfn,true>,
    HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey0_idxfn,false>
  > COUNT_pRETWEET13_map;
  typedef HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey012_idxfn,true> HashIndex_COUNT_pRETWEET13_map_012;
  typedef HashIndex<COUNT_pRETWEET13_entry,long,COUNT_pRETWEET13_mapkey0_idxfn,false> HashIndex_COUNT_pRETWEET13_map_0;
  
  struct COUNT_mRETWEET3_entry {
    long Z_USER_ID; long Z_FOLLOWER_ID; long COUNT_mRETWEETRETWEET_TWEET_ID; long __av;  COUNT_mRETWEET3_entry* nxt; COUNT_mRETWEET3_entry* prv;
    explicit COUNT_mRETWEET3_entry() : nxt(nullptr), prv(nullptr) { /*Z_USER_ID = 0L; Z_FOLLOWER_ID = 0L; COUNT_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET3_entry(const long c0, const long c1, const long c2, const long c3) { Z_USER_ID = c0; Z_FOLLOWER_ID = c1; COUNT_mRETWEETRETWEET_TWEET_ID = c2; __av = c3; }
    COUNT_mRETWEET3_entry(const COUNT_mRETWEET3_entry& other) : Z_USER_ID( other.Z_USER_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET3_entry& modify(const long c0, const long c1, const long c2) { Z_USER_ID = c0; Z_FOLLOWER_ID = c1; COUNT_mRETWEETRETWEET_TWEET_ID = c2;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_entry& modify2(const long c2) { COUNT_mRETWEETRETWEET_TWEET_ID = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET3_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.Z_USER_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_entry& x, const COUNT_mRETWEET3_entry& y) {
      return x.Z_USER_ID == y.Z_USER_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID && x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mapkey2_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_entry& x, const COUNT_mRETWEET3_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET3_entry,long,
    HashIndex<COUNT_mRETWEET3_entry,long,COUNT_mRETWEET3_mapkey012_idxfn,true>,
    HashIndex<COUNT_mRETWEET3_entry,long,COUNT_mRETWEET3_mapkey2_idxfn,false>
  > COUNT_mRETWEET3_map;
  typedef HashIndex<COUNT_mRETWEET3_entry,long,COUNT_mRETWEET3_mapkey012_idxfn,true> HashIndex_COUNT_mRETWEET3_map_012;
  typedef HashIndex<COUNT_mRETWEET3_entry,long,COUNT_mRETWEET3_mapkey2_idxfn,false> HashIndex_COUNT_mRETWEET3_map_2;
  
  struct COUNT_mRETWEET3_mFOLLOWER1_entry {
    long COUNT_mRETWEETRETWEET_TWEET_ID; long COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID; long __av;  COUNT_mRETWEET3_mFOLLOWER1_entry* nxt; COUNT_mRETWEET3_mFOLLOWER1_entry* prv;
    explicit COUNT_mRETWEET3_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_TWEET_ID = 0L; COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET3_mFOLLOWER1_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_TWEET_ID = c0; COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c1; __av = c2; }
    COUNT_mRETWEET3_mFOLLOWER1_entry(const COUNT_mRETWEET3_mFOLLOWER1_entry& other) : COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_TWEET_ID = c0; COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_entry& modify1(const long c1) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET3_mFOLLOWER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID && x.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_entry& y) {
      return x.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET3_mFOLLOWER1_entry,long,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn,false>,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn,false>
  > COUNT_mRETWEET3_mFOLLOWER1_map;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_01;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn,false> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0;
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry {
    long COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID; long COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID; long __av;  COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* nxt; COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* prv;
    explicit COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = 0L; COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1; __av = c2; }
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& other) : COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID ), COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& modify(const long c0, const long c1) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& modify0(const long c0) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& modify1(const long c1) { COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& y) {
      return x.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID && x.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& y) {
      return x.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry& y) {
      return x.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn,false>,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn,false>
  > COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_01;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn,false> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1;
  
  struct COUNT_mRETWEET6_entry {
    long Z_USER_ID; long Z_FOLLOWER_ID; long COUNT_mRETWEETRETWEET_TWEET_ID; long __av;  COUNT_mRETWEET6_entry* nxt; COUNT_mRETWEET6_entry* prv;
    explicit COUNT_mRETWEET6_entry() : nxt(nullptr), prv(nullptr) { /*Z_USER_ID = 0L; Z_FOLLOWER_ID = 0L; COUNT_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET6_entry(const long c0, const long c1, const long c2, const long c3) { Z_USER_ID = c0; Z_FOLLOWER_ID = c1; COUNT_mRETWEETRETWEET_TWEET_ID = c2; __av = c3; }
    COUNT_mRETWEET6_entry(const COUNT_mRETWEET6_entry& other) : Z_USER_ID( other.Z_USER_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET6_entry& modify(const long c0, const long c1, const long c2) { Z_USER_ID = c0; Z_FOLLOWER_ID = c1; COUNT_mRETWEETRETWEET_TWEET_ID = c2;  return *this; }
    FORCE_INLINE COUNT_mRETWEET6_entry& modify2(const long c2) { COUNT_mRETWEETRETWEET_TWEET_ID = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET6_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.Z_USER_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET6_entry& x, const COUNT_mRETWEET6_entry& y) {
      return x.Z_USER_ID == y.Z_USER_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID && x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET6_mapkey2_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET6_entry& x, const COUNT_mRETWEET6_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET6_entry,long,
    HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey012_idxfn,true>,
    HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey2_idxfn,false>
  > COUNT_mRETWEET6_map;
  typedef HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey012_idxfn,true> HashIndex_COUNT_mRETWEET6_map_012;
  typedef HashIndex<COUNT_mRETWEET6_entry,long,COUNT_mRETWEET6_mapkey2_idxfn,false> HashIndex_COUNT_mRETWEET6_map_2;
  
  struct COUNT_mRETWEET7_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long RETWEET_TWEET_ID_1; long __av;  COUNT_mRETWEET7_entry* nxt; COUNT_mRETWEET7_entry* prv;
    explicit COUNT_mRETWEET7_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; RETWEET_TWEET_ID_1 = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET7_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; RETWEET_TWEET_ID_1 = c1; __av = c2; }
    COUNT_mRETWEET7_entry(const COUNT_mRETWEET7_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), RETWEET_TWEET_ID_1( other.RETWEET_TWEET_ID_1 ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET7_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; RETWEET_TWEET_ID_1 = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET7_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, RETWEET_TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET7_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.RETWEET_TWEET_ID_1);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET7_entry& x, const COUNT_mRETWEET7_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.RETWEET_TWEET_ID_1 == y.RETWEET_TWEET_ID_1;
    }
  };
  
  struct COUNT_mRETWEET7_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET7_entry& x, const COUNT_mRETWEET7_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET7_entry,long,
    HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey0_idxfn,false>
  > COUNT_mRETWEET7_map;
  typedef HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET7_map_01;
  typedef HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET7_map_0;
  
  struct COUNT_mRETWEET14_entry {
    long COUNT_mRETWEETRETWEET_USER_ID; long Z_FOLLOWER_ID; long __av;  COUNT_mRETWEET14_entry* nxt; COUNT_mRETWEET14_entry* prv;
    explicit COUNT_mRETWEET14_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_USER_ID = 0L; Z_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET14_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_USER_ID = c0; Z_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mRETWEET14_entry(const COUNT_mRETWEET14_entry& other) : COUNT_mRETWEETRETWEET_USER_ID( other.COUNT_mRETWEETRETWEET_USER_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET14_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_USER_ID = c0; Z_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET14_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_USER_ID = c0;  return *this; }
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
  struct COUNT_mRETWEET14_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET14_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET14_entry& x, const COUNT_mRETWEET14_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID;
    }
  };
  
  struct COUNT_mRETWEET14_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET14_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET14_entry& x, const COUNT_mRETWEET14_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET14_entry,long,
    HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey0_idxfn,false>
  > COUNT_mRETWEET14_map;
  typedef HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET14_map_01;
  typedef HashIndex<COUNT_mRETWEET14_entry,long,COUNT_mRETWEET14_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET14_map_0;
  
  struct COUNT_mRETWEET15_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long RETWEET_TWEET_ID_1; long TWEET_ID_1; long __av;  COUNT_mRETWEET15_entry* nxt; COUNT_mRETWEET15_entry* prv;
    explicit COUNT_mRETWEET15_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; RETWEET_TWEET_ID_1 = 0L; TWEET_ID_1 = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET15_entry(const long c0, const long c1, const long c2, const long c3) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; RETWEET_TWEET_ID_1 = c1; TWEET_ID_1 = c2; __av = c3; }
    COUNT_mRETWEET15_entry(const COUNT_mRETWEET15_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), RETWEET_TWEET_ID_1( other.RETWEET_TWEET_ID_1 ), TWEET_ID_1( other.TWEET_ID_1 ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET15_entry& modify(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; RETWEET_TWEET_ID_1 = c1; TWEET_ID_1 = c2;  return *this; }
    FORCE_INLINE COUNT_mRETWEET15_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, RETWEET_TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET15_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET15_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.RETWEET_TWEET_ID_1);
      hash_combine(h, e.TWEET_ID_1);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET15_entry& x, const COUNT_mRETWEET15_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.RETWEET_TWEET_ID_1 == y.RETWEET_TWEET_ID_1 && x.TWEET_ID_1 == y.TWEET_ID_1;
    }
  };
  
  struct COUNT_mRETWEET15_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET15_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET15_entry& x, const COUNT_mRETWEET15_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET15_entry,long,
    HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey012_idxfn,true>,
    HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey0_idxfn,false>
  > COUNT_mRETWEET15_map;
  typedef HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey012_idxfn,true> HashIndex_COUNT_mRETWEET15_map_012;
  typedef HashIndex<COUNT_mRETWEET15_entry,long,COUNT_mRETWEET15_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET15_map_0;
  
  struct COUNT_mRETWEET21_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long COUNT_mRETWEETRETWEET_TWEET_ID; long __av;  COUNT_mRETWEET21_entry* nxt; COUNT_mRETWEET21_entry* prv;
    explicit COUNT_mRETWEET21_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; COUNT_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET21_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; COUNT_mRETWEETRETWEET_TWEET_ID = c1; __av = c2; }
    COUNT_mRETWEET21_entry(const COUNT_mRETWEET21_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET21_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; COUNT_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET21_entry& modify1(const long c1) { COUNT_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET21_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET21_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET21_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET21_entry& x, const COUNT_mRETWEET21_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET21_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET21_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET21_entry& x, const COUNT_mRETWEET21_entry& y) {
      return x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET21_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET21_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET21_entry& x, const COUNT_mRETWEET21_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET21_entry,long,
    HashIndex<COUNT_mRETWEET21_entry,long,COUNT_mRETWEET21_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET21_entry,long,COUNT_mRETWEET21_mapkey1_idxfn,false>,
    HashIndex<COUNT_mRETWEET21_entry,long,COUNT_mRETWEET21_mapkey0_idxfn,false>
  > COUNT_mRETWEET21_map;
  typedef HashIndex<COUNT_mRETWEET21_entry,long,COUNT_mRETWEET21_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET21_map_01;
  typedef HashIndex<COUNT_mRETWEET21_entry,long,COUNT_mRETWEET21_mapkey1_idxfn,false> HashIndex_COUNT_mRETWEET21_map_1;
  typedef HashIndex<COUNT_mRETWEET21_entry,long,COUNT_mRETWEET21_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET21_map_0;
  
  struct COUNT_mFOLLOWER1_entry {
    long COUNT_mFOLLOWERFOLLOWER_USER_ID; long TWEET_ID_1; long RETWEET_TWEET_ID_1; long __av;  COUNT_mFOLLOWER1_entry* nxt; COUNT_mFOLLOWER1_entry* prv;
    explicit COUNT_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; TWEET_ID_1 = 0L; RETWEET_TWEET_ID_1 = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER1_entry(const long c0, const long c1, const long c2, const long c3) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; TWEET_ID_1 = c1; RETWEET_TWEET_ID_1 = c2; __av = c3; }
    COUNT_mFOLLOWER1_entry(const COUNT_mFOLLOWER1_entry& other) : COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), TWEET_ID_1( other.TWEET_ID_1 ), RETWEET_TWEET_ID_1( other.RETWEET_TWEET_ID_1 ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0; TWEET_ID_1 = c1; RETWEET_TWEET_ID_1 = c2;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER1_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, RETWEET_TWEET_ID_1);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER1_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.TWEET_ID_1);
      hash_combine(h, e.RETWEET_TWEET_ID_1);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER1_entry& x, const COUNT_mFOLLOWER1_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID && x.TWEET_ID_1 == y.TWEET_ID_1 && x.RETWEET_TWEET_ID_1 == y.RETWEET_TWEET_ID_1;
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
  
  struct tuple3_LL_L {
    long _1; long _2; long __av; tuple3_LL_L* nxt; tuple3_LL_L* prv;
    explicit tuple3_LL_L() : nxt(nullptr), prv(nullptr) { }
    explicit tuple3_LL_L(const long c1, const long c2, long c__av=0L) : nxt(nullptr), prv(nullptr) { _1 = c1; _2 = c2; __av = c__av;}
    int operator<(const tuple3_LL_L &rhs) const { 
      if(this->_1!=rhs._1) return (this->_1<rhs._1);
      if(this->_2!=rhs._2) return (this->_2<rhs._2);
      return 0;
    }
    int operator==(const tuple3_LL_L &rhs) const { return ((this->_1==rhs._1) && (this->_2==rhs._2)); }
    FORCE_INLINE tuple3_LL_L& modify(const long c0, const long c1, long c__av) { _1 = c0; _2 = c1; __av = c__av; return *this; }
    static bool equals(const tuple3_LL_L &x, const tuple3_LL_L &y) { return ((x._1==y._1) && (x._2==y._2)); }
    static long hash(const tuple3_LL_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      hash_combine(h, e._2);
      return h;
    }
  };
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
    data_t(): tlq_t(), _c16(16U), _c1(16U), _c4(16U), _c10(16U), _c13(16U), _c7(16U), _c6(16U), _c9(16U), _c15(16U), _c3(16U), _c12(16U), _c2(16U), _c5(16U), _c11(16U), _c14(16U), _c8(16U) {
      
    }
  
    #ifdef DBT_PROFILE
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> exec_stats;
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> ivc_stats;
    #endif
  
    /* Registering relations and trigger functions */
    void register_data(ProgramBase& pb) {
  
      pb.add_map<COUNT_map>( "COUNT", COUNT );
      pb.add_map<COUNT_pRETWEET13_map>( "COUNT_pRETWEET13", COUNT_pRETWEET13 );
      pb.add_map<COUNT_mRETWEET3_map>( "COUNT_mRETWEET3", COUNT_mRETWEET3 );
      pb.add_map<COUNT_mRETWEET3_mFOLLOWER1_map>( "COUNT_mRETWEET3_mFOLLOWER1", COUNT_mRETWEET3_mFOLLOWER1 );
      pb.add_map<COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map>( "COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2", COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2 );
      pb.add_map<COUNT_mRETWEET6_map>( "COUNT_mRETWEET6", COUNT_mRETWEET6 );
      pb.add_map<COUNT_mRETWEET7_map>( "COUNT_mRETWEET7", COUNT_mRETWEET7 );
      pb.add_map<COUNT_mRETWEET14_map>( "COUNT_mRETWEET14", COUNT_mRETWEET14 );
      pb.add_map<COUNT_mRETWEET15_map>( "COUNT_mRETWEET15", COUNT_mRETWEET15 );
      pb.add_map<COUNT_mRETWEET21_map>( "COUNT_mRETWEET21", COUNT_mRETWEET21 );
      pb.add_map<COUNT_mFOLLOWER1_map>( "COUNT_mFOLLOWER1", COUNT_mFOLLOWER1 );
  
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
      exec_stats->register_probe(17, "insert_RETWEET_s9");
      exec_stats->register_probe(18, "delete_RETWEET_s0");
      exec_stats->register_probe(19, "delete_RETWEET_s1");
      exec_stats->register_probe(20, "delete_RETWEET_s2");
      exec_stats->register_probe(21, "delete_RETWEET_s3");
      exec_stats->register_probe(22, "delete_RETWEET_s4");
      exec_stats->register_probe(23, "delete_RETWEET_s5");
      exec_stats->register_probe(24, "delete_RETWEET_s6");
      exec_stats->register_probe(25, "delete_RETWEET_s7");
      exec_stats->register_probe(26, "delete_RETWEET_s8");
      exec_stats->register_probe(27, "delete_RETWEET_s9");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_FOLLOWER( long follower_user_id,  long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"insert_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"insert_FOLLOWER")
//	std::vector<void*> r;
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = COUNT_mFOLLOWER1_mapkey0_idxfn::hash(se2.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_0* i1 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_0*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mFOLLOWER1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mFOLLOWER1_mapkey0_idxfn::equals(se2, *e1)) {
            long tweet_id_1 = e1->TWEET_ID_1;
            long retweet_tweet_id_1 = e1->RETWEET_TWEET_ID_1;
            long v1 = e1->__av;
		for(int i = 0; i < v1; i++) {
			api.useDiff(retweet_tweet_id_1, tweet_id_1, follower_user_id, follower_follower_id);
		//	void* rr[4];
		//	r.push_back(rr);
		//	rr[0] = &retweet_tweet_id_1;
		//	rr[1] = &tweet_id_1;
		//	rr[2] = &follower_user_id;
		//	rr[3] = &follower_follower_id;
            	}
	    //COUNT.addOrDelOnZero(se1.modify(tweet_id_1,retweet_tweet_id_1,follower_user_id,follower_follower_id),v1);
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::hash(se4.modify1(follower_user_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1* i2 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1*>(COUNT_mRETWEET3_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::equals(se4, *e2)) {
            long count_mretweetretweet_tweet_id = e2->COUNT_mRETWEETRETWEET_TWEET_ID;
            long v2 = e2->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se3.modify(follower_user_id,follower_follower_id,count_mretweetretweet_tweet_id),v2);
          } while ((n2=n2->nxt));
        }
        { //slice
          const HASH_RES_t h3 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::hash(se6.modify0(follower_user_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0* i3 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[1]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::equals(se6, *e3)) {
            long count_mretweetretweet_tweet_id = e3->COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long v3 = e3->__av;
            COUNT_mRETWEET6.addOrDelOnZero(se5.modify(follower_user_id,follower_follower_id,count_mretweetretweet_tweet_id),v3);
          } while ((n3=n3->nxt));
        }
        COUNT_mRETWEET14.addOrDelOnZero(se7.modify(follower_user_id,follower_follower_id),1L);
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
          const HASH_RES_t h4 = COUNT_mFOLLOWER1_mapkey0_idxfn::hash(se9.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_0* i4 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_0*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_0::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mFOLLOWER1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mFOLLOWER1_mapkey0_idxfn::equals(se9, *e4)) {
            long tweet_id_1 = e4->TWEET_ID_1;
            long retweet_tweet_id_1 = e4->RETWEET_TWEET_ID_1;
            long v4 = e4->__av;
            COUNT.addOrDelOnZero(se8.modify(tweet_id_1,retweet_tweet_id_1,follower_user_id,follower_follower_id),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
        { //slice
          const HASH_RES_t h5 = COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::hash(se11.modify1(follower_user_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1* i5 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1*>(COUNT_mRETWEET3_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::equals(se11, *e5)) {
            long count_mretweetretweet_tweet_id = e5->COUNT_mRETWEETRETWEET_TWEET_ID;
            long v5 = e5->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se10.modify(follower_user_id,follower_follower_id,count_mretweetretweet_tweet_id),(v5 * -1L));
          } while ((n5=n5->nxt));
        }
        { //slice
          const HASH_RES_t h6 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::hash(se13.modify0(follower_user_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0* i6 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[1]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey0_idxfn::equals(se13, *e6)) {
            long count_mretweetretweet_tweet_id = e6->COUNT_mRETWEET3_mFOLLOWER1_mRETWEETRETWEET_TWEET_ID;
            long v6 = e6->__av;
            COUNT_mRETWEET6.addOrDelOnZero(se12.modify(follower_user_id,follower_follower_id,count_mretweetretweet_tweet_id),(v6 * -1L));
          } while ((n6=n6->nxt));
        }
        COUNT_mRETWEET14.addOrDelOnZero(se14.modify(follower_user_id,follower_follower_id),-1L);
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
          const HASH_RES_t h7 = COUNT_mRETWEET6_mapkey2_idxfn::hash(se16.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_2* i7 = static_cast<HashIndex_COUNT_mRETWEET6_map_2*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_2::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          COUNT_mRETWEET6_entry* e7;
          do if ((e7=n7->obj) && h7 == n7->hash && COUNT_mRETWEET6_mapkey2_idxfn::equals(se16, *e7)) {
            long z_user_id = e7->Z_USER_ID;
            long z_follower_id = e7->Z_FOLLOWER_ID;
            long v8 = e7->__av;
            (/*if */(retweet_retweet_tweet_id == retweet_tweet_id) ? add_to_temp_map(_c1, st1.modify(z_follower_id,z_user_id,v8)) : voidFunc());
          } while ((n7=n7->nxt));
        }
        { //slice
          const HASH_RES_t h8 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se17.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i8 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n8 = &(i8->buckets_[h8 % i8->size_]);
          COUNT_mRETWEET14_entry* e8;
          do if ((e8=n8->obj) && h8 == n8->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se17, *e8)) {
            long z_follower_id = e8->Z_FOLLOWER_ID;
            long v9 = e8->__av;
            (/*if */(retweet_retweet_tweet_id == retweet_tweet_id && retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c1, st2.modify(z_follower_id,retweet_user_id,v9)) : voidFunc());
          } while ((n8=n8->nxt));
        }
        { //slice
          const HASH_RES_t h9 = COUNT_mRETWEET3_mapkey2_idxfn::hash(se18.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_map_2* i9 = static_cast<HashIndex_COUNT_mRETWEET3_map_2*>(COUNT_mRETWEET3.index[1]);
          HashIndex_COUNT_mRETWEET3_map_2::IdxNode* n9 = &(i9->buckets_[h9 % i9->size_]);
          COUNT_mRETWEET3_entry* e9;
          do if ((e9=n9->obj) && h9 == n9->hash && COUNT_mRETWEET3_mapkey2_idxfn::equals(se18, *e9)) {
            long z_user_id = e9->Z_USER_ID;
            long z_follower_id = e9->Z_FOLLOWER_ID;
            long v10 = e9->__av;
            add_to_temp_map(_c1, st3.modify(z_follower_id,z_user_id,v10));
          } while ((n9=n9->nxt));
        }
        { //slice
          const HASH_RES_t h10 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se20.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i10 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n10 = &(i10->buckets_[h10 % i10->size_]);
          COUNT_mRETWEET14_entry* e10;
          do if ((e10=n10->obj) && h10 == n10->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se20, *e10)) {
            long z_follower_id = e10->Z_FOLLOWER_ID;
            long v11 = e10->__av;
            add_to_temp_map(_c1, st4.modify(z_follower_id,retweet_user_id,(v11 * COUNT_mRETWEET21.getValueOrDefault(se19.modify(retweet_retweet_tweet_id,retweet_tweet_id)))));
          } while ((n10=n10->nxt));
        }
        { //temp foreach
          tuple3_LL_L* e11 = _c1.head;
          while(e11){
            long z_follower_id = e11->_1;
            long z_user_id = e11->_2;
            long v12 = e11->__av;
            COUNT.addOrDelOnZero(se15.modify(retweet_tweet_id,retweet_retweet_tweet_id,z_user_id,z_follower_id),v12);
            e11 = e11->nxt;
          }
        }
        { //slice
          const HASH_RES_t h13 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se23.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i11 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n11 = &(i11->buckets_[h13 % i11->size_]);
          COUNT_mRETWEET7_entry* e12;
          do if ((e12=n11->obj) && h13 == n11->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se23, *e12)) {
            long retweet_tweet_id_1 = e12->RETWEET_TWEET_ID_1;
            long v13 = e12->__av;
            _c2.clear();
            { //slice
              const HASH_RES_t h11 = COUNT_mRETWEET6_mapkey2_idxfn::hash(se21.modify2(retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET6_map_2* i12 = static_cast<HashIndex_COUNT_mRETWEET6_map_2*>(COUNT_mRETWEET6.index[1]);
              HashIndex_COUNT_mRETWEET6_map_2::IdxNode* n12 = &(i12->buckets_[h11 % i12->size_]);
              COUNT_mRETWEET6_entry* e13;
              do if ((e13=n12->obj) && h11 == n12->hash && COUNT_mRETWEET6_mapkey2_idxfn::equals(se21, *e13)) {
                long z_user_id = e13->Z_USER_ID;
                long z_follower_id = e13->Z_FOLLOWER_ID;
                long v15 = e13->__av;
                add_to_temp_map(_c2, st5.modify(z_follower_id,z_user_id,v15));
              } while ((n12=n12->nxt));
            }
            { //slice
              const HASH_RES_t h12 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se22.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET14_map_0* i13 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
              HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n13 = &(i13->buckets_[h12 % i13->size_]);
              COUNT_mRETWEET14_entry* e14;
              do if ((e14=n13->obj) && h12 == n13->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se22, *e14)) {
                long z_follower_id = e14->Z_FOLLOWER_ID;
                long v16 = e14->__av;
                (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c2, st6.modify(z_follower_id,retweet_user_id,v16)) : voidFunc());
              } while ((n13=n13->nxt));
            }
            { //temp foreach
              tuple3_LL_L* e15 = _c2.head;
              while(e15){
                long z_follower_id = e15->_1;
                long z_user_id = e15->_2;
                long v17 = e15->__av;
                COUNT.addOrDelOnZero(se15.modify(retweet_retweet_tweet_id,retweet_tweet_id_1,z_user_id,z_follower_id),(v13 * v17));
                e15 = e15->nxt;
              }
            }
          } while ((n11=n11->nxt));
        }
        { //slice
          const HASH_RES_t h15 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se25.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i14 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n14 = &(i14->buckets_[h15 % i14->size_]);
          COUNT_mRETWEET14_entry* e16;
          do if ((e16=n14->obj) && h15 == n14->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se25, *e16)) {
            long z_follower_id = e16->Z_FOLLOWER_ID;
            long v18 = e16->__av;
            { //slice
              const HASH_RES_t h14 = COUNT_pRETWEET13_mapkey0_idxfn::hash(se24.modify0(retweet_retweet_tweet_id));
              const HashIndex_COUNT_pRETWEET13_map_0* i15 = static_cast<HashIndex_COUNT_pRETWEET13_map_0*>(COUNT_pRETWEET13.index[1]);
              HashIndex_COUNT_pRETWEET13_map_0::IdxNode* n15 = &(i15->buckets_[h14 % i15->size_]);
              COUNT_pRETWEET13_entry* e17;
              do if ((e17=n15->obj) && h14 == n15->hash && COUNT_pRETWEET13_mapkey0_idxfn::equals(se24, *e17)) {
                long tweet_id_1 = e17->TWEET_ID_1;
                long retweet_tweet_id_1 = e17->RETWEET_TWEET_ID_1;
                long v19 = e17->__av;
                COUNT.addOrDelOnZero(se15.modify(tweet_id_1,retweet_tweet_id_1,retweet_user_id,z_follower_id),(v18 * v19));
              } while ((n15=n15->nxt));
            }
          } while ((n14=n14->nxt));
        }
        _c3.clear();
        { //slice
          const HASH_RES_t h16 = COUNT_mRETWEET21_mapkey1_idxfn::hash(se27.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET21_map_1* i16 = static_cast<HashIndex_COUNT_mRETWEET21_map_1*>(COUNT_mRETWEET21.index[1]);
          HashIndex_COUNT_mRETWEET21_map_1::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          COUNT_mRETWEET21_entry* e18;
          do if ((e18=n16->obj) && h16 == n16->hash && COUNT_mRETWEET21_mapkey1_idxfn::equals(se27, *e18)) {
            long count_pretweetretweet_retweet_tweet_id = e18->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v21 = e18->__av;
            add_to_temp_map(_c3, st7.modify(count_pretweetretweet_retweet_tweet_id,retweet_tweet_id,v21));
          } while ((n16=n16->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c3, st8.modify(retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
        { //temp foreach
          tuple3_LL_L* e19 = _c3.head;
          while(e19){
            long count_pretweetretweet_retweet_tweet_id = e19->_1;
            long tweet_id_1 = e19->_2;
            long v22 = e19->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se26.modify(count_pretweetretweet_retweet_tweet_id,tweet_id_1,retweet_retweet_tweet_id),v22);
            e19 = e19->nxt;
          }
        }
        { //slice
          const HASH_RES_t h17 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se28.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i17 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n17 = &(i17->buckets_[h17 % i17->size_]);
          COUNT_mRETWEET7_entry* e20;
          do if ((e20=n17->obj) && h17 == n17->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se28, *e20)) {
            long retweet_tweet_id_1 = e20->RETWEET_TWEET_ID_1;
            long v23 = e20->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se26.modify(retweet_tweet_id,retweet_retweet_tweet_id,retweet_tweet_id_1),v23);
          } while ((n17=n17->nxt));
        }
        _c4.clear();
        { //slice
          const HASH_RES_t h18 = COUNT_mRETWEET6_mapkey2_idxfn::hash(se30.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_2* i18 = static_cast<HashIndex_COUNT_mRETWEET6_map_2*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_2::IdxNode* n18 = &(i18->buckets_[h18 % i18->size_]);
          COUNT_mRETWEET6_entry* e21;
          do if ((e21=n18->obj) && h18 == n18->hash && COUNT_mRETWEET6_mapkey2_idxfn::equals(se30, *e21)) {
            long z_user_id = e21->Z_USER_ID;
            long z_follower_id = e21->Z_FOLLOWER_ID;
            long v25 = e21->__av;
            add_to_temp_map(_c4, st9.modify(z_follower_id,z_user_id,v25));
          } while ((n18=n18->nxt));
        }
        { //slice
          const HASH_RES_t h19 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se31.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i19 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n19 = &(i19->buckets_[h19 % i19->size_]);
          COUNT_mRETWEET14_entry* e22;
          do if ((e22=n19->obj) && h19 == n19->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se31, *e22)) {
            long z_follower_id = e22->Z_FOLLOWER_ID;
            long v26 = e22->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c4, st10.modify(z_follower_id,retweet_user_id,v26)) : voidFunc());
          } while ((n19=n19->nxt));
        }
        { //temp foreach
          tuple3_LL_L* e23 = _c4.head;
          while(e23){
            long z_follower_id = e23->_1;
            long z_user_id = e23->_2;
            long v27 = e23->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se29.modify(z_user_id,z_follower_id,retweet_retweet_tweet_id),v27);
            e23 = e23->nxt;
          }
        }
        { //slice
          const HASH_RES_t h21 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se33.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i20 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n20 = &(i20->buckets_[h21 % i20->size_]);
          COUNT_mRETWEET14_entry* e24;
          do if ((e24=n20->obj) && h21 == n20->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se33, *e24)) {
            long z_follower_id = e24->Z_FOLLOWER_ID;
            long v28 = e24->__av;
            { //slice
              const HASH_RES_t h20 = COUNT_mRETWEET21_mapkey0_idxfn::hash(se32.modify0(retweet_retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET21_map_0* i21 = static_cast<HashIndex_COUNT_mRETWEET21_map_0*>(COUNT_mRETWEET21.index[2]);
              HashIndex_COUNT_mRETWEET21_map_0::IdxNode* n21 = &(i21->buckets_[h20 % i21->size_]);
              COUNT_mRETWEET21_entry* e25;
              do if ((e25=n21->obj) && h20 == n21->hash && COUNT_mRETWEET21_mapkey0_idxfn::equals(se32, *e25)) {
                long count_mretweetretweet_tweet_id = e25->COUNT_mRETWEETRETWEET_TWEET_ID;
                long v29 = e25->__av;
                COUNT_mRETWEET3.addOrDelOnZero(se29.modify(retweet_user_id,z_follower_id,count_mretweetretweet_tweet_id),(v28 * v29));
              } while ((n21=n21->nxt));
            }
          } while ((n20=n20->nxt));
        }
        { //slice
          const HASH_RES_t h22 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se35.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i22 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n22 = &(i22->buckets_[h22 % i22->size_]);
          COUNT_mRETWEET14_entry* e26;
          do if ((e26=n22->obj) && h22 == n22->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se35, *e26)) {
            long z_follower_id = e26->Z_FOLLOWER_ID;
            long v30 = e26->__av;
            COUNT_mRETWEET6.addOrDelOnZero(se34.modify(retweet_user_id,z_follower_id,retweet_retweet_tweet_id),v30);
          } while ((n22=n22->nxt));
        }
        _c5.clear();
        { //slice
          const HASH_RES_t h23 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se37.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1* i23 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n23 = &(i23->buckets_[h23 % i23->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e27;
          do if ((e27=n23->obj) && h23 == n23->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se37, *e27)) {
            long count_mfollowerfollower_user_id = e27->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v32 = e27->__av;
            (/*if */(retweet_retweet_tweet_id == retweet_tweet_id) ? add_to_temp_map(_c5, st11.modify(count_mfollowerfollower_user_id,v32)) : voidFunc());
          } while ((n23=n23->nxt));
        }
        (/*if */(retweet_retweet_tweet_id == retweet_tweet_id && retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c5, st12.modify(retweet_user_id,1L)) : voidFunc());
        { //slice
          const HASH_RES_t h24 = COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::hash(se38.modify0(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0* i24 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0*>(COUNT_mRETWEET3_mFOLLOWER1.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0::IdxNode* n24 = &(i24->buckets_[h24 % i24->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e28;
          do if ((e28=n24->obj) && h24 == n24->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::equals(se38, *e28)) {
            long count_mfollowerfollower_user_id = e28->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v33 = e28->__av;
            add_to_temp_map(_c5, st13.modify(count_mfollowerfollower_user_id,v33));
          } while ((n24=n24->nxt));
        }
        add_to_temp_map(_c5, st14.modify(retweet_user_id,COUNT_mRETWEET21.getValueOrDefault(se39.modify(retweet_retweet_tweet_id,retweet_tweet_id))));
        { //temp foreach
          tuple2_L_L* e29 = _c5.head;
          while(e29){
            long count_mfollowerfollower_user_id = e29->_1;
            long v34 = e29->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se36.modify(count_mfollowerfollower_user_id,retweet_tweet_id,retweet_retweet_tweet_id),v34);
            e29 = e29->nxt;
          }
        }
        { //slice
          const HASH_RES_t h26 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se41.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i25 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n25 = &(i25->buckets_[h26 % i25->size_]);
          COUNT_mRETWEET7_entry* e30;
          do if ((e30=n25->obj) && h26 == n25->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se41, *e30)) {
            long retweet_tweet_id_1 = e30->RETWEET_TWEET_ID_1;
            long v35 = e30->__av;
            _c6.clear();
            { //slice
              const HASH_RES_t h25 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se40.modify1(retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1* i26 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[2]);
              HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n26 = &(i26->buckets_[h25 % i26->size_]);
              COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e31;
              do if ((e31=n26->obj) && h25 == n26->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se40, *e31)) {
                long count_mfollowerfollower_user_id = e31->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
                long v37 = e31->__av;
                add_to_temp_map(_c6, st15.modify(count_mfollowerfollower_user_id,v37));
              } while ((n26=n26->nxt));
            }
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c6, st16.modify(retweet_user_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e32 = _c6.head;
              while(e32){
                long count_mfollowerfollower_user_id = e32->_1;
                long v38 = e32->__av;
                COUNT_mFOLLOWER1.addOrDelOnZero(se36.modify(count_mfollowerfollower_user_id,retweet_retweet_tweet_id,retweet_tweet_id_1),(v35 * v38));
                e32 = e32->nxt;
              }
            }
          } while ((n25=n25->nxt));
        }
        { //slice
          const HASH_RES_t h27 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se42.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET15_map_0* i27 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
          HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n27 = &(i27->buckets_[h27 % i27->size_]);
          COUNT_mRETWEET15_entry* e33;
          do if ((e33=n27->obj) && h27 == n27->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se42, *e33)) {
            long retweet_tweet_id_1 = e33->RETWEET_TWEET_ID_1;
            long tweet_id_1 = e33->TWEET_ID_1;
            long v39 = e33->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se36.modify(retweet_user_id,tweet_id_1,retweet_tweet_id_1),v39);
          } while ((n27=n27->nxt));
        }
        _c7.clear();
        { //slice
          const HASH_RES_t h28 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se44.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1* i28 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n28 = &(i28->buckets_[h28 % i28->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e34;
          do if ((e34=n28->obj) && h28 == n28->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se44, *e34)) {
            long count_mretweet3_mfollowerfollower_user_id = e34->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v41 = e34->__av;
            add_to_temp_map(_c7, st17.modify(count_mretweet3_mfollowerfollower_user_id,v41));
          } while ((n28=n28->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c7, st18.modify(retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e35 = _c7.head;
          while(e35){
            long count_mretweet3_mfollowerfollower_user_id = e35->_1;
            long v42 = e35->__av;
            COUNT_mRETWEET3_mFOLLOWER1.addOrDelOnZero(se43.modify(retweet_retweet_tweet_id,count_mretweet3_mfollowerfollower_user_id),v42);
            e35 = e35->nxt;
          }
        }
        { //slice
          const HASH_RES_t h29 = COUNT_mRETWEET21_mapkey0_idxfn::hash(se45.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET21_map_0* i29 = static_cast<HashIndex_COUNT_mRETWEET21_map_0*>(COUNT_mRETWEET21.index[2]);
          HashIndex_COUNT_mRETWEET21_map_0::IdxNode* n29 = &(i29->buckets_[h29 % i29->size_]);
          COUNT_mRETWEET21_entry* e36;
          do if ((e36=n29->obj) && h29 == n29->hash && COUNT_mRETWEET21_mapkey0_idxfn::equals(se45, *e36)) {
            long count_mretweetretweet_tweet_id = e36->COUNT_mRETWEETRETWEET_TWEET_ID;
            long v43 = e36->__av;
            COUNT_mRETWEET3_mFOLLOWER1.addOrDelOnZero(se43.modify(count_mretweetretweet_tweet_id,retweet_user_id),v43);
          } while ((n29=n29->nxt));
        }
        COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.addOrDelOnZero(se46.modify(retweet_user_id,retweet_retweet_tweet_id),1L);
        _c8.clear();
        { //slice
          const HASH_RES_t h30 = COUNT_mRETWEET21_mapkey1_idxfn::hash(se48.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET21_map_1* i30 = static_cast<HashIndex_COUNT_mRETWEET21_map_1*>(COUNT_mRETWEET21.index[1]);
          HashIndex_COUNT_mRETWEET21_map_1::IdxNode* n30 = &(i30->buckets_[h30 % i30->size_]);
          COUNT_mRETWEET21_entry* e37;
          do if ((e37=n30->obj) && h30 == n30->hash && COUNT_mRETWEET21_mapkey1_idxfn::equals(se48, *e37)) {
            long count_mretweetretweet_retweet_tweet_id = e37->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v45 = e37->__av;
            add_to_temp_map(_c8, st19.modify(count_mretweetretweet_retweet_tweet_id,retweet_tweet_id,v45));
          } while ((n30=n30->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c8, st20.modify(retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
        { //temp foreach
          tuple3_LL_L* e38 = _c8.head;
          while(e38){
            long count_mretweetretweet_retweet_tweet_id = e38->_1;
            long tweet_id_1 = e38->_2;
            long v46 = e38->__av;
            COUNT_mRETWEET15.addOrDelOnZero(se47.modify(count_mretweetretweet_retweet_tweet_id,retweet_retweet_tweet_id,tweet_id_1),v46);
            e38 = e38->nxt;
          }
        }
        { //slice
          const HASH_RES_t h31 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se49.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i31 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n31 = &(i31->buckets_[h31 % i31->size_]);
          COUNT_mRETWEET7_entry* e39;
          do if ((e39=n31->obj) && h31 == n31->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se49, *e39)) {
            long retweet_tweet_id_1 = e39->RETWEET_TWEET_ID_1;
            long v47 = e39->__av;
            COUNT_mRETWEET15.addOrDelOnZero(se47.modify(retweet_tweet_id,retweet_tweet_id_1,retweet_retweet_tweet_id),v47);
          } while ((n31=n31->nxt));
        }
        COUNT_mRETWEET21.addOrDelOnZero(se50.modify(retweet_tweet_id,retweet_retweet_tweet_id),1L);
        COUNT_mRETWEET7.addOrDelOnZero(se51.modify(retweet_tweet_id,retweet_retweet_tweet_id),1L);
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
        _c9.clear();
        { //slice
          const HASH_RES_t h32 = COUNT_mRETWEET3_mapkey2_idxfn::hash(se53.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_map_2* i32 = static_cast<HashIndex_COUNT_mRETWEET3_map_2*>(COUNT_mRETWEET3.index[1]);
          HashIndex_COUNT_mRETWEET3_map_2::IdxNode* n32 = &(i32->buckets_[h32 % i32->size_]);
          COUNT_mRETWEET3_entry* e40;
          do if ((e40=n32->obj) && h32 == n32->hash && COUNT_mRETWEET3_mapkey2_idxfn::equals(se53, *e40)) {
            long z_user_id = e40->Z_USER_ID;
            long z_follower_id = e40->Z_FOLLOWER_ID;
            long v49 = e40->__av;
            add_to_temp_map(_c9, st21.modify(z_follower_id,z_user_id,(v49 * -1L)));
          } while ((n32=n32->nxt));
        }
        { //slice
          const HASH_RES_t h33 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se54.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i33 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n33 = &(i33->buckets_[h33 % i33->size_]);
          COUNT_mRETWEET14_entry* e41;
          do if ((e41=n33->obj) && h33 == n33->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se54, *e41)) {
            long z_follower_id = e41->Z_FOLLOWER_ID;
            long v50 = e41->__av;
            (/*if */(retweet_retweet_tweet_id == retweet_tweet_id && retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c9, st22.modify(z_follower_id,retweet_user_id,(v50 * -1L))) : voidFunc());
          } while ((n33=n33->nxt));
        }
        { //slice
          const HASH_RES_t h34 = COUNT_mRETWEET6_mapkey2_idxfn::hash(se55.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_2* i34 = static_cast<HashIndex_COUNT_mRETWEET6_map_2*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_2::IdxNode* n34 = &(i34->buckets_[h34 % i34->size_]);
          COUNT_mRETWEET6_entry* e42;
          do if ((e42=n34->obj) && h34 == n34->hash && COUNT_mRETWEET6_mapkey2_idxfn::equals(se55, *e42)) {
            long z_user_id = e42->Z_USER_ID;
            long z_follower_id = e42->Z_FOLLOWER_ID;
            long v51 = e42->__av;
            (/*if */(retweet_retweet_tweet_id == retweet_tweet_id) ? add_to_temp_map(_c9, st23.modify(z_follower_id,z_user_id,v51)) : voidFunc());
          } while ((n34=n34->nxt));
        }
        { //slice
          const HASH_RES_t h35 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se57.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i35 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n35 = &(i35->buckets_[h35 % i35->size_]);
          COUNT_mRETWEET14_entry* e43;
          do if ((e43=n35->obj) && h35 == n35->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se57, *e43)) {
            long z_follower_id = e43->Z_FOLLOWER_ID;
            long v52 = e43->__av;
            add_to_temp_map(_c9, st24.modify(z_follower_id,retweet_user_id,(v52 * COUNT_mRETWEET21.getValueOrDefault(se56.modify(retweet_retweet_tweet_id,retweet_tweet_id)))));
          } while ((n35=n35->nxt));
        }
        { //temp foreach
          tuple3_LL_L* e44 = _c9.head;
          while(e44){
            long z_follower_id = e44->_1;
            long z_user_id = e44->_2;
            long v53 = e44->__av;
            COUNT.addOrDelOnZero(se52.modify(retweet_tweet_id,retweet_retweet_tweet_id,z_user_id,z_follower_id),v53);
            e44 = e44->nxt;
          }
        }
        { //slice
          const HASH_RES_t h38 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se60.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i36 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n36 = &(i36->buckets_[h38 % i36->size_]);
          COUNT_mRETWEET7_entry* e45;
          do if ((e45=n36->obj) && h38 == n36->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se60, *e45)) {
            long retweet_tweet_id_1 = e45->RETWEET_TWEET_ID_1;
            long v54 = e45->__av;
            _c10.clear();
            { //slice
              const HASH_RES_t h36 = COUNT_mRETWEET6_mapkey2_idxfn::hash(se58.modify2(retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET6_map_2* i37 = static_cast<HashIndex_COUNT_mRETWEET6_map_2*>(COUNT_mRETWEET6.index[1]);
              HashIndex_COUNT_mRETWEET6_map_2::IdxNode* n37 = &(i37->buckets_[h36 % i37->size_]);
              COUNT_mRETWEET6_entry* e46;
              do if ((e46=n37->obj) && h36 == n37->hash && COUNT_mRETWEET6_mapkey2_idxfn::equals(se58, *e46)) {
                long z_user_id = e46->Z_USER_ID;
                long z_follower_id = e46->Z_FOLLOWER_ID;
                long v56 = e46->__av;
                add_to_temp_map(_c10, st25.modify(z_follower_id,z_user_id,(v56 * -1L)));
              } while ((n37=n37->nxt));
            }
            { //slice
              const HASH_RES_t h37 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se59.modify0(retweet_user_id));
              const HashIndex_COUNT_mRETWEET14_map_0* i38 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
              HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n38 = &(i38->buckets_[h37 % i38->size_]);
              COUNT_mRETWEET14_entry* e47;
              do if ((e47=n38->obj) && h37 == n38->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se59, *e47)) {
                long z_follower_id = e47->Z_FOLLOWER_ID;
                long v57 = e47->__av;
                (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c10, st26.modify(z_follower_id,retweet_user_id,v57)) : voidFunc());
              } while ((n38=n38->nxt));
            }
            { //temp foreach
              tuple3_LL_L* e48 = _c10.head;
              while(e48){
                long z_follower_id = e48->_1;
                long z_user_id = e48->_2;
                long v58 = e48->__av;
                COUNT.addOrDelOnZero(se52.modify(retweet_retweet_tweet_id,retweet_tweet_id_1,z_user_id,z_follower_id),(v54 * v58));
                e48 = e48->nxt;
              }
            }
          } while ((n36=n36->nxt));
        }
        { //slice
          const HASH_RES_t h40 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se62.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i39 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n39 = &(i39->buckets_[h40 % i39->size_]);
          COUNT_mRETWEET14_entry* e49;
          do if ((e49=n39->obj) && h40 == n39->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se62, *e49)) {
            long z_follower_id = e49->Z_FOLLOWER_ID;
            long v59 = e49->__av;
            { //slice
              const HASH_RES_t h39 = COUNT_mRETWEET15_mapkey0_idxfn::hash(se61.modify0(retweet_retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET15_map_0* i40 = static_cast<HashIndex_COUNT_mRETWEET15_map_0*>(COUNT_mRETWEET15.index[1]);
              HashIndex_COUNT_mRETWEET15_map_0::IdxNode* n40 = &(i40->buckets_[h39 % i40->size_]);
              COUNT_mRETWEET15_entry* e50;
              do if ((e50=n40->obj) && h39 == n40->hash && COUNT_mRETWEET15_mapkey0_idxfn::equals(se61, *e50)) {
                long retweet_tweet_id_1 = e50->RETWEET_TWEET_ID_1;
                long tweet_id_1 = e50->TWEET_ID_1;
                long v60 = e50->__av;
                COUNT.addOrDelOnZero(se52.modify(tweet_id_1,retweet_tweet_id_1,retweet_user_id,z_follower_id),(v59 * (v60 * -1L)));
              } while ((n40=n40->nxt));
            }
          } while ((n39=n39->nxt));
        }
        _c11.clear();
        { //slice
          const HASH_RES_t h41 = COUNT_mRETWEET6_mapkey2_idxfn::hash(se64.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET6_map_2* i41 = static_cast<HashIndex_COUNT_mRETWEET6_map_2*>(COUNT_mRETWEET6.index[1]);
          HashIndex_COUNT_mRETWEET6_map_2::IdxNode* n41 = &(i41->buckets_[h41 % i41->size_]);
          COUNT_mRETWEET6_entry* e51;
          do if ((e51=n41->obj) && h41 == n41->hash && COUNT_mRETWEET6_mapkey2_idxfn::equals(se64, *e51)) {
            long z_user_id = e51->Z_USER_ID;
            long z_follower_id = e51->Z_FOLLOWER_ID;
            long v62 = e51->__av;
            add_to_temp_map(_c11, st27.modify(z_follower_id,z_user_id,(v62 * -1L)));
          } while ((n41=n41->nxt));
        }
        { //slice
          const HASH_RES_t h42 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se65.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i42 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n42 = &(i42->buckets_[h42 % i42->size_]);
          COUNT_mRETWEET14_entry* e52;
          do if ((e52=n42->obj) && h42 == n42->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se65, *e52)) {
            long z_follower_id = e52->Z_FOLLOWER_ID;
            long v63 = e52->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c11, st28.modify(z_follower_id,retweet_user_id,v63)) : voidFunc());
          } while ((n42=n42->nxt));
        }
        { //temp foreach
          tuple3_LL_L* e53 = _c11.head;
          while(e53){
            long z_follower_id = e53->_1;
            long z_user_id = e53->_2;
            long v64 = e53->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se63.modify(z_user_id,z_follower_id,retweet_retweet_tweet_id),v64);
            e53 = e53->nxt;
          }
        }
        { //slice
          const HASH_RES_t h44 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se67.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i43 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n43 = &(i43->buckets_[h44 % i43->size_]);
          COUNT_mRETWEET14_entry* e54;
          do if ((e54=n43->obj) && h44 == n43->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se67, *e54)) {
            long z_follower_id = e54->Z_FOLLOWER_ID;
            long v65 = e54->__av;
            { //slice
              const HASH_RES_t h43 = COUNT_mRETWEET21_mapkey0_idxfn::hash(se66.modify0(retweet_retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET21_map_0* i44 = static_cast<HashIndex_COUNT_mRETWEET21_map_0*>(COUNT_mRETWEET21.index[2]);
              HashIndex_COUNT_mRETWEET21_map_0::IdxNode* n44 = &(i44->buckets_[h43 % i44->size_]);
              COUNT_mRETWEET21_entry* e55;
              do if ((e55=n44->obj) && h43 == n44->hash && COUNT_mRETWEET21_mapkey0_idxfn::equals(se66, *e55)) {
                long count_mretweetretweet_tweet_id = e55->COUNT_mRETWEETRETWEET_TWEET_ID;
                long v66 = e55->__av;
                COUNT_mRETWEET3.addOrDelOnZero(se63.modify(retweet_user_id,z_follower_id,count_mretweetretweet_tweet_id),(v65 * (v66 * -1L)));
              } while ((n44=n44->nxt));
            }
          } while ((n43=n43->nxt));
        }
        { //slice
          const HASH_RES_t h45 = COUNT_mRETWEET14_mapkey0_idxfn::hash(se69.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET14_map_0* i45 = static_cast<HashIndex_COUNT_mRETWEET14_map_0*>(COUNT_mRETWEET14.index[1]);
          HashIndex_COUNT_mRETWEET14_map_0::IdxNode* n45 = &(i45->buckets_[h45 % i45->size_]);
          COUNT_mRETWEET14_entry* e56;
          do if ((e56=n45->obj) && h45 == n45->hash && COUNT_mRETWEET14_mapkey0_idxfn::equals(se69, *e56)) {
            long z_follower_id = e56->Z_FOLLOWER_ID;
            long v67 = e56->__av;
            COUNT_mRETWEET6.addOrDelOnZero(se68.modify(retweet_user_id,z_follower_id,retweet_retweet_tweet_id),(v67 * -1L));
          } while ((n45=n45->nxt));
        }
        _c12.clear();
        { //slice
          const HASH_RES_t h46 = COUNT_mRETWEET21_mapkey1_idxfn::hash(se71.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET21_map_1* i46 = static_cast<HashIndex_COUNT_mRETWEET21_map_1*>(COUNT_mRETWEET21.index[1]);
          HashIndex_COUNT_mRETWEET21_map_1::IdxNode* n46 = &(i46->buckets_[h46 % i46->size_]);
          COUNT_mRETWEET21_entry* e57;
          do if ((e57=n46->obj) && h46 == n46->hash && COUNT_mRETWEET21_mapkey1_idxfn::equals(se71, *e57)) {
            long count_mretweetretweet_retweet_tweet_id = e57->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v69 = e57->__av;
            add_to_temp_map(_c12, st29.modify(count_mretweetretweet_retweet_tweet_id,retweet_tweet_id,(v69 * -1L)));
          } while ((n46=n46->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c12, st30.modify(retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
        { //temp foreach
          tuple3_LL_L* e58 = _c12.head;
          while(e58){
            long count_mretweetretweet_retweet_tweet_id = e58->_1;
            long tweet_id_1 = e58->_2;
            long v70 = e58->__av;
            COUNT_mRETWEET15.addOrDelOnZero(se70.modify(count_mretweetretweet_retweet_tweet_id,retweet_retweet_tweet_id,tweet_id_1),v70);
            e58 = e58->nxt;
          }
        }
        { //slice
          const HASH_RES_t h47 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se72.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i47 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n47 = &(i47->buckets_[h47 % i47->size_]);
          COUNT_mRETWEET7_entry* e59;
          do if ((e59=n47->obj) && h47 == n47->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se72, *e59)) {
            long retweet_tweet_id_1 = e59->RETWEET_TWEET_ID_1;
            long v71 = e59->__av;
            COUNT_mRETWEET15.addOrDelOnZero(se70.modify(retweet_tweet_id,retweet_tweet_id_1,retweet_retweet_tweet_id),(v71 * -1L));
          } while ((n47=n47->nxt));
        }
        _c13.clear();
        { //slice
          const HASH_RES_t h48 = COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::hash(se74.modify0(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0* i48 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0*>(COUNT_mRETWEET3_mFOLLOWER1.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0::IdxNode* n48 = &(i48->buckets_[h48 % i48->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e60;
          do if ((e60=n48->obj) && h48 == n48->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::equals(se74, *e60)) {
            long count_mfollowerfollower_user_id = e60->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v73 = e60->__av;
            add_to_temp_map(_c13, st31.modify(count_mfollowerfollower_user_id,(v73 * -1L)));
          } while ((n48=n48->nxt));
        }
        (/*if */(retweet_retweet_tweet_id == retweet_tweet_id && retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c13, st32.modify(retweet_user_id,-1L)) : voidFunc());
        { //slice
          const HASH_RES_t h49 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se75.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1* i49 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n49 = &(i49->buckets_[h49 % i49->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e61;
          do if ((e61=n49->obj) && h49 == n49->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se75, *e61)) {
            long count_mfollowerfollower_user_id = e61->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v74 = e61->__av;
            (/*if */(retweet_retweet_tweet_id == retweet_tweet_id) ? add_to_temp_map(_c13, st33.modify(count_mfollowerfollower_user_id,v74)) : voidFunc());
          } while ((n49=n49->nxt));
        }
        add_to_temp_map(_c13, st34.modify(retweet_user_id,COUNT_mRETWEET21.getValueOrDefault(se76.modify(retweet_retweet_tweet_id,retweet_tweet_id))));
        { //temp foreach
          tuple2_L_L* e62 = _c13.head;
          while(e62){
            long count_mfollowerfollower_user_id = e62->_1;
            long v75 = e62->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se73.modify(count_mfollowerfollower_user_id,retweet_tweet_id,retweet_retweet_tweet_id),v75);
            e62 = e62->nxt;
          }
        }
        { //slice
          const HASH_RES_t h51 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se78.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i50 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n50 = &(i50->buckets_[h51 % i50->size_]);
          COUNT_mRETWEET7_entry* e63;
          do if ((e63=n50->obj) && h51 == n50->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se78, *e63)) {
            long retweet_tweet_id_1 = e63->RETWEET_TWEET_ID_1;
            long v76 = e63->__av;
            _c14.clear();
            { //slice
              const HASH_RES_t h50 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se77.modify1(retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1* i51 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[2]);
              HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n51 = &(i51->buckets_[h50 % i51->size_]);
              COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e64;
              do if ((e64=n51->obj) && h50 == n51->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se77, *e64)) {
                long count_mfollowerfollower_user_id = e64->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
                long v78 = e64->__av;
                add_to_temp_map(_c14, st35.modify(count_mfollowerfollower_user_id,(v78 * -1L)));
              } while ((n51=n51->nxt));
            }
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c14, st36.modify(retweet_user_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e65 = _c14.head;
              while(e65){
                long count_mfollowerfollower_user_id = e65->_1;
                long v79 = e65->__av;
                COUNT_mFOLLOWER1.addOrDelOnZero(se73.modify(count_mfollowerfollower_user_id,retweet_retweet_tweet_id,retweet_tweet_id_1),(v76 * v79));
                e65 = e65->nxt;
              }
            }
          } while ((n50=n50->nxt));
        }
        { //slice
          const HASH_RES_t h52 = COUNT_pRETWEET13_mapkey0_idxfn::hash(se79.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_pRETWEET13_map_0* i52 = static_cast<HashIndex_COUNT_pRETWEET13_map_0*>(COUNT_pRETWEET13.index[1]);
          HashIndex_COUNT_pRETWEET13_map_0::IdxNode* n52 = &(i52->buckets_[h52 % i52->size_]);
          COUNT_pRETWEET13_entry* e66;
          do if ((e66=n52->obj) && h52 == n52->hash && COUNT_pRETWEET13_mapkey0_idxfn::equals(se79, *e66)) {
            long tweet_id_1 = e66->TWEET_ID_1;
            long retweet_tweet_id_1 = e66->RETWEET_TWEET_ID_1;
            long v80 = e66->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se73.modify(retweet_user_id,tweet_id_1,retweet_tweet_id_1),(v80 * -1L));
          } while ((n52=n52->nxt));
        }
        _c15.clear();
        { //slice
          const HASH_RES_t h53 = COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::hash(se81.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1* i53 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1*>(COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map_1::IdxNode* n53 = &(i53->buckets_[h53 % i53->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry* e67;
          do if ((e67=n53->obj) && h53 == n53->hash && COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_mapkey1_idxfn::equals(se81, *e67)) {
            long count_mretweet3_mfollowerfollower_user_id = e67->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v82 = e67->__av;
            add_to_temp_map(_c15, st37.modify(count_mretweet3_mfollowerfollower_user_id,(v82 * -1L)));
          } while ((n53=n53->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c15, st38.modify(retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e68 = _c15.head;
          while(e68){
            long count_mretweet3_mfollowerfollower_user_id = e68->_1;
            long v83 = e68->__av;
            COUNT_mRETWEET3_mFOLLOWER1.addOrDelOnZero(se80.modify(retweet_retweet_tweet_id,count_mretweet3_mfollowerfollower_user_id),v83);
            e68 = e68->nxt;
          }
        }
        { //slice
          const HASH_RES_t h54 = COUNT_mRETWEET21_mapkey0_idxfn::hash(se82.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET21_map_0* i54 = static_cast<HashIndex_COUNT_mRETWEET21_map_0*>(COUNT_mRETWEET21.index[2]);
          HashIndex_COUNT_mRETWEET21_map_0::IdxNode* n54 = &(i54->buckets_[h54 % i54->size_]);
          COUNT_mRETWEET21_entry* e69;
          do if ((e69=n54->obj) && h54 == n54->hash && COUNT_mRETWEET21_mapkey0_idxfn::equals(se82, *e69)) {
            long count_mretweetretweet_tweet_id = e69->COUNT_mRETWEETRETWEET_TWEET_ID;
            long v84 = e69->__av;
            COUNT_mRETWEET3_mFOLLOWER1.addOrDelOnZero(se80.modify(count_mretweetretweet_tweet_id,retweet_user_id),(v84 * -1L));
          } while ((n54=n54->nxt));
        }
        COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2.addOrDelOnZero(se83.modify(retweet_user_id,retweet_retweet_tweet_id),-1L);
        _c16.clear();
        { //slice
          const HASH_RES_t h55 = COUNT_mRETWEET21_mapkey1_idxfn::hash(se85.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET21_map_1* i55 = static_cast<HashIndex_COUNT_mRETWEET21_map_1*>(COUNT_mRETWEET21.index[1]);
          HashIndex_COUNT_mRETWEET21_map_1::IdxNode* n55 = &(i55->buckets_[h55 % i55->size_]);
          COUNT_mRETWEET21_entry* e70;
          do if ((e70=n55->obj) && h55 == n55->hash && COUNT_mRETWEET21_mapkey1_idxfn::equals(se85, *e70)) {
            long count_pretweetretweet_retweet_tweet_id = e70->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            long v86 = e70->__av;
            add_to_temp_map(_c16, st39.modify(count_pretweetretweet_retweet_tweet_id,retweet_tweet_id,(v86 * -1L)));
          } while ((n55=n55->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c16, st40.modify(retweet_tweet_id,retweet_retweet_tweet_id,1L)) : voidFunc());
        { //temp foreach
          tuple3_LL_L* e71 = _c16.head;
          while(e71){
            long count_pretweetretweet_retweet_tweet_id = e71->_1;
            long tweet_id_1 = e71->_2;
            long v87 = e71->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se84.modify(count_pretweetretweet_retweet_tweet_id,tweet_id_1,retweet_retweet_tweet_id),v87);
            e71 = e71->nxt;
          }
        }
        { //slice
          const HASH_RES_t h56 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se86.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i56 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n56 = &(i56->buckets_[h56 % i56->size_]);
          COUNT_mRETWEET7_entry* e72;
          do if ((e72=n56->obj) && h56 == n56->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se86, *e72)) {
            long retweet_tweet_id_1 = e72->RETWEET_TWEET_ID_1;
            long v88 = e72->__av;
            COUNT_pRETWEET13.addOrDelOnZero(se84.modify(retweet_tweet_id,retweet_retweet_tweet_id,retweet_tweet_id_1),(v88 * -1L));
          } while ((n56=n56->nxt));
        }
        COUNT_mRETWEET21.addOrDelOnZero(se87.modify(retweet_tweet_id,retweet_retweet_tweet_id),-1L);
        COUNT_mRETWEET7.addOrDelOnZero(se88.modify(retweet_tweet_id,retweet_retweet_tweet_id),-1L);
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
    COUNT_mRETWEET3_entry se3;
    COUNT_mRETWEET3_mFOLLOWER1_entry se4;
    COUNT_mRETWEET6_entry se5;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se6;
    COUNT_mRETWEET14_entry se7;
    COUNT_entry se8;
    COUNT_mFOLLOWER1_entry se9;
    COUNT_mRETWEET3_entry se10;
    COUNT_mRETWEET3_mFOLLOWER1_entry se11;
    COUNT_mRETWEET6_entry se12;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se13;
    COUNT_mRETWEET14_entry se14;
    COUNT_entry se15;
    tuple3_LL_L st1;
    COUNT_mRETWEET6_entry se16;
    tuple3_LL_L st2;
    COUNT_mRETWEET14_entry se17;
    tuple3_LL_L st3;
    COUNT_mRETWEET3_entry se18;
    COUNT_mRETWEET21_entry se19;
    tuple3_LL_L st4;
    COUNT_mRETWEET14_entry se20;
    tuple3_LL_L st5;
    COUNT_mRETWEET6_entry se21;
    tuple3_LL_L st6;
    COUNT_mRETWEET14_entry se22;
    COUNT_mRETWEET7_entry se23;
    COUNT_pRETWEET13_entry se24;
    COUNT_mRETWEET14_entry se25;
    COUNT_pRETWEET13_entry se26;
    tuple3_LL_L st7;
    COUNT_mRETWEET21_entry se27;
    tuple3_LL_L st8;
    COUNT_mRETWEET7_entry se28;
    COUNT_mRETWEET3_entry se29;
    tuple3_LL_L st9;
    COUNT_mRETWEET6_entry se30;
    tuple3_LL_L st10;
    COUNT_mRETWEET14_entry se31;
    COUNT_mRETWEET21_entry se32;
    COUNT_mRETWEET14_entry se33;
    COUNT_mRETWEET6_entry se34;
    COUNT_mRETWEET14_entry se35;
    COUNT_mFOLLOWER1_entry se36;
    tuple2_L_L st11;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se37;
    tuple2_L_L st12;
    tuple2_L_L st13;
    COUNT_mRETWEET3_mFOLLOWER1_entry se38;
    COUNT_mRETWEET21_entry se39;
    tuple2_L_L st14;
    tuple2_L_L st15;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se40;
    tuple2_L_L st16;
    COUNT_mRETWEET7_entry se41;
    COUNT_mRETWEET15_entry se42;
    COUNT_mRETWEET3_mFOLLOWER1_entry se43;
    tuple2_L_L st17;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se44;
    tuple2_L_L st18;
    COUNT_mRETWEET21_entry se45;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se46;
    COUNT_mRETWEET15_entry se47;
    tuple3_LL_L st19;
    COUNT_mRETWEET21_entry se48;
    tuple3_LL_L st20;
    COUNT_mRETWEET7_entry se49;
    COUNT_mRETWEET21_entry se50;
    COUNT_mRETWEET7_entry se51;
    COUNT_entry se52;
    tuple3_LL_L st21;
    COUNT_mRETWEET3_entry se53;
    tuple3_LL_L st22;
    COUNT_mRETWEET14_entry se54;
    tuple3_LL_L st23;
    COUNT_mRETWEET6_entry se55;
    COUNT_mRETWEET21_entry se56;
    tuple3_LL_L st24;
    COUNT_mRETWEET14_entry se57;
    tuple3_LL_L st25;
    COUNT_mRETWEET6_entry se58;
    tuple3_LL_L st26;
    COUNT_mRETWEET14_entry se59;
    COUNT_mRETWEET7_entry se60;
    COUNT_mRETWEET15_entry se61;
    COUNT_mRETWEET14_entry se62;
    COUNT_mRETWEET3_entry se63;
    tuple3_LL_L st27;
    COUNT_mRETWEET6_entry se64;
    tuple3_LL_L st28;
    COUNT_mRETWEET14_entry se65;
    COUNT_mRETWEET21_entry se66;
    COUNT_mRETWEET14_entry se67;
    COUNT_mRETWEET6_entry se68;
    COUNT_mRETWEET14_entry se69;
    COUNT_mRETWEET15_entry se70;
    tuple3_LL_L st29;
    COUNT_mRETWEET21_entry se71;
    tuple3_LL_L st30;
    COUNT_mRETWEET7_entry se72;
    COUNT_mFOLLOWER1_entry se73;
    tuple2_L_L st31;
    COUNT_mRETWEET3_mFOLLOWER1_entry se74;
    tuple2_L_L st32;
    tuple2_L_L st33;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se75;
    COUNT_mRETWEET21_entry se76;
    tuple2_L_L st34;
    tuple2_L_L st35;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se77;
    tuple2_L_L st36;
    COUNT_mRETWEET7_entry se78;
    COUNT_pRETWEET13_entry se79;
    COUNT_mRETWEET3_mFOLLOWER1_entry se80;
    tuple2_L_L st37;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se81;
    tuple2_L_L st38;
    COUNT_mRETWEET21_entry se82;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_entry se83;
    COUNT_pRETWEET13_entry se84;
    tuple3_LL_L st39;
    COUNT_mRETWEET21_entry se85;
    tuple3_LL_L st40;
    COUNT_mRETWEET7_entry se86;
    COUNT_mRETWEET21_entry se87;
    COUNT_mRETWEET7_entry se88;
  
    /* Data structures used for storing materialized views */
    COUNT_pRETWEET13_map COUNT_pRETWEET13;
    COUNT_mRETWEET3_map COUNT_mRETWEET3;
    COUNT_mRETWEET3_mFOLLOWER1_map COUNT_mRETWEET3_mFOLLOWER1;
    COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2_map COUNT_mRETWEET3_mFOLLOWER1_mRETWEET2;
    COUNT_mRETWEET6_map COUNT_mRETWEET6;
    COUNT_mRETWEET7_map COUNT_mRETWEET7;
    COUNT_mRETWEET14_map COUNT_mRETWEET14;
    COUNT_mRETWEET15_map COUNT_mRETWEET15;
    COUNT_mRETWEET21_map COUNT_mRETWEET21;
    COUNT_mFOLLOWER1_map COUNT_mFOLLOWER1;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c16;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c1;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c4;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c10;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c13;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c7;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c6;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c9;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c15;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c3;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c12;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c2;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c5;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c11;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c14;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c8;
    
  
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

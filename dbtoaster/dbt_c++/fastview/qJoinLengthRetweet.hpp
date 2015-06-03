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
    long COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID; long COUNT_mRETWEETRETWEET_TWEET_ID; long __av;  COUNT_mRETWEET3_mFOLLOWER1_entry* nxt; COUNT_mRETWEET3_mFOLLOWER1_entry* prv;
    explicit COUNT_mRETWEET3_mFOLLOWER1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = 0L; COUNT_mRETWEETRETWEET_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET3_mFOLLOWER1_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mRETWEETRETWEET_TWEET_ID = c1; __av = c2; }
    COUNT_mRETWEET3_mFOLLOWER1_entry(const COUNT_mRETWEET3_mFOLLOWER1_entry& other) : COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID ), COUNT_mRETWEETRETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_entry& modify(const long c0, const long c1) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c0; COUNT_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_entry& modify0(const long c0) { COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID = c0;  return *this; }
    FORCE_INLINE COUNT_mRETWEET3_mFOLLOWER1_entry& modify1(const long c1) { COUNT_mRETWEETRETWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET3_mFOLLOWER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      hash_combine(h, e.COUNT_mRETWEETRETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_entry& y) {
      return x.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID && x.COUNT_mRETWEETRETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET3_mFOLLOWER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET3_mFOLLOWER1_entry& x, const COUNT_mRETWEET3_mFOLLOWER1_entry& y) {
      return x.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn {
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
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn,false>,
    HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn,false>
  > COUNT_mRETWEET3_mFOLLOWER1_map;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_01;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0;
  typedef HashIndex<COUNT_mRETWEET3_mFOLLOWER1_entry,long,COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn,false> HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1;
  
  struct COUNT_mRETWEET7_entry {
    long COUNT_mRETWEETRETWEET_USER_ID; long Z_FOLLOWER_ID; long __av;  COUNT_mRETWEET7_entry* nxt; COUNT_mRETWEET7_entry* prv;
    explicit COUNT_mRETWEET7_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_USER_ID = 0L; Z_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET7_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_USER_ID = c0; Z_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mRETWEET7_entry(const COUNT_mRETWEET7_entry& other) : COUNT_mRETWEETRETWEET_USER_ID( other.COUNT_mRETWEETRETWEET_USER_ID ), Z_FOLLOWER_ID( other.Z_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET7_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_USER_ID = c0; Z_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET7_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_USER_ID = c0;  return *this; }
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
  struct COUNT_mRETWEET7_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      hash_combine(h, e.Z_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET7_entry& x, const COUNT_mRETWEET7_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID && x.Z_FOLLOWER_ID == y.Z_FOLLOWER_ID;
    }
  };
  
  struct COUNT_mRETWEET7_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET7_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET7_entry& x, const COUNT_mRETWEET7_entry& y) {
      return x.COUNT_mRETWEETRETWEET_USER_ID == y.COUNT_mRETWEETRETWEET_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET7_entry,long,
    HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey0_idxfn,false>
  > COUNT_mRETWEET7_map;
  typedef HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET7_map_01;
  typedef HashIndex<COUNT_mRETWEET7_entry,long,COUNT_mRETWEET7_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET7_map_0;
  
  struct COUNT_mRETWEET8_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long RETWEET_TWEET_ID_1; long __av;  COUNT_mRETWEET8_entry* nxt; COUNT_mRETWEET8_entry* prv;
    explicit COUNT_mRETWEET8_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; RETWEET_TWEET_ID_1 = 0L; __av = 0L; */ }
    explicit COUNT_mRETWEET8_entry(const long c0, const long c1, const long c2) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; RETWEET_TWEET_ID_1 = c1; __av = c2; }
    COUNT_mRETWEET8_entry(const COUNT_mRETWEET8_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), RETWEET_TWEET_ID_1( other.RETWEET_TWEET_ID_1 ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET8_entry& modify(const long c0, const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; RETWEET_TWEET_ID_1 = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET8_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
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
  struct COUNT_mRETWEET8_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET8_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.RETWEET_TWEET_ID_1);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET8_entry& x, const COUNT_mRETWEET8_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.RETWEET_TWEET_ID_1 == y.RETWEET_TWEET_ID_1;
    }
  };
  
  struct COUNT_mRETWEET8_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET8_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET8_entry& x, const COUNT_mRETWEET8_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET8_entry,long,
    HashIndex<COUNT_mRETWEET8_entry,long,COUNT_mRETWEET8_mapkey01_idxfn,true>,
    HashIndex<COUNT_mRETWEET8_entry,long,COUNT_mRETWEET8_mapkey0_idxfn,false>
  > COUNT_mRETWEET8_map;
  typedef HashIndex<COUNT_mRETWEET8_entry,long,COUNT_mRETWEET8_mapkey01_idxfn,true> HashIndex_COUNT_mRETWEET8_map_01;
  typedef HashIndex<COUNT_mRETWEET8_entry,long,COUNT_mRETWEET8_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET8_map_0;
  
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
  struct tuple4_LLL_L {
    long _1; long _2; long _3; long __av; tuple4_LLL_L* nxt; tuple4_LLL_L* prv;
    explicit tuple4_LLL_L() : nxt(nullptr), prv(nullptr) { }
    explicit tuple4_LLL_L(const long c1, const long c2, const long c3, long c__av=0L) : nxt(nullptr), prv(nullptr) { _1 = c1; _2 = c2; _3 = c3; __av = c__av;}
    int operator<(const tuple4_LLL_L &rhs) const { 
      if(this->_1!=rhs._1) return (this->_1<rhs._1);
      if(this->_2!=rhs._2) return (this->_2<rhs._2);
      if(this->_3!=rhs._3) return (this->_3<rhs._3);
      return 0;
    }
    int operator==(const tuple4_LLL_L &rhs) const { return ((this->_1==rhs._1) && (this->_2==rhs._2) && (this->_3==rhs._3)); }
    FORCE_INLINE tuple4_LLL_L& modify(const long c0, const long c1, const long c2, long c__av) { _1 = c0; _2 = c1; _3 = c2; __av = c__av; return *this; }
    static bool equals(const tuple4_LLL_L &x, const tuple4_LLL_L &y) { return ((x._1==y._1) && (x._2==y._2) && (x._3==y._3)); }
    static long hash(const tuple4_LLL_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      hash_combine(h, e._2);
      hash_combine(h, e._3);
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
      pb.add_map<COUNT_mRETWEET3_map>( "COUNT_mRETWEET3", COUNT_mRETWEET3 );
      pb.add_map<COUNT_mRETWEET3_mFOLLOWER1_map>( "COUNT_mRETWEET3_mFOLLOWER1", COUNT_mRETWEET3_mFOLLOWER1 );
      pb.add_map<COUNT_mRETWEET7_map>( "COUNT_mRETWEET7", COUNT_mRETWEET7 );
      pb.add_map<COUNT_mRETWEET8_map>( "COUNT_mRETWEET8", COUNT_mRETWEET8 );
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
    void on_insert_FOLLOWER( long follower_user_id,  long follower_follower_id) {
      BEGIN_TRIGGER(exec_stats,"insert_FOLLOWER")
      BEGIN_TRIGGER(ivc_stats,"insert_FOLLOWER")
	//std::vector<void*> r;
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
		//for(int i = 0; i < v1; i++) {
		//	api.useDiff(retweet_tweet_id_1, tweet_id_1, follower_user_id, follower_follower_id);
		//	void* rr[4];
		//	r.push_back(rr);
		//	rr[0] = &retweet_tweet_id_1;
		//	rr[1] = &tweet_id_1;
		//	rr[2] = &follower_user_id;
		//	rr[3] = &follower_follower_id;
            	//}
            COUNT.addOrDelOnZero(se1.modify(tweet_id_1,retweet_tweet_id_1,follower_user_id,follower_follower_id),v1);
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::hash(se4.modify0(follower_user_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0* i2 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0*>(COUNT_mRETWEET3_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::equals(se4, *e2)) {
            long count_mretweetretweet_tweet_id = e2->COUNT_mRETWEETRETWEET_TWEET_ID;
            long v2 = e2->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se3.modify(follower_user_id,follower_follower_id,count_mretweetretweet_tweet_id),v2);
          } while ((n2=n2->nxt));
        }
        COUNT_mRETWEET7.addOrDelOnZero(se5.modify(follower_user_id,follower_follower_id),1L);
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
          const HASH_RES_t h3 = COUNT_mFOLLOWER1_mapkey0_idxfn::hash(se7.modify0(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER1_map_0* i3 = static_cast<HashIndex_COUNT_mFOLLOWER1_map_0*>(COUNT_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mFOLLOWER1_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mFOLLOWER1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mFOLLOWER1_mapkey0_idxfn::equals(se7, *e3)) {
            long tweet_id_1 = e3->TWEET_ID_1;
            long retweet_tweet_id_1 = e3->RETWEET_TWEET_ID_1;
            long v3 = e3->__av;
            COUNT.addOrDelOnZero(se6.modify(tweet_id_1,retweet_tweet_id_1,follower_user_id,follower_follower_id),(v3 * -1L));
          } while ((n3=n3->nxt));
        }
        { //slice
          const HASH_RES_t h4 = COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::hash(se9.modify0(follower_user_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0* i4 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0*>(COUNT_mRETWEET3_mFOLLOWER1.index[1]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_0::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey0_idxfn::equals(se9, *e4)) {
            long count_mretweetretweet_tweet_id = e4->COUNT_mRETWEETRETWEET_TWEET_ID;
            long v4 = e4->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se8.modify(follower_user_id,follower_follower_id,count_mretweetretweet_tweet_id),(v4 * -1L));
          } while ((n4=n4->nxt));
        }
        COUNT_mRETWEET7.addOrDelOnZero(se10.modify(follower_user_id,follower_follower_id),-1L);
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
          const HASH_RES_t h5 = COUNT_mRETWEET3_mapkey2_idxfn::hash(se12.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_map_2* i5 = static_cast<HashIndex_COUNT_mRETWEET3_map_2*>(COUNT_mRETWEET3.index[1]);
          HashIndex_COUNT_mRETWEET3_map_2::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mRETWEET3_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mRETWEET3_mapkey2_idxfn::equals(se12, *e5)) {
            long z_user_id = e5->Z_USER_ID;
            long z_follower_id = e5->Z_FOLLOWER_ID;
            long v6 = e5->__av;
            add_to_temp_map(_c1, st1.modify(z_follower_id,z_user_id,retweet_tweet_id,v6));
          } while ((n5=n5->nxt));
        }
        { //slice
          const HASH_RES_t h6 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se13.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i6 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mRETWEET7_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se13, *e6)) {
            long z_follower_id = e6->Z_FOLLOWER_ID;
            long v7 = e6->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c1, st2.modify(z_follower_id,retweet_user_id,retweet_retweet_tweet_id,v7)) : voidFunc());
          } while ((n6=n6->nxt));
        }
        { //temp foreach
          tuple4_LLL_L* e7 = _c1.head;
          while(e7){
            long z_follower_id = e7->_1;
            long z_user_id = e7->_2;
            long tweet_id_1 = e7->_3;
            long v8 = e7->__av;
            COUNT.addOrDelOnZero(se11.modify(tweet_id_1,retweet_retweet_tweet_id,z_user_id,z_follower_id),v8);
            e7 = e7->nxt;
          }
        }
        { //slice
          const HASH_RES_t h8 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se15.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i7 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n7 = &(i7->buckets_[h8 % i7->size_]);
          COUNT_mRETWEET7_entry* e8;
          do if ((e8=n7->obj) && h8 == n7->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se15, *e8)) {
            long z_follower_id = e8->Z_FOLLOWER_ID;
            long v9 = e8->__av;
            { //slice
              const HASH_RES_t h7 = COUNT_mRETWEET8_mapkey0_idxfn::hash(se14.modify0(retweet_retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET8_map_0* i8 = static_cast<HashIndex_COUNT_mRETWEET8_map_0*>(COUNT_mRETWEET8.index[1]);
              HashIndex_COUNT_mRETWEET8_map_0::IdxNode* n8 = &(i8->buckets_[h7 % i8->size_]);
              COUNT_mRETWEET8_entry* e9;
              do if ((e9=n8->obj) && h7 == n8->hash && COUNT_mRETWEET8_mapkey0_idxfn::equals(se14, *e9)) {
                long retweet_tweet_id_1 = e9->RETWEET_TWEET_ID_1;
                long v10 = e9->__av;
                COUNT.addOrDelOnZero(se11.modify(retweet_retweet_tweet_id,retweet_tweet_id_1,retweet_user_id,z_follower_id),(v9 * v10));
              } while ((n8=n8->nxt));
            }
          } while ((n7=n7->nxt));
        }
        { //slice
          const HASH_RES_t h9 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se17.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i9 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n9 = &(i9->buckets_[h9 % i9->size_]);
          COUNT_mRETWEET7_entry* e10;
          do if ((e10=n9->obj) && h9 == n9->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se17, *e10)) {
            long z_follower_id = e10->Z_FOLLOWER_ID;
            long v11 = e10->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se16.modify(retweet_user_id,z_follower_id,retweet_retweet_tweet_id),v11);
          } while ((n9=n9->nxt));
        }
        _c2.clear();
        { //slice
          const HASH_RES_t h10 = COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::hash(se19.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1* i10 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1*>(COUNT_mRETWEET3_mFOLLOWER1.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1::IdxNode* n10 = &(i10->buckets_[h10 % i10->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e11;
          do if ((e11=n10->obj) && h10 == n10->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::equals(se19, *e11)) {
            long count_mfollowerfollower_user_id = e11->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v13 = e11->__av;
            add_to_temp_map(_c2, st3.modify(retweet_tweet_id,count_mfollowerfollower_user_id,v13));
          } while ((n10=n10->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c2, st4.modify(retweet_retweet_tweet_id,retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple3_LL_L* e12 = _c2.head;
          while(e12){
            long tweet_id_1 = e12->_1;
            long count_mfollowerfollower_user_id = e12->_2;
            long v14 = e12->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se18.modify(count_mfollowerfollower_user_id,tweet_id_1,retweet_retweet_tweet_id),v14);
            e12 = e12->nxt;
          }
        }
        { //slice
          const HASH_RES_t h11 = COUNT_mRETWEET8_mapkey0_idxfn::hash(se20.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET8_map_0* i11 = static_cast<HashIndex_COUNT_mRETWEET8_map_0*>(COUNT_mRETWEET8.index[1]);
          HashIndex_COUNT_mRETWEET8_map_0::IdxNode* n11 = &(i11->buckets_[h11 % i11->size_]);
          COUNT_mRETWEET8_entry* e13;
          do if ((e13=n11->obj) && h11 == n11->hash && COUNT_mRETWEET8_mapkey0_idxfn::equals(se20, *e13)) {
            long retweet_tweet_id_1 = e13->RETWEET_TWEET_ID_1;
            long v15 = e13->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se18.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id_1),v15);
          } while ((n11=n11->nxt));
        }
        COUNT_mRETWEET3_mFOLLOWER1.addOrDelOnZero(se21.modify(retweet_user_id,retweet_retweet_tweet_id),1L);
        COUNT_mRETWEET8.addOrDelOnZero(se22.modify(retweet_tweet_id,retweet_retweet_tweet_id),1L);
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
          const HASH_RES_t h12 = COUNT_mRETWEET3_mapkey2_idxfn::hash(se24.modify2(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_map_2* i12 = static_cast<HashIndex_COUNT_mRETWEET3_map_2*>(COUNT_mRETWEET3.index[1]);
          HashIndex_COUNT_mRETWEET3_map_2::IdxNode* n12 = &(i12->buckets_[h12 % i12->size_]);
          COUNT_mRETWEET3_entry* e14;
          do if ((e14=n12->obj) && h12 == n12->hash && COUNT_mRETWEET3_mapkey2_idxfn::equals(se24, *e14)) {
            long z_user_id = e14->Z_USER_ID;
            long z_follower_id = e14->Z_FOLLOWER_ID;
            long v17 = e14->__av;
            add_to_temp_map(_c3, st5.modify(z_follower_id,z_user_id,retweet_tweet_id,(v17 * -1L)));
          } while ((n12=n12->nxt));
        }
        { //slice
          const HASH_RES_t h13 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se25.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i13 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n13 = &(i13->buckets_[h13 % i13->size_]);
          COUNT_mRETWEET7_entry* e15;
          do if ((e15=n13->obj) && h13 == n13->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se25, *e15)) {
            long z_follower_id = e15->Z_FOLLOWER_ID;
            long v18 = e15->__av;
            (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c3, st6.modify(z_follower_id,retweet_user_id,retweet_retweet_tweet_id,v18)) : voidFunc());
          } while ((n13=n13->nxt));
        }
        { //temp foreach
          tuple4_LLL_L* e16 = _c3.head;
          while(e16){
            long z_follower_id = e16->_1;
            long z_user_id = e16->_2;
            long tweet_id_1 = e16->_3;
            long v19 = e16->__av;
            COUNT.addOrDelOnZero(se23.modify(tweet_id_1,retweet_retweet_tweet_id,z_user_id,z_follower_id),v19);
            e16 = e16->nxt;
          }
        }
        { //slice
          const HASH_RES_t h15 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se27.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i14 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n14 = &(i14->buckets_[h15 % i14->size_]);
          COUNT_mRETWEET7_entry* e17;
          do if ((e17=n14->obj) && h15 == n14->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se27, *e17)) {
            long z_follower_id = e17->Z_FOLLOWER_ID;
            long v20 = e17->__av;
            { //slice
              const HASH_RES_t h14 = COUNT_mRETWEET8_mapkey0_idxfn::hash(se26.modify0(retweet_retweet_tweet_id));
              const HashIndex_COUNT_mRETWEET8_map_0* i15 = static_cast<HashIndex_COUNT_mRETWEET8_map_0*>(COUNT_mRETWEET8.index[1]);
              HashIndex_COUNT_mRETWEET8_map_0::IdxNode* n15 = &(i15->buckets_[h14 % i15->size_]);
              COUNT_mRETWEET8_entry* e18;
              do if ((e18=n15->obj) && h14 == n15->hash && COUNT_mRETWEET8_mapkey0_idxfn::equals(se26, *e18)) {
                long retweet_tweet_id_1 = e18->RETWEET_TWEET_ID_1;
                long v21 = e18->__av;
                COUNT.addOrDelOnZero(se23.modify(retweet_retweet_tweet_id,retweet_tweet_id_1,retweet_user_id,z_follower_id),(v20 * (v21 * -1L)));
              } while ((n15=n15->nxt));
            }
          } while ((n14=n14->nxt));
        }
        { //slice
          const HASH_RES_t h16 = COUNT_mRETWEET7_mapkey0_idxfn::hash(se29.modify0(retweet_user_id));
          const HashIndex_COUNT_mRETWEET7_map_0* i16 = static_cast<HashIndex_COUNT_mRETWEET7_map_0*>(COUNT_mRETWEET7.index[1]);
          HashIndex_COUNT_mRETWEET7_map_0::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          COUNT_mRETWEET7_entry* e19;
          do if ((e19=n16->obj) && h16 == n16->hash && COUNT_mRETWEET7_mapkey0_idxfn::equals(se29, *e19)) {
            long z_follower_id = e19->Z_FOLLOWER_ID;
            long v22 = e19->__av;
            COUNT_mRETWEET3.addOrDelOnZero(se28.modify(retweet_user_id,z_follower_id,retweet_retweet_tweet_id),(v22 * -1L));
          } while ((n16=n16->nxt));
        }
        _c4.clear();
        { //slice
          const HASH_RES_t h17 = COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::hash(se31.modify1(retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1* i17 = static_cast<HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1*>(COUNT_mRETWEET3_mFOLLOWER1.index[2]);
          HashIndex_COUNT_mRETWEET3_mFOLLOWER1_map_1::IdxNode* n17 = &(i17->buckets_[h17 % i17->size_]);
          COUNT_mRETWEET3_mFOLLOWER1_entry* e20;
          do if ((e20=n17->obj) && h17 == n17->hash && COUNT_mRETWEET3_mFOLLOWER1_mapkey1_idxfn::equals(se31, *e20)) {
            long count_mfollowerfollower_user_id = e20->COUNT_mRETWEET3_mFOLLOWERFOLLOWER_USER_ID;
            long v24 = e20->__av;
            add_to_temp_map(_c4, st7.modify(retweet_tweet_id,count_mfollowerfollower_user_id,(v24 * -1L)));
          } while ((n17=n17->nxt));
        }
        (/*if */(retweet_tweet_id == retweet_retweet_tweet_id) ? add_to_temp_map(_c4, st8.modify(retweet_retweet_tweet_id,retweet_user_id,1L)) : voidFunc());
        { //temp foreach
          tuple3_LL_L* e21 = _c4.head;
          while(e21){
            long tweet_id_1 = e21->_1;
            long count_mfollowerfollower_user_id = e21->_2;
            long v25 = e21->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se30.modify(count_mfollowerfollower_user_id,tweet_id_1,retweet_retweet_tweet_id),v25);
            e21 = e21->nxt;
          }
        }
        { //slice
          const HASH_RES_t h18 = COUNT_mRETWEET8_mapkey0_idxfn::hash(se32.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET8_map_0* i18 = static_cast<HashIndex_COUNT_mRETWEET8_map_0*>(COUNT_mRETWEET8.index[1]);
          HashIndex_COUNT_mRETWEET8_map_0::IdxNode* n18 = &(i18->buckets_[h18 % i18->size_]);
          COUNT_mRETWEET8_entry* e22;
          do if ((e22=n18->obj) && h18 == n18->hash && COUNT_mRETWEET8_mapkey0_idxfn::equals(se32, *e22)) {
            long retweet_tweet_id_1 = e22->RETWEET_TWEET_ID_1;
            long v26 = e22->__av;
            COUNT_mFOLLOWER1.addOrDelOnZero(se30.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id_1),(v26 * -1L));
          } while ((n18=n18->nxt));
        }
        COUNT_mRETWEET3_mFOLLOWER1.addOrDelOnZero(se33.modify(retweet_user_id,retweet_retweet_tweet_id),-1L);
        COUNT_mRETWEET8.addOrDelOnZero(se34.modify(retweet_tweet_id,retweet_retweet_tweet_id),-1L);
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
    COUNT_mRETWEET7_entry se5;
    COUNT_entry se6;
    COUNT_mFOLLOWER1_entry se7;
    COUNT_mRETWEET3_entry se8;
    COUNT_mRETWEET3_mFOLLOWER1_entry se9;
    COUNT_mRETWEET7_entry se10;
    COUNT_entry se11;
    tuple4_LLL_L st1;
    COUNT_mRETWEET3_entry se12;
    tuple4_LLL_L st2;
    COUNT_mRETWEET7_entry se13;
    COUNT_mRETWEET8_entry se14;
    COUNT_mRETWEET7_entry se15;
    COUNT_mRETWEET3_entry se16;
    COUNT_mRETWEET7_entry se17;
    COUNT_mFOLLOWER1_entry se18;
    tuple3_LL_L st3;
    COUNT_mRETWEET3_mFOLLOWER1_entry se19;
    tuple3_LL_L st4;
    COUNT_mRETWEET8_entry se20;
    COUNT_mRETWEET3_mFOLLOWER1_entry se21;
    COUNT_mRETWEET8_entry se22;
    COUNT_entry se23;
    tuple4_LLL_L st5;
    COUNT_mRETWEET3_entry se24;
    tuple4_LLL_L st6;
    COUNT_mRETWEET7_entry se25;
    COUNT_mRETWEET8_entry se26;
    COUNT_mRETWEET7_entry se27;
    COUNT_mRETWEET3_entry se28;
    COUNT_mRETWEET7_entry se29;
    COUNT_mFOLLOWER1_entry se30;
    tuple3_LL_L st7;
    COUNT_mRETWEET3_mFOLLOWER1_entry se31;
    tuple3_LL_L st8;
    COUNT_mRETWEET8_entry se32;
    COUNT_mRETWEET3_mFOLLOWER1_entry se33;
    COUNT_mRETWEET8_entry se34;
  
    /* Data structures used for storing materialized views */
    COUNT_mRETWEET3_map COUNT_mRETWEET3;
    COUNT_mRETWEET3_mFOLLOWER1_map COUNT_mRETWEET3_mFOLLOWER1;
    COUNT_mRETWEET7_map COUNT_mRETWEET7;
    COUNT_mRETWEET8_map COUNT_mRETWEET8;
    COUNT_mFOLLOWER1_map COUNT_mFOLLOWER1;
    MultiHashMap<tuple4_LLL_L,long,HashIndex<tuple4_LLL_L,long> > _c1;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c4;
    MultiHashMap<tuple4_LLL_L,long,HashIndex<tuple4_LLL_L,long> > _c3;
    MultiHashMap<tuple3_LL_L,long,HashIndex<tuple3_LL_L,long> > _c2;
    
  
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

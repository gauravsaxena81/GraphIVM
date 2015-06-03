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
    long Z_TWEET_ID; long COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID; long __av;  COUNT_mFOLLOWER5_entry* nxt; COUNT_mFOLLOWER5_entry* prv;
    explicit COUNT_mFOLLOWER5_entry() : nxt(nullptr), prv(nullptr) { /*Z_TWEET_ID = 0L; COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER5_entry(const long c0, const long c1, const long c2) { Z_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c1; __av = c2; }
    COUNT_mFOLLOWER5_entry(const COUNT_mFOLLOWER5_entry& other) : Z_TWEET_ID( other.Z_TWEET_ID ), COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID( other.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER5_entry& modify(const long c0, const long c1) { Z_TWEET_ID = c0; COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER5_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, Z_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mFOLLOWER5_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER5_entry& e) {
      size_t h = 0;
      hash_combine(h, e.Z_TWEET_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER5_entry& x, const COUNT_mFOLLOWER5_entry& y) {
      return x.Z_TWEET_ID == y.Z_TWEET_ID && x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER5_mapkey1_idxfn {
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
    HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey1_idxfn,false>
  > COUNT_mFOLLOWER5_map;
  typedef HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER5_map_01;
  typedef HashIndex<COUNT_mFOLLOWER5_entry,long,COUNT_mFOLLOWER5_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER5_map_1;
  
  struct COUNT_mFOLLOWER10_entry {
    long A_USER_ID; long COUNT_mFOLLOWERFOLLOWER_USER_ID; long __av;  COUNT_mFOLLOWER10_entry* nxt; COUNT_mFOLLOWER10_entry* prv;
    explicit COUNT_mFOLLOWER10_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER10_entry(const long c0, const long c1, const long c2) { A_USER_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; __av = c2; }
    COUNT_mFOLLOWER10_entry(const COUNT_mFOLLOWER10_entry& other) : A_USER_ID( other.A_USER_ID ), COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER10_entry& modify(const long c0, const long c1) { A_USER_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER10_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER10_entry& modify0(const long c0) { A_USER_ID = c0;  return *this; }
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
  struct COUNT_mFOLLOWER10_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER10_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER10_entry& x, const COUNT_mFOLLOWER10_entry& y) {
      return x.A_USER_ID == y.A_USER_ID && x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER10_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER10_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER10_entry& x, const COUNT_mFOLLOWER10_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER10_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER10_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER10_entry& x, const COUNT_mFOLLOWER10_entry& y) {
      return x.A_USER_ID == y.A_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER10_entry,long,
    HashIndex<COUNT_mFOLLOWER10_entry,long,COUNT_mFOLLOWER10_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER10_entry,long,COUNT_mFOLLOWER10_mapkey1_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER10_entry,long,COUNT_mFOLLOWER10_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER10_map;
  typedef HashIndex<COUNT_mFOLLOWER10_entry,long,COUNT_mFOLLOWER10_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER10_map_01;
  typedef HashIndex<COUNT_mFOLLOWER10_entry,long,COUNT_mFOLLOWER10_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER10_map_1;
  typedef HashIndex<COUNT_mFOLLOWER10_entry,long,COUNT_mFOLLOWER10_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER10_map_0;
  
  struct COUNT_mFOLLOWER11_entry {
    long COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID; long Z_TWEET_ID; long __av;  COUNT_mFOLLOWER11_entry* nxt; COUNT_mFOLLOWER11_entry* prv;
    explicit COUNT_mFOLLOWER11_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = 0L; Z_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER11_entry(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1; __av = c2; }
    COUNT_mFOLLOWER11_entry(const COUNT_mFOLLOWER11_entry& other) : COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID( other.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID ), Z_TWEET_ID( other.Z_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER11_entry& modify(const long c0, const long c1) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER11_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0;  return *this; }
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
  struct COUNT_mFOLLOWER11_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      hash_combine(h, e.Z_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER11_entry& x, const COUNT_mFOLLOWER11_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID && x.Z_TWEET_ID == y.Z_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER11_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER11_entry& x, const COUNT_mFOLLOWER11_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER11_entry,long,
    HashIndex<COUNT_mFOLLOWER11_entry,long,COUNT_mFOLLOWER11_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER11_entry,long,COUNT_mFOLLOWER11_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER11_map;
  typedef HashIndex<COUNT_mFOLLOWER11_entry,long,COUNT_mFOLLOWER11_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER11_map_01;
  typedef HashIndex<COUNT_mFOLLOWER11_entry,long,COUNT_mFOLLOWER11_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER11_map_0;
  
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
    data_t(): tlq_t(), _c1(16U), _c10(16U), _c4(16U), _c7(16U), _c6(16U), _c9(16U), _c3(16U), _c12(16U), _c2(16U), _c11(16U), _c5(16U), _c8(16U) {
      
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
      pb.add_map<COUNT_mFOLLOWER10_map>( "COUNT_mFOLLOWER10", COUNT_mFOLLOWER10 );
      pb.add_map<COUNT_mFOLLOWER11_map>( "COUNT_mFOLLOWER11", COUNT_mFOLLOWER11 );
  
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
      exec_stats->register_probe(4, "insert_FOLLOWER_s4");
      exec_stats->register_probe(5, "insert_FOLLOWER_s5");
      exec_stats->register_probe(6, "delete_FOLLOWER_s0");
      exec_stats->register_probe(7, "delete_FOLLOWER_s1");
      exec_stats->register_probe(8, "delete_FOLLOWER_s2");
      exec_stats->register_probe(9, "delete_FOLLOWER_s3");
      exec_stats->register_probe(10, "delete_FOLLOWER_s4");
      exec_stats->register_probe(11, "delete_FOLLOWER_s5");
      exec_stats->register_probe(12, "insert_TWEET_s0");
      exec_stats->register_probe(13, "insert_TWEET_s1");
      exec_stats->register_probe(14, "insert_TWEET_s2");
      exec_stats->register_probe(15, "insert_TWEET_s3");
      exec_stats->register_probe(16, "delete_TWEET_s0");
      exec_stats->register_probe(17, "delete_TWEET_s1");
      exec_stats->register_probe(18, "delete_TWEET_s2");
      exec_stats->register_probe(19, "delete_TWEET_s3");
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
          const HASH_RES_t h1 = COUNT_mFOLLOWER5_mapkey1_idxfn::hash(se2.modify1(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_1* i1 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_1*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_1::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mFOLLOWER5_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mFOLLOWER5_mapkey1_idxfn::equals(se2, *e1)) {
            long z_tweet_id = e1->Z_TWEET_ID;
            long v2 = e1->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c1, st1.modify(z_tweet_id,v2)) : voidFunc());
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se3.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i2 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mFOLLOWER11_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se3, *e2)) {
            long z_tweet_id = e2->Z_TWEET_ID;
            long v3 = e2->__av;
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c1, st2.modify(z_tweet_id,v3)) : voidFunc());
          } while ((n2=n2->nxt));
        }
        { //slice
          const HASH_RES_t h3 = COUNT_mFOLLOWER2_mapkey0_idxfn::hash(se4.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER2_map_0* i3 = static_cast<HashIndex_COUNT_mFOLLOWER2_map_0*>(COUNT_mFOLLOWER2.index[1]);
          HashIndex_COUNT_mFOLLOWER2_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mFOLLOWER2_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mFOLLOWER2_mapkey0_idxfn::equals(se4, *e3)) {
            long z_tweet_id = e3->Z_TWEET_ID;
            long v4 = e3->__av;
            add_to_temp_map(_c1, st3.modify(z_tweet_id,v4));
          } while ((n3=n3->nxt));
        }
        { //slice
          const HASH_RES_t h4 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se6.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i4 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mFOLLOWER11_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se6, *e4)) {
            long z_tweet_id = e4->Z_TWEET_ID;
            long v5 = e4->__av;
            add_to_temp_map(_c1, st4.modify(z_tweet_id,(v5 * COUNT_mFOLLOWER4.getValueOrDefault(se5.modify(follower_follower_id,follower_user_id)))));
          } while ((n4=n4->nxt));
        }
        { //temp foreach
          tuple2_L_L* e5 = _c1.head;
          while(e5){
            long z_tweet_id = e5->_1;
            long v6 = e5->__av;
            COUNT.addOrDelOnZero(se1.modify(follower_user_id,z_tweet_id),v6);
            e5 = e5->nxt;
          }
        }
        { //slice
          const HASH_RES_t h7 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se9.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i5 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n5 = &(i5->buckets_[h7 % i5->size_]);
          COUNT_mFOLLOWER4_entry* e6;
          do if ((e6=n5->obj) && h7 == n5->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se9, *e6)) {
            long a_user_id = e6->A_USER_ID;
            long v7 = e6->__av;
            _c2.clear();
            { //slice
              const HASH_RES_t h5 = COUNT_mFOLLOWER5_mapkey1_idxfn::hash(se7.modify1(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER5_map_1* i6 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_1*>(COUNT_mFOLLOWER5.index[1]);
              HashIndex_COUNT_mFOLLOWER5_map_1::IdxNode* n6 = &(i6->buckets_[h5 % i6->size_]);
              COUNT_mFOLLOWER5_entry* e7;
              do if ((e7=n6->obj) && h5 == n6->hash && COUNT_mFOLLOWER5_mapkey1_idxfn::equals(se7, *e7)) {
                long z_tweet_id = e7->Z_TWEET_ID;
                long v9 = e7->__av;
                add_to_temp_map(_c2, st5.modify(z_tweet_id,v9));
              } while ((n6=n6->nxt));
            }
            { //slice
              const HASH_RES_t h6 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se8.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i7 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n7 = &(i7->buckets_[h6 % i7->size_]);
              COUNT_mFOLLOWER11_entry* e8;
              do if ((e8=n7->obj) && h6 == n7->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se8, *e8)) {
                long z_tweet_id = e8->Z_TWEET_ID;
                long v10 = e8->__av;
                (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c2, st6.modify(z_tweet_id,v10)) : voidFunc());
              } while ((n7=n7->nxt));
            }
            { //temp foreach
              tuple2_L_L* e9 = _c2.head;
              while(e9){
                long z_tweet_id = e9->_1;
                long v11 = e9->__av;
                COUNT.addOrDelOnZero(se1.modify(a_user_id,z_tweet_id),(v7 * v11));
                e9 = e9->nxt;
              }
            }
          } while ((n5=n5->nxt));
        }
        { //slice
          const HASH_RES_t h9 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se11.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i8 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n8 = &(i8->buckets_[h9 % i8->size_]);
          COUNT_mFOLLOWER10_entry* e10;
          do if ((e10=n8->obj) && h9 == n8->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se11, *e10)) {
            long a_user_id = e10->A_USER_ID;
            long v12 = e10->__av;
            { //slice
              const HASH_RES_t h8 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se10.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i9 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n9 = &(i9->buckets_[h8 % i9->size_]);
              COUNT_mFOLLOWER11_entry* e11;
              do if ((e11=n9->obj) && h8 == n9->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se10, *e11)) {
                long z_tweet_id = e11->Z_TWEET_ID;
                long v13 = e11->__av;
                COUNT.addOrDelOnZero(se1.modify(a_user_id,z_tweet_id),(v12 * v13));
              } while ((n9=n9->nxt));
            }
          } while ((n8=n8->nxt));
        }
        _c3.clear();
        { //slice
          const HASH_RES_t h10 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se13.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i10 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n10 = &(i10->buckets_[h10 % i10->size_]);
          COUNT_mFOLLOWER4_entry* e12;
          do if ((e12=n10->obj) && h10 == n10->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se13, *e12)) {
            long count_mtweettweet_user_id = e12->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v15 = e12->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c3, st7.modify(count_mtweettweet_user_id,v15)) : voidFunc());
          } while ((n10=n10->nxt));
        }
        (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c3, st8.modify(follower_follower_id,1L)) : voidFunc());
        { //slice
          const HASH_RES_t h11 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se14.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER10_map_0* i11 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
          HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n11 = &(i11->buckets_[h11 % i11->size_]);
          COUNT_mFOLLOWER10_entry* e13;
          do if ((e13=n11->obj) && h11 == n11->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se14, *e13)) {
            long count_mtweettweet_user_id = e13->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v16 = e13->__av;
            add_to_temp_map(_c3, st9.modify(count_mtweettweet_user_id,v16));
          } while ((n11=n11->nxt));
        }
        add_to_temp_map(_c3, st10.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se15.modify(follower_follower_id,follower_user_id))));
        { //temp foreach
          tuple2_L_L* e14 = _c3.head;
          while(e14){
            long count_mtweettweet_user_id = e14->_1;
            long v17 = e14->__av;
            COUNT_mTWEET1.addOrDelOnZero(se12.modify(follower_user_id,count_mtweettweet_user_id),v17);
            e14 = e14->nxt;
          }
        }
        { //slice
          const HASH_RES_t h13 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se17.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i12 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n12 = &(i12->buckets_[h13 % i12->size_]);
          COUNT_mFOLLOWER4_entry* e15;
          do if ((e15=n12->obj) && h13 == n12->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se17, *e15)) {
            long a_user_id = e15->A_USER_ID;
            long v18 = e15->__av;
            _c4.clear();
            { //slice
              const HASH_RES_t h12 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se16.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i13 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n13 = &(i13->buckets_[h12 % i13->size_]);
              COUNT_mFOLLOWER4_entry* e16;
              do if ((e16=n13->obj) && h12 == n13->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se16, *e16)) {
                long count_mtweettweet_user_id = e16->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v20 = e16->__av;
                add_to_temp_map(_c4, st11.modify(count_mtweettweet_user_id,v20));
              } while ((n13=n13->nxt));
            }
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c4, st12.modify(follower_follower_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e17 = _c4.head;
              while(e17){
                long count_mtweettweet_user_id = e17->_1;
                long v21 = e17->__av;
                COUNT_mTWEET1.addOrDelOnZero(se12.modify(a_user_id,count_mtweettweet_user_id),(v18 * v21));
                e17 = e17->nxt;
              }
            }
          } while ((n12=n12->nxt));
        }
        { //slice
          const HASH_RES_t h14 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se18.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i14 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n14 = &(i14->buckets_[h14 % i14->size_]);
          COUNT_mFOLLOWER10_entry* e18;
          do if ((e18=n14->obj) && h14 == n14->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se18, *e18)) {
            long a_user_id = e18->A_USER_ID;
            long v22 = e18->__av;
            COUNT_mTWEET1.addOrDelOnZero(se12.modify(a_user_id,follower_follower_id),v22);
          } while ((n14=n14->nxt));
        }
        _c5.clear();
        { //slice
          const HASH_RES_t h15 = COUNT_mFOLLOWER5_mapkey1_idxfn::hash(se20.modify1(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_1* i15 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_1*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_1::IdxNode* n15 = &(i15->buckets_[h15 % i15->size_]);
          COUNT_mFOLLOWER5_entry* e19;
          do if ((e19=n15->obj) && h15 == n15->hash && COUNT_mFOLLOWER5_mapkey1_idxfn::equals(se20, *e19)) {
            long z_tweet_id = e19->Z_TWEET_ID;
            long v24 = e19->__av;
            add_to_temp_map(_c5, st13.modify(z_tweet_id,v24));
          } while ((n15=n15->nxt));
        }
        { //slice
          const HASH_RES_t h16 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se21.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i16 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          COUNT_mFOLLOWER11_entry* e20;
          do if ((e20=n16->obj) && h16 == n16->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se21, *e20)) {
            long z_tweet_id = e20->Z_TWEET_ID;
            long v25 = e20->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c5, st14.modify(z_tweet_id,v25)) : voidFunc());
          } while ((n16=n16->nxt));
        }
        { //temp foreach
          tuple2_L_L* e21 = _c5.head;
          while(e21){
            long z_tweet_id = e21->_1;
            long v26 = e21->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se19.modify(follower_user_id,z_tweet_id),v26);
            e21 = e21->nxt;
          }
        }
        { //slice
          const HASH_RES_t h18 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se23.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i17 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n17 = &(i17->buckets_[h18 % i17->size_]);
          COUNT_mFOLLOWER4_entry* e22;
          do if ((e22=n17->obj) && h18 == n17->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se23, *e22)) {
            long count_mfollowerfollower_follower_id = e22->A_USER_ID;
            long v27 = e22->__av;
            { //slice
              const HASH_RES_t h17 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se22.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i18 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n18 = &(i18->buckets_[h17 % i18->size_]);
              COUNT_mFOLLOWER11_entry* e23;
              do if ((e23=n18->obj) && h17 == n18->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se22, *e23)) {
                long z_tweet_id = e23->Z_TWEET_ID;
                long v28 = e23->__av;
                COUNT_mFOLLOWER2.addOrDelOnZero(se19.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v27 * v28));
              } while ((n18=n18->nxt));
            }
          } while ((n17=n17->nxt));
        }
        { //slice
          const HASH_RES_t h19 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se25.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i19 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n19 = &(i19->buckets_[h19 % i19->size_]);
          COUNT_mFOLLOWER11_entry* e24;
          do if ((e24=n19->obj) && h19 == n19->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se25, *e24)) {
            long z_tweet_id = e24->Z_TWEET_ID;
            long v29 = e24->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se24.modify(z_tweet_id,follower_user_id),v29);
          } while ((n19=n19->nxt));
        }
        _c6.clear();
        { //slice
          const HASH_RES_t h20 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se27.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i20 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n20 = &(i20->buckets_[h20 % i20->size_]);
          COUNT_mFOLLOWER4_entry* e25;
          do if ((e25=n20->obj) && h20 == n20->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se27, *e25)) {
            long count_mfollowerfollower_user_id = e25->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v31 = e25->__av;
            add_to_temp_map(_c6, st15.modify(count_mfollowerfollower_user_id,v31));
          } while ((n20=n20->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c6, st16.modify(follower_follower_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e26 = _c6.head;
          while(e26){
            long count_mfollowerfollower_user_id = e26->_1;
            long v32 = e26->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se26.modify(follower_user_id,count_mfollowerfollower_user_id),v32);
            e26 = e26->nxt;
          }
        }
        { //slice
          const HASH_RES_t h21 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se28.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i21 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n21 = &(i21->buckets_[h21 % i21->size_]);
          COUNT_mFOLLOWER4_entry* e27;
          do if ((e27=n21->obj) && h21 == n21->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se28, *e27)) {
            long a_user_id = e27->A_USER_ID;
            long v33 = e27->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se26.modify(a_user_id,follower_follower_id),v33);
          } while ((n21=n21->nxt));
        }
        COUNT_mFOLLOWER4.addOrDelOnZero(se29.modify(follower_user_id,follower_follower_id),1L);
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
        _c7.clear();
        { //slice
          const HASH_RES_t h22 = COUNT_mFOLLOWER2_mapkey0_idxfn::hash(se31.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER2_map_0* i22 = static_cast<HashIndex_COUNT_mFOLLOWER2_map_0*>(COUNT_mFOLLOWER2.index[1]);
          HashIndex_COUNT_mFOLLOWER2_map_0::IdxNode* n22 = &(i22->buckets_[h22 % i22->size_]);
          COUNT_mFOLLOWER2_entry* e28;
          do if ((e28=n22->obj) && h22 == n22->hash && COUNT_mFOLLOWER2_mapkey0_idxfn::equals(se31, *e28)) {
            long z_tweet_id = e28->Z_TWEET_ID;
            long v35 = e28->__av;
            add_to_temp_map(_c7, st17.modify(z_tweet_id,(v35 * -1L)));
          } while ((n22=n22->nxt));
        }
        { //slice
          const HASH_RES_t h23 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se32.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i23 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n23 = &(i23->buckets_[h23 % i23->size_]);
          COUNT_mFOLLOWER11_entry* e29;
          do if ((e29=n23->obj) && h23 == n23->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se32, *e29)) {
            long z_tweet_id = e29->Z_TWEET_ID;
            long v36 = e29->__av;
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c7, st18.modify(z_tweet_id,(v36 * -1L))) : voidFunc());
          } while ((n23=n23->nxt));
        }
        { //slice
          const HASH_RES_t h24 = COUNT_mFOLLOWER5_mapkey1_idxfn::hash(se33.modify1(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_1* i24 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_1*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_1::IdxNode* n24 = &(i24->buckets_[h24 % i24->size_]);
          COUNT_mFOLLOWER5_entry* e30;
          do if ((e30=n24->obj) && h24 == n24->hash && COUNT_mFOLLOWER5_mapkey1_idxfn::equals(se33, *e30)) {
            long z_tweet_id = e30->Z_TWEET_ID;
            long v37 = e30->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c7, st19.modify(z_tweet_id,v37)) : voidFunc());
          } while ((n24=n24->nxt));
        }
        { //slice
          const HASH_RES_t h25 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se35.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i25 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n25 = &(i25->buckets_[h25 % i25->size_]);
          COUNT_mFOLLOWER11_entry* e31;
          do if ((e31=n25->obj) && h25 == n25->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se35, *e31)) {
            long z_tweet_id = e31->Z_TWEET_ID;
            long v38 = e31->__av;
            add_to_temp_map(_c7, st20.modify(z_tweet_id,(v38 * COUNT_mFOLLOWER4.getValueOrDefault(se34.modify(follower_follower_id,follower_user_id)))));
          } while ((n25=n25->nxt));
        }
        { //temp foreach
          tuple2_L_L* e32 = _c7.head;
          while(e32){
            long z_tweet_id = e32->_1;
            long v39 = e32->__av;
            COUNT.addOrDelOnZero(se30.modify(follower_user_id,z_tweet_id),v39);
            e32 = e32->nxt;
          }
        }
        { //slice
          const HASH_RES_t h28 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se38.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i26 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n26 = &(i26->buckets_[h28 % i26->size_]);
          COUNT_mFOLLOWER4_entry* e33;
          do if ((e33=n26->obj) && h28 == n26->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se38, *e33)) {
            long a_user_id = e33->A_USER_ID;
            long v40 = e33->__av;
            _c8.clear();
            { //slice
              const HASH_RES_t h26 = COUNT_mFOLLOWER5_mapkey1_idxfn::hash(se36.modify1(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER5_map_1* i27 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_1*>(COUNT_mFOLLOWER5.index[1]);
              HashIndex_COUNT_mFOLLOWER5_map_1::IdxNode* n27 = &(i27->buckets_[h26 % i27->size_]);
              COUNT_mFOLLOWER5_entry* e34;
              do if ((e34=n27->obj) && h26 == n27->hash && COUNT_mFOLLOWER5_mapkey1_idxfn::equals(se36, *e34)) {
                long z_tweet_id = e34->Z_TWEET_ID;
                long v42 = e34->__av;
                add_to_temp_map(_c8, st21.modify(z_tweet_id,(v42 * -1L)));
              } while ((n27=n27->nxt));
            }
            { //slice
              const HASH_RES_t h27 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se37.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i28 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n28 = &(i28->buckets_[h27 % i28->size_]);
              COUNT_mFOLLOWER11_entry* e35;
              do if ((e35=n28->obj) && h27 == n28->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se37, *e35)) {
                long z_tweet_id = e35->Z_TWEET_ID;
                long v43 = e35->__av;
                (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c8, st22.modify(z_tweet_id,v43)) : voidFunc());
              } while ((n28=n28->nxt));
            }
            { //temp foreach
              tuple2_L_L* e36 = _c8.head;
              while(e36){
                long z_tweet_id = e36->_1;
                long v44 = e36->__av;
                COUNT.addOrDelOnZero(se30.modify(a_user_id,z_tweet_id),(v40 * v44));
                e36 = e36->nxt;
              }
            }
          } while ((n26=n26->nxt));
        }
        { //slice
          const HASH_RES_t h30 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se40.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i29 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n29 = &(i29->buckets_[h30 % i29->size_]);
          COUNT_mFOLLOWER10_entry* e37;
          do if ((e37=n29->obj) && h30 == n29->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se40, *e37)) {
            long a_user_id = e37->A_USER_ID;
            long v45 = e37->__av;
            { //slice
              const HASH_RES_t h29 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se39.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i30 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n30 = &(i30->buckets_[h29 % i30->size_]);
              COUNT_mFOLLOWER11_entry* e38;
              do if ((e38=n30->obj) && h29 == n30->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se39, *e38)) {
                long z_tweet_id = e38->Z_TWEET_ID;
                long v46 = e38->__av;
                COUNT.addOrDelOnZero(se30.modify(a_user_id,z_tweet_id),(v45 * (v46 * -1L)));
              } while ((n30=n30->nxt));
            }
          } while ((n29=n29->nxt));
        }
        _c9.clear();
        { //slice
          const HASH_RES_t h31 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se42.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER10_map_0* i31 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
          HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n31 = &(i31->buckets_[h31 % i31->size_]);
          COUNT_mFOLLOWER10_entry* e39;
          do if ((e39=n31->obj) && h31 == n31->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se42, *e39)) {
            long count_mtweettweet_user_id = e39->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v48 = e39->__av;
            add_to_temp_map(_c9, st23.modify(count_mtweettweet_user_id,(v48 * -1L)));
          } while ((n31=n31->nxt));
        }
        (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c9, st24.modify(follower_follower_id,-1L)) : voidFunc());
        { //slice
          const HASH_RES_t h32 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se43.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i32 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n32 = &(i32->buckets_[h32 % i32->size_]);
          COUNT_mFOLLOWER4_entry* e40;
          do if ((e40=n32->obj) && h32 == n32->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se43, *e40)) {
            long count_mtweettweet_user_id = e40->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v49 = e40->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c9, st25.modify(count_mtweettweet_user_id,v49)) : voidFunc());
          } while ((n32=n32->nxt));
        }
        add_to_temp_map(_c9, st26.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se44.modify(follower_follower_id,follower_user_id))));
        { //temp foreach
          tuple2_L_L* e41 = _c9.head;
          while(e41){
            long count_mtweettweet_user_id = e41->_1;
            long v50 = e41->__av;
            COUNT_mTWEET1.addOrDelOnZero(se41.modify(follower_user_id,count_mtweettweet_user_id),v50);
            e41 = e41->nxt;
          }
        }
        { //slice
          const HASH_RES_t h34 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se46.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i33 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n33 = &(i33->buckets_[h34 % i33->size_]);
          COUNT_mFOLLOWER4_entry* e42;
          do if ((e42=n33->obj) && h34 == n33->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se46, *e42)) {
            long a_user_id = e42->A_USER_ID;
            long v51 = e42->__av;
            _c10.clear();
            { //slice
              const HASH_RES_t h33 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se45.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i34 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n34 = &(i34->buckets_[h33 % i34->size_]);
              COUNT_mFOLLOWER4_entry* e43;
              do if ((e43=n34->obj) && h33 == n34->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se45, *e43)) {
                long count_mtweettweet_user_id = e43->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v53 = e43->__av;
                add_to_temp_map(_c10, st27.modify(count_mtweettweet_user_id,(v53 * -1L)));
              } while ((n34=n34->nxt));
            }
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c10, st28.modify(follower_follower_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e44 = _c10.head;
              while(e44){
                long count_mtweettweet_user_id = e44->_1;
                long v54 = e44->__av;
                COUNT_mTWEET1.addOrDelOnZero(se41.modify(a_user_id,count_mtweettweet_user_id),(v51 * v54));
                e44 = e44->nxt;
              }
            }
          } while ((n33=n33->nxt));
        }
        { //slice
          const HASH_RES_t h35 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se47.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i35 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n35 = &(i35->buckets_[h35 % i35->size_]);
          COUNT_mFOLLOWER10_entry* e45;
          do if ((e45=n35->obj) && h35 == n35->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se47, *e45)) {
            long a_user_id = e45->A_USER_ID;
            long v55 = e45->__av;
            COUNT_mTWEET1.addOrDelOnZero(se41.modify(a_user_id,follower_follower_id),(v55 * -1L));
          } while ((n35=n35->nxt));
        }
        _c11.clear();
        { //slice
          const HASH_RES_t h36 = COUNT_mFOLLOWER5_mapkey1_idxfn::hash(se49.modify1(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_1* i36 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_1*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_1::IdxNode* n36 = &(i36->buckets_[h36 % i36->size_]);
          COUNT_mFOLLOWER5_entry* e46;
          do if ((e46=n36->obj) && h36 == n36->hash && COUNT_mFOLLOWER5_mapkey1_idxfn::equals(se49, *e46)) {
            long z_tweet_id = e46->Z_TWEET_ID;
            long v57 = e46->__av;
            add_to_temp_map(_c11, st29.modify(z_tweet_id,(v57 * -1L)));
          } while ((n36=n36->nxt));
        }
        { //slice
          const HASH_RES_t h37 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se50.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i37 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n37 = &(i37->buckets_[h37 % i37->size_]);
          COUNT_mFOLLOWER11_entry* e47;
          do if ((e47=n37->obj) && h37 == n37->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se50, *e47)) {
            long z_tweet_id = e47->Z_TWEET_ID;
            long v58 = e47->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c11, st30.modify(z_tweet_id,v58)) : voidFunc());
          } while ((n37=n37->nxt));
        }
        { //temp foreach
          tuple2_L_L* e48 = _c11.head;
          while(e48){
            long z_tweet_id = e48->_1;
            long v59 = e48->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se48.modify(follower_user_id,z_tweet_id),v59);
            e48 = e48->nxt;
          }
        }
        { //slice
          const HASH_RES_t h39 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se52.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i38 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n38 = &(i38->buckets_[h39 % i38->size_]);
          COUNT_mFOLLOWER4_entry* e49;
          do if ((e49=n38->obj) && h39 == n38->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se52, *e49)) {
            long count_mfollowerfollower_follower_id = e49->A_USER_ID;
            long v60 = e49->__av;
            { //slice
              const HASH_RES_t h38 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se51.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i39 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n39 = &(i39->buckets_[h38 % i39->size_]);
              COUNT_mFOLLOWER11_entry* e50;
              do if ((e50=n39->obj) && h38 == n39->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se51, *e50)) {
                long z_tweet_id = e50->Z_TWEET_ID;
                long v61 = e50->__av;
                COUNT_mFOLLOWER2.addOrDelOnZero(se48.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v60 * (v61 * -1L)));
              } while ((n39=n39->nxt));
            }
          } while ((n38=n38->nxt));
        }
        { //slice
          const HASH_RES_t h40 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se54.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i40 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n40 = &(i40->buckets_[h40 % i40->size_]);
          COUNT_mFOLLOWER11_entry* e51;
          do if ((e51=n40->obj) && h40 == n40->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se54, *e51)) {
            long z_tweet_id = e51->Z_TWEET_ID;
            long v62 = e51->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se53.modify(z_tweet_id,follower_user_id),(v62 * -1L));
          } while ((n40=n40->nxt));
        }
        _c12.clear();
        { //slice
          const HASH_RES_t h41 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se56.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i41 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n41 = &(i41->buckets_[h41 % i41->size_]);
          COUNT_mFOLLOWER4_entry* e52;
          do if ((e52=n41->obj) && h41 == n41->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se56, *e52)) {
            long count_mfollowerfollower_user_id = e52->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v64 = e52->__av;
            add_to_temp_map(_c12, st31.modify(count_mfollowerfollower_user_id,(v64 * -1L)));
          } while ((n41=n41->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c12, st32.modify(follower_follower_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e53 = _c12.head;
          while(e53){
            long count_mfollowerfollower_user_id = e53->_1;
            long v65 = e53->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se55.modify(follower_user_id,count_mfollowerfollower_user_id),v65);
            e53 = e53->nxt;
          }
        }
        { //slice
          const HASH_RES_t h42 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se57.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i42 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n42 = &(i42->buckets_[h42 % i42->size_]);
          COUNT_mFOLLOWER4_entry* e54;
          do if ((e54=n42->obj) && h42 == n42->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se57, *e54)) {
            long a_user_id = e54->A_USER_ID;
            long v66 = e54->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se55.modify(a_user_id,follower_follower_id),(v66 * -1L));
          } while ((n42=n42->nxt));
        }
        COUNT_mFOLLOWER4.addOrDelOnZero(se58.modify(follower_user_id,follower_follower_id),-1L);
      }
      END_TRIGGER(exec_stats,"delete_FOLLOWER")
      END_TRIGGER(ivc_stats,"delete_FOLLOWER")
    }
    void unwrap_delete_FOLLOWER(const event_args_t& ea) {
      on_delete_FOLLOWER(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])));
    }
    
    void on_insert_TWEET(long tweet_user_id, long tweet_tweet_id, const STRING_TYPE& tweet_tweet_date) {
      BEGIN_TRIGGER(exec_stats,"insert_TWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_TWEET")
	std::vector<void*> r;
      {  //++tN;
        { //slice
          const HASH_RES_t h43 = COUNT_mTWEET1_mapkey1_idxfn::hash(se60.modify1(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_1* i43 = static_cast<HashIndex_COUNT_mTWEET1_map_1*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_1::IdxNode* n43 = &(i43->buckets_[h43 % i43->size_]);
          COUNT_mTWEET1_entry* e55;
          do if ((e55=n43->obj) && h43 == n43->hash && COUNT_mTWEET1_mapkey1_idxfn::equals(se60, *e55)) {
            long a_user_id = e55->A_USER_ID;
            long v67 = e55->__av;
		for(int i = 0; i < v67; i++) {
			void *rr[3];
			r.push_back(rr);
			rr[0] = &tweet_user_id;
			rr[1] = &tweet_tweet_id;
			rr[2] = &a_user_id;
		}
		//api.useDiff(tweet_user_id, tweet_tweet_id, a_user_id);
            //COUNT.addOrDelOnZero(se59.modify(a_user_id,tweet_tweet_id),v67);
          } while ((n43=n43->nxt));
        }
        { //slice
          const HASH_RES_t h44 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se62.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i44 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n44 = &(i44->buckets_[h44 % i44->size_]);
          COUNT_mFOLLOWER10_entry* e56;
          do if ((e56=n44->obj) && h44 == n44->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se62, *e56)) {
            long count_mfollowerfollower_follower_id = e56->A_USER_ID;
            long v68 = e56->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se61.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),v68);
          } while ((n44=n44->nxt));
        }
        { //slice
          const HASH_RES_t h45 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se64.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i45 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n45 = &(i45->buckets_[h45 % i45->size_]);
          COUNT_mFOLLOWER4_entry* e57;
          do if ((e57=n45->obj) && h45 == n45->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se64, *e57)) {
            long count_mfollowerfollower_follower_id = e57->A_USER_ID;
            long v69 = e57->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se63.modify(tweet_tweet_id,count_mfollowerfollower_follower_id),v69);
          } while ((n45=n45->nxt));
        }
        COUNT_mFOLLOWER11.addOrDelOnZero(se65.modify(tweet_user_id,tweet_tweet_id),1L);
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
          const HASH_RES_t h46 = COUNT_mTWEET1_mapkey1_idxfn::hash(se67.modify1(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_1* i46 = static_cast<HashIndex_COUNT_mTWEET1_map_1*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_1::IdxNode* n46 = &(i46->buckets_[h46 % i46->size_]);
          COUNT_mTWEET1_entry* e58;
          do if ((e58=n46->obj) && h46 == n46->hash && COUNT_mTWEET1_mapkey1_idxfn::equals(se67, *e58)) {
            long a_user_id = e58->A_USER_ID;
            long v70 = e58->__av;
            COUNT.addOrDelOnZero(se66.modify(a_user_id,tweet_tweet_id),(v70 * -1L));
          } while ((n46=n46->nxt));
        }
        { //slice
          const HASH_RES_t h47 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se69.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i47 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n47 = &(i47->buckets_[h47 % i47->size_]);
          COUNT_mFOLLOWER10_entry* e59;
          do if ((e59=n47->obj) && h47 == n47->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se69, *e59)) {
            long count_mfollowerfollower_follower_id = e59->A_USER_ID;
            long v71 = e59->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se68.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),(v71 * -1L));
          } while ((n47=n47->nxt));
        }
        { //slice
          const HASH_RES_t h48 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se71.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i48 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n48 = &(i48->buckets_[h48 % i48->size_]);
          COUNT_mFOLLOWER4_entry* e60;
          do if ((e60=n48->obj) && h48 == n48->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se71, *e60)) {
            long count_mfollowerfollower_follower_id = e60->A_USER_ID;
            long v72 = e60->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se70.modify(tweet_tweet_id,count_mfollowerfollower_follower_id),(v72 * -1L));
          } while ((n48=n48->nxt));
        }
        COUNT_mFOLLOWER11.addOrDelOnZero(se72.modify(tweet_user_id,tweet_tweet_id),-1L);
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
    COUNT_mFOLLOWER5_entry se2;
    tuple2_L_L st2;
    COUNT_mFOLLOWER11_entry se3;
    tuple2_L_L st3;
    COUNT_mFOLLOWER2_entry se4;
    COUNT_mFOLLOWER4_entry se5;
    tuple2_L_L st4;
    COUNT_mFOLLOWER11_entry se6;
    tuple2_L_L st5;
    COUNT_mFOLLOWER5_entry se7;
    tuple2_L_L st6;
    COUNT_mFOLLOWER11_entry se8;
    COUNT_mFOLLOWER4_entry se9;
    COUNT_mFOLLOWER11_entry se10;
    COUNT_mFOLLOWER10_entry se11;
    COUNT_mTWEET1_entry se12;
    tuple2_L_L st7;
    COUNT_mFOLLOWER4_entry se13;
    tuple2_L_L st8;
    tuple2_L_L st9;
    COUNT_mFOLLOWER10_entry se14;
    COUNT_mFOLLOWER4_entry se15;
    tuple2_L_L st10;
    tuple2_L_L st11;
    COUNT_mFOLLOWER4_entry se16;
    tuple2_L_L st12;
    COUNT_mFOLLOWER4_entry se17;
    COUNT_mFOLLOWER10_entry se18;
    COUNT_mFOLLOWER2_entry se19;
    tuple2_L_L st13;
    COUNT_mFOLLOWER5_entry se20;
    tuple2_L_L st14;
    COUNT_mFOLLOWER11_entry se21;
    COUNT_mFOLLOWER11_entry se22;
    COUNT_mFOLLOWER4_entry se23;
    COUNT_mFOLLOWER5_entry se24;
    COUNT_mFOLLOWER11_entry se25;
    COUNT_mFOLLOWER10_entry se26;
    tuple2_L_L st15;
    COUNT_mFOLLOWER4_entry se27;
    tuple2_L_L st16;
    COUNT_mFOLLOWER4_entry se28;
    COUNT_mFOLLOWER4_entry se29;
    COUNT_entry se30;
    tuple2_L_L st17;
    COUNT_mFOLLOWER2_entry se31;
    tuple2_L_L st18;
    COUNT_mFOLLOWER11_entry se32;
    tuple2_L_L st19;
    COUNT_mFOLLOWER5_entry se33;
    COUNT_mFOLLOWER4_entry se34;
    tuple2_L_L st20;
    COUNT_mFOLLOWER11_entry se35;
    tuple2_L_L st21;
    COUNT_mFOLLOWER5_entry se36;
    tuple2_L_L st22;
    COUNT_mFOLLOWER11_entry se37;
    COUNT_mFOLLOWER4_entry se38;
    COUNT_mFOLLOWER11_entry se39;
    COUNT_mFOLLOWER10_entry se40;
    COUNT_mTWEET1_entry se41;
    tuple2_L_L st23;
    COUNT_mFOLLOWER10_entry se42;
    tuple2_L_L st24;
    tuple2_L_L st25;
    COUNT_mFOLLOWER4_entry se43;
    COUNT_mFOLLOWER4_entry se44;
    tuple2_L_L st26;
    tuple2_L_L st27;
    COUNT_mFOLLOWER4_entry se45;
    tuple2_L_L st28;
    COUNT_mFOLLOWER4_entry se46;
    COUNT_mFOLLOWER10_entry se47;
    COUNT_mFOLLOWER2_entry se48;
    tuple2_L_L st29;
    COUNT_mFOLLOWER5_entry se49;
    tuple2_L_L st30;
    COUNT_mFOLLOWER11_entry se50;
    COUNT_mFOLLOWER11_entry se51;
    COUNT_mFOLLOWER4_entry se52;
    COUNT_mFOLLOWER5_entry se53;
    COUNT_mFOLLOWER11_entry se54;
    COUNT_mFOLLOWER10_entry se55;
    tuple2_L_L st31;
    COUNT_mFOLLOWER4_entry se56;
    tuple2_L_L st32;
    COUNT_mFOLLOWER4_entry se57;
    COUNT_mFOLLOWER4_entry se58;
    COUNT_entry se59;
    COUNT_mTWEET1_entry se60;
    COUNT_mFOLLOWER2_entry se61;
    COUNT_mFOLLOWER10_entry se62;
    COUNT_mFOLLOWER5_entry se63;
    COUNT_mFOLLOWER4_entry se64;
    COUNT_mFOLLOWER11_entry se65;
    COUNT_entry se66;
    COUNT_mTWEET1_entry se67;
    COUNT_mFOLLOWER2_entry se68;
    COUNT_mFOLLOWER10_entry se69;
    COUNT_mFOLLOWER5_entry se70;
    COUNT_mFOLLOWER4_entry se71;
    COUNT_mFOLLOWER11_entry se72;
  
    /* Data structures used for storing materialized views */
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mFOLLOWER2_map COUNT_mFOLLOWER2;
    COUNT_mFOLLOWER4_map COUNT_mFOLLOWER4;
    COUNT_mFOLLOWER5_map COUNT_mFOLLOWER5;
    COUNT_mFOLLOWER10_map COUNT_mFOLLOWER10;
    COUNT_mFOLLOWER11_map COUNT_mFOLLOWER11;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c1;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c10;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c4;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c7;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c6;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c9;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c3;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c12;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c2;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c11;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c5;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c8;
    
  
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

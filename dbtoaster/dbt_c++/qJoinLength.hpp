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
  
  struct COUNT_mFOLLOWER23_entry {
    long A_USER_ID; long COUNT_mFOLLOWERFOLLOWER_USER_ID; long __av;  COUNT_mFOLLOWER23_entry* nxt; COUNT_mFOLLOWER23_entry* prv;
    explicit COUNT_mFOLLOWER23_entry() : nxt(nullptr), prv(nullptr) { /*A_USER_ID = 0L; COUNT_mFOLLOWERFOLLOWER_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER23_entry(const long c0, const long c1, const long c2) { A_USER_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1; __av = c2; }
    COUNT_mFOLLOWER23_entry(const COUNT_mFOLLOWER23_entry& other) : A_USER_ID( other.A_USER_ID ), COUNT_mFOLLOWERFOLLOWER_USER_ID( other.COUNT_mFOLLOWERFOLLOWER_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER23_entry& modify(const long c0, const long c1) { A_USER_ID = c0; COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER23_entry& modify1(const long c1) { COUNT_mFOLLOWERFOLLOWER_USER_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER23_entry& modify0(const long c0) { A_USER_ID = c0;  return *this; }
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
  struct COUNT_mFOLLOWER23_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER23_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER23_entry& x, const COUNT_mFOLLOWER23_entry& y) {
      return x.A_USER_ID == y.A_USER_ID && x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER23_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER23_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER23_entry& x, const COUNT_mFOLLOWER23_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_USER_ID == y.COUNT_mFOLLOWERFOLLOWER_USER_ID;
    }
  };
  
  struct COUNT_mFOLLOWER23_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER23_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER23_entry& x, const COUNT_mFOLLOWER23_entry& y) {
      return x.A_USER_ID == y.A_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER23_entry,long,
    HashIndex<COUNT_mFOLLOWER23_entry,long,COUNT_mFOLLOWER23_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER23_entry,long,COUNT_mFOLLOWER23_mapkey1_idxfn,false>,
    HashIndex<COUNT_mFOLLOWER23_entry,long,COUNT_mFOLLOWER23_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER23_map;
  typedef HashIndex<COUNT_mFOLLOWER23_entry,long,COUNT_mFOLLOWER23_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER23_map_01;
  typedef HashIndex<COUNT_mFOLLOWER23_entry,long,COUNT_mFOLLOWER23_mapkey1_idxfn,false> HashIndex_COUNT_mFOLLOWER23_map_1;
  typedef HashIndex<COUNT_mFOLLOWER23_entry,long,COUNT_mFOLLOWER23_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER23_map_0;
  
  struct COUNT_mFOLLOWER24_entry {
    long COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID; long Z_TWEET_ID; long __av;  COUNT_mFOLLOWER24_entry* nxt; COUNT_mFOLLOWER24_entry* prv;
    explicit COUNT_mFOLLOWER24_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = 0L; Z_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mFOLLOWER24_entry(const long c0, const long c1, const long c2) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1; __av = c2; }
    COUNT_mFOLLOWER24_entry(const COUNT_mFOLLOWER24_entry& other) : COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID( other.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID ), Z_TWEET_ID( other.Z_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mFOLLOWER24_entry& modify(const long c0, const long c1) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0; Z_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mFOLLOWER24_entry& modify0(const long c0) { COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID = c0;  return *this; }
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
  struct COUNT_mFOLLOWER24_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER24_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      hash_combine(h, e.Z_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER24_entry& x, const COUNT_mFOLLOWER24_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID && x.Z_TWEET_ID == y.Z_TWEET_ID;
    }
  };
  
  struct COUNT_mFOLLOWER24_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mFOLLOWER24_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mFOLLOWER24_entry& x, const COUNT_mFOLLOWER24_entry& y) {
      return x.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID == y.COUNT_mFOLLOWERFOLLOWER_FOLLOWER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mFOLLOWER24_entry,long,
    HashIndex<COUNT_mFOLLOWER24_entry,long,COUNT_mFOLLOWER24_mapkey01_idxfn,true>,
    HashIndex<COUNT_mFOLLOWER24_entry,long,COUNT_mFOLLOWER24_mapkey0_idxfn,false>
  > COUNT_mFOLLOWER24_map;
  typedef HashIndex<COUNT_mFOLLOWER24_entry,long,COUNT_mFOLLOWER24_mapkey01_idxfn,true> HashIndex_COUNT_mFOLLOWER24_map_01;
  typedef HashIndex<COUNT_mFOLLOWER24_entry,long,COUNT_mFOLLOWER24_mapkey0_idxfn,false> HashIndex_COUNT_mFOLLOWER24_map_0;
  
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
    data_t(): tlq_t(), _c16(16U), _c1(16U), _c22(16U), _c19(16U), _c4(16U), _c10(16U), _c13(16U), _c7(16U), _c6(16U), _c21(16U), _c9(16U), _c15(16U), _c18(16U), _c24(16U), _c3(16U), _c12(16U), _c2(16U), _c5(16U), _c11(16U), _c14(16U), _c8(16U), _c20(16U), _c23(16U), _c17(16U) {
      
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
      pb.add_map<COUNT_mFOLLOWER23_map>( "COUNT_mFOLLOWER23", COUNT_mFOLLOWER23 );
      pb.add_map<COUNT_mFOLLOWER24_map>( "COUNT_mFOLLOWER24", COUNT_mFOLLOWER24 );
  
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
      exec_stats->register_probe(6, "insert_FOLLOWER_s6");
      exec_stats->register_probe(7, "insert_FOLLOWER_s7");
      exec_stats->register_probe(8, "delete_FOLLOWER_s0");
      exec_stats->register_probe(9, "delete_FOLLOWER_s1");
      exec_stats->register_probe(10, "delete_FOLLOWER_s2");
      exec_stats->register_probe(11, "delete_FOLLOWER_s3");
      exec_stats->register_probe(12, "delete_FOLLOWER_s4");
      exec_stats->register_probe(13, "delete_FOLLOWER_s5");
      exec_stats->register_probe(14, "delete_FOLLOWER_s6");
      exec_stats->register_probe(15, "delete_FOLLOWER_s7");
      exec_stats->register_probe(16, "insert_TWEET_s0");
      exec_stats->register_probe(17, "insert_TWEET_s1");
      exec_stats->register_probe(18, "insert_TWEET_s2");
      exec_stats->register_probe(19, "insert_TWEET_s3");
      exec_stats->register_probe(20, "insert_TWEET_s4");
      exec_stats->register_probe(21, "delete_TWEET_s0");
      exec_stats->register_probe(22, "delete_TWEET_s1");
      exec_stats->register_probe(23, "delete_TWEET_s2");
      exec_stats->register_probe(24, "delete_TWEET_s3");
      exec_stats->register_probe(25, "delete_TWEET_s4");
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
          const HASH_RES_t h1 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se3.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i1 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mFOLLOWER24_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se3, *e1)) {
            long z_tweet_id = e1->Z_TWEET_ID;
            long v2 = e1->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c1, st1.modify(z_tweet_id,(v2 * (COUNT_mFOLLOWER4.getValueOrDefault(se2.modify(follower_follower_id,follower_user_id)) + (/*if */(follower_follower_id == follower_user_id) ? 1L : 0L))))) : voidFunc());
          } while ((n1=n1->nxt));
        }
        { //slice
          const HASH_RES_t h2 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se4.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i2 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mFOLLOWER5_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se4, *e2)) {
            long z_tweet_id = e2->Z_TWEET_ID;
            long v3 = e2->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c1, st2.modify(z_tweet_id,v3)) : voidFunc());
          } while ((n2=n2->nxt));
        }
        { //slice
          const HASH_RES_t h3 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se5.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i3 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mFOLLOWER11_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se5, *e3)) {
            long z_tweet_id = e3->Z_TWEET_ID;
            long v4 = e3->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c1, st3.modify(z_tweet_id,v4)) : voidFunc());
          } while ((n3=n3->nxt));
        }
        { //slice
          const HASH_RES_t h4 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se7.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i4 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mFOLLOWER11_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se7, *e4)) {
            long z_tweet_id = e4->Z_TWEET_ID;
            long v5 = e4->__av;
            add_to_temp_map(_c1, st4.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se6.modify(follower_follower_id,follower_user_id)) * v5)));
          } while ((n4=n4->nxt));
        }
        { //slice
          const HASH_RES_t h5 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se8.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i5 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mFOLLOWER24_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se8, *e5)) {
            long z_tweet_id = e5->Z_TWEET_ID;
            long v6 = e5->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c1, st5.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se6.modify(follower_follower_id,follower_user_id)) * v6))) : voidFunc());
          } while ((n5=n5->nxt));
        }
        { //slice
          const HASH_RES_t h6 = COUNT_mFOLLOWER2_mapkey0_idxfn::hash(se9.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER2_map_0* i6 = static_cast<HashIndex_COUNT_mFOLLOWER2_map_0*>(COUNT_mFOLLOWER2.index[1]);
          HashIndex_COUNT_mFOLLOWER2_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mFOLLOWER2_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mFOLLOWER2_mapkey0_idxfn::equals(se9, *e6)) {
            long z_tweet_id = e6->Z_TWEET_ID;
            long v7 = e6->__av;
            add_to_temp_map(_c1, st6.modify(z_tweet_id,v7));
          } while ((n6=n6->nxt));
        }
        { //slice
          const HASH_RES_t h7 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se11.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i7 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          COUNT_mFOLLOWER24_entry* e7;
          do if ((e7=n7->obj) && h7 == n7->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se11, *e7)) {
            long z_tweet_id = e7->Z_TWEET_ID;
            long v8 = e7->__av;
            add_to_temp_map(_c1, st7.modify(z_tweet_id,(COUNT_mFOLLOWER10.getValueOrDefault(se10.modify(follower_follower_id,follower_user_id)) * v8)));
          } while ((n7=n7->nxt));
        }
        { //temp foreach
          tuple2_L_L* e8 = _c1.head;
          while(e8){
            long z_tweet_id = e8->_1;
            long v9 = e8->__av;
            COUNT.addOrDelOnZero(se1.modify(follower_user_id,z_tweet_id),v9);
            e8 = e8->nxt;
          }
        }
        { //slice
          const HASH_RES_t h12 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se17.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i8 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n8 = &(i8->buckets_[h12 % i8->size_]);
          COUNT_mFOLLOWER4_entry* e9;
          do if ((e9=n8->obj) && h12 == n8->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se17, *e9)) {
            long a_user_id = e9->A_USER_ID;
            long v10 = e9->__av;
            _c2.clear();
            { //slice
              const HASH_RES_t h8 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se12.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i9 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n9 = &(i9->buckets_[h8 % i9->size_]);
              COUNT_mFOLLOWER11_entry* e10;
              do if ((e10=n9->obj) && h8 == n9->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se12, *e10)) {
                long z_tweet_id = e10->Z_TWEET_ID;
                long v12 = e10->__av;
                (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c2, st8.modify(z_tweet_id,v12)) : voidFunc());
              } while ((n9=n9->nxt));
            }
            { //slice
              const HASH_RES_t h9 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se13.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i10 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n10 = &(i10->buckets_[h9 % i10->size_]);
              COUNT_mFOLLOWER24_entry* e11;
              do if ((e11=n10->obj) && h9 == n10->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se13, *e11)) {
                long z_tweet_id = e11->Z_TWEET_ID;
                long v13 = e11->__av;
                (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c2, st9.modify(z_tweet_id,v13)) : voidFunc());
              } while ((n10=n10->nxt));
            }
            { //slice
              const HASH_RES_t h10 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se14.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER5_map_0* i11 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
              HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n11 = &(i11->buckets_[h10 % i11->size_]);
              COUNT_mFOLLOWER5_entry* e12;
              do if ((e12=n11->obj) && h10 == n11->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se14, *e12)) {
                long z_tweet_id = e12->Z_TWEET_ID;
                long v14 = e12->__av;
                add_to_temp_map(_c2, st10.modify(z_tweet_id,v14));
              } while ((n11=n11->nxt));
            }
            { //slice
              const HASH_RES_t h11 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se16.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i12 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n12 = &(i12->buckets_[h11 % i12->size_]);
              COUNT_mFOLLOWER24_entry* e13;
              do if ((e13=n12->obj) && h11 == n12->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se16, *e13)) {
                long z_tweet_id = e13->Z_TWEET_ID;
                long v15 = e13->__av;
                add_to_temp_map(_c2, st11.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se15.modify(follower_follower_id,follower_user_id)) * v15)));
              } while ((n12=n12->nxt));
            }
            { //temp foreach
              tuple2_L_L* e14 = _c2.head;
              while(e14){
                long z_tweet_id = e14->_1;
                long v16 = e14->__av;
                COUNT.addOrDelOnZero(se1.modify(a_user_id,z_tweet_id),(v10 * v16));
                e14 = e14->nxt;
              }
            }
          } while ((n8=n8->nxt));
        }
        { //slice
          const HASH_RES_t h15 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se20.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i13 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n13 = &(i13->buckets_[h15 % i13->size_]);
          COUNT_mFOLLOWER10_entry* e15;
          do if ((e15=n13->obj) && h15 == n13->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se20, *e15)) {
            long a_user_id = e15->A_USER_ID;
            long v17 = e15->__av;
            _c3.clear();
            { //slice
              const HASH_RES_t h13 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se18.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i14 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n14 = &(i14->buckets_[h13 % i14->size_]);
              COUNT_mFOLLOWER11_entry* e16;
              do if ((e16=n14->obj) && h13 == n14->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se18, *e16)) {
                long z_tweet_id = e16->Z_TWEET_ID;
                long v19 = e16->__av;
                add_to_temp_map(_c3, st12.modify(z_tweet_id,v19));
              } while ((n14=n14->nxt));
            }
            { //slice
              const HASH_RES_t h14 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se19.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i15 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n15 = &(i15->buckets_[h14 % i15->size_]);
              COUNT_mFOLLOWER24_entry* e17;
              do if ((e17=n15->obj) && h14 == n15->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se19, *e17)) {
                long z_tweet_id = e17->Z_TWEET_ID;
                long v20 = e17->__av;
                (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c3, st13.modify(z_tweet_id,v20)) : voidFunc());
              } while ((n15=n15->nxt));
            }
            { //temp foreach
              tuple2_L_L* e18 = _c3.head;
              while(e18){
                long z_tweet_id = e18->_1;
                long v21 = e18->__av;
                COUNT.addOrDelOnZero(se1.modify(a_user_id,z_tweet_id),(v17 * v21));
                e18 = e18->nxt;
              }
            }
          } while ((n13=n13->nxt));
        }
        { //slice
          const HASH_RES_t h17 = COUNT_mFOLLOWER23_mapkey1_idxfn::hash(se22.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER23_map_1* i16 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_1*>(COUNT_mFOLLOWER23.index[1]);
          HashIndex_COUNT_mFOLLOWER23_map_1::IdxNode* n16 = &(i16->buckets_[h17 % i16->size_]);
          COUNT_mFOLLOWER23_entry* e19;
          do if ((e19=n16->obj) && h17 == n16->hash && COUNT_mFOLLOWER23_mapkey1_idxfn::equals(se22, *e19)) {
            long a_user_id = e19->A_USER_ID;
            long v22 = e19->__av;
            { //slice
              const HASH_RES_t h16 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se21.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i17 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n17 = &(i17->buckets_[h16 % i17->size_]);
              COUNT_mFOLLOWER24_entry* e20;
              do if ((e20=n17->obj) && h16 == n17->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se21, *e20)) {
                long z_tweet_id = e20->Z_TWEET_ID;
                long v23 = e20->__av;
                COUNT.addOrDelOnZero(se1.modify(a_user_id,z_tweet_id),(v22 * v23));
              } while ((n17=n17->nxt));
            }
          } while ((n16=n16->nxt));
        }
        _c4.clear();
        (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c4, st14.modify(follower_follower_id,(COUNT_mFOLLOWER4.getValueOrDefault(se24.modify(follower_follower_id,follower_user_id)) + (/*if */(follower_follower_id == follower_user_id) ? 1L : 0L)))) : voidFunc());
        { //slice
          const HASH_RES_t h18 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se25.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER10_map_0* i18 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
          HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n18 = &(i18->buckets_[h18 % i18->size_]);
          COUNT_mFOLLOWER10_entry* e21;
          do if ((e21=n18->obj) && h18 == n18->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se25, *e21)) {
            long count_mtweettweet_user_id = e21->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v25 = e21->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c4, st15.modify(count_mtweettweet_user_id,v25)) : voidFunc());
          } while ((n18=n18->nxt));
        }
        { //slice
          const HASH_RES_t h19 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se26.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i19 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n19 = &(i19->buckets_[h19 % i19->size_]);
          COUNT_mFOLLOWER4_entry* e22;
          do if ((e22=n19->obj) && h19 == n19->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se26, *e22)) {
            long count_mtweettweet_user_id = e22->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v26 = e22->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c4, st16.modify(count_mtweettweet_user_id,v26)) : voidFunc());
          } while ((n19=n19->nxt));
        }
        { //slice
          const HASH_RES_t h20 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se28.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i20 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n20 = &(i20->buckets_[h20 % i20->size_]);
          COUNT_mFOLLOWER4_entry* e23;
          do if ((e23=n20->obj) && h20 == n20->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se28, *e23)) {
            long count_mtweettweet_user_id = e23->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v27 = e23->__av;
            add_to_temp_map(_c4, st17.modify(count_mtweettweet_user_id,(COUNT_mFOLLOWER4.getValueOrDefault(se27.modify(follower_follower_id,follower_user_id)) * v27)));
          } while ((n20=n20->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c4, st18.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se27.modify(follower_follower_id,follower_user_id)))) : voidFunc());
        { //slice
          const HASH_RES_t h21 = COUNT_mFOLLOWER23_mapkey0_idxfn::hash(se29.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER23_map_0* i21 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_0*>(COUNT_mFOLLOWER23.index[2]);
          HashIndex_COUNT_mFOLLOWER23_map_0::IdxNode* n21 = &(i21->buckets_[h21 % i21->size_]);
          COUNT_mFOLLOWER23_entry* e24;
          do if ((e24=n21->obj) && h21 == n21->hash && COUNT_mFOLLOWER23_mapkey0_idxfn::equals(se29, *e24)) {
            long count_mtweettweet_user_id = e24->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v28 = e24->__av;
            add_to_temp_map(_c4, st19.modify(count_mtweettweet_user_id,v28));
          } while ((n21=n21->nxt));
        }
        add_to_temp_map(_c4, st20.modify(follower_follower_id,COUNT_mFOLLOWER10.getValueOrDefault(se30.modify(follower_follower_id,follower_user_id))));
        { //temp foreach
          tuple2_L_L* e25 = _c4.head;
          while(e25){
            long count_mtweettweet_user_id = e25->_1;
            long v29 = e25->__av;
            COUNT_mTWEET1.addOrDelOnZero(se23.modify(follower_user_id,count_mtweettweet_user_id),v29);
            e25 = e25->nxt;
          }
        }
        { //slice
          const HASH_RES_t h24 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se34.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i22 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n22 = &(i22->buckets_[h24 % i22->size_]);
          COUNT_mFOLLOWER4_entry* e26;
          do if ((e26=n22->obj) && h24 == n22->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se34, *e26)) {
            long a_user_id = e26->A_USER_ID;
            long v30 = e26->__av;
            _c5.clear();
            { //slice
              const HASH_RES_t h22 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se31.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i23 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n23 = &(i23->buckets_[h22 % i23->size_]);
              COUNT_mFOLLOWER4_entry* e27;
              do if ((e27=n23->obj) && h22 == n23->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se31, *e27)) {
                long count_mtweettweet_user_id = e27->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v32 = e27->__av;
                (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c5, st21.modify(count_mtweettweet_user_id,v32)) : voidFunc());
              } while ((n23=n23->nxt));
            }
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c5, st22.modify(follower_follower_id,1L)) : voidFunc());
            { //slice
              const HASH_RES_t h23 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se32.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER10_map_0* i24 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
              HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n24 = &(i24->buckets_[h23 % i24->size_]);
              COUNT_mFOLLOWER10_entry* e28;
              do if ((e28=n24->obj) && h23 == n24->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se32, *e28)) {
                long count_mtweettweet_user_id = e28->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v33 = e28->__av;
                add_to_temp_map(_c5, st23.modify(count_mtweettweet_user_id,v33));
              } while ((n24=n24->nxt));
            }
            add_to_temp_map(_c5, st24.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se33.modify(follower_follower_id,follower_user_id))));
            { //temp foreach
              tuple2_L_L* e29 = _c5.head;
              while(e29){
                long count_mtweettweet_user_id = e29->_1;
                long v34 = e29->__av;
                COUNT_mTWEET1.addOrDelOnZero(se23.modify(a_user_id,count_mtweettweet_user_id),(v30 * v34));
                e29 = e29->nxt;
              }
            }
          } while ((n22=n22->nxt));
        }
        { //slice
          const HASH_RES_t h26 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se36.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i25 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n25 = &(i25->buckets_[h26 % i25->size_]);
          COUNT_mFOLLOWER10_entry* e30;
          do if ((e30=n25->obj) && h26 == n25->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se36, *e30)) {
            long a_user_id = e30->A_USER_ID;
            long v35 = e30->__av;
            _c6.clear();
            { //slice
              const HASH_RES_t h25 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se35.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i26 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n26 = &(i26->buckets_[h25 % i26->size_]);
              COUNT_mFOLLOWER4_entry* e31;
              do if ((e31=n26->obj) && h25 == n26->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se35, *e31)) {
                long count_mtweettweet_user_id = e31->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v37 = e31->__av;
                add_to_temp_map(_c6, st25.modify(count_mtweettweet_user_id,v37));
              } while ((n26=n26->nxt));
            }
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c6, st26.modify(follower_follower_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e32 = _c6.head;
              while(e32){
                long count_mtweettweet_user_id = e32->_1;
                long v38 = e32->__av;
                COUNT_mTWEET1.addOrDelOnZero(se23.modify(a_user_id,count_mtweettweet_user_id),(v35 * v38));
                e32 = e32->nxt;
              }
            }
          } while ((n25=n25->nxt));
        }
        { //slice
          const HASH_RES_t h27 = COUNT_mFOLLOWER23_mapkey1_idxfn::hash(se37.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER23_map_1* i27 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_1*>(COUNT_mFOLLOWER23.index[1]);
          HashIndex_COUNT_mFOLLOWER23_map_1::IdxNode* n27 = &(i27->buckets_[h27 % i27->size_]);
          COUNT_mFOLLOWER23_entry* e33;
          do if ((e33=n27->obj) && h27 == n27->hash && COUNT_mFOLLOWER23_mapkey1_idxfn::equals(se37, *e33)) {
            long a_user_id = e33->A_USER_ID;
            long v39 = e33->__av;
            COUNT_mTWEET1.addOrDelOnZero(se23.modify(a_user_id,follower_follower_id),v39);
          } while ((n27=n27->nxt));
        }
        _c7.clear();
        { //slice
          const HASH_RES_t h28 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se39.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i28 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n28 = &(i28->buckets_[h28 % i28->size_]);
          COUNT_mFOLLOWER11_entry* e34;
          do if ((e34=n28->obj) && h28 == n28->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se39, *e34)) {
            long z_tweet_id = e34->Z_TWEET_ID;
            long v41 = e34->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c7, st27.modify(z_tweet_id,v41)) : voidFunc());
          } while ((n28=n28->nxt));
        }
        { //slice
          const HASH_RES_t h29 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se40.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i29 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n29 = &(i29->buckets_[h29 % i29->size_]);
          COUNT_mFOLLOWER24_entry* e35;
          do if ((e35=n29->obj) && h29 == n29->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se40, *e35)) {
            long z_tweet_id = e35->Z_TWEET_ID;
            long v42 = e35->__av;
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c7, st28.modify(z_tweet_id,v42)) : voidFunc());
          } while ((n29=n29->nxt));
        }
        { //slice
          const HASH_RES_t h30 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se41.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i30 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n30 = &(i30->buckets_[h30 % i30->size_]);
          COUNT_mFOLLOWER5_entry* e36;
          do if ((e36=n30->obj) && h30 == n30->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se41, *e36)) {
            long z_tweet_id = e36->Z_TWEET_ID;
            long v43 = e36->__av;
            add_to_temp_map(_c7, st29.modify(z_tweet_id,v43));
          } while ((n30=n30->nxt));
        }
        { //slice
          const HASH_RES_t h31 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se43.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i31 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n31 = &(i31->buckets_[h31 % i31->size_]);
          COUNT_mFOLLOWER24_entry* e37;
          do if ((e37=n31->obj) && h31 == n31->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se43, *e37)) {
            long z_tweet_id = e37->Z_TWEET_ID;
            long v44 = e37->__av;
            add_to_temp_map(_c7, st30.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se42.modify(follower_follower_id,follower_user_id)) * v44)));
          } while ((n31=n31->nxt));
        }
        { //temp foreach
          tuple2_L_L* e38 = _c7.head;
          while(e38){
            long z_tweet_id = e38->_1;
            long v45 = e38->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se38.modify(follower_user_id,z_tweet_id),v45);
            e38 = e38->nxt;
          }
        }
        { //slice
          const HASH_RES_t h34 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se46.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i32 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n32 = &(i32->buckets_[h34 % i32->size_]);
          COUNT_mFOLLOWER4_entry* e39;
          do if ((e39=n32->obj) && h34 == n32->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se46, *e39)) {
            long count_mfollowerfollower_follower_id = e39->A_USER_ID;
            long v46 = e39->__av;
            _c8.clear();
            { //slice
              const HASH_RES_t h32 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se44.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i33 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n33 = &(i33->buckets_[h32 % i33->size_]);
              COUNT_mFOLLOWER11_entry* e40;
              do if ((e40=n33->obj) && h32 == n33->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se44, *e40)) {
                long z_tweet_id = e40->Z_TWEET_ID;
                long v48 = e40->__av;
                add_to_temp_map(_c8, st31.modify(z_tweet_id,v48));
              } while ((n33=n33->nxt));
            }
            { //slice
              const HASH_RES_t h33 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se45.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i34 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n34 = &(i34->buckets_[h33 % i34->size_]);
              COUNT_mFOLLOWER24_entry* e41;
              do if ((e41=n34->obj) && h33 == n34->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se45, *e41)) {
                long z_tweet_id = e41->Z_TWEET_ID;
                long v49 = e41->__av;
                (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c8, st32.modify(z_tweet_id,v49)) : voidFunc());
              } while ((n34=n34->nxt));
            }
            { //temp foreach
              tuple2_L_L* e42 = _c8.head;
              while(e42){
                long z_tweet_id = e42->_1;
                long v50 = e42->__av;
                COUNT_mFOLLOWER2.addOrDelOnZero(se38.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v46 * v50));
                e42 = e42->nxt;
              }
            }
          } while ((n32=n32->nxt));
        }
        { //slice
          const HASH_RES_t h36 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se48.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i35 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n35 = &(i35->buckets_[h36 % i35->size_]);
          COUNT_mFOLLOWER10_entry* e43;
          do if ((e43=n35->obj) && h36 == n35->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se48, *e43)) {
            long count_mfollowerfollower_follower_id = e43->A_USER_ID;
            long v51 = e43->__av;
            { //slice
              const HASH_RES_t h35 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se47.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i36 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n36 = &(i36->buckets_[h35 % i36->size_]);
              COUNT_mFOLLOWER24_entry* e44;
              do if ((e44=n36->obj) && h35 == n36->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se47, *e44)) {
                long z_tweet_id = e44->Z_TWEET_ID;
                long v52 = e44->__av;
                COUNT_mFOLLOWER2.addOrDelOnZero(se38.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v51 * v52));
              } while ((n36=n36->nxt));
            }
          } while ((n35=n35->nxt));
        }
        _c9.clear();
        { //slice
          const HASH_RES_t h37 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se50.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i37 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n37 = &(i37->buckets_[h37 % i37->size_]);
          COUNT_mFOLLOWER11_entry* e45;
          do if ((e45=n37->obj) && h37 == n37->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se50, *e45)) {
            long z_tweet_id = e45->Z_TWEET_ID;
            long v54 = e45->__av;
            add_to_temp_map(_c9, st33.modify(z_tweet_id,v54));
          } while ((n37=n37->nxt));
        }
        { //slice
          const HASH_RES_t h38 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se51.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i38 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n38 = &(i38->buckets_[h38 % i38->size_]);
          COUNT_mFOLLOWER24_entry* e46;
          do if ((e46=n38->obj) && h38 == n38->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se51, *e46)) {
            long z_tweet_id = e46->Z_TWEET_ID;
            long v55 = e46->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c9, st34.modify(z_tweet_id,v55)) : voidFunc());
          } while ((n38=n38->nxt));
        }
        { //temp foreach
          tuple2_L_L* e47 = _c9.head;
          while(e47){
            long z_tweet_id = e47->_1;
            long v56 = e47->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se49.modify(follower_user_id,z_tweet_id),v56);
            e47 = e47->nxt;
          }
        }
        { //slice
          const HASH_RES_t h40 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se53.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i39 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n39 = &(i39->buckets_[h40 % i39->size_]);
          COUNT_mFOLLOWER4_entry* e48;
          do if ((e48=n39->obj) && h40 == n39->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se53, *e48)) {
            long count_mfollowerfollower_follower_id = e48->A_USER_ID;
            long v57 = e48->__av;
            { //slice
              const HASH_RES_t h39 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se52.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i40 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n40 = &(i40->buckets_[h39 % i40->size_]);
              COUNT_mFOLLOWER24_entry* e49;
              do if ((e49=n40->obj) && h39 == n40->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se52, *e49)) {
                long z_tweet_id = e49->Z_TWEET_ID;
                long v58 = e49->__av;
                COUNT_mFOLLOWER5.addOrDelOnZero(se49.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v57 * v58));
              } while ((n40=n40->nxt));
            }
          } while ((n39=n39->nxt));
        }
        { //slice
          const HASH_RES_t h41 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se55.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i41 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n41 = &(i41->buckets_[h41 % i41->size_]);
          COUNT_mFOLLOWER24_entry* e50;
          do if ((e50=n41->obj) && h41 == n41->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se55, *e50)) {
            long z_tweet_id = e50->Z_TWEET_ID;
            long v59 = e50->__av;
            COUNT_mFOLLOWER11.addOrDelOnZero(se54.modify(follower_user_id,z_tweet_id),v59);
          } while ((n41=n41->nxt));
        }
        _c10.clear();
        { //slice
          const HASH_RES_t h42 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se57.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i42 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n42 = &(i42->buckets_[h42 % i42->size_]);
          COUNT_mFOLLOWER4_entry* e51;
          do if ((e51=n42->obj) && h42 == n42->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se57, *e51)) {
            long count_mfollowerfollower_user_id = e51->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v61 = e51->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c10, st35.modify(count_mfollowerfollower_user_id,v61)) : voidFunc());
          } while ((n42=n42->nxt));
        }
        (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c10, st36.modify(follower_follower_id,1L)) : voidFunc());
        { //slice
          const HASH_RES_t h43 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se58.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER10_map_0* i43 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
          HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n43 = &(i43->buckets_[h43 % i43->size_]);
          COUNT_mFOLLOWER10_entry* e52;
          do if ((e52=n43->obj) && h43 == n43->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se58, *e52)) {
            long count_mfollowerfollower_user_id = e52->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v62 = e52->__av;
            add_to_temp_map(_c10, st37.modify(count_mfollowerfollower_user_id,v62));
          } while ((n43=n43->nxt));
        }
        add_to_temp_map(_c10, st38.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se59.modify(follower_follower_id,follower_user_id))));
        { //temp foreach
          tuple2_L_L* e53 = _c10.head;
          while(e53){
            long count_mfollowerfollower_user_id = e53->_1;
            long v63 = e53->__av;
            COUNT_mFOLLOWER23.addOrDelOnZero(se56.modify(follower_user_id,count_mfollowerfollower_user_id),v63);
            e53 = e53->nxt;
          }
        }
        { //slice
          const HASH_RES_t h45 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se61.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i44 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n44 = &(i44->buckets_[h45 % i44->size_]);
          COUNT_mFOLLOWER4_entry* e54;
          do if ((e54=n44->obj) && h45 == n44->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se61, *e54)) {
            long a_user_id = e54->A_USER_ID;
            long v64 = e54->__av;
            _c11.clear();
            { //slice
              const HASH_RES_t h44 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se60.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i45 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n45 = &(i45->buckets_[h44 % i45->size_]);
              COUNT_mFOLLOWER4_entry* e55;
              do if ((e55=n45->obj) && h44 == n45->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se60, *e55)) {
                long count_mfollowerfollower_user_id = e55->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v66 = e55->__av;
                add_to_temp_map(_c11, st39.modify(count_mfollowerfollower_user_id,v66));
              } while ((n45=n45->nxt));
            }
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c11, st40.modify(follower_follower_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e56 = _c11.head;
              while(e56){
                long count_mfollowerfollower_user_id = e56->_1;
                long v67 = e56->__av;
                COUNT_mFOLLOWER23.addOrDelOnZero(se56.modify(a_user_id,count_mfollowerfollower_user_id),(v64 * v67));
                e56 = e56->nxt;
              }
            }
          } while ((n44=n44->nxt));
        }
        { //slice
          const HASH_RES_t h46 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se62.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i46 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n46 = &(i46->buckets_[h46 % i46->size_]);
          COUNT_mFOLLOWER10_entry* e57;
          do if ((e57=n46->obj) && h46 == n46->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se62, *e57)) {
            long a_user_id = e57->A_USER_ID;
            long v68 = e57->__av;
            COUNT_mFOLLOWER23.addOrDelOnZero(se56.modify(a_user_id,follower_follower_id),v68);
          } while ((n46=n46->nxt));
        }
        _c12.clear();
        { //slice
          const HASH_RES_t h47 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se64.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i47 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n47 = &(i47->buckets_[h47 % i47->size_]);
          COUNT_mFOLLOWER4_entry* e58;
          do if ((e58=n47->obj) && h47 == n47->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se64, *e58)) {
            long count_mfollowerfollower_user_id = e58->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v70 = e58->__av;
            add_to_temp_map(_c12, st41.modify(count_mfollowerfollower_user_id,v70));
          } while ((n47=n47->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c12, st42.modify(follower_follower_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e59 = _c12.head;
          while(e59){
            long count_mfollowerfollower_user_id = e59->_1;
            long v71 = e59->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se63.modify(follower_user_id,count_mfollowerfollower_user_id),v71);
            e59 = e59->nxt;
          }
        }
        { //slice
          const HASH_RES_t h48 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se65.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i48 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n48 = &(i48->buckets_[h48 % i48->size_]);
          COUNT_mFOLLOWER4_entry* e60;
          do if ((e60=n48->obj) && h48 == n48->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se65, *e60)) {
            long a_user_id = e60->A_USER_ID;
            long v72 = e60->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se63.modify(a_user_id,follower_follower_id),v72);
          } while ((n48=n48->nxt));
        }
        COUNT_mFOLLOWER4.addOrDelOnZero(se66.modify(follower_user_id,follower_follower_id),1L);
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
        _c13.clear();
        { //slice
          const HASH_RES_t h49 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se68.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i49 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n49 = &(i49->buckets_[h49 % i49->size_]);
          COUNT_mFOLLOWER11_entry* e61;
          do if ((e61=n49->obj) && h49 == n49->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se68, *e61)) {
            long z_tweet_id = e61->Z_TWEET_ID;
            long v74 = e61->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c13, st43.modify(z_tweet_id,(v74 * -1L))) : voidFunc());
          } while ((n49=n49->nxt));
        }
        { //slice
          const HASH_RES_t h50 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se70.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i50 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n50 = &(i50->buckets_[h50 % i50->size_]);
          COUNT_mFOLLOWER24_entry* e62;
          do if ((e62=n50->obj) && h50 == n50->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se70, *e62)) {
            long z_tweet_id = e62->Z_TWEET_ID;
            long v75 = e62->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c13, st44.modify(z_tweet_id,((COUNT_mFOLLOWER4.getValueOrDefault(se69.modify(follower_follower_id,follower_user_id)) * v75) * -1L))) : voidFunc());
          } while ((n50=n50->nxt));
        }
        { //slice
          const HASH_RES_t h51 = COUNT_mFOLLOWER2_mapkey0_idxfn::hash(se71.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER2_map_0* i51 = static_cast<HashIndex_COUNT_mFOLLOWER2_map_0*>(COUNT_mFOLLOWER2.index[1]);
          HashIndex_COUNT_mFOLLOWER2_map_0::IdxNode* n51 = &(i51->buckets_[h51 % i51->size_]);
          COUNT_mFOLLOWER2_entry* e63;
          do if ((e63=n51->obj) && h51 == n51->hash && COUNT_mFOLLOWER2_mapkey0_idxfn::equals(se71, *e63)) {
            long z_tweet_id = e63->Z_TWEET_ID;
            long v76 = e63->__av;
            add_to_temp_map(_c13, st45.modify(z_tweet_id,(v76 * -1L)));
          } while ((n51=n51->nxt));
        }
        { //slice
          const HASH_RES_t h52 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se73.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i52 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n52 = &(i52->buckets_[h52 % i52->size_]);
          COUNT_mFOLLOWER24_entry* e64;
          do if ((e64=n52->obj) && h52 == n52->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se73, *e64)) {
            long z_tweet_id = e64->Z_TWEET_ID;
            long v77 = e64->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c13, st46.modify(z_tweet_id,((COUNT_mFOLLOWER4.getValueOrDefault(se72.modify(follower_follower_id,follower_user_id)) * v77) * -1L))) : voidFunc());
          } while ((n52=n52->nxt));
        }
        { //slice
          const HASH_RES_t h53 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se74.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i53 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n53 = &(i53->buckets_[h53 % i53->size_]);
          COUNT_mFOLLOWER5_entry* e65;
          do if ((e65=n53->obj) && h53 == n53->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se74, *e65)) {
            long z_tweet_id = e65->Z_TWEET_ID;
            long v78 = e65->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c13, st47.modify(z_tweet_id,v78)) : voidFunc());
          } while ((n53=n53->nxt));
        }
        { //slice
          const HASH_RES_t h54 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se75.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i54 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n54 = &(i54->buckets_[h54 % i54->size_]);
          COUNT_mFOLLOWER24_entry* e66;
          do if ((e66=n54->obj) && h54 == n54->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se75, *e66)) {
            long z_tweet_id = e66->Z_TWEET_ID;
            long v79 = e66->__av;
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c13, st48.modify(z_tweet_id,v79)) : voidFunc());
          } while ((n54=n54->nxt));
        }
        { //slice
          const HASH_RES_t h55 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se77.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i55 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n55 = &(i55->buckets_[h55 % i55->size_]);
          COUNT_mFOLLOWER11_entry* e67;
          do if ((e67=n55->obj) && h55 == n55->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se77, *e67)) {
            long z_tweet_id = e67->Z_TWEET_ID;
            long v80 = e67->__av;
            add_to_temp_map(_c13, st49.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se76.modify(follower_follower_id,follower_user_id)) * v80)));
          } while ((n55=n55->nxt));
        }
        { //slice
          const HASH_RES_t h56 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se79.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i56 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n56 = &(i56->buckets_[h56 % i56->size_]);
          COUNT_mFOLLOWER24_entry* e68;
          do if ((e68=n56->obj) && h56 == n56->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se79, *e68)) {
            long z_tweet_id = e68->Z_TWEET_ID;
            long v81 = e68->__av;
            add_to_temp_map(_c13, st50.modify(z_tweet_id,(COUNT_mFOLLOWER10.getValueOrDefault(se78.modify(follower_follower_id,follower_user_id)) * v81)));
          } while ((n56=n56->nxt));
        }
        { //temp foreach
          tuple2_L_L* e69 = _c13.head;
          while(e69){
            long z_tweet_id = e69->_1;
            long v82 = e69->__av;
            COUNT.addOrDelOnZero(se67.modify(follower_user_id,z_tweet_id),v82);
            e69 = e69->nxt;
          }
        }
        { //slice
          const HASH_RES_t h61 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se85.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i57 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n57 = &(i57->buckets_[h61 % i57->size_]);
          COUNT_mFOLLOWER4_entry* e70;
          do if ((e70=n57->obj) && h61 == n57->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se85, *e70)) {
            long a_user_id = e70->A_USER_ID;
            long v83 = e70->__av;
            _c14.clear();
            { //slice
              const HASH_RES_t h57 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se80.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER5_map_0* i58 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
              HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n58 = &(i58->buckets_[h57 % i58->size_]);
              COUNT_mFOLLOWER5_entry* e71;
              do if ((e71=n58->obj) && h57 == n58->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se80, *e71)) {
                long z_tweet_id = e71->Z_TWEET_ID;
                long v85 = e71->__av;
                add_to_temp_map(_c14, st51.modify(z_tweet_id,(v85 * -1L)));
              } while ((n58=n58->nxt));
            }
            { //slice
              const HASH_RES_t h58 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se81.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i59 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n59 = &(i59->buckets_[h58 % i59->size_]);
              COUNT_mFOLLOWER24_entry* e72;
              do if ((e72=n59->obj) && h58 == n59->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se81, *e72)) {
                long z_tweet_id = e72->Z_TWEET_ID;
                long v86 = e72->__av;
                (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c14, st52.modify(z_tweet_id,(v86 * -1L))) : voidFunc());
              } while ((n59=n59->nxt));
            }
            { //slice
              const HASH_RES_t h59 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se82.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i60 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n60 = &(i60->buckets_[h59 % i60->size_]);
              COUNT_mFOLLOWER11_entry* e73;
              do if ((e73=n60->obj) && h59 == n60->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se82, *e73)) {
                long z_tweet_id = e73->Z_TWEET_ID;
                long v87 = e73->__av;
                (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c14, st53.modify(z_tweet_id,v87)) : voidFunc());
              } while ((n60=n60->nxt));
            }
            { //slice
              const HASH_RES_t h60 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se84.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i61 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n61 = &(i61->buckets_[h60 % i61->size_]);
              COUNT_mFOLLOWER24_entry* e74;
              do if ((e74=n61->obj) && h60 == n61->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se84, *e74)) {
                long z_tweet_id = e74->Z_TWEET_ID;
                long v88 = e74->__av;
                add_to_temp_map(_c14, st54.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se83.modify(follower_follower_id,follower_user_id)) * v88)));
              } while ((n61=n61->nxt));
            }
            { //temp foreach
              tuple2_L_L* e75 = _c14.head;
              while(e75){
                long z_tweet_id = e75->_1;
                long v89 = e75->__av;
                COUNT.addOrDelOnZero(se67.modify(a_user_id,z_tweet_id),(v83 * v89));
                e75 = e75->nxt;
              }
            }
          } while ((n57=n57->nxt));
        }
        { //slice
          const HASH_RES_t h64 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se88.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i62 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n62 = &(i62->buckets_[h64 % i62->size_]);
          COUNT_mFOLLOWER10_entry* e76;
          do if ((e76=n62->obj) && h64 == n62->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se88, *e76)) {
            long a_user_id = e76->A_USER_ID;
            long v90 = e76->__av;
            _c15.clear();
            { //slice
              const HASH_RES_t h62 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se86.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i63 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n63 = &(i63->buckets_[h62 % i63->size_]);
              COUNT_mFOLLOWER11_entry* e77;
              do if ((e77=n63->obj) && h62 == n63->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se86, *e77)) {
                long z_tweet_id = e77->Z_TWEET_ID;
                long v92 = e77->__av;
                add_to_temp_map(_c15, st55.modify(z_tweet_id,(v92 * -1L)));
              } while ((n63=n63->nxt));
            }
            { //slice
              const HASH_RES_t h63 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se87.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i64 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n64 = &(i64->buckets_[h63 % i64->size_]);
              COUNT_mFOLLOWER24_entry* e78;
              do if ((e78=n64->obj) && h63 == n64->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se87, *e78)) {
                long z_tweet_id = e78->Z_TWEET_ID;
                long v93 = e78->__av;
                (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c15, st56.modify(z_tweet_id,v93)) : voidFunc());
              } while ((n64=n64->nxt));
            }
            { //temp foreach
              tuple2_L_L* e79 = _c15.head;
              while(e79){
                long z_tweet_id = e79->_1;
                long v94 = e79->__av;
                COUNT.addOrDelOnZero(se67.modify(a_user_id,z_tweet_id),(v90 * v94));
                e79 = e79->nxt;
              }
            }
          } while ((n62=n62->nxt));
        }
        { //slice
          const HASH_RES_t h66 = COUNT_mFOLLOWER23_mapkey1_idxfn::hash(se90.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER23_map_1* i65 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_1*>(COUNT_mFOLLOWER23.index[1]);
          HashIndex_COUNT_mFOLLOWER23_map_1::IdxNode* n65 = &(i65->buckets_[h66 % i65->size_]);
          COUNT_mFOLLOWER23_entry* e80;
          do if ((e80=n65->obj) && h66 == n65->hash && COUNT_mFOLLOWER23_mapkey1_idxfn::equals(se90, *e80)) {
            long a_user_id = e80->A_USER_ID;
            long v95 = e80->__av;
            { //slice
              const HASH_RES_t h65 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se89.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i66 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n66 = &(i66->buckets_[h65 % i66->size_]);
              COUNT_mFOLLOWER24_entry* e81;
              do if ((e81=n66->obj) && h65 == n66->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se89, *e81)) {
                long z_tweet_id = e81->Z_TWEET_ID;
                long v96 = e81->__av;
                COUNT.addOrDelOnZero(se67.modify(a_user_id,z_tweet_id),(v95 * (v96 * -1L)));
              } while ((n66=n66->nxt));
            }
          } while ((n65=n65->nxt));
        }
        _c16.clear();
        { //slice
          const HASH_RES_t h67 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se92.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i67 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n67 = &(i67->buckets_[h67 % i67->size_]);
          COUNT_mFOLLOWER4_entry* e82;
          do if ((e82=n67->obj) && h67 == n67->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se92, *e82)) {
            long count_mtweettweet_user_id = e82->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v98 = e82->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c16, st57.modify(count_mtweettweet_user_id,(v98 * -1L))) : voidFunc());
          } while ((n67=n67->nxt));
        }
        (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c16, st58.modify(follower_follower_id,(COUNT_mFOLLOWER4.getValueOrDefault(se93.modify(follower_follower_id,follower_user_id)) * -1L))) : voidFunc());
        { //slice
          const HASH_RES_t h68 = COUNT_mFOLLOWER23_mapkey0_idxfn::hash(se94.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER23_map_0* i68 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_0*>(COUNT_mFOLLOWER23.index[2]);
          HashIndex_COUNT_mFOLLOWER23_map_0::IdxNode* n68 = &(i68->buckets_[h68 % i68->size_]);
          COUNT_mFOLLOWER23_entry* e83;
          do if ((e83=n68->obj) && h68 == n68->hash && COUNT_mFOLLOWER23_mapkey0_idxfn::equals(se94, *e83)) {
            long count_mtweettweet_user_id = e83->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v99 = e83->__av;
            add_to_temp_map(_c16, st59.modify(count_mtweettweet_user_id,(v99 * -1L)));
          } while ((n68=n68->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c16, st60.modify(follower_follower_id,(COUNT_mFOLLOWER4.getValueOrDefault(se95.modify(follower_follower_id,follower_user_id)) * -1L))) : voidFunc());
        { //slice
          const HASH_RES_t h69 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se96.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER10_map_0* i69 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
          HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n69 = &(i69->buckets_[h69 % i69->size_]);
          COUNT_mFOLLOWER10_entry* e84;
          do if ((e84=n69->obj) && h69 == n69->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se96, *e84)) {
            long count_mtweettweet_user_id = e84->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v100 = e84->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c16, st61.modify(count_mtweettweet_user_id,v100)) : voidFunc());
          } while ((n69=n69->nxt));
        }
        (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c16, st62.modify(follower_follower_id,1L)) : voidFunc());
        { //slice
          const HASH_RES_t h70 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se98.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i70 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n70 = &(i70->buckets_[h70 % i70->size_]);
          COUNT_mFOLLOWER4_entry* e85;
          do if ((e85=n70->obj) && h70 == n70->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se98, *e85)) {
            long count_mtweettweet_user_id = e85->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v101 = e85->__av;
            add_to_temp_map(_c16, st63.modify(count_mtweettweet_user_id,(COUNT_mFOLLOWER4.getValueOrDefault(se97.modify(follower_follower_id,follower_user_id)) * v101)));
          } while ((n70=n70->nxt));
        }
        add_to_temp_map(_c16, st64.modify(follower_follower_id,COUNT_mFOLLOWER10.getValueOrDefault(se99.modify(follower_follower_id,follower_user_id))));
        { //temp foreach
          tuple2_L_L* e86 = _c16.head;
          while(e86){
            long count_mtweettweet_user_id = e86->_1;
            long v102 = e86->__av;
            COUNT_mTWEET1.addOrDelOnZero(se91.modify(follower_user_id,count_mtweettweet_user_id),v102);
            e86 = e86->nxt;
          }
        }
        { //slice
          const HASH_RES_t h73 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se103.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i71 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n71 = &(i71->buckets_[h73 % i71->size_]);
          COUNT_mFOLLOWER4_entry* e87;
          do if ((e87=n71->obj) && h73 == n71->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se103, *e87)) {
            long a_user_id = e87->A_USER_ID;
            long v103 = e87->__av;
            _c17.clear();
            { //slice
              const HASH_RES_t h71 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se100.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER10_map_0* i72 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
              HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n72 = &(i72->buckets_[h71 % i72->size_]);
              COUNT_mFOLLOWER10_entry* e88;
              do if ((e88=n72->obj) && h71 == n72->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se100, *e88)) {
                long count_mtweettweet_user_id = e88->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v105 = e88->__av;
                add_to_temp_map(_c17, st65.modify(count_mtweettweet_user_id,(v105 * -1L)));
              } while ((n72=n72->nxt));
            }
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c17, st66.modify(follower_follower_id,-1L)) : voidFunc());
            { //slice
              const HASH_RES_t h72 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se101.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i73 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n73 = &(i73->buckets_[h72 % i73->size_]);
              COUNT_mFOLLOWER4_entry* e89;
              do if ((e89=n73->obj) && h72 == n73->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se101, *e89)) {
                long count_mtweettweet_user_id = e89->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v106 = e89->__av;
                (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c17, st67.modify(count_mtweettweet_user_id,v106)) : voidFunc());
              } while ((n73=n73->nxt));
            }
            add_to_temp_map(_c17, st68.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se102.modify(follower_follower_id,follower_user_id))));
            { //temp foreach
              tuple2_L_L* e90 = _c17.head;
              while(e90){
                long count_mtweettweet_user_id = e90->_1;
                long v107 = e90->__av;
                COUNT_mTWEET1.addOrDelOnZero(se91.modify(a_user_id,count_mtweettweet_user_id),(v103 * v107));
                e90 = e90->nxt;
              }
            }
          } while ((n71=n71->nxt));
        }
        { //slice
          const HASH_RES_t h75 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se105.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i74 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n74 = &(i74->buckets_[h75 % i74->size_]);
          COUNT_mFOLLOWER10_entry* e91;
          do if ((e91=n74->obj) && h75 == n74->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se105, *e91)) {
            long a_user_id = e91->A_USER_ID;
            long v108 = e91->__av;
            _c18.clear();
            { //slice
              const HASH_RES_t h74 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se104.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i75 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n75 = &(i75->buckets_[h74 % i75->size_]);
              COUNT_mFOLLOWER4_entry* e92;
              do if ((e92=n75->obj) && h74 == n75->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se104, *e92)) {
                long count_mtweettweet_user_id = e92->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v110 = e92->__av;
                add_to_temp_map(_c18, st69.modify(count_mtweettweet_user_id,(v110 * -1L)));
              } while ((n75=n75->nxt));
            }
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c18, st70.modify(follower_follower_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e93 = _c18.head;
              while(e93){
                long count_mtweettweet_user_id = e93->_1;
                long v111 = e93->__av;
                COUNT_mTWEET1.addOrDelOnZero(se91.modify(a_user_id,count_mtweettweet_user_id),(v108 * v111));
                e93 = e93->nxt;
              }
            }
          } while ((n74=n74->nxt));
        }
        { //slice
          const HASH_RES_t h76 = COUNT_mFOLLOWER23_mapkey1_idxfn::hash(se106.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER23_map_1* i76 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_1*>(COUNT_mFOLLOWER23.index[1]);
          HashIndex_COUNT_mFOLLOWER23_map_1::IdxNode* n76 = &(i76->buckets_[h76 % i76->size_]);
          COUNT_mFOLLOWER23_entry* e94;
          do if ((e94=n76->obj) && h76 == n76->hash && COUNT_mFOLLOWER23_mapkey1_idxfn::equals(se106, *e94)) {
            long a_user_id = e94->A_USER_ID;
            long v112 = e94->__av;
            COUNT_mTWEET1.addOrDelOnZero(se91.modify(a_user_id,follower_follower_id),(v112 * -1L));
          } while ((n76=n76->nxt));
        }
        _c19.clear();
        { //slice
          const HASH_RES_t h77 = COUNT_mFOLLOWER5_mapkey0_idxfn::hash(se108.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER5_map_0* i77 = static_cast<HashIndex_COUNT_mFOLLOWER5_map_0*>(COUNT_mFOLLOWER5.index[1]);
          HashIndex_COUNT_mFOLLOWER5_map_0::IdxNode* n77 = &(i77->buckets_[h77 % i77->size_]);
          COUNT_mFOLLOWER5_entry* e95;
          do if ((e95=n77->obj) && h77 == n77->hash && COUNT_mFOLLOWER5_mapkey0_idxfn::equals(se108, *e95)) {
            long z_tweet_id = e95->Z_TWEET_ID;
            long v114 = e95->__av;
            add_to_temp_map(_c19, st71.modify(z_tweet_id,(v114 * -1L)));
          } while ((n77=n77->nxt));
        }
        { //slice
          const HASH_RES_t h78 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se109.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i78 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n78 = &(i78->buckets_[h78 % i78->size_]);
          COUNT_mFOLLOWER24_entry* e96;
          do if ((e96=n78->obj) && h78 == n78->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se109, *e96)) {
            long z_tweet_id = e96->Z_TWEET_ID;
            long v115 = e96->__av;
            (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c19, st72.modify(z_tweet_id,(v115 * -1L))) : voidFunc());
          } while ((n78=n78->nxt));
        }
        { //slice
          const HASH_RES_t h79 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se110.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i79 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n79 = &(i79->buckets_[h79 % i79->size_]);
          COUNT_mFOLLOWER11_entry* e97;
          do if ((e97=n79->obj) && h79 == n79->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se110, *e97)) {
            long z_tweet_id = e97->Z_TWEET_ID;
            long v116 = e97->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c19, st73.modify(z_tweet_id,v116)) : voidFunc());
          } while ((n79=n79->nxt));
        }
        { //slice
          const HASH_RES_t h80 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se112.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i80 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n80 = &(i80->buckets_[h80 % i80->size_]);
          COUNT_mFOLLOWER24_entry* e98;
          do if ((e98=n80->obj) && h80 == n80->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se112, *e98)) {
            long z_tweet_id = e98->Z_TWEET_ID;
            long v117 = e98->__av;
            add_to_temp_map(_c19, st74.modify(z_tweet_id,(COUNT_mFOLLOWER4.getValueOrDefault(se111.modify(follower_follower_id,follower_user_id)) * v117)));
          } while ((n80=n80->nxt));
        }
        { //temp foreach
          tuple2_L_L* e99 = _c19.head;
          while(e99){
            long z_tweet_id = e99->_1;
            long v118 = e99->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se107.modify(follower_user_id,z_tweet_id),v118);
            e99 = e99->nxt;
          }
        }
        { //slice
          const HASH_RES_t h83 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se115.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i81 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n81 = &(i81->buckets_[h83 % i81->size_]);
          COUNT_mFOLLOWER4_entry* e100;
          do if ((e100=n81->obj) && h83 == n81->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se115, *e100)) {
            long count_mfollowerfollower_follower_id = e100->A_USER_ID;
            long v119 = e100->__av;
            _c20.clear();
            { //slice
              const HASH_RES_t h81 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se113.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER11_map_0* i82 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
              HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n82 = &(i82->buckets_[h81 % i82->size_]);
              COUNT_mFOLLOWER11_entry* e101;
              do if ((e101=n82->obj) && h81 == n82->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se113, *e101)) {
                long z_tweet_id = e101->Z_TWEET_ID;
                long v121 = e101->__av;
                add_to_temp_map(_c20, st75.modify(z_tweet_id,(v121 * -1L)));
              } while ((n82=n82->nxt));
            }
            { //slice
              const HASH_RES_t h82 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se114.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i83 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n83 = &(i83->buckets_[h82 % i83->size_]);
              COUNT_mFOLLOWER24_entry* e102;
              do if ((e102=n83->obj) && h82 == n83->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se114, *e102)) {
                long z_tweet_id = e102->Z_TWEET_ID;
                long v122 = e102->__av;
                (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c20, st76.modify(z_tweet_id,v122)) : voidFunc());
              } while ((n83=n83->nxt));
            }
            { //temp foreach
              tuple2_L_L* e103 = _c20.head;
              while(e103){
                long z_tweet_id = e103->_1;
                long v123 = e103->__av;
                COUNT_mFOLLOWER2.addOrDelOnZero(se107.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v119 * v123));
                e103 = e103->nxt;
              }
            }
          } while ((n81=n81->nxt));
        }
        { //slice
          const HASH_RES_t h85 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se117.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i84 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n84 = &(i84->buckets_[h85 % i84->size_]);
          COUNT_mFOLLOWER10_entry* e104;
          do if ((e104=n84->obj) && h85 == n84->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se117, *e104)) {
            long count_mfollowerfollower_follower_id = e104->A_USER_ID;
            long v124 = e104->__av;
            { //slice
              const HASH_RES_t h84 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se116.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i85 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n85 = &(i85->buckets_[h84 % i85->size_]);
              COUNT_mFOLLOWER24_entry* e105;
              do if ((e105=n85->obj) && h84 == n85->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se116, *e105)) {
                long z_tweet_id = e105->Z_TWEET_ID;
                long v125 = e105->__av;
                COUNT_mFOLLOWER2.addOrDelOnZero(se107.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v124 * (v125 * -1L)));
              } while ((n85=n85->nxt));
            }
          } while ((n84=n84->nxt));
        }
        _c21.clear();
        { //slice
          const HASH_RES_t h86 = COUNT_mFOLLOWER11_mapkey0_idxfn::hash(se119.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER11_map_0* i86 = static_cast<HashIndex_COUNT_mFOLLOWER11_map_0*>(COUNT_mFOLLOWER11.index[1]);
          HashIndex_COUNT_mFOLLOWER11_map_0::IdxNode* n86 = &(i86->buckets_[h86 % i86->size_]);
          COUNT_mFOLLOWER11_entry* e106;
          do if ((e106=n86->obj) && h86 == n86->hash && COUNT_mFOLLOWER11_mapkey0_idxfn::equals(se119, *e106)) {
            long z_tweet_id = e106->Z_TWEET_ID;
            long v127 = e106->__av;
            add_to_temp_map(_c21, st77.modify(z_tweet_id,(v127 * -1L)));
          } while ((n86=n86->nxt));
        }
        { //slice
          const HASH_RES_t h87 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se120.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i87 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n87 = &(i87->buckets_[h87 % i87->size_]);
          COUNT_mFOLLOWER24_entry* e107;
          do if ((e107=n87->obj) && h87 == n87->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se120, *e107)) {
            long z_tweet_id = e107->Z_TWEET_ID;
            long v128 = e107->__av;
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c21, st78.modify(z_tweet_id,v128)) : voidFunc());
          } while ((n87=n87->nxt));
        }
        { //temp foreach
          tuple2_L_L* e108 = _c21.head;
          while(e108){
            long z_tweet_id = e108->_1;
            long v129 = e108->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se118.modify(follower_user_id,z_tweet_id),v129);
            e108 = e108->nxt;
          }
        }
        { //slice
          const HASH_RES_t h89 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se122.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i88 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n88 = &(i88->buckets_[h89 % i88->size_]);
          COUNT_mFOLLOWER4_entry* e109;
          do if ((e109=n88->obj) && h89 == n88->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se122, *e109)) {
            long count_mfollowerfollower_follower_id = e109->A_USER_ID;
            long v130 = e109->__av;
            { //slice
              const HASH_RES_t h88 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se121.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER24_map_0* i89 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
              HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n89 = &(i89->buckets_[h88 % i89->size_]);
              COUNT_mFOLLOWER24_entry* e110;
              do if ((e110=n89->obj) && h88 == n89->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se121, *e110)) {
                long z_tweet_id = e110->Z_TWEET_ID;
                long v131 = e110->__av;
                COUNT_mFOLLOWER5.addOrDelOnZero(se118.modify(count_mfollowerfollower_follower_id,z_tweet_id),(v130 * (v131 * -1L)));
              } while ((n89=n89->nxt));
            }
          } while ((n88=n88->nxt));
        }
        { //slice
          const HASH_RES_t h90 = COUNT_mFOLLOWER24_mapkey0_idxfn::hash(se124.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER24_map_0* i90 = static_cast<HashIndex_COUNT_mFOLLOWER24_map_0*>(COUNT_mFOLLOWER24.index[1]);
          HashIndex_COUNT_mFOLLOWER24_map_0::IdxNode* n90 = &(i90->buckets_[h90 % i90->size_]);
          COUNT_mFOLLOWER24_entry* e111;
          do if ((e111=n90->obj) && h90 == n90->hash && COUNT_mFOLLOWER24_mapkey0_idxfn::equals(se124, *e111)) {
            long z_tweet_id = e111->Z_TWEET_ID;
            long v132 = e111->__av;
            COUNT_mFOLLOWER11.addOrDelOnZero(se123.modify(follower_user_id,z_tweet_id),(v132 * -1L));
          } while ((n90=n90->nxt));
        }
        _c22.clear();
        { //slice
          const HASH_RES_t h91 = COUNT_mFOLLOWER10_mapkey0_idxfn::hash(se126.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER10_map_0* i91 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_0*>(COUNT_mFOLLOWER10.index[2]);
          HashIndex_COUNT_mFOLLOWER10_map_0::IdxNode* n91 = &(i91->buckets_[h91 % i91->size_]);
          COUNT_mFOLLOWER10_entry* e112;
          do if ((e112=n91->obj) && h91 == n91->hash && COUNT_mFOLLOWER10_mapkey0_idxfn::equals(se126, *e112)) {
            long count_mfollowerfollower_user_id = e112->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v134 = e112->__av;
            add_to_temp_map(_c22, st79.modify(count_mfollowerfollower_user_id,(v134 * -1L)));
          } while ((n91=n91->nxt));
        }
        (/*if */(follower_user_id == follower_follower_id && follower_follower_id == follower_user_id) ? add_to_temp_map(_c22, st80.modify(follower_follower_id,-1L)) : voidFunc());
        { //slice
          const HASH_RES_t h92 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se127.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i92 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n92 = &(i92->buckets_[h92 % i92->size_]);
          COUNT_mFOLLOWER4_entry* e113;
          do if ((e113=n92->obj) && h92 == n92->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se127, *e113)) {
            long count_mfollowerfollower_user_id = e113->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v135 = e113->__av;
            (/*if */(follower_user_id == follower_follower_id) ? add_to_temp_map(_c22, st81.modify(count_mfollowerfollower_user_id,v135)) : voidFunc());
          } while ((n92=n92->nxt));
        }
        add_to_temp_map(_c22, st82.modify(follower_follower_id,COUNT_mFOLLOWER4.getValueOrDefault(se128.modify(follower_follower_id,follower_user_id))));
        { //temp foreach
          tuple2_L_L* e114 = _c22.head;
          while(e114){
            long count_mfollowerfollower_user_id = e114->_1;
            long v136 = e114->__av;
            COUNT_mFOLLOWER23.addOrDelOnZero(se125.modify(follower_user_id,count_mfollowerfollower_user_id),v136);
            e114 = e114->nxt;
          }
        }
        { //slice
          const HASH_RES_t h94 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se130.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i93 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n93 = &(i93->buckets_[h94 % i93->size_]);
          COUNT_mFOLLOWER4_entry* e115;
          do if ((e115=n93->obj) && h94 == n93->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se130, *e115)) {
            long a_user_id = e115->A_USER_ID;
            long v137 = e115->__av;
            _c23.clear();
            { //slice
              const HASH_RES_t h93 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se129.modify0(follower_follower_id));
              const HashIndex_COUNT_mFOLLOWER4_map_0* i94 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
              HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n94 = &(i94->buckets_[h93 % i94->size_]);
              COUNT_mFOLLOWER4_entry* e116;
              do if ((e116=n94->obj) && h93 == n94->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se129, *e116)) {
                long count_mfollowerfollower_user_id = e116->COUNT_mFOLLOWERFOLLOWER_USER_ID;
                long v139 = e116->__av;
                add_to_temp_map(_c23, st83.modify(count_mfollowerfollower_user_id,(v139 * -1L)));
              } while ((n94=n94->nxt));
            }
            (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c23, st84.modify(follower_follower_id,1L)) : voidFunc());
            { //temp foreach
              tuple2_L_L* e117 = _c23.head;
              while(e117){
                long count_mfollowerfollower_user_id = e117->_1;
                long v140 = e117->__av;
                COUNT_mFOLLOWER23.addOrDelOnZero(se125.modify(a_user_id,count_mfollowerfollower_user_id),(v137 * v140));
                e117 = e117->nxt;
              }
            }
          } while ((n93=n93->nxt));
        }
        { //slice
          const HASH_RES_t h95 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se131.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i95 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n95 = &(i95->buckets_[h95 % i95->size_]);
          COUNT_mFOLLOWER10_entry* e118;
          do if ((e118=n95->obj) && h95 == n95->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se131, *e118)) {
            long a_user_id = e118->A_USER_ID;
            long v141 = e118->__av;
            COUNT_mFOLLOWER23.addOrDelOnZero(se125.modify(a_user_id,follower_follower_id),(v141 * -1L));
          } while ((n95=n95->nxt));
        }
        _c24.clear();
        { //slice
          const HASH_RES_t h96 = COUNT_mFOLLOWER4_mapkey0_idxfn::hash(se133.modify0(follower_follower_id));
          const HashIndex_COUNT_mFOLLOWER4_map_0* i96 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_0*>(COUNT_mFOLLOWER4.index[2]);
          HashIndex_COUNT_mFOLLOWER4_map_0::IdxNode* n96 = &(i96->buckets_[h96 % i96->size_]);
          COUNT_mFOLLOWER4_entry* e119;
          do if ((e119=n96->obj) && h96 == n96->hash && COUNT_mFOLLOWER4_mapkey0_idxfn::equals(se133, *e119)) {
            long count_mfollowerfollower_user_id = e119->COUNT_mFOLLOWERFOLLOWER_USER_ID;
            long v143 = e119->__av;
            add_to_temp_map(_c24, st85.modify(count_mfollowerfollower_user_id,(v143 * -1L)));
          } while ((n96=n96->nxt));
        }
        (/*if */(follower_follower_id == follower_user_id) ? add_to_temp_map(_c24, st86.modify(follower_follower_id,1L)) : voidFunc());
        { //temp foreach
          tuple2_L_L* e120 = _c24.head;
          while(e120){
            long count_mfollowerfollower_user_id = e120->_1;
            long v144 = e120->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se132.modify(follower_user_id,count_mfollowerfollower_user_id),v144);
            e120 = e120->nxt;
          }
        }
        { //slice
          const HASH_RES_t h97 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se134.modify1(follower_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i97 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n97 = &(i97->buckets_[h97 % i97->size_]);
          COUNT_mFOLLOWER4_entry* e121;
          do if ((e121=n97->obj) && h97 == n97->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se134, *e121)) {
            long a_user_id = e121->A_USER_ID;
            long v145 = e121->__av;
            COUNT_mFOLLOWER10.addOrDelOnZero(se132.modify(a_user_id,follower_follower_id),(v145 * -1L));
          } while ((n97=n97->nxt));
        }
        COUNT_mFOLLOWER4.addOrDelOnZero(se135.modify(follower_user_id,follower_follower_id),-1L);
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
          const HASH_RES_t h98 = COUNT_mTWEET1_mapkey1_idxfn::hash(se137.modify1(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_1* i98 = static_cast<HashIndex_COUNT_mTWEET1_map_1*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_1::IdxNode* n98 = &(i98->buckets_[h98 % i98->size_]);
          COUNT_mTWEET1_entry* e122;
          do if ((e122=n98->obj) && h98 == n98->hash && COUNT_mTWEET1_mapkey1_idxfn::equals(se137, *e122)) {
            long a_user_id = e122->A_USER_ID;
            long v146 = e122->__av;
		api.useDiff(a_user_id, tweet_tweet_id);
            COUNT.addOrDelOnZero(se136.modify(a_user_id,tweet_tweet_id),v146);
          } while ((n98=n98->nxt));
        }
        { //slice
          const HASH_RES_t h99 = COUNT_mFOLLOWER23_mapkey1_idxfn::hash(se139.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER23_map_1* i99 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_1*>(COUNT_mFOLLOWER23.index[1]);
          HashIndex_COUNT_mFOLLOWER23_map_1::IdxNode* n99 = &(i99->buckets_[h99 % i99->size_]);
          COUNT_mFOLLOWER23_entry* e123;
          do if ((e123=n99->obj) && h99 == n99->hash && COUNT_mFOLLOWER23_mapkey1_idxfn::equals(se139, *e123)) {
            long count_mfollowerfollower_follower_id = e123->A_USER_ID;
            long v147 = e123->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se138.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),v147);
          } while ((n99=n99->nxt));
        }
        { //slice
          const HASH_RES_t h100 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se141.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i100 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n100 = &(i100->buckets_[h100 % i100->size_]);
          COUNT_mFOLLOWER10_entry* e124;
          do if ((e124=n100->obj) && h100 == n100->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se141, *e124)) {
            long count_mfollowerfollower_follower_id = e124->A_USER_ID;
            long v148 = e124->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se140.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),v148);
          } while ((n100=n100->nxt));
        }
        { //slice
          const HASH_RES_t h101 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se143.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i101 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n101 = &(i101->buckets_[h101 % i101->size_]);
          COUNT_mFOLLOWER4_entry* e125;
          do if ((e125=n101->obj) && h101 == n101->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se143, *e125)) {
            long count_mfollowerfollower_follower_id = e125->A_USER_ID;
            long v149 = e125->__av;
            COUNT_mFOLLOWER11.addOrDelOnZero(se142.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),v149);
          } while ((n101=n101->nxt));
        }
        COUNT_mFOLLOWER24.addOrDelOnZero(se144.modify(tweet_user_id,tweet_tweet_id),1L);
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
          const HASH_RES_t h102 = COUNT_mTWEET1_mapkey1_idxfn::hash(se146.modify1(tweet_user_id));
          const HashIndex_COUNT_mTWEET1_map_1* i102 = static_cast<HashIndex_COUNT_mTWEET1_map_1*>(COUNT_mTWEET1.index[1]);
          HashIndex_COUNT_mTWEET1_map_1::IdxNode* n102 = &(i102->buckets_[h102 % i102->size_]);
          COUNT_mTWEET1_entry* e126;
          do if ((e126=n102->obj) && h102 == n102->hash && COUNT_mTWEET1_mapkey1_idxfn::equals(se146, *e126)) {
            long a_user_id = e126->A_USER_ID;
            long v150 = e126->__av;
            COUNT.addOrDelOnZero(se145.modify(a_user_id,tweet_tweet_id),(v150 * -1L));
          } while ((n102=n102->nxt));
        }
        { //slice
          const HASH_RES_t h103 = COUNT_mFOLLOWER23_mapkey1_idxfn::hash(se148.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER23_map_1* i103 = static_cast<HashIndex_COUNT_mFOLLOWER23_map_1*>(COUNT_mFOLLOWER23.index[1]);
          HashIndex_COUNT_mFOLLOWER23_map_1::IdxNode* n103 = &(i103->buckets_[h103 % i103->size_]);
          COUNT_mFOLLOWER23_entry* e127;
          do if ((e127=n103->obj) && h103 == n103->hash && COUNT_mFOLLOWER23_mapkey1_idxfn::equals(se148, *e127)) {
            long count_mfollowerfollower_follower_id = e127->A_USER_ID;
            long v151 = e127->__av;
            COUNT_mFOLLOWER2.addOrDelOnZero(se147.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),(v151 * -1L));
          } while ((n103=n103->nxt));
        }
        { //slice
          const HASH_RES_t h104 = COUNT_mFOLLOWER10_mapkey1_idxfn::hash(se150.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER10_map_1* i104 = static_cast<HashIndex_COUNT_mFOLLOWER10_map_1*>(COUNT_mFOLLOWER10.index[1]);
          HashIndex_COUNT_mFOLLOWER10_map_1::IdxNode* n104 = &(i104->buckets_[h104 % i104->size_]);
          COUNT_mFOLLOWER10_entry* e128;
          do if ((e128=n104->obj) && h104 == n104->hash && COUNT_mFOLLOWER10_mapkey1_idxfn::equals(se150, *e128)) {
            long count_mfollowerfollower_follower_id = e128->A_USER_ID;
            long v152 = e128->__av;
            COUNT_mFOLLOWER5.addOrDelOnZero(se149.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),(v152 * -1L));
          } while ((n104=n104->nxt));
        }
        { //slice
          const HASH_RES_t h105 = COUNT_mFOLLOWER4_mapkey1_idxfn::hash(se152.modify1(tweet_user_id));
          const HashIndex_COUNT_mFOLLOWER4_map_1* i105 = static_cast<HashIndex_COUNT_mFOLLOWER4_map_1*>(COUNT_mFOLLOWER4.index[1]);
          HashIndex_COUNT_mFOLLOWER4_map_1::IdxNode* n105 = &(i105->buckets_[h105 % i105->size_]);
          COUNT_mFOLLOWER4_entry* e129;
          do if ((e129=n105->obj) && h105 == n105->hash && COUNT_mFOLLOWER4_mapkey1_idxfn::equals(se152, *e129)) {
            long count_mfollowerfollower_follower_id = e129->A_USER_ID;
            long v153 = e129->__av;
            COUNT_mFOLLOWER11.addOrDelOnZero(se151.modify(count_mfollowerfollower_follower_id,tweet_tweet_id),(v153 * -1L));
          } while ((n105=n105->nxt));
        }
        COUNT_mFOLLOWER24.addOrDelOnZero(se153.modify(tweet_user_id,tweet_tweet_id),-1L);
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
    COUNT_mFOLLOWER4_entry se2;
    tuple2_L_L st1;
    COUNT_mFOLLOWER24_entry se3;
    tuple2_L_L st2;
    COUNT_mFOLLOWER5_entry se4;
    tuple2_L_L st3;
    COUNT_mFOLLOWER11_entry se5;
    COUNT_mFOLLOWER4_entry se6;
    tuple2_L_L st4;
    COUNT_mFOLLOWER11_entry se7;
    tuple2_L_L st5;
    COUNT_mFOLLOWER24_entry se8;
    tuple2_L_L st6;
    COUNT_mFOLLOWER2_entry se9;
    COUNT_mFOLLOWER10_entry se10;
    tuple2_L_L st7;
    COUNT_mFOLLOWER24_entry se11;
    tuple2_L_L st8;
    COUNT_mFOLLOWER11_entry se12;
    tuple2_L_L st9;
    COUNT_mFOLLOWER24_entry se13;
    tuple2_L_L st10;
    COUNT_mFOLLOWER5_entry se14;
    COUNT_mFOLLOWER4_entry se15;
    tuple2_L_L st11;
    COUNT_mFOLLOWER24_entry se16;
    COUNT_mFOLLOWER4_entry se17;
    tuple2_L_L st12;
    COUNT_mFOLLOWER11_entry se18;
    tuple2_L_L st13;
    COUNT_mFOLLOWER24_entry se19;
    COUNT_mFOLLOWER10_entry se20;
    COUNT_mFOLLOWER24_entry se21;
    COUNT_mFOLLOWER23_entry se22;
    COUNT_mTWEET1_entry se23;
    COUNT_mFOLLOWER4_entry se24;
    tuple2_L_L st14;
    tuple2_L_L st15;
    COUNT_mFOLLOWER10_entry se25;
    tuple2_L_L st16;
    COUNT_mFOLLOWER4_entry se26;
    COUNT_mFOLLOWER4_entry se27;
    tuple2_L_L st17;
    COUNT_mFOLLOWER4_entry se28;
    tuple2_L_L st18;
    tuple2_L_L st19;
    COUNT_mFOLLOWER23_entry se29;
    COUNT_mFOLLOWER10_entry se30;
    tuple2_L_L st20;
    tuple2_L_L st21;
    COUNT_mFOLLOWER4_entry se31;
    tuple2_L_L st22;
    tuple2_L_L st23;
    COUNT_mFOLLOWER10_entry se32;
    COUNT_mFOLLOWER4_entry se33;
    tuple2_L_L st24;
    COUNT_mFOLLOWER4_entry se34;
    tuple2_L_L st25;
    COUNT_mFOLLOWER4_entry se35;
    tuple2_L_L st26;
    COUNT_mFOLLOWER10_entry se36;
    COUNT_mFOLLOWER23_entry se37;
    COUNT_mFOLLOWER2_entry se38;
    tuple2_L_L st27;
    COUNT_mFOLLOWER11_entry se39;
    tuple2_L_L st28;
    COUNT_mFOLLOWER24_entry se40;
    tuple2_L_L st29;
    COUNT_mFOLLOWER5_entry se41;
    COUNT_mFOLLOWER4_entry se42;
    tuple2_L_L st30;
    COUNT_mFOLLOWER24_entry se43;
    tuple2_L_L st31;
    COUNT_mFOLLOWER11_entry se44;
    tuple2_L_L st32;
    COUNT_mFOLLOWER24_entry se45;
    COUNT_mFOLLOWER4_entry se46;
    COUNT_mFOLLOWER24_entry se47;
    COUNT_mFOLLOWER10_entry se48;
    COUNT_mFOLLOWER5_entry se49;
    tuple2_L_L st33;
    COUNT_mFOLLOWER11_entry se50;
    tuple2_L_L st34;
    COUNT_mFOLLOWER24_entry se51;
    COUNT_mFOLLOWER24_entry se52;
    COUNT_mFOLLOWER4_entry se53;
    COUNT_mFOLLOWER11_entry se54;
    COUNT_mFOLLOWER24_entry se55;
    COUNT_mFOLLOWER23_entry se56;
    tuple2_L_L st35;
    COUNT_mFOLLOWER4_entry se57;
    tuple2_L_L st36;
    tuple2_L_L st37;
    COUNT_mFOLLOWER10_entry se58;
    COUNT_mFOLLOWER4_entry se59;
    tuple2_L_L st38;
    tuple2_L_L st39;
    COUNT_mFOLLOWER4_entry se60;
    tuple2_L_L st40;
    COUNT_mFOLLOWER4_entry se61;
    COUNT_mFOLLOWER10_entry se62;
    COUNT_mFOLLOWER10_entry se63;
    tuple2_L_L st41;
    COUNT_mFOLLOWER4_entry se64;
    tuple2_L_L st42;
    COUNT_mFOLLOWER4_entry se65;
    COUNT_mFOLLOWER4_entry se66;
    COUNT_entry se67;
    tuple2_L_L st43;
    COUNT_mFOLLOWER11_entry se68;
    COUNT_mFOLLOWER4_entry se69;
    tuple2_L_L st44;
    COUNT_mFOLLOWER24_entry se70;
    tuple2_L_L st45;
    COUNT_mFOLLOWER2_entry se71;
    COUNT_mFOLLOWER4_entry se72;
    tuple2_L_L st46;
    COUNT_mFOLLOWER24_entry se73;
    tuple2_L_L st47;
    COUNT_mFOLLOWER5_entry se74;
    tuple2_L_L st48;
    COUNT_mFOLLOWER24_entry se75;
    COUNT_mFOLLOWER4_entry se76;
    tuple2_L_L st49;
    COUNT_mFOLLOWER11_entry se77;
    COUNT_mFOLLOWER10_entry se78;
    tuple2_L_L st50;
    COUNT_mFOLLOWER24_entry se79;
    tuple2_L_L st51;
    COUNT_mFOLLOWER5_entry se80;
    tuple2_L_L st52;
    COUNT_mFOLLOWER24_entry se81;
    tuple2_L_L st53;
    COUNT_mFOLLOWER11_entry se82;
    COUNT_mFOLLOWER4_entry se83;
    tuple2_L_L st54;
    COUNT_mFOLLOWER24_entry se84;
    COUNT_mFOLLOWER4_entry se85;
    tuple2_L_L st55;
    COUNT_mFOLLOWER11_entry se86;
    tuple2_L_L st56;
    COUNT_mFOLLOWER24_entry se87;
    COUNT_mFOLLOWER10_entry se88;
    COUNT_mFOLLOWER24_entry se89;
    COUNT_mFOLLOWER23_entry se90;
    COUNT_mTWEET1_entry se91;
    tuple2_L_L st57;
    COUNT_mFOLLOWER4_entry se92;
    COUNT_mFOLLOWER4_entry se93;
    tuple2_L_L st58;
    tuple2_L_L st59;
    COUNT_mFOLLOWER23_entry se94;
    COUNT_mFOLLOWER4_entry se95;
    tuple2_L_L st60;
    tuple2_L_L st61;
    COUNT_mFOLLOWER10_entry se96;
    tuple2_L_L st62;
    COUNT_mFOLLOWER4_entry se97;
    tuple2_L_L st63;
    COUNT_mFOLLOWER4_entry se98;
    COUNT_mFOLLOWER10_entry se99;
    tuple2_L_L st64;
    tuple2_L_L st65;
    COUNT_mFOLLOWER10_entry se100;
    tuple2_L_L st66;
    tuple2_L_L st67;
    COUNT_mFOLLOWER4_entry se101;
    COUNT_mFOLLOWER4_entry se102;
    tuple2_L_L st68;
    COUNT_mFOLLOWER4_entry se103;
    tuple2_L_L st69;
    COUNT_mFOLLOWER4_entry se104;
    tuple2_L_L st70;
    COUNT_mFOLLOWER10_entry se105;
    COUNT_mFOLLOWER23_entry se106;
    COUNT_mFOLLOWER2_entry se107;
    tuple2_L_L st71;
    COUNT_mFOLLOWER5_entry se108;
    tuple2_L_L st72;
    COUNT_mFOLLOWER24_entry se109;
    tuple2_L_L st73;
    COUNT_mFOLLOWER11_entry se110;
    COUNT_mFOLLOWER4_entry se111;
    tuple2_L_L st74;
    COUNT_mFOLLOWER24_entry se112;
    tuple2_L_L st75;
    COUNT_mFOLLOWER11_entry se113;
    tuple2_L_L st76;
    COUNT_mFOLLOWER24_entry se114;
    COUNT_mFOLLOWER4_entry se115;
    COUNT_mFOLLOWER24_entry se116;
    COUNT_mFOLLOWER10_entry se117;
    COUNT_mFOLLOWER5_entry se118;
    tuple2_L_L st77;
    COUNT_mFOLLOWER11_entry se119;
    tuple2_L_L st78;
    COUNT_mFOLLOWER24_entry se120;
    COUNT_mFOLLOWER24_entry se121;
    COUNT_mFOLLOWER4_entry se122;
    COUNT_mFOLLOWER11_entry se123;
    COUNT_mFOLLOWER24_entry se124;
    COUNT_mFOLLOWER23_entry se125;
    tuple2_L_L st79;
    COUNT_mFOLLOWER10_entry se126;
    tuple2_L_L st80;
    tuple2_L_L st81;
    COUNT_mFOLLOWER4_entry se127;
    COUNT_mFOLLOWER4_entry se128;
    tuple2_L_L st82;
    tuple2_L_L st83;
    COUNT_mFOLLOWER4_entry se129;
    tuple2_L_L st84;
    COUNT_mFOLLOWER4_entry se130;
    COUNT_mFOLLOWER10_entry se131;
    COUNT_mFOLLOWER10_entry se132;
    tuple2_L_L st85;
    COUNT_mFOLLOWER4_entry se133;
    tuple2_L_L st86;
    COUNT_mFOLLOWER4_entry se134;
    COUNT_mFOLLOWER4_entry se135;
    COUNT_entry se136;
    COUNT_mTWEET1_entry se137;
    COUNT_mFOLLOWER2_entry se138;
    COUNT_mFOLLOWER23_entry se139;
    COUNT_mFOLLOWER5_entry se140;
    COUNT_mFOLLOWER10_entry se141;
    COUNT_mFOLLOWER11_entry se142;
    COUNT_mFOLLOWER4_entry se143;
    COUNT_mFOLLOWER24_entry se144;
    COUNT_entry se145;
    COUNT_mTWEET1_entry se146;
    COUNT_mFOLLOWER2_entry se147;
    COUNT_mFOLLOWER23_entry se148;
    COUNT_mFOLLOWER5_entry se149;
    COUNT_mFOLLOWER10_entry se150;
    COUNT_mFOLLOWER11_entry se151;
    COUNT_mFOLLOWER4_entry se152;
    COUNT_mFOLLOWER24_entry se153;
  
    /* Data structures used for storing materialized views */
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mFOLLOWER2_map COUNT_mFOLLOWER2;
    COUNT_mFOLLOWER4_map COUNT_mFOLLOWER4;
    COUNT_mFOLLOWER5_map COUNT_mFOLLOWER5;
    COUNT_mFOLLOWER10_map COUNT_mFOLLOWER10;
    COUNT_mFOLLOWER11_map COUNT_mFOLLOWER11;
    COUNT_mFOLLOWER23_map COUNT_mFOLLOWER23;
    COUNT_mFOLLOWER24_map COUNT_mFOLLOWER24;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c16;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c1;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c22;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c19;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c4;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c10;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c13;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c7;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c6;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c21;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c9;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c15;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c18;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c24;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c3;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c12;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c2;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c5;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c11;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c14;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c8;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c20;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c23;
    MultiHashMap<tuple2_L_L,long,HashIndex<tuple2_L_L,long> > _c17;
    
  
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

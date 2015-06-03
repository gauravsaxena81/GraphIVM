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
    long F_USER_ID; long T_USER_ID; long B_TWEET_ID; STRING_TYPE T_TWEET_DATE; long C_RETWEET_TWEET_ID; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long D_USER_ID; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*F_USER_ID = 0L; T_USER_ID = 0L; B_TWEET_ID = 0L; T_TWEET_DATE = ""; C_RETWEET_TWEET_ID = 0L; R_TWEET_ID = 0L; R_TWEET_DATE = ""; D_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const STRING_TYPE& c6, const long c7, const long c8) { F_USER_ID = c0; T_USER_ID = c1; B_TWEET_ID = c2; T_TWEET_DATE = c3; C_RETWEET_TWEET_ID = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6; D_USER_ID = c7; __av = c8; }
    COUNT_entry(const COUNT_entry& other) : F_USER_ID( other.F_USER_ID ), T_USER_ID( other.T_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), D_USER_ID( other.D_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const STRING_TYPE& c6, const long c7) { F_USER_ID = c0; T_USER_ID = c1; B_TWEET_ID = c2; T_TWEET_DATE = c3; C_RETWEET_TWEET_ID = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6; D_USER_ID = c7;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      hash_combine(h, e.D_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.F_USER_ID == y.F_USER_ID && x.T_USER_ID == y.T_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE && x.D_USER_ID == y.D_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey01234567_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey01234567_idxfn,true> HashIndex_COUNT_map_01234567;
  
  struct COUNT_mRETWEET1_entry {
    long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; long F_USER_ID; long T_USER_ID; STRING_TYPE T_TWEET_DATE; long __av;  COUNT_mRETWEET1_entry* nxt; COUNT_mRETWEET1_entry* prv;
    explicit COUNT_mRETWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; F_USER_ID = 0L; T_USER_ID = 0L; T_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mRETWEET1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; F_USER_ID = c1; T_USER_ID = c2; T_TWEET_DATE = c3; __av = c4; }
    COUNT_mRETWEET1_entry(const COUNT_mRETWEET1_entry& other) : COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), F_USER_ID( other.F_USER_ID ), T_USER_ID( other.T_USER_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; F_USER_ID = c1; T_USER_ID = c2; T_TWEET_DATE = c3;  return *this; }
    FORCE_INLINE COUNT_mRETWEET1_entry& modify0(const long c0) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET1_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.T_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET1_entry& x, const COUNT_mRETWEET1_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.F_USER_ID == y.F_USER_ID && x.T_USER_ID == y.T_USER_ID && x.T_TWEET_DATE == y.T_TWEET_DATE;
    }
  };
  
  struct COUNT_mRETWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET1_entry& x, const COUNT_mRETWEET1_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET1_entry,long,
    HashIndex<COUNT_mRETWEET1_entry,long,COUNT_mRETWEET1_mapkey0123_idxfn,true>,
    HashIndex<COUNT_mRETWEET1_entry,long,COUNT_mRETWEET1_mapkey0_idxfn,false>
  > COUNT_mRETWEET1_map;
  typedef HashIndex<COUNT_mRETWEET1_entry,long,COUNT_mRETWEET1_mapkey0123_idxfn,true> HashIndex_COUNT_mRETWEET1_map_0123;
  typedef HashIndex<COUNT_mRETWEET1_entry,long,COUNT_mRETWEET1_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET1_map_0;
  
  struct COUNT_mRETWEET1_mUSERS1_entry {
    long COUNT_mRETWEET1_mUSERSUSERS_USER_ID; long COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID; STRING_TYPE T_TWEET_DATE; long __av;  COUNT_mRETWEET1_mUSERS1_entry* nxt; COUNT_mRETWEET1_mUSERS1_entry* prv;
    explicit COUNT_mRETWEET1_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mRETWEET1_mUSERSUSERS_USER_ID = 0L; COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; T_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mRETWEET1_mUSERS1_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3) { COUNT_mRETWEET1_mUSERSUSERS_USER_ID = c0; COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; T_TWEET_DATE = c2; __av = c3; }
    COUNT_mRETWEET1_mUSERS1_entry(const COUNT_mRETWEET1_mUSERS1_entry& other) : COUNT_mRETWEET1_mUSERSUSERS_USER_ID( other.COUNT_mRETWEET1_mUSERSUSERS_USER_ID ), COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mRETWEET1_mUSERS1_entry& modify(const long c0, const long c1, const STRING_TYPE& c2) { COUNT_mRETWEET1_mUSERSUSERS_USER_ID = c0; COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c1; T_TWEET_DATE = c2;  return *this; }
    FORCE_INLINE COUNT_mRETWEET1_mUSERS1_entry& modify1(const long c1) { COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mRETWEET1_mUSERS1_entry& modify0(const long c0) { COUNT_mRETWEET1_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEET1_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mRETWEET1_mUSERS1_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET1_mUSERSUSERS_USER_ID);
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET1_mUSERS1_entry& x, const COUNT_mRETWEET1_mUSERS1_entry& y) {
      return x.COUNT_mRETWEET1_mUSERSUSERS_USER_ID == y.COUNT_mRETWEET1_mUSERSUSERS_USER_ID && x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE;
    }
  };
  
  struct COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET1_mUSERS1_entry& x, const COUNT_mRETWEET1_mUSERS1_entry& y) {
      return x.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mRETWEET1_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mRETWEET1_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mRETWEET1_mUSERS1_entry& x, const COUNT_mRETWEET1_mUSERS1_entry& y) {
      return x.COUNT_mRETWEET1_mUSERSUSERS_USER_ID == y.COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mRETWEET1_mUSERS1_entry,long,
    HashIndex<COUNT_mRETWEET1_mUSERS1_entry,long,COUNT_mRETWEET1_mUSERS1_mapkey012_idxfn,true>,
    HashIndex<COUNT_mRETWEET1_mUSERS1_entry,long,COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn,false>,
    HashIndex<COUNT_mRETWEET1_mUSERS1_entry,long,COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn,false>
  > COUNT_mRETWEET1_mUSERS1_map;
  typedef HashIndex<COUNT_mRETWEET1_mUSERS1_entry,long,COUNT_mRETWEET1_mUSERS1_mapkey012_idxfn,true> HashIndex_COUNT_mRETWEET1_mUSERS1_map_012;
  typedef HashIndex<COUNT_mRETWEET1_mUSERS1_entry,long,COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn,false> HashIndex_COUNT_mRETWEET1_mUSERS1_map_1;
  typedef HashIndex<COUNT_mRETWEET1_mUSERS1_entry,long,COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn,false> HashIndex_COUNT_mRETWEET1_mUSERS1_map_0;
  
  struct COUNT_mTWEET1_entry {
    long COUNT_mTWEETTWEET_USER_ID; long __av;  COUNT_mTWEET1_entry* nxt; COUNT_mTWEET1_entry* prv;
    explicit COUNT_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mTWEETTWEET_USER_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET1_entry(const long c0, const long c1) { COUNT_mTWEETTWEET_USER_ID = c0; __av = c1; }
    COUNT_mTWEET1_entry(const COUNT_mTWEET1_entry& other) : COUNT_mTWEETTWEET_USER_ID( other.COUNT_mTWEETTWEET_USER_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET1_entry& modify(const long c0) { COUNT_mTWEETTWEET_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
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
    HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,true>
  > COUNT_mTWEET1_map;
  typedef HashIndex<COUNT_mTWEET1_entry,long,COUNT_mTWEET1_mapkey0_idxfn,true> HashIndex_COUNT_mTWEET1_map_0;
  
  struct COUNT_mTWEET2_entry {
    long D_USER_ID; long COUNT_mTWEETTWEET_TWEET_ID; STRING_TYPE R_TWEET_DATE; long R_TWEET_ID; long __av;  COUNT_mTWEET2_entry* nxt; COUNT_mTWEET2_entry* prv;
    explicit COUNT_mTWEET2_entry() : nxt(nullptr), prv(nullptr) { /*D_USER_ID = 0L; COUNT_mTWEETTWEET_TWEET_ID = 0L; R_TWEET_DATE = ""; R_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET2_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3, const long c4) { D_USER_ID = c0; COUNT_mTWEETTWEET_TWEET_ID = c1; R_TWEET_DATE = c2; R_TWEET_ID = c3; __av = c4; }
    COUNT_mTWEET2_entry(const COUNT_mTWEET2_entry& other) : D_USER_ID( other.D_USER_ID ), COUNT_mTWEETTWEET_TWEET_ID( other.COUNT_mTWEETTWEET_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET2_entry& modify(const long c0, const long c1, const STRING_TYPE& c2, const long c3) { D_USER_ID = c0; COUNT_mTWEETTWEET_TWEET_ID = c1; R_TWEET_DATE = c2; R_TWEET_ID = c3;  return *this; }
    FORCE_INLINE COUNT_mTWEET2_entry& modify1(const long c1) { COUNT_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mTWEET2_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.D_USER_ID);
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET2_entry& x, const COUNT_mTWEET2_entry& y) {
      return x.D_USER_ID == y.D_USER_ID && x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID;
    }
  };
  
  struct COUNT_mTWEET2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET2_entry& x, const COUNT_mTWEET2_entry& y) {
      return x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mTWEET2_entry,long,
    HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey0123_idxfn,true>,
    HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey1_idxfn,false>
  > COUNT_mTWEET2_map;
  typedef HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey0123_idxfn,true> HashIndex_COUNT_mTWEET2_map_0123;
  typedef HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey1_idxfn,false> HashIndex_COUNT_mTWEET2_map_1;
  
  struct COUNT_mTWEET2_mUSERS1_entry {
    long COUNT_mTWEET2_mUSERSUSERS_USER_ID; long COUNT_mTWEETTWEET_TWEET_ID; STRING_TYPE R_TWEET_DATE; long R_TWEET_ID; long __av;  COUNT_mTWEET2_mUSERS1_entry* nxt; COUNT_mTWEET2_mUSERS1_entry* prv;
    explicit COUNT_mTWEET2_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mTWEET2_mUSERSUSERS_USER_ID = 0L; COUNT_mTWEETTWEET_TWEET_ID = 0L; R_TWEET_DATE = ""; R_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET2_mUSERS1_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3, const long c4) { COUNT_mTWEET2_mUSERSUSERS_USER_ID = c0; COUNT_mTWEETTWEET_TWEET_ID = c1; R_TWEET_DATE = c2; R_TWEET_ID = c3; __av = c4; }
    COUNT_mTWEET2_mUSERS1_entry(const COUNT_mTWEET2_mUSERS1_entry& other) : COUNT_mTWEET2_mUSERSUSERS_USER_ID( other.COUNT_mTWEET2_mUSERSUSERS_USER_ID ), COUNT_mTWEETTWEET_TWEET_ID( other.COUNT_mTWEETTWEET_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET2_mUSERS1_entry& modify(const long c0, const long c1, const STRING_TYPE& c2, const long c3) { COUNT_mTWEET2_mUSERSUSERS_USER_ID = c0; COUNT_mTWEETTWEET_TWEET_ID = c1; R_TWEET_DATE = c2; R_TWEET_ID = c3;  return *this; }
    FORCE_INLINE COUNT_mTWEET2_mUSERS1_entry& modify1(const long c1) { COUNT_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mTWEET2_mUSERS1_entry& modify0(const long c0) { COUNT_mTWEET2_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEET2_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mTWEET2_mUSERS1_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET2_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEET2_mUSERSUSERS_USER_ID);
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET2_mUSERS1_entry& x, const COUNT_mTWEET2_mUSERS1_entry& y) {
      return x.COUNT_mTWEET2_mUSERSUSERS_USER_ID == y.COUNT_mTWEET2_mUSERSUSERS_USER_ID && x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID;
    }
  };
  
  struct COUNT_mTWEET2_mUSERS1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET2_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET2_mUSERS1_entry& x, const COUNT_mTWEET2_mUSERS1_entry& y) {
      return x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mTWEET2_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET2_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEET2_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET2_mUSERS1_entry& x, const COUNT_mTWEET2_mUSERS1_entry& y) {
      return x.COUNT_mTWEET2_mUSERSUSERS_USER_ID == y.COUNT_mTWEET2_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mTWEET2_mUSERS1_entry,long,
    HashIndex<COUNT_mTWEET2_mUSERS1_entry,long,COUNT_mTWEET2_mUSERS1_mapkey0123_idxfn,true>,
    HashIndex<COUNT_mTWEET2_mUSERS1_entry,long,COUNT_mTWEET2_mUSERS1_mapkey1_idxfn,false>,
    HashIndex<COUNT_mTWEET2_mUSERS1_entry,long,COUNT_mTWEET2_mUSERS1_mapkey0_idxfn,false>
  > COUNT_mTWEET2_mUSERS1_map;
  typedef HashIndex<COUNT_mTWEET2_mUSERS1_entry,long,COUNT_mTWEET2_mUSERS1_mapkey0123_idxfn,true> HashIndex_COUNT_mTWEET2_mUSERS1_map_0123;
  typedef HashIndex<COUNT_mTWEET2_mUSERS1_entry,long,COUNT_mTWEET2_mUSERS1_mapkey1_idxfn,false> HashIndex_COUNT_mTWEET2_mUSERS1_map_1;
  typedef HashIndex<COUNT_mTWEET2_mUSERS1_entry,long,COUNT_mTWEET2_mUSERS1_mapkey0_idxfn,false> HashIndex_COUNT_mTWEET2_mUSERS1_map_0;
  
  struct COUNT_mUSERS2_entry {
    long B_TWEET_ID; long COUNT_mUSERSUSERS_USER_ID; long F_USER_ID; long T_USER_ID; long C_RETWEET_TWEET_ID; STRING_TYPE T_TWEET_DATE; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_mUSERS2_entry* nxt; COUNT_mUSERS2_entry* prv;
    explicit COUNT_mUSERS2_entry() : nxt(nullptr), prv(nullptr) { /*B_TWEET_ID = 0L; COUNT_mUSERSUSERS_USER_ID = 0L; F_USER_ID = 0L; T_USER_ID = 0L; C_RETWEET_TWEET_ID = 0L; T_TWEET_DATE = ""; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS2_entry(const long c0, const long c1, const long c2, const long c3, const long c4, const STRING_TYPE& c5, const long c6, const STRING_TYPE& c7, const long c8) { B_TWEET_ID = c0; COUNT_mUSERSUSERS_USER_ID = c1; F_USER_ID = c2; T_USER_ID = c3; C_RETWEET_TWEET_ID = c4; T_TWEET_DATE = c5; R_TWEET_ID = c6; R_TWEET_DATE = c7; __av = c8; }
    COUNT_mUSERS2_entry(const COUNT_mUSERS2_entry& other) : B_TWEET_ID( other.B_TWEET_ID ), COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), F_USER_ID( other.F_USER_ID ), T_USER_ID( other.T_USER_ID ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS2_entry& modify(const long c0, const long c1, const long c2, const long c3, const long c4, const STRING_TYPE& c5, const long c6, const STRING_TYPE& c7) { B_TWEET_ID = c0; COUNT_mUSERSUSERS_USER_ID = c1; F_USER_ID = c2; T_USER_ID = c3; C_RETWEET_TWEET_ID = c4; T_TWEET_DATE = c5; R_TWEET_ID = c6; R_TWEET_DATE = c7;  return *this; }
    FORCE_INLINE COUNT_mUSERS2_entry& modify1(const long c1) { COUNT_mUSERSUSERS_USER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, F_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS2_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_entry& x, const COUNT_mUSERS2_entry& y) {
      return x.B_TWEET_ID == y.B_TWEET_ID && x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.F_USER_ID == y.F_USER_ID && x.T_USER_ID == y.T_USER_ID && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_entry& x, const COUNT_mUSERS2_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS2_entry,long,
    HashIndex<COUNT_mUSERS2_entry,long,COUNT_mUSERS2_mapkey01234567_idxfn,true>,
    HashIndex<COUNT_mUSERS2_entry,long,COUNT_mUSERS2_mapkey1_idxfn,false>
  > COUNT_mUSERS2_map;
  typedef HashIndex<COUNT_mUSERS2_entry,long,COUNT_mUSERS2_mapkey01234567_idxfn,true> HashIndex_COUNT_mUSERS2_map_01234567;
  typedef HashIndex<COUNT_mUSERS2_entry,long,COUNT_mUSERS2_mapkey1_idxfn,false> HashIndex_COUNT_mUSERS2_map_1;
  
  struct COUNT_mUSERS2_mUSERS1_entry {
    long COUNT_mUSERS2_mUSERSUSERS_USER_ID; long B_TWEET_ID; long COUNT_mUSERSUSERS_USER_ID; long C_RETWEET_TWEET_ID; STRING_TYPE T_TWEET_DATE; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_mUSERS2_mUSERS1_entry* nxt; COUNT_mUSERS2_mUSERS1_entry* prv;
    explicit COUNT_mUSERS2_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERS2_mUSERSUSERS_USER_ID = 0L; B_TWEET_ID = 0L; COUNT_mUSERSUSERS_USER_ID = 0L; C_RETWEET_TWEET_ID = 0L; T_TWEET_DATE = ""; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS2_mUSERS1_entry(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const long c7) { COUNT_mUSERS2_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; COUNT_mUSERSUSERS_USER_ID = c2; C_RETWEET_TWEET_ID = c3; T_TWEET_DATE = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6; __av = c7; }
    COUNT_mUSERS2_mUSERS1_entry(const COUNT_mUSERS2_mUSERS1_entry& other) : COUNT_mUSERS2_mUSERSUSERS_USER_ID( other.COUNT_mUSERS2_mUSERSUSERS_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS2_mUSERS1_entry& modify(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6) { COUNT_mUSERS2_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; COUNT_mUSERSUSERS_USER_ID = c2; C_RETWEET_TWEET_ID = c3; T_TWEET_DATE = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6;  return *this; }
    FORCE_INLINE COUNT_mUSERS2_mUSERS1_entry& modify0(const long c0) { COUNT_mUSERS2_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERS2_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS2_mUSERS1_mapkey0123456_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS2_mUSERSUSERS_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_mUSERS1_entry& x, const COUNT_mUSERS2_mUSERS1_entry& y) {
      return x.COUNT_mUSERS2_mUSERSUSERS_USER_ID == y.COUNT_mUSERS2_mUSERSUSERS_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS2_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS2_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_mUSERS1_entry& x, const COUNT_mUSERS2_mUSERS1_entry& y) {
      return x.COUNT_mUSERS2_mUSERSUSERS_USER_ID == y.COUNT_mUSERS2_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS2_mUSERS1_entry,long,
    HashIndex<COUNT_mUSERS2_mUSERS1_entry,long,COUNT_mUSERS2_mUSERS1_mapkey0123456_idxfn,true>,
    HashIndex<COUNT_mUSERS2_mUSERS1_entry,long,COUNT_mUSERS2_mUSERS1_mapkey0_idxfn,false>
  > COUNT_mUSERS2_mUSERS1_map;
  typedef HashIndex<COUNT_mUSERS2_mUSERS1_entry,long,COUNT_mUSERS2_mUSERS1_mapkey0123456_idxfn,true> HashIndex_COUNT_mUSERS2_mUSERS1_map_0123456;
  typedef HashIndex<COUNT_mUSERS2_mUSERS1_entry,long,COUNT_mUSERS2_mUSERS1_mapkey0_idxfn,false> HashIndex_COUNT_mUSERS2_mUSERS1_map_0;
  
  struct COUNT_mUSERS2_mUSERS1_mTWEET1_entry {
    long COUNT_mUSERSUSERS_USER_ID; long COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_mUSERS2_mUSERS1_mTWEET1_entry* nxt; COUNT_mUSERS2_mUSERS1_mTWEET1_entry* prv;
    explicit COUNT_mUSERS2_mUSERS1_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERSUSERS_USER_ID = 0L; COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID = 0L; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS2_mUSERS1_mTWEET1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4) { COUNT_mUSERSUSERS_USER_ID = c0; COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID = c1; R_TWEET_ID = c2; R_TWEET_DATE = c3; __av = c4; }
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& other) : COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID( other.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS2_mUSERS1_mTWEET1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3) { COUNT_mUSERSUSERS_USER_ID = c0; COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID = c1; R_TWEET_ID = c2; R_TWEET_DATE = c3;  return *this; }
    FORCE_INLINE COUNT_mUSERS2_mUSERS1_mTWEET1_entry& modify1(const long c1) { COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    FORCE_INLINE COUNT_mUSERS2_mUSERS1_mTWEET1_entry& modify0(const long c0) { COUNT_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& x, const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID == y.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& x, const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& y) {
      return x.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID == y.COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID;
    }
  };
  
  struct COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& x, const COUNT_mUSERS2_mUSERS1_mTWEET1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,
    HashIndex<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0123_idxfn,true>,
    HashIndex<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn,false>,
    HashIndex<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn,false>
  > COUNT_mUSERS2_mUSERS1_mTWEET1_map;
  typedef HashIndex<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0123_idxfn,true> HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0123;
  typedef HashIndex<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn,false> HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1;
  typedef HashIndex<COUNT_mUSERS2_mUSERS1_mTWEET1_entry,long,COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn,false> HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0;
  
  struct COUNT_mUSERS6_entry {
    long COUNT_mUSERSUSERS_USER_ID; long B_TWEET_ID; long D_USER_ID; long C_RETWEET_TWEET_ID; STRING_TYPE T_TWEET_DATE; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_mUSERS6_entry* nxt; COUNT_mUSERS6_entry* prv;
    explicit COUNT_mUSERS6_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERSUSERS_USER_ID = 0L; B_TWEET_ID = 0L; D_USER_ID = 0L; C_RETWEET_TWEET_ID = 0L; T_TWEET_DATE = ""; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS6_entry(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const long c7) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; D_USER_ID = c2; C_RETWEET_TWEET_ID = c3; T_TWEET_DATE = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6; __av = c7; }
    COUNT_mUSERS6_entry(const COUNT_mUSERS6_entry& other) : COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), D_USER_ID( other.D_USER_ID ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS6_entry& modify(const long c0, const long c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; D_USER_ID = c2; C_RETWEET_TWEET_ID = c3; T_TWEET_DATE = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6;  return *this; }
    FORCE_INLINE COUNT_mUSERS6_entry& modify0(const long c0) { COUNT_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS6_mapkey0123456_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.D_USER_ID);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS6_entry& x, const COUNT_mUSERS6_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.D_USER_ID == y.D_USER_ID && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS6_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS6_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS6_entry& x, const COUNT_mUSERS6_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS6_entry,long,
    HashIndex<COUNT_mUSERS6_entry,long,COUNT_mUSERS6_mapkey0123456_idxfn,true>,
    HashIndex<COUNT_mUSERS6_entry,long,COUNT_mUSERS6_mapkey0_idxfn,false>
  > COUNT_mUSERS6_map;
  typedef HashIndex<COUNT_mUSERS6_entry,long,COUNT_mUSERS6_mapkey0123456_idxfn,true> HashIndex_COUNT_mUSERS6_map_0123456;
  typedef HashIndex<COUNT_mUSERS6_entry,long,COUNT_mUSERS6_mapkey0_idxfn,false> HashIndex_COUNT_mUSERS6_map_0;
  
  struct COUNT_mUSERS6_mTWEET1_entry {
    long D_USER_ID; long COUNT_mUSERS6_mTWEETTWEET_TWEET_ID; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_mUSERS6_mTWEET1_entry* nxt; COUNT_mUSERS6_mTWEET1_entry* prv;
    explicit COUNT_mUSERS6_mTWEET1_entry() : nxt(nullptr), prv(nullptr) { /*D_USER_ID = 0L; COUNT_mUSERS6_mTWEETTWEET_TWEET_ID = 0L; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS6_mTWEET1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4) { D_USER_ID = c0; COUNT_mUSERS6_mTWEETTWEET_TWEET_ID = c1; R_TWEET_ID = c2; R_TWEET_DATE = c3; __av = c4; }
    COUNT_mUSERS6_mTWEET1_entry(const COUNT_mUSERS6_mTWEET1_entry& other) : D_USER_ID( other.D_USER_ID ), COUNT_mUSERS6_mTWEETTWEET_TWEET_ID( other.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS6_mTWEET1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3) { D_USER_ID = c0; COUNT_mUSERS6_mTWEETTWEET_TWEET_ID = c1; R_TWEET_ID = c2; R_TWEET_DATE = c3;  return *this; }
    FORCE_INLINE COUNT_mUSERS6_mTWEET1_entry& modify1(const long c1) { COUNT_mUSERS6_mTWEETTWEET_TWEET_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, D_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERS6_mTWEETTWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS6_mTWEET1_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS6_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.D_USER_ID);
      hash_combine(h, e.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS6_mTWEET1_entry& x, const COUNT_mUSERS6_mTWEET1_entry& y) {
      return x.D_USER_ID == y.D_USER_ID && x.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID == y.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS6_mTWEET1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS6_mTWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS6_mTWEET1_entry& x, const COUNT_mUSERS6_mTWEET1_entry& y) {
      return x.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID == y.COUNT_mUSERS6_mTWEETTWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS6_mTWEET1_entry,long,
    HashIndex<COUNT_mUSERS6_mTWEET1_entry,long,COUNT_mUSERS6_mTWEET1_mapkey0123_idxfn,true>,
    HashIndex<COUNT_mUSERS6_mTWEET1_entry,long,COUNT_mUSERS6_mTWEET1_mapkey1_idxfn,false>
  > COUNT_mUSERS6_mTWEET1_map;
  typedef HashIndex<COUNT_mUSERS6_mTWEET1_entry,long,COUNT_mUSERS6_mTWEET1_mapkey0123_idxfn,true> HashIndex_COUNT_mUSERS6_mTWEET1_map_0123;
  typedef HashIndex<COUNT_mUSERS6_mTWEET1_entry,long,COUNT_mUSERS6_mTWEET1_mapkey1_idxfn,false> HashIndex_COUNT_mUSERS6_mTWEET1_map_1;
  
  struct COUNT_mUSERS6_mUSERS1_entry {
    long COUNT_mUSERSUSERS_USER_ID; long B_TWEET_ID; long COUNT_mUSERS6_mUSERSUSERS_USER_ID; STRING_TYPE T_TWEET_DATE; long C_RETWEET_TWEET_ID; STRING_TYPE R_TWEET_DATE; long R_TWEET_ID; long __av;  COUNT_mUSERS6_mUSERS1_entry* nxt; COUNT_mUSERS6_mUSERS1_entry* prv;
    explicit COUNT_mUSERS6_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERSUSERS_USER_ID = 0L; B_TWEET_ID = 0L; COUNT_mUSERS6_mUSERSUSERS_USER_ID = 0L; T_TWEET_DATE = ""; C_RETWEET_TWEET_ID = 0L; R_TWEET_DATE = ""; R_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mUSERS6_mUSERS1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const STRING_TYPE& c5, const long c6, const long c7) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; COUNT_mUSERS6_mUSERSUSERS_USER_ID = c2; T_TWEET_DATE = c3; C_RETWEET_TWEET_ID = c4; R_TWEET_DATE = c5; R_TWEET_ID = c6; __av = c7; }
    COUNT_mUSERS6_mUSERS1_entry(const COUNT_mUSERS6_mUSERS1_entry& other) : COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), COUNT_mUSERS6_mUSERSUSERS_USER_ID( other.COUNT_mUSERS6_mUSERSUSERS_USER_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS6_mUSERS1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const STRING_TYPE& c5, const long c6) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; COUNT_mUSERS6_mUSERSUSERS_USER_ID = c2; T_TWEET_DATE = c3; C_RETWEET_TWEET_ID = c4; R_TWEET_DATE = c5; R_TWEET_ID = c6;  return *this; }
    FORCE_INLINE COUNT_mUSERS6_mUSERS1_entry& modify2(const long c2) { COUNT_mUSERS6_mUSERSUSERS_USER_ID = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERS6_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS6_mUSERS1_mapkey0123456_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS6_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.COUNT_mUSERS6_mUSERSUSERS_USER_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS6_mUSERS1_entry& x, const COUNT_mUSERS6_mUSERS1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.COUNT_mUSERS6_mUSERSUSERS_USER_ID == y.COUNT_mUSERS6_mUSERSUSERS_USER_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID;
    }
  };
  
  struct COUNT_mUSERS6_mUSERS1_mapkey2_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS6_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS6_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS6_mUSERS1_entry& x, const COUNT_mUSERS6_mUSERS1_entry& y) {
      return x.COUNT_mUSERS6_mUSERSUSERS_USER_ID == y.COUNT_mUSERS6_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS6_mUSERS1_entry,long,
    HashIndex<COUNT_mUSERS6_mUSERS1_entry,long,COUNT_mUSERS6_mUSERS1_mapkey0123456_idxfn,true>,
    HashIndex<COUNT_mUSERS6_mUSERS1_entry,long,COUNT_mUSERS6_mUSERS1_mapkey2_idxfn,false>
  > COUNT_mUSERS6_mUSERS1_map;
  typedef HashIndex<COUNT_mUSERS6_mUSERS1_entry,long,COUNT_mUSERS6_mUSERS1_mapkey0123456_idxfn,true> HashIndex_COUNT_mUSERS6_mUSERS1_map_0123456;
  typedef HashIndex<COUNT_mUSERS6_mUSERS1_entry,long,COUNT_mUSERS6_mUSERS1_mapkey2_idxfn,false> HashIndex_COUNT_mUSERS6_mUSERS1_map_2;
  
  struct COUNT_mUSERS11_entry {
    long B_TWEET_ID; long COUNT_mUSERSUSERS_USER_ID; STRING_TYPE T_TWEET_DATE; long C_RETWEET_TWEET_ID; STRING_TYPE R_TWEET_DATE; long R_TWEET_ID; long __av;  COUNT_mUSERS11_entry* nxt; COUNT_mUSERS11_entry* prv;
    explicit COUNT_mUSERS11_entry() : nxt(nullptr), prv(nullptr) { /*B_TWEET_ID = 0L; COUNT_mUSERSUSERS_USER_ID = 0L; T_TWEET_DATE = ""; C_RETWEET_TWEET_ID = 0L; R_TWEET_DATE = ""; R_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mUSERS11_entry(const long c0, const long c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const long c5, const long c6) { B_TWEET_ID = c0; COUNT_mUSERSUSERS_USER_ID = c1; T_TWEET_DATE = c2; C_RETWEET_TWEET_ID = c3; R_TWEET_DATE = c4; R_TWEET_ID = c5; __av = c6; }
    COUNT_mUSERS11_entry(const COUNT_mUSERS11_entry& other) : B_TWEET_ID( other.B_TWEET_ID ), COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS11_entry& modify(const long c0, const long c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const long c5) { B_TWEET_ID = c0; COUNT_mUSERSUSERS_USER_ID = c1; T_TWEET_DATE = c2; C_RETWEET_TWEET_ID = c3; R_TWEET_DATE = c4; R_TWEET_ID = c5;  return *this; }
    FORCE_INLINE COUNT_mUSERS11_entry& modify1(const long c1) { COUNT_mUSERSUSERS_USER_ID = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, R_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS11_mapkey012345_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS11_entry& x, const COUNT_mUSERS11_entry& y) {
      return x.B_TWEET_ID == y.B_TWEET_ID && x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID;
    }
  };
  
  struct COUNT_mUSERS11_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS11_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS11_entry& x, const COUNT_mUSERS11_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS11_entry,long,
    HashIndex<COUNT_mUSERS11_entry,long,COUNT_mUSERS11_mapkey012345_idxfn,true>,
    HashIndex<COUNT_mUSERS11_entry,long,COUNT_mUSERS11_mapkey1_idxfn,false>
  > COUNT_mUSERS11_map;
  typedef HashIndex<COUNT_mUSERS11_entry,long,COUNT_mUSERS11_mapkey012345_idxfn,true> HashIndex_COUNT_mUSERS11_map_012345;
  typedef HashIndex<COUNT_mUSERS11_entry,long,COUNT_mUSERS11_mapkey1_idxfn,false> HashIndex_COUNT_mUSERS11_map_1;
  
  struct COUNT_mUSERS11_mRETWEET1_entry {
    long COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID; STRING_TYPE T_TWEET_DATE; long __av;  COUNT_mUSERS11_mRETWEET1_entry* nxt; COUNT_mUSERS11_mRETWEET1_entry* prv;
    explicit COUNT_mUSERS11_mRETWEET1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID = 0L; T_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS11_mRETWEET1_entry(const long c0, const STRING_TYPE& c1, const long c2) { COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; T_TWEET_DATE = c1; __av = c2; }
    COUNT_mUSERS11_mRETWEET1_entry(const COUNT_mUSERS11_mRETWEET1_entry& other) : COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID( other.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS11_mRETWEET1_entry& modify(const long c0, const STRING_TYPE& c1) { COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID = c0; T_TWEET_DATE = c1;  return *this; }
    FORCE_INLINE COUNT_mUSERS11_mRETWEET1_entry& modify0(const long c0) { COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, T_TWEET_DATE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mUSERS11_mRETWEET1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS11_mRETWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS11_mRETWEET1_entry& x, const COUNT_mUSERS11_mRETWEET1_entry& y) {
      return x.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS11_mRETWEET1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS11_mRETWEET1_entry& x, const COUNT_mUSERS11_mRETWEET1_entry& y) {
      return x.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID == y.COUNT_mUSERS11_mRETWEETRETWEET_RETWEET_TWEET_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS11_mRETWEET1_entry,long,
    HashIndex<COUNT_mUSERS11_mRETWEET1_entry,long,COUNT_mUSERS11_mRETWEET1_mapkey01_idxfn,true>,
    HashIndex<COUNT_mUSERS11_mRETWEET1_entry,long,COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn,false>
  > COUNT_mUSERS11_mRETWEET1_map;
  typedef HashIndex<COUNT_mUSERS11_mRETWEET1_entry,long,COUNT_mUSERS11_mRETWEET1_mapkey01_idxfn,true> HashIndex_COUNT_mUSERS11_mRETWEET1_map_01;
  typedef HashIndex<COUNT_mUSERS11_mRETWEET1_entry,long,COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn,false> HashIndex_COUNT_mUSERS11_mRETWEET1_map_0;
  
  struct tuple8_SLLSLLL_L {
    STRING_TYPE _1; long _2; long _3; STRING_TYPE _4; long _5; long _6; long _7; long __av; tuple8_SLLSLLL_L* nxt; tuple8_SLLSLLL_L* prv;
    explicit tuple8_SLLSLLL_L() : nxt(nullptr), prv(nullptr) { }
    explicit tuple8_SLLSLLL_L(const STRING_TYPE& c1, const long c2, const long c3, const STRING_TYPE& c4, const long c5, const long c6, const long c7, long c__av=0L) : nxt(nullptr), prv(nullptr) { _1 = c1; _2 = c2; _3 = c3; _4 = c4; _5 = c5; _6 = c6; _7 = c7; __av = c__av;}
    int operator<(const tuple8_SLLSLLL_L &rhs) const { 
      if(this->_1!=rhs._1) return (this->_1<rhs._1);
      if(this->_2!=rhs._2) return (this->_2<rhs._2);
      if(this->_3!=rhs._3) return (this->_3<rhs._3);
      if(this->_4!=rhs._4) return (this->_4<rhs._4);
      if(this->_5!=rhs._5) return (this->_5<rhs._5);
      if(this->_6!=rhs._6) return (this->_6<rhs._6);
      if(this->_7!=rhs._7) return (this->_7<rhs._7);
      return 0;
    }
    int operator==(const tuple8_SLLSLLL_L &rhs) const { return ((this->_1==rhs._1) && (this->_2==rhs._2) && (this->_3==rhs._3) && (this->_4==rhs._4) && (this->_5==rhs._5) && (this->_6==rhs._6) && (this->_7==rhs._7)); }
    FORCE_INLINE tuple8_SLLSLLL_L& modify(const STRING_TYPE& c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const long c6, long c__av) { _1 = c0; _2 = c1; _3 = c2; _4 = c3; _5 = c4; _6 = c5; _7 = c6; __av = c__av; return *this; }
    static bool equals(const tuple8_SLLSLLL_L &x, const tuple8_SLLSLLL_L &y) { return ((x._1==y._1) && (x._2==y._2) && (x._3==y._3) && (x._4==y._4) && (x._5==y._5) && (x._6==y._6) && (x._7==y._7)); }
    static long hash(const tuple8_SLLSLLL_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      hash_combine(h, e._2);
      hash_combine(h, e._3);
      hash_combine(h, e._4);
      hash_combine(h, e._5);
      hash_combine(h, e._6);
      hash_combine(h, e._7);
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
    data_t(): tlq_t(), _c1(16U), _c2(16U) {
      
    }
  
    #ifdef DBT_PROFILE
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> exec_stats;
    std::shared_ptr<dbtoaster::statistics::trigger_exec_stats> ivc_stats;
    #endif
  
    /* Registering relations and trigger functions */
    void register_data(ProgramBase& pb) {
  
      pb.add_map<COUNT_map>( "COUNT", COUNT );
      pb.add_map<COUNT_mRETWEET1_map>( "COUNT_mRETWEET1", COUNT_mRETWEET1 );
      pb.add_map<COUNT_mRETWEET1_mUSERS1_map>( "COUNT_mRETWEET1_mUSERS1", COUNT_mRETWEET1_mUSERS1 );
      pb.add_map<COUNT_mTWEET1_map>( "COUNT_mTWEET1", COUNT_mTWEET1 );
      pb.add_map<COUNT_mTWEET2_map>( "COUNT_mTWEET2", COUNT_mTWEET2 );
      pb.add_map<COUNT_mTWEET2_mUSERS1_map>( "COUNT_mTWEET2_mUSERS1", COUNT_mTWEET2_mUSERS1 );
      pb.add_map<COUNT_mUSERS2_map>( "COUNT_mUSERS2", COUNT_mUSERS2 );
      pb.add_map<COUNT_mUSERS2_mUSERS1_map>( "COUNT_mUSERS2_mUSERS1", COUNT_mUSERS2_mUSERS1 );
      pb.add_map<COUNT_mUSERS2_mUSERS1_mTWEET1_map>( "COUNT_mUSERS2_mUSERS1_mTWEET1", COUNT_mUSERS2_mUSERS1_mTWEET1 );
      pb.add_map<COUNT_mUSERS6_map>( "COUNT_mUSERS6", COUNT_mUSERS6 );
      pb.add_map<COUNT_mUSERS6_mTWEET1_map>( "COUNT_mUSERS6_mTWEET1", COUNT_mUSERS6_mTWEET1 );
      pb.add_map<COUNT_mUSERS6_mUSERS1_map>( "COUNT_mUSERS6_mUSERS1", COUNT_mUSERS6_mUSERS1 );
      pb.add_map<COUNT_mUSERS11_map>( "COUNT_mUSERS11", COUNT_mUSERS11 );
      pb.add_map<COUNT_mUSERS11_mRETWEET1_map>( "COUNT_mUSERS11_mRETWEET1", COUNT_mUSERS11_mRETWEET1 );
  
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
      exec_stats->register_probe(3, "insert_RETWEET_s3");
      exec_stats->register_probe(4, "insert_RETWEET_s4");
      exec_stats->register_probe(5, "insert_RETWEET_s5");
      exec_stats->register_probe(6, "insert_RETWEET_s6");
      exec_stats->register_probe(7, "insert_RETWEET_s7");
      exec_stats->register_probe(8, "insert_RETWEET_s8");
      exec_stats->register_probe(9, "insert_RETWEET_s9");
      exec_stats->register_probe(10, "delete_RETWEET_s0");
      exec_stats->register_probe(11, "delete_RETWEET_s1");
      exec_stats->register_probe(12, "delete_RETWEET_s2");
      exec_stats->register_probe(13, "delete_RETWEET_s3");
      exec_stats->register_probe(14, "delete_RETWEET_s4");
      exec_stats->register_probe(15, "delete_RETWEET_s5");
      exec_stats->register_probe(16, "delete_RETWEET_s6");
      exec_stats->register_probe(17, "delete_RETWEET_s7");
      exec_stats->register_probe(18, "delete_RETWEET_s8");
      exec_stats->register_probe(19, "delete_RETWEET_s9");
      exec_stats->register_probe(20, "insert_TWEET_s0");
      exec_stats->register_probe(21, "insert_TWEET_s1");
      exec_stats->register_probe(22, "insert_TWEET_s2");
      exec_stats->register_probe(23, "insert_TWEET_s3");
      exec_stats->register_probe(24, "insert_TWEET_s4");
      exec_stats->register_probe(25, "insert_TWEET_s5");
      exec_stats->register_probe(26, "insert_TWEET_s6");
      exec_stats->register_probe(27, "insert_TWEET_s7");
      exec_stats->register_probe(28, "insert_TWEET_s8");
      exec_stats->register_probe(29, "delete_TWEET_s0");
      exec_stats->register_probe(30, "delete_TWEET_s1");
      exec_stats->register_probe(31, "delete_TWEET_s2");
      exec_stats->register_probe(32, "delete_TWEET_s3");
      exec_stats->register_probe(33, "delete_TWEET_s4");
      exec_stats->register_probe(34, "delete_TWEET_s5");
      exec_stats->register_probe(35, "delete_TWEET_s6");
      exec_stats->register_probe(36, "delete_TWEET_s7");
      exec_stats->register_probe(37, "delete_TWEET_s8");
      exec_stats->register_probe(38, "insert_USERS_s0");
      exec_stats->register_probe(39, "insert_USERS_s1");
      exec_stats->register_probe(40, "insert_USERS_s2");
      exec_stats->register_probe(41, "insert_USERS_s3");
      exec_stats->register_probe(42, "insert_USERS_s4");
      exec_stats->register_probe(43, "insert_USERS_s5");
      exec_stats->register_probe(44, "insert_USERS_s6");
      exec_stats->register_probe(45, "delete_USERS_s0");
      exec_stats->register_probe(46, "delete_USERS_s1");
      exec_stats->register_probe(47, "delete_USERS_s2");
      exec_stats->register_probe(48, "delete_USERS_s3");
      exec_stats->register_probe(49, "delete_USERS_s4");
      exec_stats->register_probe(50, "delete_USERS_s5");
      exec_stats->register_probe(51, "delete_USERS_s6");
      #endif // DBT_PROFILE
  
    }
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_insert_RETWEET(const long retweet_user_id, const long retweet_tweet_id, const STRING_TYPE& retweet_tweet_date, const long retweet_retweet_tweet_id) {
      BEGIN_TRIGGER(exec_stats,"insert_RETWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_RETWEET")
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = COUNT_mRETWEET1_mapkey0_idxfn::hash(se3.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_map_0* i1 = static_cast<HashIndex_COUNT_mRETWEET1_map_0*>(COUNT_mRETWEET1.index[1]);
          HashIndex_COUNT_mRETWEET1_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mRETWEET1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mRETWEET1_mapkey0_idxfn::equals(se3, *e1)) {
            long f_user_id = e1->F_USER_ID;
            long t_user_id = e1->T_USER_ID;
            STRING_TYPE t_tweet_date = e1->T_TWEET_DATE;
            long v1 = e1->__av;
            COUNT.addOrDelOnZero(se1.modify(f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date,retweet_user_id),(v1 * COUNT_mTWEET1.getValueOrDefault(se2.modify(retweet_user_id))));
          } while ((n1=n1->nxt));
        }
        COUNT_mTWEET2.addOrDelOnZero(se4.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),COUNT_mTWEET1.getValueOrDefault(se5.modify(retweet_user_id)));
        COUNT_mTWEET2_mUSERS1.addOrDelOnZero(se6.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),1L);
        { //slice
          const HASH_RES_t h2 = COUNT_mRETWEET1_mapkey0_idxfn::hash(se8.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_map_0* i2 = static_cast<HashIndex_COUNT_mRETWEET1_map_0*>(COUNT_mRETWEET1.index[1]);
          HashIndex_COUNT_mRETWEET1_map_0::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mRETWEET1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mRETWEET1_mapkey0_idxfn::equals(se8, *e2)) {
            long f_user_id = e2->F_USER_ID;
            long t_user_id = e2->T_USER_ID;
            STRING_TYPE t_tweet_date = e2->T_TWEET_DATE;
            long v2 = e2->__av;
            COUNT_mUSERS2.addOrDelOnZero(se7.modify(retweet_retweet_tweet_id,retweet_user_id,f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),v2);
          } while ((n2=n2->nxt));
        }
        { //slice
          const HASH_RES_t h3 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se10.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i3 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se10, *e3)) {
            long count_musers2_musersusers_user_id = e3->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e3->T_TWEET_DATE;
            long v3 = e3->__av;
            COUNT_mUSERS2_mUSERS1.addOrDelOnZero(se9.modify(count_musers2_musersusers_user_id,retweet_retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),v3);
          } while ((n3=n3->nxt));
        }
        COUNT_mUSERS2_mUSERS1_mTWEET1.addOrDelOnZero(se11.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date),1L);
        { //slice
          const HASH_RES_t h4 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se14.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i4 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se14, *e4)) {
            long count_musersusers_user_id = e4->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e4->T_TWEET_DATE;
            long v4 = e4->__av;
            COUNT_mUSERS6.addOrDelOnZero(se12.modify(count_musersusers_user_id,retweet_retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),(v4 * COUNT_mTWEET1.getValueOrDefault(se13.modify(retweet_user_id))));
          } while ((n4=n4->nxt));
        }
        COUNT_mUSERS6_mTWEET1.addOrDelOnZero(se15.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date),COUNT_mTWEET1.getValueOrDefault(se16.modify(retweet_user_id)));
        { //slice
          const HASH_RES_t h5 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se18.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i5 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se18, *e5)) {
            long count_musersusers_user_id = e5->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e5->T_TWEET_DATE;
            long v5 = e5->__av;
            COUNT_mUSERS6_mUSERS1.addOrDelOnZero(se17.modify(count_musersusers_user_id,retweet_retweet_tweet_id,retweet_user_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),v5);
          } while ((n5=n5->nxt));
        }
        { //slice
          const HASH_RES_t h6 = COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn::hash(se20.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mUSERS11_mRETWEET1_map_0* i6 = static_cast<HashIndex_COUNT_mUSERS11_mRETWEET1_map_0*>(COUNT_mUSERS11_mRETWEET1.index[1]);
          HashIndex_COUNT_mUSERS11_mRETWEET1_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mUSERS11_mRETWEET1_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn::equals(se20, *e6)) {
            STRING_TYPE t_tweet_date = e6->T_TWEET_DATE;
            long v6 = e6->__av;
            COUNT_mUSERS11.addOrDelOnZero(se19.modify(retweet_retweet_tweet_id,retweet_user_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),v6);
          } while ((n6=n6->nxt));
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
          const HASH_RES_t h7 = COUNT_mRETWEET1_mapkey0_idxfn::hash(se23.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_map_0* i7 = static_cast<HashIndex_COUNT_mRETWEET1_map_0*>(COUNT_mRETWEET1.index[1]);
          HashIndex_COUNT_mRETWEET1_map_0::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          COUNT_mRETWEET1_entry* e7;
          do if ((e7=n7->obj) && h7 == n7->hash && COUNT_mRETWEET1_mapkey0_idxfn::equals(se23, *e7)) {
            long f_user_id = e7->F_USER_ID;
            long t_user_id = e7->T_USER_ID;
            STRING_TYPE t_tweet_date = e7->T_TWEET_DATE;
            long v7 = e7->__av;
            COUNT.addOrDelOnZero(se21.modify(f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date,retweet_user_id),(v7 * (COUNT_mTWEET1.getValueOrDefault(se22.modify(retweet_user_id)) * -1L)));
          } while ((n7=n7->nxt));
        }
        COUNT_mTWEET2.addOrDelOnZero(se24.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),(COUNT_mTWEET1.getValueOrDefault(se25.modify(retweet_user_id)) * -1L));
        COUNT_mTWEET2_mUSERS1.addOrDelOnZero(se26.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),-1L);
        { //slice
          const HASH_RES_t h8 = COUNT_mRETWEET1_mapkey0_idxfn::hash(se28.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_map_0* i8 = static_cast<HashIndex_COUNT_mRETWEET1_map_0*>(COUNT_mRETWEET1.index[1]);
          HashIndex_COUNT_mRETWEET1_map_0::IdxNode* n8 = &(i8->buckets_[h8 % i8->size_]);
          COUNT_mRETWEET1_entry* e8;
          do if ((e8=n8->obj) && h8 == n8->hash && COUNT_mRETWEET1_mapkey0_idxfn::equals(se28, *e8)) {
            long f_user_id = e8->F_USER_ID;
            long t_user_id = e8->T_USER_ID;
            STRING_TYPE t_tweet_date = e8->T_TWEET_DATE;
            long v8 = e8->__av;
            COUNT_mUSERS2.addOrDelOnZero(se27.modify(retweet_retweet_tweet_id,retweet_user_id,f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),(v8 * -1L));
          } while ((n8=n8->nxt));
        }
        { //slice
          const HASH_RES_t h9 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se30.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i9 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n9 = &(i9->buckets_[h9 % i9->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e9;
          do if ((e9=n9->obj) && h9 == n9->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se30, *e9)) {
            long count_musers2_musersusers_user_id = e9->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e9->T_TWEET_DATE;
            long v9 = e9->__av;
            COUNT_mUSERS2_mUSERS1.addOrDelOnZero(se29.modify(count_musers2_musersusers_user_id,retweet_retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),(v9 * -1L));
          } while ((n9=n9->nxt));
        }
        COUNT_mUSERS2_mUSERS1_mTWEET1.addOrDelOnZero(se31.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date),-1L);
        { //slice
          const HASH_RES_t h10 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se34.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i10 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n10 = &(i10->buckets_[h10 % i10->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e10;
          do if ((e10=n10->obj) && h10 == n10->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se34, *e10)) {
            long count_musersusers_user_id = e10->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e10->T_TWEET_DATE;
            long v10 = e10->__av;
            COUNT_mUSERS6.addOrDelOnZero(se32.modify(count_musersusers_user_id,retweet_retweet_tweet_id,retweet_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),(v10 * (COUNT_mTWEET1.getValueOrDefault(se33.modify(retweet_user_id)) * -1L)));
          } while ((n10=n10->nxt));
        }
        COUNT_mUSERS6_mTWEET1.addOrDelOnZero(se35.modify(retweet_user_id,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se36.modify(retweet_user_id)) * -1L));
        { //slice
          const HASH_RES_t h11 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se38.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i11 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n11 = &(i11->buckets_[h11 % i11->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e11;
          do if ((e11=n11->obj) && h11 == n11->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se38, *e11)) {
            long count_musersusers_user_id = e11->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e11->T_TWEET_DATE;
            long v11 = e11->__av;
            COUNT_mUSERS6_mUSERS1.addOrDelOnZero(se37.modify(count_musersusers_user_id,retweet_retweet_tweet_id,retweet_user_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),(v11 * -1L));
          } while ((n11=n11->nxt));
        }
        { //slice
          const HASH_RES_t h12 = COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn::hash(se40.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mUSERS11_mRETWEET1_map_0* i12 = static_cast<HashIndex_COUNT_mUSERS11_mRETWEET1_map_0*>(COUNT_mUSERS11_mRETWEET1.index[1]);
          HashIndex_COUNT_mUSERS11_mRETWEET1_map_0::IdxNode* n12 = &(i12->buckets_[h12 % i12->size_]);
          COUNT_mUSERS11_mRETWEET1_entry* e12;
          do if ((e12=n12->obj) && h12 == n12->hash && COUNT_mUSERS11_mRETWEET1_mapkey0_idxfn::equals(se40, *e12)) {
            STRING_TYPE t_tweet_date = e12->T_TWEET_DATE;
            long v12 = e12->__av;
            COUNT_mUSERS11.addOrDelOnZero(se39.modify(retweet_retweet_tweet_id,retweet_user_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),(v12 * -1L));
          } while ((n12=n12->nxt));
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
        { //slice
          const HASH_RES_t h13 = COUNT_mTWEET2_mapkey1_idxfn::hash(se43.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_map_1* i13 = static_cast<HashIndex_COUNT_mTWEET2_map_1*>(COUNT_mTWEET2.index[1]);
          HashIndex_COUNT_mTWEET2_map_1::IdxNode* n13 = &(i13->buckets_[h13 % i13->size_]);
          COUNT_mTWEET2_entry* e13;
          do if ((e13=n13->obj) && h13 == n13->hash && COUNT_mTWEET2_mapkey1_idxfn::equals(se43, *e13)) {
            long d_user_id = e13->D_USER_ID;
            STRING_TYPE r_tweet_date = e13->R_TWEET_DATE;
            long r_tweet_id = e13->R_TWEET_ID;
            long v13 = e13->__av;
            COUNT.addOrDelOnZero(se41.modify(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,tweet_tweet_id,r_tweet_id,r_tweet_date,d_user_id),(COUNT_mTWEET1.getValueOrDefault(se42.modify(tweet_user_id)) * v13));
          } while ((n13=n13->nxt));
        }
        COUNT_mRETWEET1.addOrDelOnZero(se44.modify(tweet_tweet_id,tweet_user_id,tweet_user_id,tweet_tweet_date),COUNT_mTWEET1.getValueOrDefault(se45.modify(tweet_user_id)));
        COUNT_mRETWEET1_mUSERS1.addOrDelOnZero(se46.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),1L);
        { //slice
          const HASH_RES_t h14 = COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::hash(se49.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_1* i14 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_1*>(COUNT_mTWEET2_mUSERS1.index[1]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_1::IdxNode* n14 = &(i14->buckets_[h14 % i14->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e14;
          do if ((e14=n14->obj) && h14 == n14->hash && COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::equals(se49, *e14)) {
            long count_musersusers_user_id = e14->COUNT_mTWEET2_mUSERSUSERS_USER_ID;
            STRING_TYPE r_tweet_date = e14->R_TWEET_DATE;
            long r_tweet_id = e14->R_TWEET_ID;
            long v14 = e14->__av;
            COUNT_mUSERS2.addOrDelOnZero(se47.modify(tweet_tweet_id,count_musersusers_user_id,tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se48.modify(tweet_user_id)) * v14));
          } while ((n14=n14->nxt));
        }
        { //slice
          const HASH_RES_t h15 = COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn::hash(se51.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1* i15 = static_cast<HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1*>(COUNT_mUSERS2_mUSERS1_mTWEET1.index[1]);
          HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1::IdxNode* n15 = &(i15->buckets_[h15 % i15->size_]);
          COUNT_mUSERS2_mUSERS1_mTWEET1_entry* e15;
          do if ((e15=n15->obj) && h15 == n15->hash && COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn::equals(se51, *e15)) {
            long count_musersusers_user_id = e15->COUNT_mUSERSUSERS_USER_ID;
            long r_tweet_id = e15->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e15->R_TWEET_DATE;
            long v15 = e15->__av;
            COUNT_mUSERS2_mUSERS1.addOrDelOnZero(se50.modify(tweet_user_id,tweet_tweet_id,count_musersusers_user_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),v15);
          } while ((n15=n15->nxt));
        }
        { //slice
          const HASH_RES_t h16 = COUNT_mUSERS6_mTWEET1_mapkey1_idxfn::hash(se53.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mUSERS6_mTWEET1_map_1* i16 = static_cast<HashIndex_COUNT_mUSERS6_mTWEET1_map_1*>(COUNT_mUSERS6_mTWEET1.index[1]);
          HashIndex_COUNT_mUSERS6_mTWEET1_map_1::IdxNode* n16 = &(i16->buckets_[h16 % i16->size_]);
          COUNT_mUSERS6_mTWEET1_entry* e16;
          do if ((e16=n16->obj) && h16 == n16->hash && COUNT_mUSERS6_mTWEET1_mapkey1_idxfn::equals(se53, *e16)) {
            long d_user_id = e16->D_USER_ID;
            long r_tweet_id = e16->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e16->R_TWEET_DATE;
            long v16 = e16->__av;
            COUNT_mUSERS6.addOrDelOnZero(se52.modify(tweet_user_id,tweet_tweet_id,d_user_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),v16);
          } while ((n16=n16->nxt));
        }
        { //slice
          const HASH_RES_t h17 = COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::hash(se55.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_1* i17 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_1*>(COUNT_mTWEET2_mUSERS1.index[1]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_1::IdxNode* n17 = &(i17->buckets_[h17 % i17->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e17;
          do if ((e17=n17->obj) && h17 == n17->hash && COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::equals(se55, *e17)) {
            long count_musers6_musersusers_user_id = e17->COUNT_mTWEET2_mUSERSUSERS_USER_ID;
            STRING_TYPE r_tweet_date = e17->R_TWEET_DATE;
            long r_tweet_id = e17->R_TWEET_ID;
            long v17 = e17->__av;
            COUNT_mUSERS6_mUSERS1.addOrDelOnZero(se54.modify(tweet_user_id,tweet_tweet_id,count_musers6_musersusers_user_id,tweet_tweet_date,tweet_tweet_id,r_tweet_date,r_tweet_id),v17);
          } while ((n17=n17->nxt));
        }
        { //slice
          const HASH_RES_t h18 = COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::hash(se57.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_1* i18 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_1*>(COUNT_mTWEET2_mUSERS1.index[1]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_1::IdxNode* n18 = &(i18->buckets_[h18 % i18->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e18;
          do if ((e18=n18->obj) && h18 == n18->hash && COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::equals(se57, *e18)) {
            long count_musersusers_user_id = e18->COUNT_mTWEET2_mUSERSUSERS_USER_ID;
            STRING_TYPE r_tweet_date = e18->R_TWEET_DATE;
            long r_tweet_id = e18->R_TWEET_ID;
            long v18 = e18->__av;
            COUNT_mUSERS11.addOrDelOnZero(se56.modify(tweet_tweet_id,count_musersusers_user_id,tweet_tweet_date,tweet_tweet_id,r_tweet_date,r_tweet_id),v18);
          } while ((n18=n18->nxt));
        }
        COUNT_mUSERS11_mRETWEET1.addOrDelOnZero(se58.modify(tweet_tweet_id,tweet_tweet_date),1L);
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
          const HASH_RES_t h19 = COUNT_mTWEET2_mapkey1_idxfn::hash(se61.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_map_1* i19 = static_cast<HashIndex_COUNT_mTWEET2_map_1*>(COUNT_mTWEET2.index[1]);
          HashIndex_COUNT_mTWEET2_map_1::IdxNode* n19 = &(i19->buckets_[h19 % i19->size_]);
          COUNT_mTWEET2_entry* e19;
          do if ((e19=n19->obj) && h19 == n19->hash && COUNT_mTWEET2_mapkey1_idxfn::equals(se61, *e19)) {
            long d_user_id = e19->D_USER_ID;
            STRING_TYPE r_tweet_date = e19->R_TWEET_DATE;
            long r_tweet_id = e19->R_TWEET_ID;
            long v19 = e19->__av;
            COUNT.addOrDelOnZero(se59.modify(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,tweet_tweet_id,r_tweet_id,r_tweet_date,d_user_id),(COUNT_mTWEET1.getValueOrDefault(se60.modify(tweet_user_id)) * (v19 * -1L)));
          } while ((n19=n19->nxt));
        }
        COUNT_mRETWEET1.addOrDelOnZero(se62.modify(tweet_tweet_id,tweet_user_id,tweet_user_id,tweet_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se63.modify(tweet_user_id)) * -1L));
        COUNT_mRETWEET1_mUSERS1.addOrDelOnZero(se64.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),-1L);
        { //slice
          const HASH_RES_t h20 = COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::hash(se67.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_1* i20 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_1*>(COUNT_mTWEET2_mUSERS1.index[1]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_1::IdxNode* n20 = &(i20->buckets_[h20 % i20->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e20;
          do if ((e20=n20->obj) && h20 == n20->hash && COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::equals(se67, *e20)) {
            long count_musersusers_user_id = e20->COUNT_mTWEET2_mUSERSUSERS_USER_ID;
            STRING_TYPE r_tweet_date = e20->R_TWEET_DATE;
            long r_tweet_id = e20->R_TWEET_ID;
            long v20 = e20->__av;
            COUNT_mUSERS2.addOrDelOnZero(se65.modify(tweet_tweet_id,count_musersusers_user_id,tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se66.modify(tweet_user_id)) * (v20 * -1L)));
          } while ((n20=n20->nxt));
        }
        { //slice
          const HASH_RES_t h21 = COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn::hash(se69.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1* i21 = static_cast<HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1*>(COUNT_mUSERS2_mUSERS1_mTWEET1.index[1]);
          HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_1::IdxNode* n21 = &(i21->buckets_[h21 % i21->size_]);
          COUNT_mUSERS2_mUSERS1_mTWEET1_entry* e21;
          do if ((e21=n21->obj) && h21 == n21->hash && COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey1_idxfn::equals(se69, *e21)) {
            long count_musersusers_user_id = e21->COUNT_mUSERSUSERS_USER_ID;
            long r_tweet_id = e21->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e21->R_TWEET_DATE;
            long v21 = e21->__av;
            COUNT_mUSERS2_mUSERS1.addOrDelOnZero(se68.modify(tweet_user_id,tweet_tweet_id,count_musersusers_user_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),(v21 * -1L));
          } while ((n21=n21->nxt));
        }
        { //slice
          const HASH_RES_t h22 = COUNT_mUSERS6_mTWEET1_mapkey1_idxfn::hash(se71.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mUSERS6_mTWEET1_map_1* i22 = static_cast<HashIndex_COUNT_mUSERS6_mTWEET1_map_1*>(COUNT_mUSERS6_mTWEET1.index[1]);
          HashIndex_COUNT_mUSERS6_mTWEET1_map_1::IdxNode* n22 = &(i22->buckets_[h22 % i22->size_]);
          COUNT_mUSERS6_mTWEET1_entry* e22;
          do if ((e22=n22->obj) && h22 == n22->hash && COUNT_mUSERS6_mTWEET1_mapkey1_idxfn::equals(se71, *e22)) {
            long d_user_id = e22->D_USER_ID;
            long r_tweet_id = e22->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e22->R_TWEET_DATE;
            long v22 = e22->__av;
            COUNT_mUSERS6.addOrDelOnZero(se70.modify(tweet_user_id,tweet_tweet_id,d_user_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),(v22 * -1L));
          } while ((n22=n22->nxt));
        }
        { //slice
          const HASH_RES_t h23 = COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::hash(se73.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_1* i23 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_1*>(COUNT_mTWEET2_mUSERS1.index[1]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_1::IdxNode* n23 = &(i23->buckets_[h23 % i23->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e23;
          do if ((e23=n23->obj) && h23 == n23->hash && COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::equals(se73, *e23)) {
            long count_musers6_musersusers_user_id = e23->COUNT_mTWEET2_mUSERSUSERS_USER_ID;
            STRING_TYPE r_tweet_date = e23->R_TWEET_DATE;
            long r_tweet_id = e23->R_TWEET_ID;
            long v23 = e23->__av;
            COUNT_mUSERS6_mUSERS1.addOrDelOnZero(se72.modify(tweet_user_id,tweet_tweet_id,count_musers6_musersusers_user_id,tweet_tweet_date,tweet_tweet_id,r_tweet_date,r_tweet_id),(v23 * -1L));
          } while ((n23=n23->nxt));
        }
        { //slice
          const HASH_RES_t h24 = COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::hash(se75.modify1(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_1* i24 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_1*>(COUNT_mTWEET2_mUSERS1.index[1]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_1::IdxNode* n24 = &(i24->buckets_[h24 % i24->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e24;
          do if ((e24=n24->obj) && h24 == n24->hash && COUNT_mTWEET2_mUSERS1_mapkey1_idxfn::equals(se75, *e24)) {
            long count_musersusers_user_id = e24->COUNT_mTWEET2_mUSERSUSERS_USER_ID;
            STRING_TYPE r_tweet_date = e24->R_TWEET_DATE;
            long r_tweet_id = e24->R_TWEET_ID;
            long v24 = e24->__av;
            COUNT_mUSERS11.addOrDelOnZero(se74.modify(tweet_tweet_id,count_musersusers_user_id,tweet_tweet_date,tweet_tweet_id,r_tweet_date,r_tweet_id),(v24 * -1L));
          } while ((n24=n24->nxt));
        }
        COUNT_mUSERS11_mRETWEET1.addOrDelOnZero(se76.modify(tweet_tweet_id,tweet_tweet_date),-1L);
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
        _c1.clear();
        { //slice
          const HASH_RES_t h25 = COUNT_mUSERS2_mapkey1_idxfn::hash(se78.modify1(users_user_id));
          const HashIndex_COUNT_mUSERS2_map_1* i25 = static_cast<HashIndex_COUNT_mUSERS2_map_1*>(COUNT_mUSERS2.index[1]);
          HashIndex_COUNT_mUSERS2_map_1::IdxNode* n25 = &(i25->buckets_[h25 % i25->size_]);
          COUNT_mUSERS2_entry* e25;
          do if ((e25=n25->obj) && h25 == n25->hash && COUNT_mUSERS2_mapkey1_idxfn::equals(se78, *e25)) {
            long b_tweet_id = e25->B_TWEET_ID;
            long f_user_id = e25->F_USER_ID;
            long t_user_id = e25->T_USER_ID;
            long c_retweet_tweet_id = e25->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e25->T_TWEET_DATE;
            long r_tweet_id = e25->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e25->R_TWEET_DATE;
            long v26 = e25->__av;
            add_to_temp_map(_c1, st1.modify(t_tweet_date,f_user_id,c_retweet_tweet_id,r_tweet_date,r_tweet_id,t_user_id,b_tweet_id,v26));
          } while ((n25=n25->nxt));
        }
        { //slice
          const HASH_RES_t h26 = COUNT_mUSERS11_mapkey1_idxfn::hash(se79.modify1(users_user_id));
          const HashIndex_COUNT_mUSERS11_map_1* i26 = static_cast<HashIndex_COUNT_mUSERS11_map_1*>(COUNT_mUSERS11.index[1]);
          HashIndex_COUNT_mUSERS11_map_1::IdxNode* n26 = &(i26->buckets_[h26 % i26->size_]);
          COUNT_mUSERS11_entry* e26;
          do if ((e26=n26->obj) && h26 == n26->hash && COUNT_mUSERS11_mapkey1_idxfn::equals(se79, *e26)) {
            long b_tweet_id = e26->B_TWEET_ID;
            STRING_TYPE t_tweet_date = e26->T_TWEET_DATE;
            long c_retweet_tweet_id = e26->C_RETWEET_TWEET_ID;
            STRING_TYPE r_tweet_date = e26->R_TWEET_DATE;
            long r_tweet_id = e26->R_TWEET_ID;
            long v27 = e26->__av;
            add_to_temp_map(_c1, st2.modify(t_tweet_date,users_user_id,c_retweet_tweet_id,r_tweet_date,r_tweet_id,users_user_id,b_tweet_id,v27));
          } while ((n26=n26->nxt));
        }
        { //temp foreach
          tuple8_SLLSLLL_L* e27 = _c1.head;
          while(e27){
            STRING_TYPE t_tweet_date = e27->_1;
            long f_user_id = e27->_2;
            long c_retweet_tweet_id = e27->_3;
            STRING_TYPE r_tweet_date = e27->_4;
            long r_tweet_id = e27->_5;
            long t_user_id = e27->_6;
            long b_tweet_id = e27->_7;
            long v28 = e27->__av;
            COUNT.addOrDelOnZero(se77.modify(f_user_id,t_user_id,b_tweet_id,t_tweet_date,c_retweet_tweet_id,r_tweet_id,r_tweet_date,users_user_id),v28);
            e27 = e27->nxt;
          }
        }
        { //slice
          const HASH_RES_t h27 = COUNT_mUSERS6_mapkey0_idxfn::hash(se80.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS6_map_0* i27 = static_cast<HashIndex_COUNT_mUSERS6_map_0*>(COUNT_mUSERS6.index[1]);
          HashIndex_COUNT_mUSERS6_map_0::IdxNode* n27 = &(i27->buckets_[h27 % i27->size_]);
          COUNT_mUSERS6_entry* e28;
          do if ((e28=n27->obj) && h27 == n27->hash && COUNT_mUSERS6_mapkey0_idxfn::equals(se80, *e28)) {
            long b_tweet_id = e28->B_TWEET_ID;
            long d_user_id = e28->D_USER_ID;
            long c_retweet_tweet_id = e28->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e28->T_TWEET_DATE;
            long r_tweet_id = e28->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e28->R_TWEET_DATE;
            long v29 = e28->__av;
            COUNT.addOrDelOnZero(se77.modify(users_user_id,users_user_id,b_tweet_id,t_tweet_date,c_retweet_tweet_id,r_tweet_id,r_tweet_date,d_user_id),v29);
          } while ((n27=n27->nxt));
        }
        { //slice
          const HASH_RES_t h28 = COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::hash(se82.modify0(users_user_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_0* i28 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_0*>(COUNT_mRETWEET1_mUSERS1.index[2]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_0::IdxNode* n28 = &(i28->buckets_[h28 % i28->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e29;
          do if ((e29=n28->obj) && h28 == n28->hash && COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::equals(se82, *e29)) {
            long count_mretweetretweet_retweet_tweet_id = e29->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e29->T_TWEET_DATE;
            long v30 = e29->__av;
            COUNT_mRETWEET1.addOrDelOnZero(se81.modify(count_mretweetretweet_retweet_tweet_id,users_user_id,users_user_id,t_tweet_date),v30);
          } while ((n28=n28->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se83.modify(users_user_id),1L);
        { //slice
          const HASH_RES_t h29 = COUNT_mTWEET2_mUSERS1_mapkey0_idxfn::hash(se85.modify0(users_user_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_0* i29 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_0*>(COUNT_mTWEET2_mUSERS1.index[2]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_0::IdxNode* n29 = &(i29->buckets_[h29 % i29->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e30;
          do if ((e30=n29->obj) && h29 == n29->hash && COUNT_mTWEET2_mUSERS1_mapkey0_idxfn::equals(se85, *e30)) {
            long count_mtweettweet_tweet_id = e30->COUNT_mTWEETTWEET_TWEET_ID;
            STRING_TYPE r_tweet_date = e30->R_TWEET_DATE;
            long r_tweet_id = e30->R_TWEET_ID;
            long v31 = e30->__av;
            COUNT_mTWEET2.addOrDelOnZero(se84.modify(users_user_id,count_mtweettweet_tweet_id,r_tweet_date,r_tweet_id),v31);
          } while ((n29=n29->nxt));
        }
        { //slice
          const HASH_RES_t h30 = COUNT_mUSERS2_mUSERS1_mapkey0_idxfn::hash(se87.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS2_mUSERS1_map_0* i30 = static_cast<HashIndex_COUNT_mUSERS2_mUSERS1_map_0*>(COUNT_mUSERS2_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS2_mUSERS1_map_0::IdxNode* n30 = &(i30->buckets_[h30 % i30->size_]);
          COUNT_mUSERS2_mUSERS1_entry* e31;
          do if ((e31=n30->obj) && h30 == n30->hash && COUNT_mUSERS2_mUSERS1_mapkey0_idxfn::equals(se87, *e31)) {
            long b_tweet_id = e31->B_TWEET_ID;
            long count_musersusers_user_id = e31->COUNT_mUSERSUSERS_USER_ID;
            long c_retweet_tweet_id = e31->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e31->T_TWEET_DATE;
            long r_tweet_id = e31->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e31->R_TWEET_DATE;
            long v32 = e31->__av;
            COUNT_mUSERS2.addOrDelOnZero(se86.modify(b_tweet_id,count_musersusers_user_id,users_user_id,users_user_id,c_retweet_tweet_id,t_tweet_date,r_tweet_id,r_tweet_date),v32);
          } while ((n30=n30->nxt));
        }
        { //slice
          const HASH_RES_t h31 = COUNT_mUSERS6_mUSERS1_mapkey2_idxfn::hash(se89.modify2(users_user_id));
          const HashIndex_COUNT_mUSERS6_mUSERS1_map_2* i31 = static_cast<HashIndex_COUNT_mUSERS6_mUSERS1_map_2*>(COUNT_mUSERS6_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS6_mUSERS1_map_2::IdxNode* n31 = &(i31->buckets_[h31 % i31->size_]);
          COUNT_mUSERS6_mUSERS1_entry* e32;
          do if ((e32=n31->obj) && h31 == n31->hash && COUNT_mUSERS6_mUSERS1_mapkey2_idxfn::equals(se89, *e32)) {
            long count_musersusers_user_id = e32->COUNT_mUSERSUSERS_USER_ID;
            long b_tweet_id = e32->B_TWEET_ID;
            STRING_TYPE t_tweet_date = e32->T_TWEET_DATE;
            long c_retweet_tweet_id = e32->C_RETWEET_TWEET_ID;
            STRING_TYPE r_tweet_date = e32->R_TWEET_DATE;
            long r_tweet_id = e32->R_TWEET_ID;
            long v33 = e32->__av;
            COUNT_mUSERS6.addOrDelOnZero(se88.modify(count_musersusers_user_id,b_tweet_id,users_user_id,c_retweet_tweet_id,t_tweet_date,r_tweet_id,r_tweet_date),v33);
          } while ((n31=n31->nxt));
        }
        { //slice
          const HASH_RES_t h32 = COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn::hash(se91.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0* i32 = static_cast<HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0*>(COUNT_mUSERS2_mUSERS1_mTWEET1.index[2]);
          HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0::IdxNode* n32 = &(i32->buckets_[h32 % i32->size_]);
          COUNT_mUSERS2_mUSERS1_mTWEET1_entry* e33;
          do if ((e33=n32->obj) && h32 == n32->hash && COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn::equals(se91, *e33)) {
            long count_musers6_mtweettweet_tweet_id = e33->COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID;
            long r_tweet_id = e33->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e33->R_TWEET_DATE;
            long v34 = e33->__av;
            COUNT_mUSERS6_mTWEET1.addOrDelOnZero(se90.modify(users_user_id,count_musers6_mtweettweet_tweet_id,r_tweet_id,r_tweet_date),v34);
          } while ((n32=n32->nxt));
        }
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
        _c2.clear();
        { //slice
          const HASH_RES_t h33 = COUNT_mUSERS2_mapkey1_idxfn::hash(se93.modify1(users_user_id));
          const HashIndex_COUNT_mUSERS2_map_1* i33 = static_cast<HashIndex_COUNT_mUSERS2_map_1*>(COUNT_mUSERS2.index[1]);
          HashIndex_COUNT_mUSERS2_map_1::IdxNode* n33 = &(i33->buckets_[h33 % i33->size_]);
          COUNT_mUSERS2_entry* e34;
          do if ((e34=n33->obj) && h33 == n33->hash && COUNT_mUSERS2_mapkey1_idxfn::equals(se93, *e34)) {
            long b_tweet_id = e34->B_TWEET_ID;
            long f_user_id = e34->F_USER_ID;
            long t_user_id = e34->T_USER_ID;
            long c_retweet_tweet_id = e34->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e34->T_TWEET_DATE;
            long r_tweet_id = e34->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e34->R_TWEET_DATE;
            long v36 = e34->__av;
            add_to_temp_map(_c2, st3.modify(t_tweet_date,f_user_id,c_retweet_tweet_id,r_tweet_date,r_tweet_id,t_user_id,b_tweet_id,(v36 * -1L)));
          } while ((n33=n33->nxt));
        }
        { //slice
          const HASH_RES_t h34 = COUNT_mUSERS11_mapkey1_idxfn::hash(se94.modify1(users_user_id));
          const HashIndex_COUNT_mUSERS11_map_1* i34 = static_cast<HashIndex_COUNT_mUSERS11_map_1*>(COUNT_mUSERS11.index[1]);
          HashIndex_COUNT_mUSERS11_map_1::IdxNode* n34 = &(i34->buckets_[h34 % i34->size_]);
          COUNT_mUSERS11_entry* e35;
          do if ((e35=n34->obj) && h34 == n34->hash && COUNT_mUSERS11_mapkey1_idxfn::equals(se94, *e35)) {
            long b_tweet_id = e35->B_TWEET_ID;
            STRING_TYPE t_tweet_date = e35->T_TWEET_DATE;
            long c_retweet_tweet_id = e35->C_RETWEET_TWEET_ID;
            STRING_TYPE r_tweet_date = e35->R_TWEET_DATE;
            long r_tweet_id = e35->R_TWEET_ID;
            long v37 = e35->__av;
            add_to_temp_map(_c2, st4.modify(t_tweet_date,users_user_id,c_retweet_tweet_id,r_tweet_date,r_tweet_id,users_user_id,b_tweet_id,v37));
          } while ((n34=n34->nxt));
        }
        { //temp foreach
          tuple8_SLLSLLL_L* e36 = _c2.head;
          while(e36){
            STRING_TYPE t_tweet_date = e36->_1;
            long f_user_id = e36->_2;
            long c_retweet_tweet_id = e36->_3;
            STRING_TYPE r_tweet_date = e36->_4;
            long r_tweet_id = e36->_5;
            long t_user_id = e36->_6;
            long b_tweet_id = e36->_7;
            long v38 = e36->__av;
            COUNT.addOrDelOnZero(se92.modify(f_user_id,t_user_id,b_tweet_id,t_tweet_date,c_retweet_tweet_id,r_tweet_id,r_tweet_date,users_user_id),v38);
            e36 = e36->nxt;
          }
        }
        { //slice
          const HASH_RES_t h35 = COUNT_mUSERS6_mapkey0_idxfn::hash(se95.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS6_map_0* i35 = static_cast<HashIndex_COUNT_mUSERS6_map_0*>(COUNT_mUSERS6.index[1]);
          HashIndex_COUNT_mUSERS6_map_0::IdxNode* n35 = &(i35->buckets_[h35 % i35->size_]);
          COUNT_mUSERS6_entry* e37;
          do if ((e37=n35->obj) && h35 == n35->hash && COUNT_mUSERS6_mapkey0_idxfn::equals(se95, *e37)) {
            long b_tweet_id = e37->B_TWEET_ID;
            long d_user_id = e37->D_USER_ID;
            long c_retweet_tweet_id = e37->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e37->T_TWEET_DATE;
            long r_tweet_id = e37->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e37->R_TWEET_DATE;
            long v39 = e37->__av;
            COUNT.addOrDelOnZero(se92.modify(users_user_id,users_user_id,b_tweet_id,t_tweet_date,c_retweet_tweet_id,r_tweet_id,r_tweet_date,d_user_id),(v39 * -1L));
          } while ((n35=n35->nxt));
        }
        { //slice
          const HASH_RES_t h36 = COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::hash(se97.modify0(users_user_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_0* i36 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_0*>(COUNT_mRETWEET1_mUSERS1.index[2]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_0::IdxNode* n36 = &(i36->buckets_[h36 % i36->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e38;
          do if ((e38=n36->obj) && h36 == n36->hash && COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::equals(se97, *e38)) {
            long count_mretweetretweet_retweet_tweet_id = e38->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e38->T_TWEET_DATE;
            long v40 = e38->__av;
            COUNT_mRETWEET1.addOrDelOnZero(se96.modify(count_mretweetretweet_retweet_tweet_id,users_user_id,users_user_id,t_tweet_date),(v40 * -1L));
          } while ((n36=n36->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se98.modify(users_user_id),-1L);
        { //slice
          const HASH_RES_t h37 = COUNT_mTWEET2_mUSERS1_mapkey0_idxfn::hash(se100.modify0(users_user_id));
          const HashIndex_COUNT_mTWEET2_mUSERS1_map_0* i37 = static_cast<HashIndex_COUNT_mTWEET2_mUSERS1_map_0*>(COUNT_mTWEET2_mUSERS1.index[2]);
          HashIndex_COUNT_mTWEET2_mUSERS1_map_0::IdxNode* n37 = &(i37->buckets_[h37 % i37->size_]);
          COUNT_mTWEET2_mUSERS1_entry* e39;
          do if ((e39=n37->obj) && h37 == n37->hash && COUNT_mTWEET2_mUSERS1_mapkey0_idxfn::equals(se100, *e39)) {
            long count_mtweettweet_tweet_id = e39->COUNT_mTWEETTWEET_TWEET_ID;
            STRING_TYPE r_tweet_date = e39->R_TWEET_DATE;
            long r_tweet_id = e39->R_TWEET_ID;
            long v41 = e39->__av;
            COUNT_mTWEET2.addOrDelOnZero(se99.modify(users_user_id,count_mtweettweet_tweet_id,r_tweet_date,r_tweet_id),(v41 * -1L));
          } while ((n37=n37->nxt));
        }
        { //slice
          const HASH_RES_t h38 = COUNT_mUSERS2_mUSERS1_mapkey0_idxfn::hash(se102.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS2_mUSERS1_map_0* i38 = static_cast<HashIndex_COUNT_mUSERS2_mUSERS1_map_0*>(COUNT_mUSERS2_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS2_mUSERS1_map_0::IdxNode* n38 = &(i38->buckets_[h38 % i38->size_]);
          COUNT_mUSERS2_mUSERS1_entry* e40;
          do if ((e40=n38->obj) && h38 == n38->hash && COUNT_mUSERS2_mUSERS1_mapkey0_idxfn::equals(se102, *e40)) {
            long b_tweet_id = e40->B_TWEET_ID;
            long count_musersusers_user_id = e40->COUNT_mUSERSUSERS_USER_ID;
            long c_retweet_tweet_id = e40->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e40->T_TWEET_DATE;
            long r_tweet_id = e40->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e40->R_TWEET_DATE;
            long v42 = e40->__av;
            COUNT_mUSERS2.addOrDelOnZero(se101.modify(b_tweet_id,count_musersusers_user_id,users_user_id,users_user_id,c_retweet_tweet_id,t_tweet_date,r_tweet_id,r_tweet_date),(v42 * -1L));
          } while ((n38=n38->nxt));
        }
        { //slice
          const HASH_RES_t h39 = COUNT_mUSERS6_mUSERS1_mapkey2_idxfn::hash(se104.modify2(users_user_id));
          const HashIndex_COUNT_mUSERS6_mUSERS1_map_2* i39 = static_cast<HashIndex_COUNT_mUSERS6_mUSERS1_map_2*>(COUNT_mUSERS6_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS6_mUSERS1_map_2::IdxNode* n39 = &(i39->buckets_[h39 % i39->size_]);
          COUNT_mUSERS6_mUSERS1_entry* e41;
          do if ((e41=n39->obj) && h39 == n39->hash && COUNT_mUSERS6_mUSERS1_mapkey2_idxfn::equals(se104, *e41)) {
            long count_musersusers_user_id = e41->COUNT_mUSERSUSERS_USER_ID;
            long b_tweet_id = e41->B_TWEET_ID;
            STRING_TYPE t_tweet_date = e41->T_TWEET_DATE;
            long c_retweet_tweet_id = e41->C_RETWEET_TWEET_ID;
            STRING_TYPE r_tweet_date = e41->R_TWEET_DATE;
            long r_tweet_id = e41->R_TWEET_ID;
            long v43 = e41->__av;
            COUNT_mUSERS6.addOrDelOnZero(se103.modify(count_musersusers_user_id,b_tweet_id,users_user_id,c_retweet_tweet_id,t_tweet_date,r_tweet_id,r_tweet_date),(v43 * -1L));
          } while ((n39=n39->nxt));
        }
        { //slice
          const HASH_RES_t h40 = COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn::hash(se106.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0* i40 = static_cast<HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0*>(COUNT_mUSERS2_mUSERS1_mTWEET1.index[2]);
          HashIndex_COUNT_mUSERS2_mUSERS1_mTWEET1_map_0::IdxNode* n40 = &(i40->buckets_[h40 % i40->size_]);
          COUNT_mUSERS2_mUSERS1_mTWEET1_entry* e42;
          do if ((e42=n40->obj) && h40 == n40->hash && COUNT_mUSERS2_mUSERS1_mTWEET1_mapkey0_idxfn::equals(se106, *e42)) {
            long count_musers6_mtweettweet_tweet_id = e42->COUNT_mUSERS2_mUSERS1_mTWEETTWEET_TWEET_ID;
            long r_tweet_id = e42->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e42->R_TWEET_DATE;
            long v44 = e42->__av;
            COUNT_mUSERS6_mTWEET1.addOrDelOnZero(se105.modify(users_user_id,count_musers6_mtweettweet_tweet_id,r_tweet_id,r_tweet_date),(v44 * -1L));
          } while ((n40=n40->nxt));
        }
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
    COUNT_entry se1;
    COUNT_mTWEET1_entry se2;
    COUNT_mRETWEET1_entry se3;
    COUNT_mTWEET2_entry se4;
    COUNT_mTWEET1_entry se5;
    COUNT_mTWEET2_mUSERS1_entry se6;
    COUNT_mUSERS2_entry se7;
    COUNT_mRETWEET1_entry se8;
    COUNT_mUSERS2_mUSERS1_entry se9;
    COUNT_mRETWEET1_mUSERS1_entry se10;
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry se11;
    COUNT_mUSERS6_entry se12;
    COUNT_mTWEET1_entry se13;
    COUNT_mRETWEET1_mUSERS1_entry se14;
    COUNT_mUSERS6_mTWEET1_entry se15;
    COUNT_mTWEET1_entry se16;
    COUNT_mUSERS6_mUSERS1_entry se17;
    COUNT_mRETWEET1_mUSERS1_entry se18;
    COUNT_mUSERS11_entry se19;
    COUNT_mUSERS11_mRETWEET1_entry se20;
    COUNT_entry se21;
    COUNT_mTWEET1_entry se22;
    COUNT_mRETWEET1_entry se23;
    COUNT_mTWEET2_entry se24;
    COUNT_mTWEET1_entry se25;
    COUNT_mTWEET2_mUSERS1_entry se26;
    COUNT_mUSERS2_entry se27;
    COUNT_mRETWEET1_entry se28;
    COUNT_mUSERS2_mUSERS1_entry se29;
    COUNT_mRETWEET1_mUSERS1_entry se30;
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry se31;
    COUNT_mUSERS6_entry se32;
    COUNT_mTWEET1_entry se33;
    COUNT_mRETWEET1_mUSERS1_entry se34;
    COUNT_mUSERS6_mTWEET1_entry se35;
    COUNT_mTWEET1_entry se36;
    COUNT_mUSERS6_mUSERS1_entry se37;
    COUNT_mRETWEET1_mUSERS1_entry se38;
    COUNT_mUSERS11_entry se39;
    COUNT_mUSERS11_mRETWEET1_entry se40;
    COUNT_entry se41;
    COUNT_mTWEET1_entry se42;
    COUNT_mTWEET2_entry se43;
    COUNT_mRETWEET1_entry se44;
    COUNT_mTWEET1_entry se45;
    COUNT_mRETWEET1_mUSERS1_entry se46;
    COUNT_mUSERS2_entry se47;
    COUNT_mTWEET1_entry se48;
    COUNT_mTWEET2_mUSERS1_entry se49;
    COUNT_mUSERS2_mUSERS1_entry se50;
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry se51;
    COUNT_mUSERS6_entry se52;
    COUNT_mUSERS6_mTWEET1_entry se53;
    COUNT_mUSERS6_mUSERS1_entry se54;
    COUNT_mTWEET2_mUSERS1_entry se55;
    COUNT_mUSERS11_entry se56;
    COUNT_mTWEET2_mUSERS1_entry se57;
    COUNT_mUSERS11_mRETWEET1_entry se58;
    COUNT_entry se59;
    COUNT_mTWEET1_entry se60;
    COUNT_mTWEET2_entry se61;
    COUNT_mRETWEET1_entry se62;
    COUNT_mTWEET1_entry se63;
    COUNT_mRETWEET1_mUSERS1_entry se64;
    COUNT_mUSERS2_entry se65;
    COUNT_mTWEET1_entry se66;
    COUNT_mTWEET2_mUSERS1_entry se67;
    COUNT_mUSERS2_mUSERS1_entry se68;
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry se69;
    COUNT_mUSERS6_entry se70;
    COUNT_mUSERS6_mTWEET1_entry se71;
    COUNT_mUSERS6_mUSERS1_entry se72;
    COUNT_mTWEET2_mUSERS1_entry se73;
    COUNT_mUSERS11_entry se74;
    COUNT_mTWEET2_mUSERS1_entry se75;
    COUNT_mUSERS11_mRETWEET1_entry se76;
    COUNT_entry se77;
    tuple8_SLLSLLL_L st1;
    COUNT_mUSERS2_entry se78;
    tuple8_SLLSLLL_L st2;
    COUNT_mUSERS11_entry se79;
    COUNT_mUSERS6_entry se80;
    COUNT_mRETWEET1_entry se81;
    COUNT_mRETWEET1_mUSERS1_entry se82;
    COUNT_mTWEET1_entry se83;
    COUNT_mTWEET2_entry se84;
    COUNT_mTWEET2_mUSERS1_entry se85;
    COUNT_mUSERS2_entry se86;
    COUNT_mUSERS2_mUSERS1_entry se87;
    COUNT_mUSERS6_entry se88;
    COUNT_mUSERS6_mUSERS1_entry se89;
    COUNT_mUSERS6_mTWEET1_entry se90;
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry se91;
    COUNT_entry se92;
    tuple8_SLLSLLL_L st3;
    COUNT_mUSERS2_entry se93;
    tuple8_SLLSLLL_L st4;
    COUNT_mUSERS11_entry se94;
    COUNT_mUSERS6_entry se95;
    COUNT_mRETWEET1_entry se96;
    COUNT_mRETWEET1_mUSERS1_entry se97;
    COUNT_mTWEET1_entry se98;
    COUNT_mTWEET2_entry se99;
    COUNT_mTWEET2_mUSERS1_entry se100;
    COUNT_mUSERS2_entry se101;
    COUNT_mUSERS2_mUSERS1_entry se102;
    COUNT_mUSERS6_entry se103;
    COUNT_mUSERS6_mUSERS1_entry se104;
    COUNT_mUSERS6_mTWEET1_entry se105;
    COUNT_mUSERS2_mUSERS1_mTWEET1_entry se106;
  
    /* Data structures used for storing materialized views */
    COUNT_mRETWEET1_map COUNT_mRETWEET1;
    COUNT_mRETWEET1_mUSERS1_map COUNT_mRETWEET1_mUSERS1;
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mTWEET2_map COUNT_mTWEET2;
    COUNT_mTWEET2_mUSERS1_map COUNT_mTWEET2_mUSERS1;
    COUNT_mUSERS2_map COUNT_mUSERS2;
    COUNT_mUSERS2_mUSERS1_map COUNT_mUSERS2_mUSERS1;
    COUNT_mUSERS2_mUSERS1_mTWEET1_map COUNT_mUSERS2_mUSERS1_mTWEET1;
    COUNT_mUSERS6_map COUNT_mUSERS6;
    COUNT_mUSERS6_mTWEET1_map COUNT_mUSERS6_mTWEET1;
    COUNT_mUSERS6_mUSERS1_map COUNT_mUSERS6_mUSERS1;
    COUNT_mUSERS11_map COUNT_mUSERS11;
    COUNT_mUSERS11_mRETWEET1_map COUNT_mUSERS11_mRETWEET1;
    MultiHashMap<tuple8_SLLSLLL_L,long,HashIndex<tuple8_SLLSLLL_L,long> > _c1;
    MultiHashMap<tuple8_SLLSLLL_L,long,HashIndex<tuple8_SLLSLLL_L,long> > _c2;
    
  
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
  class Q5p : public Program
  {
    public:
      Q5p(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

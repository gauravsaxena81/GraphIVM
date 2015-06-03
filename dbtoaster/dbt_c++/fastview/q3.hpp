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
    long F_USER_ID; long T_USER_ID; long B_TWEET_ID; STRING_TYPE T_TWEET_DATE; long C_RETWEET_TWEET_ID; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_entry* nxt; COUNT_entry* prv;
    explicit COUNT_entry() : nxt(nullptr), prv(nullptr) { /*F_USER_ID = 0L; T_USER_ID = 0L; B_TWEET_ID = 0L; T_TWEET_DATE = ""; C_RETWEET_TWEET_ID = 0L; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const STRING_TYPE& c6, const long c7) { F_USER_ID = c0; T_USER_ID = c1; B_TWEET_ID = c2; T_TWEET_DATE = c3; C_RETWEET_TWEET_ID = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6; __av = c7; }
    COUNT_entry(const COUNT_entry& other) : F_USER_ID( other.F_USER_ID ), T_USER_ID( other.T_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const long c5, const STRING_TYPE& c6) { F_USER_ID = c0; T_USER_ID = c1; B_TWEET_ID = c2; T_TWEET_DATE = c3; C_RETWEET_TWEET_ID = c4; R_TWEET_ID = c5; R_TWEET_DATE = c6;  return *this; }
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
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct COUNT_mapkey0123456_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.F_USER_ID);
      hash_combine(h, e.T_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_entry& x, const COUNT_entry& y) {
      return x.F_USER_ID == y.F_USER_ID && x.T_USER_ID == y.T_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  typedef MultiHashMap<COUNT_entry,long,
    HashIndex<COUNT_entry,long,COUNT_mapkey0123456_idxfn,true>
  > COUNT_map;
  typedef HashIndex<COUNT_entry,long,COUNT_mapkey0123456_idxfn,true> HashIndex_COUNT_map_0123456;
  
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
    long COUNT_mTWEETTWEET_TWEET_ID; STRING_TYPE R_TWEET_DATE; long R_TWEET_ID; long __av;  COUNT_mTWEET2_entry* nxt; COUNT_mTWEET2_entry* prv;
    explicit COUNT_mTWEET2_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mTWEETTWEET_TWEET_ID = 0L; R_TWEET_DATE = ""; R_TWEET_ID = 0L; __av = 0L; */ }
    explicit COUNT_mTWEET2_entry(const long c0, const STRING_TYPE& c1, const long c2, const long c3) { COUNT_mTWEETTWEET_TWEET_ID = c0; R_TWEET_DATE = c1; R_TWEET_ID = c2; __av = c3; }
    COUNT_mTWEET2_entry(const COUNT_mTWEET2_entry& other) : COUNT_mTWEETTWEET_TWEET_ID( other.COUNT_mTWEETTWEET_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mTWEET2_entry& modify(const long c0, const STRING_TYPE& c1, const long c2) { COUNT_mTWEETTWEET_TWEET_ID = c0; R_TWEET_DATE = c1; R_TWEET_ID = c2;  return *this; }
    FORCE_INLINE COUNT_mTWEET2_entry& modify0(const long c0) { COUNT_mTWEETTWEET_TWEET_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
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
  struct COUNT_mTWEET2_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mTWEET2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mTWEETTWEET_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mTWEET2_entry& x, const COUNT_mTWEET2_entry& y) {
      return x.COUNT_mTWEETTWEET_TWEET_ID == y.COUNT_mTWEETTWEET_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID;
    }
  };
  
  struct COUNT_mTWEET2_mapkey0_idxfn {
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
    HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey012_idxfn,true>,
    HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey0_idxfn,false>
  > COUNT_mTWEET2_map;
  typedef HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey012_idxfn,true> HashIndex_COUNT_mTWEET2_map_012;
  typedef HashIndex<COUNT_mTWEET2_entry,long,COUNT_mTWEET2_mapkey0_idxfn,false> HashIndex_COUNT_mTWEET2_map_0;
  
  struct COUNT_mUSERS1_entry {
    long COUNT_mUSERSUSERS_USER_ID; long B_TWEET_ID; long C_RETWEET_TWEET_ID; STRING_TYPE T_TWEET_DATE; long R_TWEET_ID; STRING_TYPE R_TWEET_DATE; long __av;  COUNT_mUSERS1_entry* nxt; COUNT_mUSERS1_entry* prv;
    explicit COUNT_mUSERS1_entry() : nxt(nullptr), prv(nullptr) { /*COUNT_mUSERSUSERS_USER_ID = 0L; B_TWEET_ID = 0L; C_RETWEET_TWEET_ID = 0L; T_TWEET_DATE = ""; R_TWEET_ID = 0L; R_TWEET_DATE = ""; __av = 0L; */ }
    explicit COUNT_mUSERS1_entry(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const STRING_TYPE& c5, const long c6) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; C_RETWEET_TWEET_ID = c2; T_TWEET_DATE = c3; R_TWEET_ID = c4; R_TWEET_DATE = c5; __av = c6; }
    COUNT_mUSERS1_entry(const COUNT_mUSERS1_entry& other) : COUNT_mUSERSUSERS_USER_ID( other.COUNT_mUSERSUSERS_USER_ID ), B_TWEET_ID( other.B_TWEET_ID ), C_RETWEET_TWEET_ID( other.C_RETWEET_TWEET_ID ), T_TWEET_DATE( other.T_TWEET_DATE ), R_TWEET_ID( other.R_TWEET_ID ), R_TWEET_DATE( other.R_TWEET_DATE ), __av( other.__av ), nxt( nullptr ), prv( nullptr ) {}
    FORCE_INLINE COUNT_mUSERS1_entry& modify(const long c0, const long c1, const long c2, const STRING_TYPE& c3, const long c4, const STRING_TYPE& c5) { COUNT_mUSERSUSERS_USER_ID = c0; B_TWEET_ID = c1; C_RETWEET_TWEET_ID = c2; T_TWEET_DATE = c3; R_TWEET_ID = c4; R_TWEET_DATE = c5;  return *this; }
    FORCE_INLINE COUNT_mUSERS1_entry& modify0(const long c0) { COUNT_mUSERSUSERS_USER_ID = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, COUNT_mUSERSUSERS_USER_ID);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B_TWEET_ID);
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
  struct COUNT_mUSERS1_mapkey012345_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      hash_combine(h, e.B_TWEET_ID);
      hash_combine(h, e.C_RETWEET_TWEET_ID);
      hash_combine(h, e.T_TWEET_DATE);
      hash_combine(h, e.R_TWEET_ID);
      hash_combine(h, e.R_TWEET_DATE);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS1_entry& x, const COUNT_mUSERS1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID && x.B_TWEET_ID == y.B_TWEET_ID && x.C_RETWEET_TWEET_ID == y.C_RETWEET_TWEET_ID && x.T_TWEET_DATE == y.T_TWEET_DATE && x.R_TWEET_ID == y.R_TWEET_ID && x.R_TWEET_DATE == y.R_TWEET_DATE;
    }
  };
  
  struct COUNT_mUSERS1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const COUNT_mUSERS1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.COUNT_mUSERSUSERS_USER_ID);
      return h;
    }
    FORCE_INLINE static bool equals(const COUNT_mUSERS1_entry& x, const COUNT_mUSERS1_entry& y) {
      return x.COUNT_mUSERSUSERS_USER_ID == y.COUNT_mUSERSUSERS_USER_ID;
    }
  };
  
  typedef MultiHashMap<COUNT_mUSERS1_entry,long,
    HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey012345_idxfn,true>,
    HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey0_idxfn,false>
  > COUNT_mUSERS1_map;
  typedef HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey012345_idxfn,true> HashIndex_COUNT_mUSERS1_map_012345;
  typedef HashIndex<COUNT_mUSERS1_entry,long,COUNT_mUSERS1_mapkey0_idxfn,false> HashIndex_COUNT_mUSERS1_map_0;
  
  
  
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
    data_t(): tlq_t() {
      
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
      pb.add_map<COUNT_mUSERS1_map>( "COUNT_mUSERS1", COUNT_mUSERS1 );
  
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
    
    
    /* Trigger functions for stream relations */
    void on_insert_RETWEET(const long retweet_user_id, const long retweet_tweet_id, const STRING_TYPE& retweet_tweet_date, const long retweet_retweet_tweet_id) {
      BEGIN_TRIGGER(exec_stats,"insert_RETWEET")
      BEGIN_TRIGGER(ivc_stats,"insert_RETWEET")
      {  //++tN;
        { //slice
          const HASH_RES_t h1 = COUNT_mRETWEET1_mapkey0_idxfn::hash(se2.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_map_0* i1 = static_cast<HashIndex_COUNT_mRETWEET1_map_0*>(COUNT_mRETWEET1.index[1]);
          HashIndex_COUNT_mRETWEET1_map_0::IdxNode* n1 = &(i1->buckets_[h1 % i1->size_]);
          COUNT_mRETWEET1_entry* e1;
          do if ((e1=n1->obj) && h1 == n1->hash && COUNT_mRETWEET1_mapkey0_idxfn::equals(se2, *e1)) {
            long f_user_id = e1->F_USER_ID;
            long t_user_id = e1->T_USER_ID;
            STRING_TYPE t_tweet_date = e1->T_TWEET_DATE;
            long v1 = e1->__av;
		api.useDiffQ3(f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date);
            //COUNT.addOrDelOnZero(se1.modify(f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date),v1);
          } while ((n1=n1->nxt));
        }
        COUNT_mTWEET2.addOrDelOnZero(se3.modify(retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),1L);
        { //slice
          const HASH_RES_t h2 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se5.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i2 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n2 = &(i2->buckets_[h2 % i2->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e2;
          do if ((e2=n2->obj) && h2 == n2->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se5, *e2)) {
            long count_musersusers_user_id = e2->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e2->T_TWEET_DATE;
            long v2 = e2->__av;
            COUNT_mUSERS1.addOrDelOnZero(se4.modify(count_musersusers_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),v2);
          } while ((n2=n2->nxt));
        }
      }
      END_TRIGGER(exec_stats,"insert_RETWEET")
      END_TRIGGER(ivc_stats,"insert_RETWEET")
    }
    void unwrap_insert_RETWEET(const event_args_t& ea) {
      on_insert_RETWEET(*(reinterpret_cast<long*>(ea[0])), *(reinterpret_cast<long*>(ea[1])), *(reinterpret_cast<STRING_TYPE*>(ea[2])), *(reinterpret_cast<long*>(ea[3])));
    }
    
    void on_delete_RETWEET(long retweet_user_id, long retweet_tweet_id, const STRING_TYPE& retweet_tweet_date, long retweet_retweet_tweet_id) {
      BEGIN_TRIGGER(exec_stats,"delete_RETWEET")
      BEGIN_TRIGGER(ivc_stats,"delete_RETWEET")
      {  //++tN;
        { //slice
          const HASH_RES_t h3 = COUNT_mRETWEET1_mapkey0_idxfn::hash(se7.modify0(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_map_0* i3 = static_cast<HashIndex_COUNT_mRETWEET1_map_0*>(COUNT_mRETWEET1.index[1]);
          HashIndex_COUNT_mRETWEET1_map_0::IdxNode* n3 = &(i3->buckets_[h3 % i3->size_]);
          COUNT_mRETWEET1_entry* e3;
          do if ((e3=n3->obj) && h3 == n3->hash && COUNT_mRETWEET1_mapkey0_idxfn::equals(se7, *e3)) {
            long f_user_id = e3->F_USER_ID;
            long t_user_id = e3->T_USER_ID;
            STRING_TYPE t_tweet_date = e3->T_TWEET_DATE;
            long v3 = e3->__av;
//		void useDiffQ3(long a_user_id, STRING_TYPE& tweet_date, long t_user_id, long t_tweet_id, long r_tweet_id, STRING_TYPE& r_tweet_date, long retweet_tweet_id){}
	//	api.useDiffQ3(f_user_id, t_tweet_date, t_user_id, retweet_tweet_id, t_user_id, retweet_tweet_date, retweet_retweet_tweet_id);
            COUNT.addOrDelOnZero(se6.modify(f_user_id,t_user_id,retweet_retweet_tweet_id,t_tweet_date,retweet_retweet_tweet_id,retweet_tweet_id,retweet_tweet_date),(v3 * -1L));
          } while ((n3=n3->nxt));
        }
        COUNT_mTWEET2.addOrDelOnZero(se8.modify(retweet_retweet_tweet_id,retweet_tweet_date,retweet_tweet_id),-1L);
        { //slice
          const HASH_RES_t h4 = COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::hash(se10.modify1(retweet_retweet_tweet_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_1* i4 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_1*>(COUNT_mRETWEET1_mUSERS1.index[1]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_1::IdxNode* n4 = &(i4->buckets_[h4 % i4->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e4;
          do if ((e4=n4->obj) && h4 == n4->hash && COUNT_mRETWEET1_mUSERS1_mapkey1_idxfn::equals(se10, *e4)) {
            long count_musersusers_user_id = e4->COUNT_mRETWEET1_mUSERSUSERS_USER_ID;
            STRING_TYPE t_tweet_date = e4->T_TWEET_DATE;
            long v4 = e4->__av;
            COUNT_mUSERS1.addOrDelOnZero(se9.modify(count_musersusers_user_id,retweet_retweet_tweet_id,retweet_retweet_tweet_id,t_tweet_date,retweet_tweet_id,retweet_tweet_date),(v4 * -1L));
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
        { //slice
          const HASH_RES_t h5 = COUNT_mTWEET2_mapkey0_idxfn::hash(se13.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_map_0* i5 = static_cast<HashIndex_COUNT_mTWEET2_map_0*>(COUNT_mTWEET2.index[1]);
          HashIndex_COUNT_mTWEET2_map_0::IdxNode* n5 = &(i5->buckets_[h5 % i5->size_]);
          COUNT_mTWEET2_entry* e5;
          do if ((e5=n5->obj) && h5 == n5->hash && COUNT_mTWEET2_mapkey0_idxfn::equals(se13, *e5)) {
            STRING_TYPE r_tweet_date = e5->R_TWEET_DATE;
            long r_tweet_id = e5->R_TWEET_ID;
            long v5 = e5->__av;
		api.useDiffQ3(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,tweet_tweet_id,r_tweet_id,r_tweet_date);
            //COUNT.addOrDelOnZero(se11.modify(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,tweet_tweet_id,r_tweet_id,r_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se12.modify(tweet_user_id)) * v5));
          } while ((n5=n5->nxt));
        }
        COUNT_mRETWEET1.addOrDelOnZero(se14.modify(tweet_tweet_id,tweet_user_id,tweet_user_id,tweet_tweet_date),COUNT_mTWEET1.getValueOrDefault(se15.modify(tweet_user_id)));
        COUNT_mRETWEET1_mUSERS1.addOrDelOnZero(se16.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),1L);
        { //slice
          const HASH_RES_t h6 = COUNT_mTWEET2_mapkey0_idxfn::hash(se18.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_map_0* i6 = static_cast<HashIndex_COUNT_mTWEET2_map_0*>(COUNT_mTWEET2.index[1]);
          HashIndex_COUNT_mTWEET2_map_0::IdxNode* n6 = &(i6->buckets_[h6 % i6->size_]);
          COUNT_mTWEET2_entry* e6;
          do if ((e6=n6->obj) && h6 == n6->hash && COUNT_mTWEET2_mapkey0_idxfn::equals(se18, *e6)) {
            STRING_TYPE r_tweet_date = e6->R_TWEET_DATE;
            long r_tweet_id = e6->R_TWEET_ID;
            long v6 = e6->__av;
            COUNT_mUSERS1.addOrDelOnZero(se17.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),v6);
          } while ((n6=n6->nxt));
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
          const HASH_RES_t h7 = COUNT_mTWEET2_mapkey0_idxfn::hash(se21.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_map_0* i7 = static_cast<HashIndex_COUNT_mTWEET2_map_0*>(COUNT_mTWEET2.index[1]);
          HashIndex_COUNT_mTWEET2_map_0::IdxNode* n7 = &(i7->buckets_[h7 % i7->size_]);
          COUNT_mTWEET2_entry* e7;
          do if ((e7=n7->obj) && h7 == n7->hash && COUNT_mTWEET2_mapkey0_idxfn::equals(se21, *e7)) {
            STRING_TYPE r_tweet_date = e7->R_TWEET_DATE;
            long r_tweet_id = e7->R_TWEET_ID;
            long v7 = e7->__av;
            COUNT.addOrDelOnZero(se19.modify(tweet_user_id,tweet_user_id,tweet_tweet_id,tweet_tweet_date,tweet_tweet_id,r_tweet_id,r_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se20.modify(tweet_user_id)) * (v7 * -1L)));
          } while ((n7=n7->nxt));
        }
        COUNT_mRETWEET1.addOrDelOnZero(se22.modify(tweet_tweet_id,tweet_user_id,tweet_user_id,tweet_tweet_date),(COUNT_mTWEET1.getValueOrDefault(se23.modify(tweet_user_id)) * -1L));
        COUNT_mRETWEET1_mUSERS1.addOrDelOnZero(se24.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_date),-1L);
        { //slice
          const HASH_RES_t h8 = COUNT_mTWEET2_mapkey0_idxfn::hash(se26.modify0(tweet_tweet_id));
          const HashIndex_COUNT_mTWEET2_map_0* i8 = static_cast<HashIndex_COUNT_mTWEET2_map_0*>(COUNT_mTWEET2.index[1]);
          HashIndex_COUNT_mTWEET2_map_0::IdxNode* n8 = &(i8->buckets_[h8 % i8->size_]);
          COUNT_mTWEET2_entry* e8;
          do if ((e8=n8->obj) && h8 == n8->hash && COUNT_mTWEET2_mapkey0_idxfn::equals(se26, *e8)) {
            STRING_TYPE r_tweet_date = e8->R_TWEET_DATE;
            long r_tweet_id = e8->R_TWEET_ID;
            long v8 = e8->__av;
            COUNT_mUSERS1.addOrDelOnZero(se25.modify(tweet_user_id,tweet_tweet_id,tweet_tweet_id,tweet_tweet_date,r_tweet_id,r_tweet_date),(v8 * -1L));
          } while ((n8=n8->nxt));
        }
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
        { //slice
          const HASH_RES_t h9 = COUNT_mUSERS1_mapkey0_idxfn::hash(se28.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS1_map_0* i9 = static_cast<HashIndex_COUNT_mUSERS1_map_0*>(COUNT_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS1_map_0::IdxNode* n9 = &(i9->buckets_[h9 % i9->size_]);
          COUNT_mUSERS1_entry* e9;
          do if ((e9=n9->obj) && h9 == n9->hash && COUNT_mUSERS1_mapkey0_idxfn::equals(se28, *e9)) {
            long b_tweet_id = e9->B_TWEET_ID;
            long c_retweet_tweet_id = e9->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e9->T_TWEET_DATE;
            long r_tweet_id = e9->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e9->R_TWEET_DATE;
            long v9 = e9->__av;
            COUNT.addOrDelOnZero(se27.modify(users_user_id,users_user_id,b_tweet_id,t_tweet_date,c_retweet_tweet_id,r_tweet_id,r_tweet_date),v9);
          } while ((n9=n9->nxt));
        }
        { //slice
          const HASH_RES_t h10 = COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::hash(se30.modify0(users_user_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_0* i10 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_0*>(COUNT_mRETWEET1_mUSERS1.index[2]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_0::IdxNode* n10 = &(i10->buckets_[h10 % i10->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e10;
          do if ((e10=n10->obj) && h10 == n10->hash && COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::equals(se30, *e10)) {
            long count_mretweetretweet_retweet_tweet_id = e10->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e10->T_TWEET_DATE;
            long v10 = e10->__av;
            COUNT_mRETWEET1.addOrDelOnZero(se29.modify(count_mretweetretweet_retweet_tweet_id,users_user_id,users_user_id,t_tweet_date),v10);
          } while ((n10=n10->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se31.modify(users_user_id),1L);
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
        { //slice
          const HASH_RES_t h11 = COUNT_mUSERS1_mapkey0_idxfn::hash(se33.modify0(users_user_id));
          const HashIndex_COUNT_mUSERS1_map_0* i11 = static_cast<HashIndex_COUNT_mUSERS1_map_0*>(COUNT_mUSERS1.index[1]);
          HashIndex_COUNT_mUSERS1_map_0::IdxNode* n11 = &(i11->buckets_[h11 % i11->size_]);
          COUNT_mUSERS1_entry* e11;
          do if ((e11=n11->obj) && h11 == n11->hash && COUNT_mUSERS1_mapkey0_idxfn::equals(se33, *e11)) {
            long b_tweet_id = e11->B_TWEET_ID;
            long c_retweet_tweet_id = e11->C_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e11->T_TWEET_DATE;
            long r_tweet_id = e11->R_TWEET_ID;
            STRING_TYPE r_tweet_date = e11->R_TWEET_DATE;
            long v11 = e11->__av;
            COUNT.addOrDelOnZero(se32.modify(users_user_id,users_user_id,b_tweet_id,t_tweet_date,c_retweet_tweet_id,r_tweet_id,r_tweet_date),(v11 * -1L));
          } while ((n11=n11->nxt));
        }
        { //slice
          const HASH_RES_t h12 = COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::hash(se35.modify0(users_user_id));
          const HashIndex_COUNT_mRETWEET1_mUSERS1_map_0* i12 = static_cast<HashIndex_COUNT_mRETWEET1_mUSERS1_map_0*>(COUNT_mRETWEET1_mUSERS1.index[2]);
          HashIndex_COUNT_mRETWEET1_mUSERS1_map_0::IdxNode* n12 = &(i12->buckets_[h12 % i12->size_]);
          COUNT_mRETWEET1_mUSERS1_entry* e12;
          do if ((e12=n12->obj) && h12 == n12->hash && COUNT_mRETWEET1_mUSERS1_mapkey0_idxfn::equals(se35, *e12)) {
            long count_mretweetretweet_retweet_tweet_id = e12->COUNT_mRETWEETRETWEET_RETWEET_TWEET_ID;
            STRING_TYPE t_tweet_date = e12->T_TWEET_DATE;
            long v12 = e12->__av;
            COUNT_mRETWEET1.addOrDelOnZero(se34.modify(count_mretweetretweet_retweet_tweet_id,users_user_id,users_user_id,t_tweet_date),(v12 * -1L));
          } while ((n12=n12->nxt));
        }
        COUNT_mTWEET1.addOrDelOnZero(se36.modify(users_user_id),-1L);
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
    COUNT_mRETWEET1_entry se2;
    COUNT_mTWEET2_entry se3;
    COUNT_mUSERS1_entry se4;
    COUNT_mRETWEET1_mUSERS1_entry se5;
    COUNT_entry se6;
    COUNT_mRETWEET1_entry se7;
    COUNT_mTWEET2_entry se8;
    COUNT_mUSERS1_entry se9;
    COUNT_mRETWEET1_mUSERS1_entry se10;
    COUNT_entry se11;
    COUNT_mTWEET1_entry se12;
    COUNT_mTWEET2_entry se13;
    COUNT_mRETWEET1_entry se14;
    COUNT_mTWEET1_entry se15;
    COUNT_mRETWEET1_mUSERS1_entry se16;
    COUNT_mUSERS1_entry se17;
    COUNT_mTWEET2_entry se18;
    COUNT_entry se19;
    COUNT_mTWEET1_entry se20;
    COUNT_mTWEET2_entry se21;
    COUNT_mRETWEET1_entry se22;
    COUNT_mTWEET1_entry se23;
    COUNT_mRETWEET1_mUSERS1_entry se24;
    COUNT_mUSERS1_entry se25;
    COUNT_mTWEET2_entry se26;
    COUNT_entry se27;
    COUNT_mUSERS1_entry se28;
    COUNT_mRETWEET1_entry se29;
    COUNT_mRETWEET1_mUSERS1_entry se30;
    COUNT_mTWEET1_entry se31;
    COUNT_entry se32;
    COUNT_mUSERS1_entry se33;
    COUNT_mRETWEET1_entry se34;
    COUNT_mRETWEET1_mUSERS1_entry se35;
    COUNT_mTWEET1_entry se36;
  
    /* Data structures used for storing materialized views */
    COUNT_mRETWEET1_map COUNT_mRETWEET1;
    COUNT_mRETWEET1_mUSERS1_map COUNT_mRETWEET1_mUSERS1;
    COUNT_mTWEET1_map COUNT_mTWEET1;
    COUNT_mTWEET2_map COUNT_mTWEET2;
    COUNT_mUSERS1_map COUNT_mUSERS1;
    
    
  
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
  class Q3 : public Program
  {
    public:
      Q3(int argc = 0, char* argv[] = 0) : Program(argc,argv) {
      }
  };

}

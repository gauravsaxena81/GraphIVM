#ifndef DBTOASTER_STANDARD_FUNCTIONS_H
#define DBTOASTER_STANDARD_FUNCTIONS_H

#include <cstdlib>
#include <string>
using namespace std;
#include "hpds/pstring.hpp"
#include "hpds/KDouble.hpp"

#include "event.hpp"
#include <regex.h>

namespace dbtoaster {

  // Date extraction functions
  // ImperativeCompiler synthesizes calls to the following from calls to 
  // date_part
  long Uyear_part(date d);
  long Umonth_part(date d);
  long Uday_part(date d);
  
  // String functions
  STRING_TYPE Usubstring(const STRING_TYPE &s, long start, long len);
  int Uregexp_match(const char *regex, const STRING_TYPE &s);
  int Upreg_match(const regex_t &preg, const STRING_TYPE &s);
  
  // Vector functions
  DOUBLE_TYPE Uvec_dot(DOUBLE_TYPE x1, DOUBLE_TYPE y1, DOUBLE_TYPE z1, 
              DOUBLE_TYPE x2, DOUBLE_TYPE y2, DOUBLE_TYPE z2);
  DOUBLE_TYPE Uvec_length(DOUBLE_TYPE x1, DOUBLE_TYPE y1, DOUBLE_TYPE z1);
  DOUBLE_TYPE Uvector_angle(DOUBLE_TYPE x1, DOUBLE_TYPE y1, DOUBLE_TYPE z1, 
              DOUBLE_TYPE x2, DOUBLE_TYPE y2, DOUBLE_TYPE z2);
  DOUBLE_TYPE Udihedral_angle(DOUBLE_TYPE x1, DOUBLE_TYPE y1, DOUBLE_TYPE z1, 
                    DOUBLE_TYPE x2, DOUBLE_TYPE y2, DOUBLE_TYPE z2,
                    DOUBLE_TYPE x3, DOUBLE_TYPE y3, DOUBLE_TYPE z3,
                    DOUBLE_TYPE x4, DOUBLE_TYPE y4, DOUBLE_TYPE z4);

  long long Uhash(long long v);
  
  // Math funtions
  DOUBLE_TYPE Uradians(DOUBLE_TYPE degree);
  DOUBLE_TYPE Udegrees(DOUBLE_TYPE radian);
  DOUBLE_TYPE Upow(DOUBLE_TYPE a, DOUBLE_TYPE b);
/*  DOUBLE_TYPE pow(DOUBLE_TYPE a, int b);
  DOUBLE_TYPE pow(int a, DOUBLE_TYPE b);
  DOUBLE_TYPE pow(int a, int b);
 */
  long UmulLng(long x, long y);
  long Ulistmax(long v1,  long v2);

  DOUBLE_TYPE Udiv(DOUBLE_TYPE x);
  DOUBLE_TYPE UmulDbl(DOUBLE_TYPE x, DOUBLE_TYPE y);
  DOUBLE_TYPE Ulistmax(DOUBLE_TYPE v1, DOUBLE_TYPE v2);
  DOUBLE_TYPE Ulistmax(long v1, DOUBLE_TYPE v2);
  DOUBLE_TYPE Ulistmax(DOUBLE_TYPE v1, long v2);
  
  // Type conversion functions
  inline long cast_int_from_float(double           d) { return (long)d; };
  inline long cast_int_from_string(const char     *c) { return atoi(c); };
  inline long cast_int_from_string(STRING_TYPE         &s) { 
    return cast_int_from_string(s.c_str()); 
  };
  inline double cast_float_from_int(long           i) { return (double)i; };
  inline double cast_float_from_string(const char *c) { return atof(c); };
  inline double cast_float_from_string(STRING_TYPE     &s) { 
    return cast_float_from_string(s.c_str()); 
  };
  
  template <class T> 
  STRING_TYPE cast_string(const T &t);
  
  inline STRING_TYPE cast_string_from_int(long      i) { return cast_string(i); }
  inline STRING_TYPE cast_string_from_double(double d) { return cast_string(d); }
  STRING_TYPE cast_string_from_date(date ymd);
  date Udate(const char *c);
  inline date Udate(const string &s) { 
    return Udate(s.c_str()); 
  }
  inline date Udate(const PString &s) { 
    return Udate(s.c_str()); 
  }
  
}

#endif //DBTOASTER_STANDARD_FUNCTIONS_H

#include <stdio.h>
#include "milieu.h"
#include "softfloat.h"

#define IEEE_8087 1
#define NO_LONG_LONG 1

#define double float64
#define ftostr(_d,_a) { \
  reallocMsg((void**)_d, 70); \
  dtoa((*(_d), _a); \
  reallocMsg((void**)_d, strlen(*(_d)) + 1); \
  } /* the _ftostr function in cc65-floatlib seems to output at
  most 64 characters */
#define fadd(_f,_a) float64_add((_f),(_a))
#define fsub(_f,_a) float64_sub((_f),(_a))
#define fmul(_f,_a) float64_mul((_f),(_a))
#define fdiv(_f,_a) float64_div((_f),(_a))
#define fcmp(_d,_s) float64_eq((_d),(_s))
#define ctof(_s) int32_to_float64(_s)
#define fneg(_f) float64_mul((_f), minusOne)
#define isnan(_f) float64_is_nan(_f)



int main(int arc, char* argv[]) {
  double minusOne = ctof(-1);
  double a = ctof(3);
  double b = ctof(10);
  double c = fdiv(a,b);
  double d = fadd(ctof(7),c);
  
  char e = NULL;
  
  ftostr(e, fdiv(d, a));
  
  printf("%s", e);
  
  free(e);

  return 0;
}

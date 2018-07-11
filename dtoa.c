#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SoftFloat-2c/softfloat/bits32/386-Win32-GCC/milieu.h"
#include "macros.h"
#include "SoftFloat-2c/softfloat/bits32/386-Win32-GCC/softfloat.h"

extern double fltMinusOne;
extern double fltZero;
extern double fltOne;
extern double fltTen;
extern int fltNotInited;

static int MAX_NUMBER_STRING_SIZE = 128;

char digitLookup[11] = "0123456789";

double mystrtod(const char *str, char **end);
/**
 * Double to ASCII
 */
char* mydtoa(char *s, double n) {
  char *c;
  char *c2;
  int sign, m, m1, carry = 0;
  int useExp, output = 0, decimalPoint = 0;
  double weight, digit;
  int i, j;

  if(fltNotInited) {
    initfp();
  }

  /* handle special cases */
  if(isnan(n)) {
    strcpy(s, "NaN");
    return s;
  }

  if(isinf(n)) {
    strcpy(s, "Infinity");
    return s;
  }

  if(feq(n, fltZero)) {
    strcpy(s, "0");
    return s;
  }

  c = s;
  sign = 0;

  if(!fgt(n, fltZero)) {
    n = fmul(n, fltMinusOne);
    sign = 1;
    *(c++) = '-';
    s++;
  }

  // calculate magnitude
  m = ftoc(floor(log10a(n)));
  m1 = abs(m);
  useExp = m1 >= 9 && (sign || m1 >= 14);

  // set up for scientific notation
  if(useExp) {
    n = fdiv(n, pow10a(m));
    m1 = m;
    m = 0;
  }
  else if(m < 0) {
    m = 0;
  }

  /* convert the number */
  while(fgt(n, fltZero) || m >= 0) {
    weight = pow10a(m);

    if(fgt(weight, fltZero)) {
      digit = floor(fdiv(n, weight));
      if(output > 13 && m < 0) {
        *c = '\0';
        carry = ftoc(digit) > 4;
        break;
      }

      n = fsub(n, fmul(digit, weight));
      if(output || fgt(digit, fltZero) || m == 0) {
        *(c++) = digitLookup[ftoc(digit)];
        output++;
      }
    }

    if(m == 0) {
      *(c++) = '.';
      decimalPoint = 1;
    }

    m--;
  }

  /* do any rounding needed on the characters directly */
  if(carry) {
    c2 = c;
    c--;

    while(carry && c != s) {
      if(*c != '.') {
        if(*c == '9') {
          *c = '0';
        }
        else {
          *c = digitLookup[strchr(digitLookup, *c)-digitLookup+1];
          carry = 0;
        }
      }

      c--;
    }

    if(carry) {
      if(*c == '9') {
        *c = '0';
        memmove((void*) c+1, (void*)c, strlen(c)+1);
        *c = '1';
      }
      else {
        *c = digitLookup[strchr(digitLookup, *c)-digitLookup+1];
      }
    }

    c = c2;
  }

  /* remove trailing zeros, with the decimal point as well if need be */
  if(decimalPoint) {
    c--;
    for(;;) {
      if(*c == '0') {
        *(c--) = 0;
        continue;
      }

      if(*c == '.') {
        *(c--) = 0;
      }

      break;
    }
    c++;
  }

  if(useExp) {
    /* convert the exponent */
    *(c++) = 'e';

    if(m1 >= 0) {
      *(c++) = '+';
    }
    else {
      *(c++) = '-';
      m1 = -m1;
    }

    m = 0;

    while(m1 > 0) {
      *(c++) = digitLookup[m1 % 10];
      m1 /= 10;
      m++;
    }

    c -= m;

    for(i = 0, j = m-1; i<j; i++, j--) {
      /* swap without temporary */
      c[i] ^= c[j];
      c[j] ^= c[i];
      c[i] ^= c[j];
    }

    c += m;
  }

  *c = '\0';

  if(sign) {
    s--;
  }

  return s;
}

int main(int argc, char** argv) {
  int i;
  char s[MAX_NUMBER_STRING_SIZE];

  char* d[] = {
    "0.0",
    "0.15",
    "0.3",
    "0.97",
    "0.34",
    "0.99",
    "0.7",
    "1.8",
    "42.0",
    "-12.5",
    "-.005",
    "100000",
    "10000000000000000",
    "2.2250738585072011e-308",
    "2.2250738585072012e-308",
    "1.333333333333333333333333333333333333",
    "1234567.890123456",
    "0.000000000000018",
    "555555.55555555555555555",
    "-888888888888888.8888888",
    "111111111111111111111111.2222222222",
    "5497558138888.551111111111111",
    "54975581388889.551111111111111",
    "899999999.9999999",
    "989999999.9999999",
    "998999999.9999999",
    "999999999.9999999",
    "9999999999.999999",
    "99999999999.99999",
    "999999999999.9999",
    "9999999999999.999",
    "99999999999999.99",
    "999999999999999.9",
    "999999998999.99",
    "9999999989999.99",
    "99999999989999.99",
    "999999998999999.99",
    "-9999999999999.99",
    "1.000000000001",
    "1e23",
    "1.3",
    "-1.000000555555555555555e+13",
    "1.333333328366279602",
    "0.1",
    "3.14159265352"
  };

  for(i = 0; i < 45; i++) {
    printf("%d: %s, printf: dtoa: %s\n",
    i+1,
    d[i],
    mydtoa(s, mystrtod(d[i], NULL)));
  }
}

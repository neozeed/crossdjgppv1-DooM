/* More subroutines needed by GCC output code on some machines.  */
/* Compile this one with gcc.  */


#include <stddef.h>
#define BITS_PER_UNIT 8

#ifndef SItype
#define SItype long int
#endif
  struct longlong {long low, high;};

typedef union
{
  struct longlong s;
  long long ll;
  SItype i[2];
  unsigned SItype ui[2];
} long_long;


#define LOW 0
#define HIGH 1
#define B 0x10000
#define low16 (B - 1)



long long
__ashrdi3 (u, b1)
     long long u;
     long long b1;
{
  long_long w;
  unsigned long carries;
  int bm;
  long_long uu;
  int b = b1;

  if (b == 0)
    return u;

  uu.ll = u;

  bm = (sizeof (int) * BITS_PER_UNIT) - b;
  if (bm <= 0)
    {
      w.s.high = uu.s.high >> 31; /* just to make w.s.high 1..1 or 0..0 */
      w.s.low = uu.s.high >> -bm;
    }
  else
    {
      carries = (unsigned long)uu.s.high << bm;
      w.s.high = uu.s.high >> b;
      w.s.low = ((unsigned long)uu.s.low >> b) | carries;
    }

  return w.ll;
}


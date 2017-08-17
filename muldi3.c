/* More subroutines needed by GCC output code on some machines.  */
/* Compile this one with gcc.  */


#include <stddef.h>



void bzero (void *b, size_t length)
{
    memset((void *) b, 0, (size_t) length);
}


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
#define big_end(n)	((n) - 1)
#define little_end(n)	0 
#define next_msd(i)	((i) + 1)
#define next_lsd(i)	((i) - 1)
#define is_not_msd(i,n)	((i) < (n))
#define is_not_lsd(i,n)	((i) >= 0)


#define B 0x10000
#define low16 (B - 1)


static void bmul (unsigned short *a,unsigned short *b,unsigned short *c, size_t m,size_t n);


long long 
__muldi3 (u, v)
     long long u, v;
{
  long a[2], b[2], c[2][2];
  long_long w;
  long_long uu, vv;

  uu.ll = u;
  vv.ll = v;

  a[HIGH] = uu.s.high;
  a[LOW] = uu.s.low;
  b[HIGH] = vv.s.high;
  b[LOW] = vv.s.low;

  bmul (a, b, c, sizeof a, sizeof b);

  w.s.high = c[LOW][HIGH];
  w.s.low = c[LOW][LOW];
  return w.ll;
}

static void 
bmul (a, b, c, m, n)
    unsigned short *a, *b, *c;
    size_t m, n;
{
  int i, j;
  unsigned long acc;

  bzero (c, m + n);

  m /= sizeof *a;
  n /= sizeof *b;

  for (j = little_end (n); is_not_msd (j, n); j = next_msd (j))
    {
      unsigned short *c1 = c + j + little_end (2);
      acc = 0;
      for (i = little_end (m); is_not_msd (i, m); i = next_msd (i))
	{
	  /* Widen before arithmetic to avoid loss of high bits.  */
	  acc += (unsigned long) a[i] * b[j] + c1[i];
	  c1[i] = acc & low16;
	  acc = acc >> 16;
	}
      c1[i] = acc;
    }
}

#define BITS_PER_UNIT 8
#define CHAR_TYPE_SIZE BITS_PER_UNIT
#define SItype long int
struct longlong {long low, high;};

typedef union
{
  struct longlong s;
  long long ll;
  SItype i[2];
  unsigned SItype ui[2];
} long_long;

long long
__ashldi3 (u, b1)
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
      w.s.low = 0;
      w.s.high = (unsigned long)uu.s.low << -bm;
    }
  else
    {
      carries = (unsigned long)uu.s.low >> bm;
      w.s.low = (unsigned long)uu.s.low << b;
      w.s.high = ((unsigned long)uu.s.high << b) | carries;
    }

  return w.ll;
}
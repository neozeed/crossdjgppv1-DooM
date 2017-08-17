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

SItype
__cmpdi2 (a, b)
     long long a, b;
{
  long_long au, bu;

  au.ll = a, bu.ll = b;

  if (au.s.high < bu.s.high)
    return 0;
  else if (au.s.high > bu.s.high)
    return 2;
  if ((unsigned) au.s.low < (unsigned) bu.s.low)
    return 0;
  else if ((unsigned) au.s.low > (unsigned) bu.s.low)
    return 2;
  return 1;
}
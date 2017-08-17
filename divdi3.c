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
__divdi3 (u, v)
     long long u, v;
{
  if (u < 0)
    if (v < 0)
      return (unsigned long long) -u / (unsigned long long) -v;
    else
      return - ((unsigned long long) -u / (unsigned long long) v);
  else
    if (v < 0)
      return - ((unsigned long long) u / (unsigned long long) -v);
    else
      return (unsigned long long) u / (unsigned long long) v;
}
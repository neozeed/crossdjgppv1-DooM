#include <stdio.h>
#include <malloc.h>

void *xmalloc(size_t size)
{
  void *rv = malloc(size);
  if (rv == 0)
  {
    fprintf(stderr, "Fatal: malloc returned NULL\n");
    exit(1);
  }
  return rv;
}

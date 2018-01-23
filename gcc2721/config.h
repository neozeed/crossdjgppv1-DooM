#include "i386/xm-i386.h"

/* Inhibit cccp.c's definition of putenv.  */
#define HAVE_PUTENV

/* Use semicolons to separate elements of a path.  */
#define PATH_SEPARATOR ';'

/* Use backslashs to separate levels of directory.  */
#define DIR_SEPARATOR '\\'

/* Suffix for executable file names.  */
#define EXECUTABLE_SUFFIX ".exe"

#define MKTEMP_EACH_FILE 1

#define NO_PRECOMPILES 1

/* sys_errlist proto in cccp.c doesn't match djgpp */
#define HAVE_STRERROR

#define bzero(a,b) memset (a,0,b)
#define bcopy(a,b,c) memcpy (b,a,c)
#define bcmp(a,b,c) memcmp (a,b,c)
#define index strchr
#define rindex strrchr
#if !defined(LOGHEADERFILE)
#define LOGHEADERFILE

#include <stdio.h>
#include<string.h>
#include <errno.h>
#include <time.h>


#define log(message) fprintf(stderr, "%d: %s %s\n", errno, strerror(errno), message);
void printTime();
#endif // LOGHEADERFILE

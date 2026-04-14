#ifndef PRELUDE_C_PCH_NIMDKONH
#define PRELUDE_C_PCH_NIMDKONH

#if defined(__APPLE__)
#import <AvailabilityMacros.h>
#endif
#import <sys/types.h>

#import <aio.h>
#import <assert.h>
#if defined(__APPLE__)
#import <copyfile.h>
#endif
#import <ctype.h>
#import <curl/curl.h>
#import <dirent.h>
#import <errno.h>
#import <execinfo.h> // stack trace for debug
#import <fcntl.h>
#import <float.h>
#import <fnmatch.h>
#import <getopt.h>
#import <glob.h>
#import <iconv.h>
#import <libgen.h>
#if defined(__APPLE__)
#import <malloc/malloc.h>
#else
#import <malloc.h>
#endif
#import <math.h>
#import <netdb.h>
#import <netinet/in.h>
#import <pthread.h>
#import <pwd.h>
#import <signal.h>
#import <spawn.h>
#import <stdarg.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/errno.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#import <sys/event.h>       /* kqueue — absent on Linux */
#endif
#import <sys/ioctl.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#import <sys/mount.h>
#endif
#import <sys/param.h>
#import <sys/socket.h>
#import <sys/stat.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#import <sys/sysctl.h>
#endif
#import <sys/time.h>
#import <sys/times.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#import <sys/ucred.h>
#endif
#import <sys/un.h>
#import <sys/wait.h>
#if defined(__APPLE__)
#import <sys/xattr.h>
#else
#import <sys/xattr.h>       /* Linux has a different but present variant */
#endif
#import <sysexits.h>
#import <unistd.h>
#import <uuid/uuid.h>

#import <zlib.h>

#endif /* end of include guard: PRELUDE_C_PCH_NIMDKONH */

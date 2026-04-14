#ifndef PRELUDE_GNUSTEP_H_TM_PORT
#define PRELUDE_GNUSTEP_H_TM_PORT

/*
 * GNUstep / Linux / FreeBSD equivalent of prelude-mac.h.
 * Uses real CoreFoundation from libs-corebase and shims the macOS-only bits
 * TextMate's prelude pulls in.
 */

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>
#include <dispatch/dispatch.h>

/* Carbon OSStatus / OSErr types (most are in CFBase already, but a couple of
 * older Carbon aliases TextMate uses via oak/compat.h are not). */
#include <stdint.h>
#ifndef noErr
#define noErr ((OSStatus)0)
#endif
typedef int16_t OSErr;
typedef uint32_t OSType;

/* os_log / os_activity — map to stderr ------------------------------------- */
#include <stdio.h>
#ifndef os_log
/* Expression-form so call sites like `os_log_error(...), false` still parse. */
#define os_log(log, fmt, ...)         ((void)(log), (void)fprintf(stderr, fmt "\n", ##__VA_ARGS__))
#define os_log_info(log, fmt, ...)    os_log(log, fmt, ##__VA_ARGS__)
#define os_log_debug(log, fmt, ...)   ((void)(log))
#define os_log_error(log, fmt, ...)   os_log(log, fmt, ##__VA_ARGS__)
#define os_log_fault(log, fmt, ...)   os_log(log, fmt, ##__VA_ARGS__)
#define OS_LOG_DEFAULT                ((void*)0)
typedef void* os_log_t;
#endif

/* OSAtomic — use GCC builtins ---------------------------------------------- */
static inline int32_t OSAtomicIncrement32Barrier(volatile int32_t *p) { return __sync_add_and_fetch(p, 1); }
static inline int32_t OSAtomicDecrement32Barrier(volatile int32_t *p) { return __sync_sub_and_fetch(p, 1); }
static inline int32_t OSAtomicAdd32Barrier(int32_t amt, volatile int32_t *p) { return __sync_add_and_fetch(p, amt); }

/* nil — ObjC keyword used in .cc files by macOS idioms. Alias to nullptr. -- */
#if !defined(__OBJC__)
#ifndef nil
#define nil nullptr
#endif
#endif

/* fcntl F_GETPATH — macOS extension; absent on Linux. Map to sentinel so
 * fcntl() returns -1 (EINVAL) and callers fall back to their slow path.   -- */
#include <fcntl.h>
#ifndef F_GETPATH
#define F_GETPATH (-1)
#endif
#ifndef F_NOCACHE
#define F_NOCACHE (-1)   /* macOS-only; fcntl fails cleanly */
#endif

/* stat st_mtimespec/st_ctimespec — macOS names; Linux uses st_mtim/st_ctim -- */
#if !defined(__APPLE__)
#include <sys/stat.h>
#ifndef st_mtimespec
#define st_mtimespec st_mtim
#define st_atimespec st_atim
#define st_ctimespec st_ctim
#endif
#endif

/* xattr — macOS signatures take extra (position, options) args; Linux lacks them.
 * Provide inline wrappers that map the Mac-style call to Linux getxattr/setxattr. */
#if !defined(__APPLE__)
#include <sys/xattr.h>
static inline ssize_t _tm_getxattr(const char* path, const char* name, void* value, size_t size, uint32_t position, int options) {
  (void)position; (void)options; return ::getxattr(path, name, value, size);
}
static inline int _tm_setxattr(const char* path, const char* name, const void* value, size_t size, uint32_t position, int options) {
  (void)position; (void)options; return ::setxattr(path, name, value, size, 0);
}
static inline int _tm_removexattr(const char* path, const char* name, int options) {
  (void)options; return ::removexattr(path, name);
}
#define getxattr     _tm_getxattr
#define setxattr     _tm_setxattr
#define removexattr  _tm_removexattr

static inline ssize_t _tm_flistxattr(int fd, char* list, size_t size, int options) {
  (void)options; return ::flistxattr(fd, list, size);
}
static inline ssize_t _tm_fgetxattr(int fd, const char* name, void* value, size_t size, uint32_t position, int options) {
  (void)position; (void)options; return ::fgetxattr(fd, name, value, size);
}
static inline int _tm_fsetxattr(int fd, const char* name, const void* value, size_t size, uint32_t position, int options) {
  (void)position; (void)options; return ::fsetxattr(fd, name, value, size, 0);
}
static inline int _tm_fremovexattr(int fd, const char* name, int options) {
  (void)options; return ::fremovexattr(fd, name);
}
#define flistxattr   _tm_flistxattr
#define fgetxattr    _tm_fgetxattr
#define fsetxattr    _tm_fsetxattr
#define fremovexattr _tm_fremovexattr
#endif

/* malloc_size — macOS; glibc has malloc_usable_size --------------------- */
#include <malloc.h>
static inline size_t malloc_size(const void* p) { return malloc_usable_size((void*)p); }

/* crt_externs.h — see Shared/include/shims/crt_externs.h ------------------ */
#if !defined(__APPLE__)
#include <crt_externs.h>
#include <CFPreferencesShim.h>
#include <FSEventsShim.h>
#endif

/* posix_spawn — Darwin extends with POSIX_SPAWN_CLOEXEC_DEFAULT.
 * 0 = no-op on Linux; caller must explicitly close FDs or use O_CLOEXEC.   */
#ifndef POSIX_SPAWN_CLOEXEC_DEFAULT
#define POSIX_SPAWN_CLOEXEC_DEFAULT 0
#endif

/* copyfile(3) — Darwin only. Minimal shim: plain copy via read/write loop.
 * Flags are ignored (callers use COPYFILE_ALL, COPYFILE_MOVE, COPYFILE_UNLINK,
 * COPYFILE_NOFOLLOW_SRC). For FreeBSD we could later upgrade to sendfile. */
#if !defined(__APPLE__)
#define COPYFILE_ACL            (1<<0)
#define COPYFILE_STAT           (1<<1)
#define COPYFILE_XATTR          (1<<2)
#define COPYFILE_DATA           (1<<3)
#define COPYFILE_SECURITY       (COPYFILE_STAT | COPYFILE_ACL)
#define COPYFILE_METADATA       (COPYFILE_SECURITY | COPYFILE_XATTR)
#define COPYFILE_ALL            (COPYFILE_METADATA | COPYFILE_DATA)
#define COPYFILE_NOFOLLOW_SRC   (1<<18)
#define COPYFILE_NOFOLLOW_DST   (1<<19)
#define COPYFILE_UNLINK         (1<<21)
#define COPYFILE_MOVE           (1<<25)
typedef void* copyfile_state_t;
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
static inline int copyfile(const char* from, const char* to, copyfile_state_t state, uint32_t flags) {
  (void)state;
  int openFlags = O_RDONLY | O_CLOEXEC;
  if(flags & COPYFILE_NOFOLLOW_SRC) openFlags |= O_NOFOLLOW;
  int in = ::open(from, openFlags);
  if(in < 0) return -1;
  struct stat st;
  if(::fstat(in, &st) != 0) { ::close(in); return -1; }
  int out = ::open(to, O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC, st.st_mode & 07777);
  if(out < 0) { ::close(in); return -1; }
  char buf[64*1024]; ssize_t n; int rc = 0;
  while((n = ::read(in, buf, sizeof(buf))) > 0) {
    if(::write(out, buf, n) != n) { rc = -1; break; }
  }
  if(n < 0) rc = -1;
  ::close(in); ::close(out);
  if(rc == 0 && (flags & COPYFILE_UNLINK)) { if(::unlink(from) != 0) rc = -1; }
  return rc;
}
#endif

/* CoreText alignment enum — some builds of libs-coretext omit this.
 * Use CTTextAlignment enum values (kCTNaturalTextAlignment is classic name). */
#ifndef kCTTextAlignmentNatural
#define kCTTextAlignmentNatural  kCTNaturalTextAlignment
#define kCTTextAlignmentLeft     kCTLeftTextAlignment
#define kCTTextAlignmentRight    kCTRightTextAlignment
#define kCTTextAlignmentCenter   kCTCenterTextAlignment
#define kCTTextAlignmentJustified kCTJustifiedTextAlignment
#endif

/* O_EVTONLY — Darwin; used by settings/track_paths.h. Map to O_RDONLY.   */
#ifndef O_EVTONLY
#define O_EVTONLY O_RDONLY
#endif

/* DISPATCH_SOURCE_TYPE_VNODE is unavailable on linux's libdispatch.
 * Provide a sentinel type object so references compile. Calls using it
 * will crash at runtime — real fix is an inotify-backed dispatch source. */
#if !defined(__APPLE__)
#ifdef DISPATCH_SOURCE_TYPE_VNODE
#undef DISPATCH_SOURCE_TYPE_VNODE
#endif
#ifdef __cplusplus
extern "C" const struct dispatch_source_type_s _fake_dispatch_source_type_vnode;
#else
extern const struct dispatch_source_type_s _fake_dispatch_source_type_vnode;
#endif
#define DISPATCH_SOURCE_TYPE_VNODE (&_fake_dispatch_source_type_vnode)
#define DISPATCH_VNODE_DELETE  0x1
#define DISPATCH_VNODE_WRITE   0x2
#define DISPATCH_VNODE_EXTEND  0x4
#define DISPATCH_VNODE_RENAME  0x8
#define DISPATCH_VNODE_REVOKE  0x10
#endif

/* ARG_MAX + misc limits — some macOS code expects these from sys/param.h  */
#include <limits.h>
#ifndef ARG_MAX
#define ARG_MAX 262144
#endif

/* uuid_string_t — Darwin typedef; Linux libuuid omits it. --------------- */
#include <uuid/uuid.h>
#ifndef uuid_string_t
typedef char uuid_string_t[37];
#endif

/* Security.framework AuthorizationRef — stubs so headers parse.
 * Runtime calls are routed to the oak::execute_with_privileges stub that
 * returns -1 (see oak/compat.h).                                          */
#if !defined(__APPLE__)
typedef struct AuthorizationOpaqueRef* AuthorizationRef;
typedef uint32_t AuthorizationFlags;
typedef struct { const char* name; uint32_t valueLength; void* value; uint32_t flags; } AuthorizationItem;
typedef struct { uint32_t count; AuthorizationItem* items; } AuthorizationItemSet;
typedef AuthorizationItemSet AuthorizationRights;
typedef AuthorizationItemSet AuthorizationEnvironment;
#define kAuthorizationFlagDefaults         0
#define kAuthorizationFlagInteractionAllowed (1<<0)
#define kAuthorizationFlagExtendRights       (1<<1)
#define kAuthorizationFlagPreAuthorize       (1<<4)
#define kAuthorizationEmptyEnvironment ((AuthorizationEnvironment*)0)
typedef struct { uint8_t bytes[32]; } AuthorizationExternalForm;
#define errAuthorizationSuccess              0
#define errAuthorizationDenied           -60005
#define errAuthorizationCanceled         -60006
#define errAuthorizationInteractionNotAllowed -60007
static inline OSStatus AuthorizationFreeItemSet(AuthorizationItemSet* set) { (void)set; return 0; }
static inline OSStatus AuthorizationCreateFromExternalForm(const AuthorizationExternalForm* extForm, AuthorizationRef* authRef) {
  (void)extForm; if(authRef) *authRef = NULL; return errAuthorizationDenied;
}
static inline OSStatus AuthorizationMakeExternalForm(AuthorizationRef authRef, AuthorizationExternalForm* extForm) {
  (void)authRef; (void)extForm; return errAuthorizationDenied;
}
static inline OSStatus AuthorizationCreate(const AuthorizationRights* rights, const AuthorizationEnvironment* env, AuthorizationFlags flags, AuthorizationRef* authRef) {
  (void)rights; (void)env; (void)flags; if(authRef) *authRef = NULL; return errAuthorizationDenied;
}
static inline OSStatus AuthorizationFree(AuthorizationRef authRef, AuthorizationFlags flags) {
  (void)authRef; (void)flags; return 0;
}
static inline OSStatus AuthorizationCopyRights(AuthorizationRef authRef, const AuthorizationRights* rights, const AuthorizationEnvironment* env, AuthorizationFlags flags, AuthorizationRights** authorizedRights) {
  (void)authRef; (void)rights; (void)env; (void)flags; if(authorizedRights) *authorizedRights = NULL; return errAuthorizationDenied;
}
#endif

/* ENOATTR — BSD/macOS name for ENODATA ---------------------------------- */
#include <errno.h>
#ifndef ENOATTR
#define ENOATTR ENODATA
#endif

/* getprogname — BSD; glibc exposes program_invocation_short_name --------- */
#if !defined(__APPLE__) && !defined(__FreeBSD__)
extern "C" { extern char* program_invocation_short_name; }
static inline const char* getprogname(void) { return program_invocation_short_name; }
#endif

/* Darwin _CS_DARWIN_USER_{TEMP,CACHE}_DIR confstr selectors —
 * map to TMPDIR / per-user cache dir. Hand back sentinel negative IDs that
 * system_directory() handles via its fallback (confstr returns 0 → uses getenv TMPDIR). */
#ifndef _CS_DARWIN_USER_TEMP_DIR
#define _CS_DARWIN_USER_TEMP_DIR  (-100)
#define _CS_DARWIN_USER_CACHE_DIR (-101)
#endif

/* Sparse CoreFoundation UI alerts (kCFUserNotification*) — libs-corebase may
 * lack these. Stub to make path.cc compile; calls hit an inline no-op. */
#ifndef kCFUserNotificationStopAlertLevel
#define kCFUserNotificationStopAlertLevel     0
#define kCFUserNotificationDefaultResponse    0
#define kCFUserNotificationAlternateResponse  1
#define kCFUserNotificationOtherResponse      2
static inline SInt32 CFUserNotificationDisplayAlert(
  CFTimeInterval t, CFOptionFlags lvl, CFURLRef iconURL, CFURLRef soundURL, CFURLRef locURL,
  CFStringRef header, CFStringRef message,
  CFStringRef defButton, CFStringRef altButton, CFStringRef otherButton,
  CFOptionFlags* responseFlags)
{
  (void)t;(void)lvl;(void)iconURL;(void)soundURL;(void)locURL;
  (void)header;(void)message;(void)defButton;(void)altButton;(void)otherButton;
  if(responseFlags) *responseFlags = kCFUserNotificationDefaultResponse;
  return 0;
}
#endif

/* LaunchServices LSOpenURLsWithRole stub — not relevant off-macOS -------- */
#ifndef kLSRolesViewer
#define kLSRolesViewer 0
typedef uint32_t LSRolesMask;
static inline OSStatus LSOpenURLsWithRole(CFArrayRef urls, LSRolesMask r, const void* inAEParam, const void* inAppParams, void* outPSN, int reqd)
{
  (void)urls;(void)r;(void)inAEParam;(void)inAppParams;(void)outPSN;(void)reqd;
  return -1;
}
#endif

/* dirname_r / basename_r — macOS thread-safe variants; glibc has only
 * the non-reentrant dirname/basename. Provide inline copies. ------------ */
#include <libgen.h>
#include <string.h>
static inline char* dirname_r(const char* path, char* buf) {
  char tmp[4096]; strncpy(tmp, path ? path : "", sizeof(tmp)-1); tmp[sizeof(tmp)-1] = 0;
  const char* d = dirname(tmp);
  if(!d || !buf) return NULL;
  strncpy(buf, d, 4095); buf[4095] = 0;
  return buf;
}
static inline char* basename_r(const char* path, char* buf) {
  char tmp[4096]; strncpy(tmp, path ? path : "", sizeof(tmp)-1); tmp[sizeof(tmp)-1] = 0;
  const char* b = basename(tmp);
  if(!b || !buf) return NULL;
  strncpy(buf, b, 4095); buf[4095] = 0;
  return buf;
}

/* digittoint — BSD extension missing from glibc -------------------------- */
#include <ctype.h>
static inline int digittoint(int c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return 0;
}

/* SIZE_T_MAX — BSD/Mac name; glibc uses SIZE_MAX --------------------------- */
#include <stdint.h>
#ifndef SIZE_T_MAX
#define SIZE_T_MAX SIZE_MAX
#endif

/* Byte order — CFByteOrder.h from libs-corebase provides the CF helpers;
 * for the raw machine/byte_order.h symbols TextMate occasionally uses,
 * fall back to endian.h. */
#include <endian.h>

/* AvailabilityMacros — stub version gates -------------------------------- */
#ifndef MAC_OS_X_VERSION_10_6
#define MAC_OS_X_VERSION_10_6   1060
#define MAC_OS_X_VERSION_10_7   1070
#define MAC_OS_X_VERSION_10_8   1080
#define MAC_OS_X_VERSION_10_9   1090
#define MAC_OS_X_VERSION_10_10 101000
#define MAC_OS_X_VERSION_10_11 101100
#define MAC_OS_X_VERSION_10_12 101200
#define MAC_OS_X_VERSION_MIN_REQUIRED 101200
#define MAC_OS_X_VERSION_MAX_ALLOWED  101200
#endif

#endif /* PRELUDE_GNUSTEP_H_TM_PORT */

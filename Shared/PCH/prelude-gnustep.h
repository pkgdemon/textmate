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

/* CommonCrypto CC_SHA1 — map to openssl */
#include <openssl/sha.h>
#ifndef CC_SHA1_DIGEST_LENGTH
#define CC_SHA1_DIGEST_LENGTH SHA_DIGEST_LENGTH
#define CC_LONG size_t
static inline unsigned char* CC_SHA1(const void* data, CC_LONG len, unsigned char* md) {
  return SHA1((const unsigned char*)data, len, md);
}
#endif

/* statfs — Linux lives in sys/vfs.h (macOS uses sys/mount.h) */
#if !defined(__APPLE__)
#include <sys/vfs.h>
#endif

/* os_activity — empty stubs */
#ifndef OS_ACTIVITY_FLAG_DEFAULT
typedef void* os_activity_t;
#define OS_ACTIVITY_FLAG_DEFAULT 0
#define OS_ACTIVITY_NONE         ((os_activity_t)0)
#define OS_ACTIVITY_CURRENT      ((os_activity_t)0)
static inline os_activity_t _os_activity_create(void* dso, const char* desc, os_activity_t parent, uint32_t flags) {
  (void)dso;(void)desc;(void)parent;(void)flags; return (os_activity_t)0;
}
#define os_activity_create(desc, parent, flags) _os_activity_create(NULL, desc, parent, flags)
#define os_activity_scope(a) ((void)(a))
#endif

/* CGEvent / CGEventFlags — Quartz Event Services stubs.
 * GNUstep's libopal does not provide these. Used by ns/* for rendering key
 * equivalent symbols. Stubs return empty strings; no functional behavior. */
#ifndef CGKeyCode
typedef unsigned short CGKeyCode;
#endif
#ifndef CGEvent_STUB_DEFINED
#define CGEvent_STUB_DEFINED
typedef struct __CGEvent* CGEventRef;
typedef uint64_t CGEventFlags;
enum {
  kCGEventFlagMaskAlphaShift   = 0x00010000,
  kCGEventFlagMaskShift        = 0x00020000,
  kCGEventFlagMaskControl      = 0x00040000,
  kCGEventFlagMaskAlternate    = 0x00080000,
  kCGEventFlagMaskCommand      = 0x00100000,
  kCGEventFlagMaskHelp         = 0x00400000,
  kCGEventFlagMaskSecondaryFn  = 0x00800000,
  kCGEventFlagMaskNumericPad   = 0x00200000,
  kCGEventFlagMaskNonCoalesced = 0x00000100,
};
static inline CGEventRef CGEventCreateKeyboardEvent(void* src, CGKeyCode key, bool down) {
  (void)src;(void)key;(void)down; return NULL;
}
static inline void CGEventSetFlags(CGEventRef e, CGEventFlags f) { (void)e;(void)f; }
static inline CGEventFlags CGEventGetFlags(CGEventRef e) { (void)e; return 0; }
static inline void CGEventPost(int loc, CGEventRef e) { (void)loc;(void)e; }
#endif

/* HIToolbox virtual key codes (kVK_*) — Carbon-only header.
 * ns/ns.mm uses ~120 of these to map physical key codes → printable chars.
 * Provide the values from <HIToolbox/Events.h>. */
#ifndef kVK_ANSI_A
#define kVK_ANSI_A           0x00
#define kVK_ANSI_S           0x01
#define kVK_ANSI_D           0x02
#define kVK_ANSI_F           0x03
#define kVK_ANSI_H           0x04
#define kVK_ANSI_G           0x05
#define kVK_ANSI_Z           0x06
#define kVK_ANSI_X           0x07
#define kVK_ANSI_C           0x08
#define kVK_ANSI_V           0x09
#define kVK_ANSI_B           0x0B
#define kVK_ANSI_Q           0x0C
#define kVK_ANSI_W           0x0D
#define kVK_ANSI_E           0x0E
#define kVK_ANSI_R           0x0F
#define kVK_ANSI_Y           0x10
#define kVK_ANSI_T           0x11
#define kVK_ANSI_1           0x12
#define kVK_ANSI_2           0x13
#define kVK_ANSI_3           0x14
#define kVK_ANSI_4           0x15
#define kVK_ANSI_6           0x16
#define kVK_ANSI_5           0x17
#define kVK_ANSI_Equal       0x18
#define kVK_ANSI_9           0x19
#define kVK_ANSI_7           0x1A
#define kVK_ANSI_Minus       0x1B
#define kVK_ANSI_8           0x1C
#define kVK_ANSI_0           0x1D
#define kVK_ANSI_RightBracket 0x1E
#define kVK_ANSI_O           0x1F
#define kVK_ANSI_U           0x20
#define kVK_ANSI_LeftBracket 0x21
#define kVK_ANSI_I           0x22
#define kVK_ANSI_P           0x23
#define kVK_ANSI_L           0x25
#define kVK_ANSI_J           0x26
#define kVK_ANSI_Quote       0x27
#define kVK_ANSI_K           0x28
#define kVK_ANSI_Semicolon   0x29
#define kVK_ANSI_Backslash   0x2A
#define kVK_ANSI_Comma       0x2B
#define kVK_ANSI_Slash       0x2C
#define kVK_ANSI_N           0x2D
#define kVK_ANSI_M           0x2E
#define kVK_ANSI_Period      0x2F
#define kVK_ANSI_Grave       0x32
#define kVK_ANSI_KeypadDecimal 0x41
#define kVK_ANSI_KeypadMultiply 0x43
#define kVK_ANSI_KeypadPlus  0x45
#define kVK_ANSI_KeypadClear 0x47
#define kVK_ANSI_KeypadDivide 0x4B
#define kVK_ANSI_KeypadEnter 0x4C
#define kVK_ANSI_KeypadMinus 0x4E
#define kVK_ANSI_KeypadEquals 0x51
#define kVK_ANSI_Keypad0     0x52
#define kVK_ANSI_Keypad1     0x53
#define kVK_ANSI_Keypad2     0x54
#define kVK_ANSI_Keypad3     0x55
#define kVK_ANSI_Keypad4     0x56
#define kVK_ANSI_Keypad5     0x57
#define kVK_ANSI_Keypad6     0x58
#define kVK_ANSI_Keypad7     0x59
#define kVK_ANSI_Keypad8     0x5B
#define kVK_ANSI_Keypad9     0x5C
#define kVK_Return           0x24
#define kVK_Tab              0x30
#define kVK_Space            0x31
#define kVK_Delete           0x33
#define kVK_Escape           0x35
#define kVK_Command          0x37
#define kVK_Shift            0x38
#define kVK_CapsLock         0x39
#define kVK_Option           0x3A
#define kVK_Control          0x3B
#define kVK_RightShift       0x3C
#define kVK_RightOption      0x3D
#define kVK_RightControl     0x3E
#define kVK_Function         0x3F
#define kVK_F17              0x40
#define kVK_VolumeUp         0x48
#define kVK_VolumeDown       0x49
#define kVK_Mute             0x4A
#define kVK_F18              0x4F
#define kVK_F19              0x50
#define kVK_F20              0x5A
#define kVK_F5               0x60
#define kVK_F6               0x61
#define kVK_F7               0x62
#define kVK_F3               0x63
#define kVK_F8               0x64
#define kVK_F9               0x65
#define kVK_F11              0x67
#define kVK_F13              0x69
#define kVK_F16              0x6A
#define kVK_F14              0x6B
#define kVK_F10              0x6D
#define kVK_F12              0x6F
#define kVK_F15              0x71
#define kVK_Help             0x72
#define kVK_Home             0x73
#define kVK_PageUp           0x74
#define kVK_ForwardDelete    0x75
#define kVK_F4               0x76
#define kVK_End              0x77
#define kVK_F2               0x78
#define kVK_PageDown         0x79
#define kVK_F1               0x7A
#define kVK_LeftArrow        0x7B
#define kVK_RightArrow       0x7C
#define kVK_DownArrow        0x7D
#define kVK_UpArrow          0x7E
#endif

/* SystemConfiguration / SCNetworkReachability — stubs */
#ifndef SCNetworkReachability_STUB_DEFINED
#define SCNetworkReachability_STUB_DEFINED
typedef struct __SCNetworkReachability* SCNetworkReachabilityRef;
typedef uint32_t SCNetworkReachabilityFlags;
enum {
  kSCNetworkReachabilityFlagsTransientConnection = 1<<0,
  kSCNetworkReachabilityFlagsReachable           = 1<<1,
  kSCNetworkReachabilityFlagsConnectionRequired  = 1<<2,
};
static inline SCNetworkReachabilityRef SCNetworkReachabilityCreateWithName(CFAllocatorRef alloc, const char* name) {
  (void)alloc;(void)name; return NULL;
}
static inline Boolean SCNetworkReachabilityGetFlags(SCNetworkReachabilityRef r, SCNetworkReachabilityFlags* f) {
  (void)r; if(f) *f = kSCNetworkReachabilityFlagsReachable; return true;
}
#endif

/* MNT_RDONLY — Linux uses MS_RDONLY (sys/mount.h); Apple uses MNT_RDONLY */
#ifndef MNT_RDONLY
#define MNT_RDONLY 0x00000001
#endif

/* gethostuuid — Apple-only; stub returns -1 (never succeeds) */
#if !defined(__APPLE__)
#include <sys/time.h>
typedef unsigned char uuid_t_compat[16];
static inline int gethostuuid(unsigned char* id, const struct timespec* wait) {
  (void)id; (void)wait; return -1;
}
#endif

/* Security.framework SecItemImport stubs */
#if !defined(__APPLE__) && !defined(SecItemImport_STUB_DEFINED)
#define SecItemImport_STUB_DEFINED
typedef struct {
  uint32_t version;
  uint32_t flags;
  CFTypeRef passphrase;
  CFStringRef alertTitle;
  CFStringRef alertPrompt;
  CFTypeRef accessRef;
  CFArrayRef keyUsage;
  CFArrayRef keyAttributes;
} SecItemImportExportKeyParameters;
typedef uint32_t SecExternalItemType;
typedef uint32_t SecExternalFormat;
typedef struct __SecKey* SecKeyRef;
#define kSecItemTypePublicKey 1
#define kSecFormatOpenSSL 1
static inline OSStatus SecItemImport(CFDataRef importedData, CFStringRef fileNameOrExtension, SecExternalFormat* inputFormat, SecExternalItemType* itemType, uint32_t flags, const SecItemImportExportKeyParameters* keyParams, void* importKeychain, CFArrayRef* outItems) {
  (void)importedData;(void)fileNameOrExtension;(void)inputFormat;(void)itemType;(void)flags;(void)keyParams;(void)importKeychain;
  if(outItems) *outItems = NULL;
  return -1;
}
typedef struct CSSM_DATA { uint32_t Length; uint8_t* Data; } CSSM_DATA;
typedef CSSM_DATA CSSM_DATA_PTR;
typedef int32_t CSSM_RETURN;
#define CSSM_OK 0
#endif

/* sysctl HW extras */
#ifndef HW_MODEL
#define HW_MODEL   2
#define HW_MACHINE 1
#endif

/* NSApplicationLaunchUserNotificationKey */
#ifndef NSApplicationLaunchUserNotificationKey
#define NSApplicationLaunchUserNotificationKey @"NSApplicationLaunchUserNotificationKey"
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

#ifndef FSEVENTS_SHIM_H
#define FSEVENTS_SHIM_H
/* Compile-only FSEvents shim for GNUstep/Linux.
 * All functions are no-ops; file watching does nothing at runtime.
 * Real implementation would back these with inotify(7)/kqueue(2).
 * Enough API to let TextMate's io/events.cc and BundlesManager compile. */
#if !defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __FSEventStream*       FSEventStreamRef;
typedef const struct __FSEventStream* ConstFSEventStreamRef;
typedef uint64_t FSEventStreamEventId;
typedef uint32_t FSEventStreamEventFlags;
typedef uint32_t FSEventStreamCreateFlags;

typedef struct {
  CFIndex version;
  void*   info;
  const void* (*retain)(const void*);
  void (*release)(const void*);
  CFStringRef (*copyDescription)(const void*);
} FSEventStreamContext;

typedef void (*FSEventStreamCallback)(
  ConstFSEventStreamRef streamRef,
  void* clientCallBackInfo,
  size_t numEvents,
  void* eventPaths,
  const FSEventStreamEventFlags eventFlags[],
  const FSEventStreamEventId eventIds[]);

#define kFSEventStreamEventIdSinceNow          ((FSEventStreamEventId)0xFFFFFFFFFFFFFFFFULL)
#define kFSEventStreamCreateFlagNone           0x00000000
#define kFSEventStreamCreateFlagUseCFTypes     0x00000001
#define kFSEventStreamCreateFlagNoDefer        0x00000002
#define kFSEventStreamCreateFlagWatchRoot      0x00000004
#define kFSEventStreamCreateFlagIgnoreSelf     0x00000008
#define kFSEventStreamCreateFlagFileEvents     0x00000010
#define kFSEventStreamCreateFlagMarkSelf       0x00000020

#define kFSEventStreamEventFlagNone            0x00000000
#define kFSEventStreamEventFlagMustScanSubDirs 0x00000001
#define kFSEventStreamEventFlagUserDropped     0x00000002
#define kFSEventStreamEventFlagKernelDropped   0x00000004
#define kFSEventStreamEventFlagEventIdsWrapped 0x00000008
#define kFSEventStreamEventFlagHistoryDone     0x00000010
#define kFSEventStreamEventFlagRootChanged     0x00000020
#define kFSEventStreamEventFlagMount           0x00000040
#define kFSEventStreamEventFlagUnmount         0x00000080

static inline FSEventStreamRef FSEventStreamCreate(
  CFAllocatorRef alloc,
  FSEventStreamCallback callback,
  FSEventStreamContext* context,
  CFArrayRef pathsToWatch,
  FSEventStreamEventId sinceWhen,
  CFTimeInterval latency,
  FSEventStreamCreateFlags flags)
{
  (void)alloc; (void)callback; (void)context; (void)pathsToWatch;
  (void)sinceWhen; (void)latency; (void)flags;
  /* Non-null sentinel so callers distinguish from allocation failure. */
  return (FSEventStreamRef)(uintptr_t)0x1;
}
static inline void FSEventStreamRetain(FSEventStreamRef s)                        { (void)s; }
static inline void FSEventStreamRelease(FSEventStreamRef s)                       { (void)s; }
static inline void FSEventStreamInvalidate(FSEventStreamRef s)                    { (void)s; }
static inline Boolean FSEventStreamStart(FSEventStreamRef s)                      { (void)s; return true; }
static inline void FSEventStreamStop(FSEventStreamRef s)                          { (void)s; }
static inline void FSEventStreamFlushSync(FSEventStreamRef s)                     { (void)s; }
static inline void FSEventStreamFlushAsync(FSEventStreamRef s)                    { (void)s; }
static inline void FSEventStreamScheduleWithRunLoop(FSEventStreamRef s, CFRunLoopRef rl, CFStringRef mode) {
  (void)s; (void)rl; (void)mode;
}
static inline void FSEventStreamUnscheduleFromRunLoop(FSEventStreamRef s, CFRunLoopRef rl, CFStringRef mode) {
  (void)s; (void)rl; (void)mode;
}
static inline void FSEventStreamSetDispatchQueue(FSEventStreamRef s, dispatch_queue_t q) {
  (void)s; (void)q;
}
static inline FSEventStreamEventId FSEventStreamGetLatestEventId(ConstFSEventStreamRef s) {
  (void)s; return 0;
}
static inline CFArrayRef FSEventStreamCopyPathsBeingWatched(ConstFSEventStreamRef s) {
  (void)s; return NULL;
}
static inline FSEventStreamEventId FSEventsGetCurrentEventId(void) { return 0; }

#ifdef __cplusplus
}
#endif
#endif /* !__APPLE__ */
#endif

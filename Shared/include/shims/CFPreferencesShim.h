#ifndef CF_PREFERENCES_SHIM_H
#define CF_PREFERENCES_SHIM_H
/* libs-corebase lacks CFPreferences. Provide a no-op shim: every lookup
 * misses, every write is a no-op. TextMate preference loading will fall back
 * to its defaults. Real GNUstep integration would use NSUserDefaults. */
#if !defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef kCFPreferencesCurrentApplication
#define kCFPreferencesCurrentApplication ((CFStringRef)0)
#define kCFPreferencesCurrentUser        ((CFStringRef)0)
#define kCFPreferencesCurrentHost        ((CFStringRef)0)
#define kCFPreferencesAnyApplication     ((CFStringRef)0)
#define kCFPreferencesAnyUser            ((CFStringRef)0)
#define kCFPreferencesAnyHost            ((CFStringRef)0)
#endif

static inline CFPropertyListRef CFPreferencesCopyAppValue(CFStringRef key, CFStringRef app) {
  (void)key; (void)app; return NULL;
}
static inline void CFPreferencesSetAppValue(CFStringRef key, CFPropertyListRef value, CFStringRef app) {
  (void)key; (void)value; (void)app;
}
static inline Boolean CFPreferencesAppSynchronize(CFStringRef app) { (void)app; return true; }
static inline Boolean CFPreferencesGetAppBooleanValue(CFStringRef key, CFStringRef app, Boolean* keyExistsAndHasValidFormat) {
  (void)key; (void)app; if(keyExistsAndHasValidFormat) *keyExistsAndHasValidFormat = false; return false;
}
static inline CFIndex CFPreferencesGetAppIntegerValue(CFStringRef key, CFStringRef app, Boolean* keyExistsAndHasValidFormat) {
  (void)key; (void)app; if(keyExistsAndHasValidFormat) *keyExistsAndHasValidFormat = false; return 0;
}
static inline CFArrayRef CFPreferencesCopyKeyList(CFStringRef app, CFStringRef user, CFStringRef host) {
  (void)app; (void)user; (void)host; return NULL;
}

#ifdef __cplusplus
}
#endif
#endif /* !__APPLE__ */
#endif

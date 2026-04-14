/* Runtime-side stubs: provides the symbols our header shims reference.
 * Compiled once and linked into libtmcore. */
#if !defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>

/* DISPATCH_SOURCE_TYPE_VNODE sentinel. Calls using it crash at runtime;
 * fine so long as TextMate doesn't exercise file-watch paths for MVP. */
struct dispatch_source_type_s {
  const char* name;
};
extern "C" const struct dispatch_source_type_s _fake_dispatch_source_type_vnode = { "vnode-stub" };

/* CFURL resource keys missing from libs-corebase. TextMate checks them for
 * locality / alias detection — weak stubs so isEqual comparisons yield false. */
extern "C" {
  const CFStringRef kCFURLVolumeIsLocalKey    = CFSTR("NSURLVolumeIsLocalKey");
  const CFStringRef kCFURLVolumeIsInternalKey = CFSTR("NSURLVolumeIsInternalKey");
  const CFStringRef kCFURLLocalizedNameKey    = CFSTR("NSURLLocalizedNameKey");
  const CFStringRef kCFURLIsAliasFileKey      = CFSTR("NSURLIsAliasFileKey");
  const CFStringRef kCFURLIsSymbolicLinkKey   = CFSTR("NSURLIsSymbolicLinkKey");
  const CFStringRef kCFURLIsPackageKey        = CFSTR("NSURLIsPackageKey");
}
#endif

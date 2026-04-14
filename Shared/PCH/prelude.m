#ifndef PRELUDE_M_PCH_KZLXVFRT
#define PRELUDE_M_PCH_KZLXVFRT

#include "prelude.c"
#include "prelude-mac.h"

#import <objc/objc-runtime.h>

#if defined(__APPLE__)
#import <AddressBook/AddressBook.h>
#import <Cocoa/Cocoa.h>
#import <ExceptionHandling/NSExceptionHandler.h>
#import <CoreFoundation/CFPlugInCOM.h> // must be loaded before QuickLook.h
#import <Quartz/Quartz.h> // includes the private QuickLookUI.h
#import <WebKit/WebKit.h>
#else
/* GNUstep: pull in Foundation + AppKit only. WebKit, Quartz, AddressBook,
 * and ExceptionHandling stay out — porting uses those selectively. */
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <AppKitPolyfill.h>
#endif

#include "oak/sdk-compat.h"

#endif /* end of include guard: PRELUDE_M_PCH_KZLXVFRT */

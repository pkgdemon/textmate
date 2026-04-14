#ifndef APPKIT_POLYFILL_H
#define APPKIT_POLYFILL_H
/*
 * GNUstep AppKit polyfills: modern (macOS 10.12+) constants, types, and
 * selectors that TextMate uses but GNUstep doesn't yet expose under the
 * newer spellings. Only for non-Apple targets.
 */
#if !defined(__APPLE__)

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

/* Bezel style name update (Apple renamed in 10.12). GNUstep ships the old names. */
#ifndef NSBezelStyleRounded
#define NSBezelStyleRounded             NSRoundedBezelStyle
#endif
#ifndef NSBezelStyleRegularSquare
#define NSBezelStyleRegularSquare       NSRegularSquareBezelStyle
#endif
#ifndef NSBezelStyleRecessed
#define NSBezelStyleRecessed            NSRecessedBezelStyle
#endif
#ifndef NSBezelStyleRoundRect
#define NSBezelStyleRoundRect           NSRoundRectBezelStyle
#endif
#ifndef NSBezelStyleSmallSquare
#define NSBezelStyleSmallSquare         NSSmallSquareBezelStyle
#endif
#ifndef NSBezelStyleTexturedRounded
#define NSBezelStyleTexturedRounded     NSTexturedRoundedBezelStyle
#endif
#ifndef NSBezelStyleShadowlessSquare
#define NSBezelStyleShadowlessSquare    NSShadowlessSquareBezelStyle
#endif
#ifndef NSBezelStyleDisclosure
#define NSBezelStyleDisclosure          NSDisclosureBezelStyle
#endif
#ifndef NSBezelStyleHelpButton
#define NSBezelStyleHelpButton          NSHelpButtonBezelStyle
#endif
#ifndef NSBezelStyleCircular
#define NSBezelStyleCircular            NSCircularBezelStyle
#endif

/* NSControlSize — older spelling on GNUstep. */
#ifndef NSControlSizeSmall
#define NSControlSizeSmall   NSSmallControlSize
#define NSControlSizeMini    NSMiniControlSize
#define NSControlSizeRegular NSRegularControlSize
#endif

/* NSControl.controlSize — property polyfill (GNUstep: via -cell methods) */
@interface NSControl (OakGNUstepCompatControlSize)
@property NSControlSize controlSize;
@end

/* NSView / NSWindow.animator — 10.5+ animation proxy. Return self for compile. */
@interface NSObject (OakGNUstepCompatAnimator)
- (instancetype)animator;
@end

/* NSEventMask — newer spellings since 10.12 */
#ifndef NSEventMaskKeyDown
#define NSEventMaskKeyDown   NSKeyDownMask
#define NSEventMaskKeyUp     NSKeyUpMask
#define NSEventMaskFlagsChanged NSFlagsChangedMask
#define NSEventMaskAny       NSAnyEventMask
#endif

/* NSWindowStyleMask — newer spellings */
#ifndef NSWindowStyleMaskBorderless
#define NSWindowStyleMaskBorderless  NSBorderlessWindowMask
#define NSWindowStyleMaskTitled      NSTitledWindowMask
#define NSWindowStyleMaskClosable    NSClosableWindowMask
#define NSWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
#define NSWindowStyleMaskResizable   NSResizableWindowMask
#endif

/* NSURL category — fileSystemRepresentation property */
@interface NSURL (OakGNUstepCompat)
@property (readonly) char const* fileSystemRepresentation;
@end

/* NSAttributedString — enumerateAttributesInRange:options:usingBlock: */
@interface NSAttributedString (OakGNUstepCompat)
- (void)enumerateAttributesInRange:(NSRange)enumerationRange options:(NSUInteger)opts usingBlock:(void (^)(NSDictionary *attrs, NSRange range, BOOL *stop))block;
@end

/* NSIndexSet — indexesPassingTest */
@interface NSIndexSet (OakGNUstepCompat)
- (NSIndexSet*)indexesPassingTest:(BOOL (^)(NSUInteger idx, BOOL *stop))predicate;
@end

/* NSBundle — imageForResource: (10.6+) */
@interface NSBundle (OakGNUstepCompat)
- (NSImage*)imageForResource:(NSString*)name;
@end

/* Forward declarations for types used before their full definition */
@class NSTitlebarAccessoryViewController;
typedef OSType DescType;
typedef OSType AEEventClass;
typedef OSType AEEventID;
typedef int16_t AEReturnID;
typedef int32_t AETransactionID;
typedef OSType  AEKeyword;

/* NSAttributedStringEnumerationOptions — just an integer mask */
#ifndef NSAttributedStringEnumerationReverse
#define NSAttributedStringEnumerationReverse                           (1UL << 1)
#define NSAttributedStringEnumerationLongestEffectiveRangeNotRequired  (1UL << 20)
#endif

/* NSButtonType — new spellings */
#ifndef NSButtonTypeMomentaryChange
#define NSButtonTypeMomentaryChange   NSMomentaryChangeButton
#define NSButtonTypePushOnPushOff     NSPushOnPushOffButton
#define NSButtonTypeMomentaryPushIn   NSMomentaryPushInButton
#define NSButtonTypeSwitch            NSSwitchButton
#define NSButtonTypeRadio             NSRadioButton
#define NSButtonTypeToggle            NSToggleButton
#define NSButtonTypeOnOff             NSOnOffButton
#define NSButtonTypeMomentaryLight    NSMomentaryLightButton
#define NSButtonTypeAccelerator       NSMomentaryChangeButton
#define NSButtonTypeMultiLevelAccelerator NSMomentaryChangeButton
#endif

/* os_log_create — return a dummy handle */
#ifdef __OBJC__
#ifndef os_log_create
static inline os_log_t os_log_create(const char* subsystem, const char* category) { (void)subsystem; (void)category; return (os_log_t)1; }
#endif
#endif

/* kUnknownFSObjectIcon — Carbon; return 0 */
#ifndef kUnknownFSObjectIcon
#define kUnknownFSObjectIcon 0
#endif

/* Core Animation stubs for compile-only. These should never be instantiated; if
 * OakPopOutAnimation actually runs, we'd need real QuartzCore (and OpenGL).   */
#ifndef CAANIMATION_STUB_DEFINED
#define CAANIMATION_STUB_DEFINED
@protocol CAAnimationDelegate <NSObject> @end
@interface CALayer : NSObject @end
@interface CABasicAnimation : NSObject
+ (instancetype)animationWithKeyPath:(NSString*)path;
@property (copy) id fromValue;
@property (copy) id toValue;
@property (strong) id<CAAnimationDelegate> delegate;
@property double duration;
@end
#endif

/* NSView — wantsLayer (10.5+ but absent on some GNUstep builds) */
@interface NSView (OakGNUstepCompat)
@property BOOL wantsLayer;
@property (strong) CALayer* layer;
@end

/* Accessibility label/role — category stubs */
@interface NSObject (OakGNUstepCompatAccessibility)
- (NSString*)accessibilityLabel;
- (void)setAccessibilityLabel:(NSString*)label;
- (NSString*)accessibilityRole;
- (void)setAccessibilityRole:(NSString*)role;
- (NSString*)accessibilityValue;
- (void)setAccessibilityValue:(id)value;
@end

/* NSButton — labelWithString: / checkboxWithTitle: convenience (10.12+) */
@interface NSButton (OakGNUstepCompat)
+ (NSButton*)buttonWithTitle:(NSString*)title target:(id)target action:(SEL)action;
+ (NSButton*)checkboxWithTitle:(NSString*)title target:(id)target action:(SEL)action;
@end

@interface NSTextField (OakGNUstepCompat)
+ (NSTextField*)labelWithString:(NSString*)stringValue;
+ (NSTextField*)wrappingLabelWithString:(NSString*)stringValue;
@end

/* Autolayout anchors — minimal stub. Compile-only. */
/* GNUstep has real NSLayoutConstraint and NSLayoutAnchor classes. We only
 * need to bolt the leading/trailing/etc. anchor accessors onto NSView,
 * which GNUstep exposes on NSLayoutGuide but not directly on NSView yet. */
@interface NSLayoutAnchorStub : NSLayoutAnchor
- (NSLayoutConstraint*)constraintEqualToAnchor:(id)other;
- (NSLayoutConstraint*)constraintEqualToAnchor:(id)other constant:(CGFloat)c;
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToAnchor:(id)other;
- (NSLayoutConstraint*)constraintLessThanOrEqualToAnchor:(id)other;
- (NSLayoutConstraint*)constraintEqualToConstant:(CGFloat)c;
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToConstant:(CGFloat)c;
@end
@interface NSView (OakGNUstepCompatAnchors)
@property (readonly, strong) NSLayoutAnchorStub* leadingAnchor;
@property (readonly, strong) NSLayoutAnchorStub* trailingAnchor;
@property (readonly, strong) NSLayoutAnchorStub* topAnchor;
@property (readonly, strong) NSLayoutAnchorStub* bottomAnchor;
@property (readonly, strong) NSLayoutAnchorStub* widthAnchor;
@property (readonly, strong) NSLayoutAnchorStub* heightAnchor;
@property (readonly, strong) NSLayoutAnchorStub* centerXAnchor;
@property (readonly, strong) NSLayoutAnchorStub* centerYAnchor;
@property (readonly, strong) NSLayoutAnchorStub* firstBaselineAnchor;
@property (readonly, strong) NSLayoutAnchorStub* lastBaselineAnchor;
@end

/* NSFont feature keys — Core Text has kCT versions */
#ifndef NSFontFeatureTypeIdentifierKey
#define NSFontFeatureTypeIdentifierKey      ((NSString*)kCTFontFeatureTypeIdentifierKey)
#define NSFontFeatureSelectorIdentifierKey  ((NSString*)kCTFontFeatureSelectorIdentifierKey)
#endif

/* NSUserDefaults initWithSuiteName: (10.9+) */
@interface NSUserDefaults (OakGNUstepCompat)
- (instancetype)initWithSuiteName:(NSString*)suiteName;
@end

/* NSButton/NSView — updateTrackingAreas (10.5+, normally present) */
@interface NSView (OakGNUstepCompatTracking)
- (void)updateTrackingAreas;
- (void)addTrackingArea:(id)area;
- (void)removeTrackingArea:(id)area;
@end

/* NSStringDrawingContext (10.11+) — stub class */
#ifndef NSStringDrawingContext_STUB_DEFINED
#define NSStringDrawingContext_STUB_DEFINED
@interface NSStringDrawingContext : NSObject
@property CGFloat minimumScaleFactor;
@property (readonly) CGFloat actualScaleFactor;
@property (readonly) NSRect totalBounds;
@end
#endif

/* Newer NSEventMask spellings (10.12 renaming) */
#ifndef NSEventMaskLeftMouseDown
#define NSEventMaskLeftMouseDown   NSLeftMouseDownMask
#define NSEventMaskLeftMouseUp     NSLeftMouseUpMask
#define NSEventMaskRightMouseDown  NSRightMouseDownMask
#define NSEventMaskRightMouseUp    NSRightMouseUpMask
#define NSEventMaskMouseMoved      NSMouseMovedMask
#define NSEventMaskLeftMouseDragged  NSLeftMouseDraggedMask
#define NSEventMaskRightMouseDragged NSRightMouseDraggedMask
#define NSEventMaskOtherMouseDown    NSOtherMouseDownMask
#define NSEventMaskOtherMouseUp      NSOtherMouseUpMask
#define NSEventMaskOtherMouseDragged NSOtherMouseDraggedMask
#define NSEventMaskMouseEntered     NSMouseEnteredMask
#define NSEventMaskMouseExited      NSMouseExitedMask
#define NSEventMaskScrollWheel      NSScrollWheelMask
#endif

/* NSApplication.effectiveAppearance (10.14+) */
@interface NSApplication (OakGNUstepCompat)
@property (readonly, strong) NSAppearance* effectiveAppearance;
@end

/* Pre-10.10 accessibility API — many GNUstep NSCell/NSView don't expose these */
@interface NSObject (OakGNUstepCompatAccessibilityOld)
- (NSArray*)accessibilityAttributeNames;
- (id)accessibilityAttributeValue:(NSString*)attribute;
- (BOOL)accessibilityIsAttributeSettable:(NSString*)attribute;
- (void)accessibilitySetValue:(id)value forAttribute:(NSString*)attribute;
@end

/* NSCell.rowHeightForText: */
@interface NSCell (OakGNUstepCompat)
- (CGFloat)rowHeightForText:(NSString*)text;
@end

/* NSPasteboard.clearContents / writeObjects: (10.6+) */
@interface NSPasteboard (OakGNUstepCompatWrite)
- (NSInteger)clearContents;
- (BOOL)writeObjects:(NSArray*)objects;
@end

/* NSWindow.addTitlebarAccessoryViewController: (10.10+) */
@interface NSWindow (OakGNUstepCompatTitlebar)
- (void)addTitlebarAccessoryViewController:(NSTitlebarAccessoryViewController*)ctrl;
- (void)removeTitlebarAccessoryViewControllerAtIndex:(NSInteger)idx;
@end

/* NSTableView.allowsTypeSelect (10.5+) */
@interface NSTableView (OakGNUstepCompat)
@property BOOL allowsTypeSelect;
@end

/* AppleEvent stubs */
#ifndef typeFileURL
#define typeFileURL            'furl'
#define kCoreEventClass        'aevt'
#define kAEOpenDocuments       'odoc'
#define kAutoGenerateReturnID  ((AEReturnID)-1)
#define kAnyTransactionID      ((AETransactionID)0)
#endif
@interface NSAppleEventDescriptor (OakGNUstepCompatDescriptors)
+ (NSAppleEventDescriptor*)descriptorWithDescriptorType:(DescType)descriptorType data:(NSData*)data;
+ (NSAppleEventDescriptor*)appleEventWithEventClass:(AEEventClass)ec eventID:(AEEventID)eid targetDescriptor:(NSAppleEventDescriptor*)target returnID:(AEReturnID)rid transactionID:(AETransactionID)tid;
- (void)setParamDescriptor:(NSAppleEventDescriptor*)desc forKeyword:(AEKeyword)keyword;
@end

#ifndef keyDirectObject
#define keyDirectObject '----'
#define NSWorkspaceLaunchConfigurationAppleEvent  @"NSWorkspaceLaunchConfigurationAppleEvent"
#define NSWorkspaceLaunchDefault                  0
#endif

@interface NSWorkspace (OakGNUstepCompatLaunch)
- (BOOL)launchApplicationAtURL:(NSURL*)url options:(NSUInteger)opts configuration:(NSDictionary*)config error:(NSError**)err;
@end

/* NSCompositingOperation — 10.12 rename of NSCompositeOperation */
#ifndef NSCompositingOperationSourceOver
#define NSCompositingOperationSourceOver  NSCompositeSourceOver
#define NSCompositingOperationCopy        NSCompositeCopy
#define NSCompositingOperationDestinationOver  NSCompositeDestinationOver
#define NSCompositingOperationDestinationIn    NSCompositeDestinationIn
#define NSCompositingOperationDestinationOut   NSCompositeDestinationOut
#define NSCompositingOperationSourceIn    NSCompositeSourceIn
#define NSCompositingOperationSourceOut   NSCompositeSourceOut
#define NSCompositingOperationSourceAtop  NSCompositeSourceAtop
#define NSCompositingOperationDestinationAtop NSCompositeDestinationAtop
#define NSCompositingOperationXOR         NSCompositeXOR
#define NSCompositingOperationPlusDarker  NSCompositePlusDarker
#define NSCompositingOperationPlusLighter NSCompositePlusLighter
#define NSCompositingOperationClear       NSCompositeClear
#endif

/* NSTableViewStyle — 10.14 */
#ifndef NSTableViewStylePlain
typedef NS_ENUM(NSInteger, NSTableViewStyle) {
  NSTableViewStyleAutomatic  = 0,
  NSTableViewStyleFullWidth  = 1,
  NSTableViewStyleInset      = 2,
  NSTableViewStyleSourceList = 3,
  NSTableViewStylePlain      = 4
};
@interface NSTableView (OakGNUstepCompatStyle)
@property NSTableViewStyle style;
@end
#endif

/* NSView.invalidateIntrinsicContentSize / intrinsicContentSize / constraints (10.7+) */
@interface NSView (OakGNUstepCompatIntrinsic)
- (void)invalidateIntrinsicContentSize;
- (NSSize)intrinsicContentSize;
- (NSArray*)constraints;
@end

/* NSStandardKeyBindingResponding — 10.7 protocol (empty stub) */
#ifndef NSStandardKeyBindingResponding_STUB_DEFINED
#define NSStandardKeyBindingResponding_STUB_DEFINED
@protocol NSStandardKeyBindingResponding <NSObject>
@optional
- (void)insertText:(id)insertString;
- (void)doCommandBySelector:(SEL)selector;
@end
#endif

/* Accessibility 10.10+ functions/constants */
#ifndef NSAccessibilityFrameInView
static inline NSRect NSAccessibilityFrameInView(id parent, NSRect frame) { (void)parent; return frame; }
#define NSAccessibilityException             @"NSAccessibilityException"
#define NSAccessibilityUnknownRole           @"NSAccessibilityUnknownRole"
#define NSAccessibilityTextAreaRole          @"NSAccessibilityTextAreaRole"
#define NSAccessibilityStaticTextRole        @"NSAccessibilityStaticTextRole"
#define NSAccessibilityGroupRole             @"NSAccessibilityGroupRole"
typedef NSString* NSAccessibilityRole;
typedef NSString* NSAccessibilitySubrole;
#endif

/* NSImage.lockFocusFlipped: — 10.6 */
@interface NSImage (OakGNUstepCompatLockFocus)
- (void)lockFocusFlipped:(BOOL)flipped;
@end

/* NSView.inputContext — 10.6 (for IME) */
@interface NSView (OakGNUstepCompatInputContext)
@property (readonly, strong) id inputContext;
@end

/* Accessibility array attribute methods — old API on NSObject */
@interface NSObject (OakGNUstepCompatA11yArray)
- (NSUInteger)accessibilityArrayAttributeCount:(NSString*)attribute;
- (NSArray*)accessibilityArrayAttributeValues:(NSString*)attribute index:(NSUInteger)index maxCount:(NSUInteger)maxCount;
- (NSUInteger)accessibilityIndexOfChild:(id)child;
@end

/* NSAccessibilityCustomRotor — GNUstep forward-declares the Parameters class
 * but doesn't give it properties. Extend via category. */
#import <AppKit/NSAccessibilityCustomRotor.h>
#define NSAccessibilityCustomRotorSearchDirectionPrevious 0
#define NSAccessibilityCustomRotorSearchDirectionNext     1
@interface NSAccessibilityCustomRotorSearchParameters : NSObject
@property (copy) NSString* filterString;
@property (strong) id currentItem;
@property NSInteger searchDirection;
@end
@interface NSAccessibilityCustomRotorItemResult (OakGNUstepCompatInit)
- (instancetype)initWithTargetElement:(id)target;
@property (strong) id targetElement;
@property NSRange targetRange;
@property (copy) NSString* customLabel;
@end

/* Carbon UAZoom — accessibility zoom focus; stub */
#ifndef kUAZoomFocusTypeInsertionPoint
#define kUAZoomFocusTypeInsertionPoint 1
static inline OSStatus UAZoomChangeFocus(void* a, void* b, int c) { (void)a; (void)b; (void)c; return 0; }
#endif

/* NSSpellChecker — modern methods */
@interface NSSpellChecker (OakGNUstepCompat)
- (BOOL)hasLearnedWord:(NSString*)word;
- (NSArray*)guessesForWordRange:(NSRange)range inString:(NSString*)str language:(NSString*)lang inSpellDocumentWithTag:(NSInteger)tag;
- (void)learnWord:(NSString*)word;
- (void)unlearnWord:(NSString*)word;
@end

/* NSImage.accessibilityDescription — 10.6 */
@interface NSImage (OakGNUstepCompatA11yDescription)
@property (copy) NSString* accessibilityDescription;
@end
@interface NSObject (OakGNUstepCompatA11yElement)
- (BOOL)accessibilityElement;
- (void)setAccessibilityElement:(BOOL)flag;
@end

/* NSImage.template flag — 10.6+. ('template' is a C++ keyword, so declare as methods, not property.) */
@interface NSImage (OakGNUstepCompatTemplate)
- (BOOL)isTemplate;
- (void)setTemplate:(BOOL)flag;
@end

/* NSView.trackingAreas — 10.5+ */
@interface NSView (OakGNUstepCompatTrackingAreas)
@property (readonly, copy) NSArray* trackingAreas;
- (void)removeConstraints:(NSArray*)constraints;
- (void)addConstraints:(NSArray*)constraints;
@end

/* NSLayoutConstraint static helpers — GNUstep has most, but types may differ.
 * (Kept as a no-op; GNUstep's own header provides the real ones.) */

/* NSColor.colorWithCGColor: — 10.8 */
@interface NSColor (OakGNUstepCompatCGColor)
+ (NSColor*)colorWithCGColor:(CGColorRef)cgColor;
@property (readonly) CGColorRef CGColor;
@end
/* NSDictionaryOfVariableBindings — 10.7 macro; empty dictionary */
#ifndef NSDictionaryOfVariableBindings
#define NSDictionaryOfVariableBindings(...) (@{})
#endif

/* NSTouchBar supporting types — GNUstep has NSTouchBar/NSTouchBarItem but
 * lacks NSTouchBarItemIdentifier typedef and NSTouchBarDelegate protocol. */
#ifndef NSTouchBar_ITEM_STUB_DEFINED
#define NSTouchBar_ITEM_STUB_DEFINED
typedef NSString* NSTouchBarItemIdentifier;
@protocol NSTouchBarDelegate <NSObject> @end
@interface NSTouchBar (OakGNUstepCompatItems)
@property (copy) NSArray<NSTouchBarItemIdentifier>* defaultItemIdentifiers;
@property (copy) NSArray<NSTouchBarItemIdentifier>* customizationAllowedItemIdentifiers;
@property (copy) NSString* customizationIdentifier;
@property (weak) id<NSTouchBarDelegate> delegate;
@end
@interface NSCustomTouchBarItem (OakGNUstepCompat)
- (instancetype)initWithIdentifier:(NSTouchBarItemIdentifier)identifier;
@property (strong) NSView* view;
@property float visibilityPriority;
@property (copy) NSString* customizationLabel;
@end
#define NSTouchBarItemPriorityHigh    1000.0f
#define NSTouchBarItemPriorityNormal     0.0f
#define NSTouchBarItemPriorityLow    -1000.0f
#endif

/* NSSegmentedControl.trackingMode — GNUstep has the enum already */
@interface NSSegmentedControl (OakGNUstepCompatTracking)
@property NSSegmentSwitchTracking trackingMode;
@end
#ifndef NSSegmentStyleSeparated
#define NSSegmentStyleSeparated  ((NSSegmentStyle)8)
#endif

/* NSAlert modernizations */
@interface NSAlert (OakGNUstepCompatAccessory)
@property (strong) NSView* accessoryView;
- (void)layout;
@end

/* Carbon UAZoom */
#ifndef UAZoomEnabled
static inline Boolean UAZoomEnabled(void) { return false; }
#endif

/* UTType constants (UniformTypeIdentifiers) */
#ifndef kUTTypeURL
#define kUTTypeURL       CFSTR("public.url")
#define kUTTypeFileURL   CFSTR("public.file-url")
#define kUTTypePlainText CFSTR("public.plain-text")
#define kUTTypeUTF8PlainText CFSTR("public.utf8-plain-text")
#define kUTTypeHTML      CFSTR("public.html")
#define kUTTypeImage     CFSTR("public.image")
#define kUTTypeData      CFSTR("public.data")
#define kUTTypeContent   CFSTR("public.content")
#define kUTTypeItem      CFSTR("public.item")
#endif

/* NSDraggingItem / drag session — 10.7+ */
#ifndef NSDraggingItem_STUB_DEFINED
#define NSDraggingItem_STUB_DEFINED
@class NSDraggingSession;
@interface NSDraggingItem : NSObject
- (instancetype)initWithPasteboardWriter:(id)writer;
- (void)setDraggingFrame:(NSRect)frame contents:(id)contents;
@property NSRect draggingFrame;
@property (copy) id imageComponentsProvider;
@end
@interface NSView (OakGNUstepCompatDragging)
- (NSDraggingSession*)beginDraggingSessionWithItems:(NSArray<NSDraggingItem*>*)items event:(NSEvent*)event source:(id)source;
- (void)showDefinitionForAttributedString:(NSAttributedString*)attr atPoint:(NSPoint)point;
@end
@interface NSDraggingSession : NSObject @end
#endif

/* NSEvent.stage — Force Touch / deep press (10.10.3+) */
@interface NSEvent (OakGNUstepCompatStage)
@property (readonly) NSInteger stage;
@property (readonly) CGFloat stageTransition;
@end

/* NSImageNameTouchBar* — system image identifiers */
#ifndef NSImageNameTouchBarGoUpTemplate
#define NSImageNameTouchBarGoUpTemplate       @"NSImageNameTouchBarGoUpTemplate"
#define NSImageNameTouchBarGoDownTemplate     @"NSImageNameTouchBarGoDownTemplate"
#define NSImageNameTouchBarGoForwardTemplate  @"NSImageNameTouchBarGoForwardTemplate"
#define NSImageNameTouchBarGoBackTemplate     @"NSImageNameTouchBarGoBackTemplate"
#define NSImageNameTouchBarAddTemplate        @"NSImageNameTouchBarAddTemplate"
#define NSImageNameTouchBarRemoveTemplate     @"NSImageNameTouchBarRemoveTemplate"
#define NSImageNameTouchBarSearchTemplate     @"NSImageNameTouchBarSearchTemplate"
#endif

/* NSButton.buttonWithImage:target:action: — 10.12 */
@interface NSButton (OakGNUstepCompatImageInit)
+ (NSButton*)buttonWithImage:(NSImage*)image target:(id)target action:(SEL)action;
@end

/* WebView stub — TextMate's HTMLOutput uses legacy WebKit. MVP stub;
 * real impl would embed WebKit2GTK or disable HTMLOutput entirely. */
#ifndef WebView_STUB_DEFINED
#define WebView_STUB_DEFINED
@interface WebView : NSView
@end
#endif

/* NSImage.imageWithSize:flipped:drawingHandler: — 10.8 */
@interface NSImage (OakGNUstepCompatDrawingHandler)
+ (NSImage*)imageWithSize:(NSSize)size flipped:(BOOL)drawingHandlerShouldBeCalledWithFlippedContext drawingHandler:(BOOL (^)(NSRect dstRect))drawingHandler;
@end

/* NSEvent local/global monitors (10.6+) */
@interface NSEvent (OakGNUstepCompatMonitor)
+ (id)addLocalMonitorForEventsMatchingMask:(NSEventMask)mask handler:(NSEvent* (^)(NSEvent*))block;
+ (id)addGlobalMonitorForEventsMatchingMask:(NSEventMask)mask handler:(void (^)(NSEvent*))block;
+ (void)removeMonitor:(id)monitor;
@end

/* NSPasteboard.readObjectsForClasses:options: (10.6+) */
@interface NSPasteboard (OakGNUstepCompat)
- (NSArray*)readObjectsForClasses:(NSArray*)classArray options:(NSDictionary*)options;
@end

/* NSView.fittingSize (10.7+) */
@interface NSView (OakGNUstepCompatFitting)
@property (readonly) NSSize fittingSize;
@end

/* os_log level checks */
#ifndef os_log_info_enabled
#define os_log_info_enabled(log) 0
#define os_log_debug_enabled(log) 0
#endif

/* Carbon kHIHotKeyModeAllDisabled — used by OakKeyEquivalentView */
#ifndef kHIHotKeyModeAllDisabled
#define kHIHotKeyModeAllDisabled 0
static inline void* PushSymbolicHotKeyMode(int mode) { (void)mode; return NULL; }
static inline OSStatus PopSymbolicHotKeyMode(void* token) { (void)token; return 0; }
#endif

/* LaunchServices LSCopyApplicationURLsForURL stub */
#ifndef LSCopyApplicationURLsForURL_DEFINED
#define LSCopyApplicationURLsForURL_DEFINED
static inline CFArrayRef LSCopyApplicationURLsForURL(CFURLRef url, LSRolesMask roles) { (void)url; (void)roles; return NULL; }
#endif

/* NSAppleEventDescriptor.listDescriptor */
@interface NSAppleEventDescriptor (OakGNUstepCompat)
+ (NSAppleEventDescriptor*)listDescriptor;
- (void)insertDescriptor:(NSAppleEventDescriptor*)desc atIndex:(NSInteger)idx;
@end

/* NSVisualEffect constants — compile-only aliases */
#ifndef NSVisualEffectMaterialHeaderView
#define NSVisualEffectMaterialHeaderView    10
#define NSVisualEffectMaterialTitlebar      3
#define NSVisualEffectMaterialMenu          5
#define NSVisualEffectMaterialPopover       6
#define NSVisualEffectMaterialSidebar       7
#define NSVisualEffectMaterialContentBackground 18
#define NSVisualEffectMaterialToolTip       17
#define NSVisualEffectMaterialHUDWindow     13
#define NSVisualEffectMaterialSheet         11
#define NSVisualEffectMaterialWindowBackground 12
#define NSVisualEffectMaterialUnderWindowBackground 15
#define NSVisualEffectMaterialUnderPageBackground   22
#define NSVisualEffectMaterialFullScreenUI  15
#define NSVisualEffectMaterialSelection     4
#endif

/* NSFileManager — trashItemAtURL:resultingItemURL:error: (10.8+) */
@interface NSFileManager (OakGNUstepCompatTrash)
- (BOOL)trashItemAtURL:(NSURL*)url resultingItemURL:(NSURL**)outResultingURL error:(NSError**)error;
@end

/* Accessibility title UI element */
@interface NSObject (OakGNUstepCompatAccessibilityMore)
- (id)accessibilityTitleUIElement;
- (void)setAccessibilityTitleUIElement:(id)element;
@end

/* NSMenu.font — 10.6+ */
@interface NSMenu (OakGNUstepCompat)
@property (strong) NSFont* font;
@end

/* LaunchServices / NSWorkspace app-for-URL (10.10+) */
#ifndef kLSRolesAll
#define kLSRolesAll 0xFFFFFFFF
#endif
@interface NSWorkspace (OakGNUstepCompat)
- (NSURL*)URLForApplicationToOpenURL:(NSURL*)url;
- (NSArray<NSURL*>*)URLsForApplicationsToOpenURL:(NSURL*)url;
@end

/* NSFontFeatureSettingsAttribute — name string constant */
#ifndef NSFontFeatureSettingsAttribute
#define NSFontFeatureSettingsAttribute @"NSFontFeatureSettingsAttribute"
#endif

/* NSSearchFieldDelegate protocol — empty stub */
#ifndef NSSearchFieldDelegate_STUB_DEFINED
#define NSSearchFieldDelegate_STUB_DEFINED
@protocol NSSearchFieldDelegate <NSTextFieldDelegate> @end
#endif

/* NSTitlebarAccessoryViewController — 10.10+ */
#ifndef NSTitlebarAccessoryViewController_STUB_DEFINED
#define NSTitlebarAccessoryViewController_STUB_DEFINED
@interface NSTitlebarAccessoryViewController : NSViewController
@property NSLayoutAttribute layoutAttribute;
@end
#endif

#endif /* !__APPLE__ */
#endif

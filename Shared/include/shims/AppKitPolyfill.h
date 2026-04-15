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

/* ==== Batch 2: Phase 4 UI-shell polyfills ==== */

/* NSBrowser 10.5+ properties */
#ifndef NSBrowser_BATCH2_DEFINED
#define NSBrowser_BATCH2_DEFINED
@interface NSBrowser (OakGNUstepCompatBatch2)
@property CGFloat defaultColumnWidth;
@property CGFloat minColumnWidth;
@property (readonly) NSInteger columnCount;
- (NSInteger)columnOfMatrix:(id)matrix;
- (NSInteger)selectedRowInColumn:(NSInteger)column;
- (id)loadedCellAtRow:(NSInteger)row column:(NSInteger)column;
@end
#endif

/* NSSplitViewItem — exists in GNUstep; add missing props */
@interface NSSplitViewItem (OakGNUstepCompatBatch2)
@property CGFloat minimumThickness;
@property CGFloat maximumThickness;
@property BOOL canCollapse;
@property NSLayoutPriority holdingPriority;
@end

/* NSSplitViewController — exists in GNUstep; add missing props */
@interface NSSplitViewController (OakGNUstepCompatBatch2)
- (void)addSplitViewItem:(NSSplitViewItem*)item;
@end

/* NSLayoutAnchor missing ≤/≥ toConstant variants */
@interface NSLayoutAnchorStub (OakGNUstepCompatBatch2)
- (NSLayoutConstraint*)constraintLessThanOrEqualToConstant:(CGFloat)c;
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToConstant:(CGFloat)c;
- (NSLayoutConstraint*)constraintLessThanOrEqualToAnchor:(NSLayoutAnchorStub*)anchor;
- (NSLayoutConstraint*)constraintLessThanOrEqualToAnchor:(NSLayoutAnchorStub*)anchor constant:(CGFloat)c;
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToAnchor:(NSLayoutAnchorStub*)anchor;
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToAnchor:(NSLayoutAnchorStub*)anchor constant:(CGFloat)c;
- (NSLayoutConstraint*)constraintEqualToAnchor:(NSLayoutAnchorStub*)anchor multiplier:(CGFloat)m;
@end

/* NSOutlineView 10.5+ */
@interface NSOutlineView (OakGNUstepCompatBatch2)
@property BOOL floatsGroupRows;
@property BOOL usesAutomaticRowHeights;
@property BOOL usesAlternatingRowBackgroundColors;
@property NSTableViewRowSizeStyle rowSizeStyle;
- (void)reloadItem:(id)item reloadChildren:(BOOL)reloadChildren;
@end

/* NSBezelStyleRoundedDisclosure / various new bezel styles */
#ifndef NSBezelStyleRoundedDisclosure
#define NSBezelStyleRoundedDisclosure ((NSBezelStyle)14)
#endif
#ifndef NSBezelStyleInline
#define NSBezelStyleInline ((NSBezelStyle)15)
#endif
#ifndef NSBezelStyleSmallSquare
#define NSBezelStyleSmallSquare ((NSBezelStyle)10)
#endif
#ifndef NSBezelStyleRoundRect
#define NSBezelStyleRoundRect ((NSBezelStyle)12)
#endif
#ifndef NSBezelStyleTexturedRounded
#define NSBezelStyleTexturedRounded ((NSBezelStyle)11)
#endif

/* NSMenuItem 10.5+ */
@interface NSMenuItem (OakGNUstepCompatBatch2)
@property (getter=isHidden) BOOL hidden;
@property BOOL allowsKeyEquivalentWhenHidden;
@property BOOL hiddenOrHasHiddenAncestor;
@end

/* NSApplication.helpMenu 10.6+ */
@interface NSApplication (OakGNUstepCompatBatch2Help)
@property (strong) NSMenu* helpMenu;
@property (strong) id<NSApplicationDelegate> delegateCompat;
@end

/* NSViewController 10.10+ */
@interface NSViewController (OakGNUstepCompatBatch2)
@property (copy) NSString* identifier;
@property (readonly, copy) NSArray<NSViewController*>* childViewControllers;
- (void)addChildViewController:(NSViewController*)vc;
- (void)removeFromParentViewController;
@end

/* NSColorSpace.sRGBColorSpace */
@interface NSColorSpace (OakGNUstepCompatBatch2)
+ (NSColorSpace*)sRGBColorSpace;
+ (NSColorSpace*)displayP3ColorSpace;
@end

/* NSAnimationContext 10.12+ */
@interface NSAnimationContext (OakGNUstepCompatBatch2)
@property BOOL allowsImplicitAnimation;
@end

/* NSDocumentEditedBinding constant */
#ifndef NSDocumentEditedBinding
#define NSDocumentEditedBinding @"documentEdited"
#endif
#ifndef NSToolTipBinding
#define NSToolTipBinding @"toolTip"
#endif
#ifndef NSTitleBinding
#define NSTitleBinding @"title"
#endif

/* CGKeyCode — from HIToolbox; make it a plain uint16 on non-Apple */
#ifndef CGKeyCode
typedef unsigned short CGKeyCode;
#endif

/* NSData 10.9+ */
@interface NSData (OakGNUstepCompatBatch2)
- (void)enumerateByteRangesUsingBlock:(void(^)(const void* bytes, NSRange byteRange, BOOL* stop))block;
@end

/* WebKit delegate protocols — GNUstep WebKit has looser typing */
#ifndef WebFrameLoadDelegate_STUB_DEFINED
#define WebFrameLoadDelegate_STUB_DEFINED
@class WebFrame, WebView, WebError, WebDataSource;
#ifndef WebScriptObject_CLASS_DEFINED
#define WebScriptObject_CLASS_DEFINED
@interface WebScriptObject : NSObject
- (id)evaluateWebScript:(NSString*)script;
- (void)setValue:(id)value forKey:(NSString*)key;
- (void)setWebScriptValueAtIndex:(unsigned)index value:(id)value;
- (id)webScriptValueAtIndex:(unsigned)index;
@end
#endif
@protocol WebFrameLoadDelegate <NSObject>
@optional
- (void)webView:(WebView*)sender didStartProvisionalLoadForFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didCommitLoadForFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didFinishLoadForFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didFailLoadWithError:(NSError*)error forFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didFailProvisionalLoadWithError:(NSError*)error forFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didReceiveTitle:(NSString*)title forFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didClearWindowObject:(id)windowScriptObject forFrame:(WebFrame*)frame;
- (void)webView:(WebView*)sender didChangeLocationWithinPageForFrame:(WebFrame*)frame;
@end
@protocol WebUIDelegate <NSObject> @end
@protocol WebPolicyDelegate <NSObject> @end
@protocol WebResourceLoadDelegate <NSObject> @end
@protocol WebDownloadDelegate <NSObject> @end
#endif
#ifndef WebFrameView_STUB_DEFINED
#define WebFrameView_STUB_DEFINED
@class WebFrameView;
@class WebFrame;
@class WebDataSource;
@class WebPreferences;
#endif

@interface WebView (OakGNUstepCompatBatch2)
@property (weak) id UIDelegate;
@property (weak) id frameLoadDelegate;
@property (weak) id policyDelegate;
@property (weak) id resourceLoadDelegate;
@property (weak) id downloadDelegate;
@property (readonly, strong) WebFrame* mainFrame;
@property (strong) WebPreferences* preferences;
@property NSString* applicationNameForUserAgent;
@property (copy) NSString* customUserAgent;
@end

/* NSFileManager replacement of FSFindFolder etc. — ignore */

/* NSSavePanel 10.6+ */
@interface NSSavePanel (OakGNUstepCompatBatch2)
@property (copy) NSString* nameFieldStringValue;
@property (copy) NSArray<NSString*>* allowedFileTypes;
@end

/* NSDocumentController 10.6+ */
@interface NSDocumentController (OakGNUstepCompatBatch2)
@property (readonly, copy) NSArray<NSURL*>* recentDocumentURLs;
- (void)clearRecentDocuments:(id)sender;
@end

/* NSString 10.7+ */
@interface NSString (OakGNUstepCompatBatch2)
- (NSString*)stringByStandardizingPath;
- (NSString*)stringByResolvingSymlinksInPath;
@end

/* NSURL 10.7+ */
@interface NSURL (OakGNUstepCompatBatch2)
@property (readonly, strong) NSURL* filePathURL;
@property (readonly, copy) NSString* absoluteString;
- (BOOL)getResourceValue:(out id*)value forKey:(NSString*)key error:(NSError**)err;
@end

/* ==== Batch 3 ==== */

/* NSWindow.toolbarStyle 11.0+ */
typedef NS_ENUM(NSInteger, NSWindowToolbarStyle) {
  NSWindowToolbarStyleAutomatic = 0,
  NSWindowToolbarStyleExpanded  = 1,
  NSWindowToolbarStylePreference= 2,
  NSWindowToolbarStyleUnified   = 3,
  NSWindowToolbarStyleUnifiedCompact = 4,
};
@interface NSWindow (OakGNUstepCompatBatch3)
@property NSWindowToolbarStyle toolbarStyle;
@property NSRect contentLayoutRect;
@property (readonly) NSInteger titlebarSeparatorStyle;
@end

/* NSGridRow/NSGridColumn 10.12+ */
/* NSGridRow/Column/Cell — GNUstep has the classes; add 10.12 properties */
@interface NSGridRow (OakGNUstepCompatBatch3)
@property NSGridRowAlignment rowAlignment;
@end

/* NSTextField.lineBreakMode */
@interface NSTextField (OakGNUstepCompatBatch3)
@property NSLineBreakMode lineBreakMode;
@property BOOL allowsDefaultTighteningForTruncation;
@end

/* NSURL.hasDirectoryPath / resourceType */
@interface NSURL (OakGNUstepCompatBatch3)
@property (readonly) BOOL hasDirectoryPath;
@end

/* NSMutableParagraphStyle */
@interface NSMutableParagraphStyle (OakGNUstepCompatBatch3)
- (void)setAllowsDefaultTighteningForTruncation:(BOOL)b;
@end

/* QuickLook — stubs */
#ifndef QLPreviewItem_STUB_DEFINED
#define QLPreviewItem_STUB_DEFINED
@protocol QLPreviewItem <NSObject> @end
#endif

/* NSView.removeConstraint / addConstraint (10.7+) */
@interface NSView (OakGNUstepCompatBatch3)
- (void)addConstraint:(NSLayoutConstraint*)c;
- (void)removeConstraint:(NSLayoutConstraint*)c;
- (void)addConstraints:(NSArray<NSLayoutConstraint*>*)cs;
- (void)removeConstraints:(NSArray<NSLayoutConstraint*>*)cs;
@end

/* WebKit policy extras */
#ifndef WebPolicyDecisionListener_STUB_DEFINED
#define WebPolicyDecisionListener_STUB_DEFINED
@protocol WebPolicyDecisionListener <NSObject>
- (void)use;
- (void)download;
- (void)ignore;
@end
@protocol WebOpenPanelResultListener <NSObject>
- (void)chooseFilename:(NSString*)fn;
- (void)chooseFilenames:(NSArray<NSString*>*)fns;
- (void)cancel;
@end
#endif

/* NSFileManager 10.5+ */
@interface NSFileManager (OakGNUstepCompatBatch3)
- (NSString*)destinationOfSymbolicLinkAtPath:(NSString*)p error:(NSError**)err;
@end

/* NSURL resource type key constant */
#ifndef NSURLFileResourceType_TYPEDEF
#define NSURLFileResourceType_TYPEDEF
typedef NSString* NSURLFileResourceType;
#endif
#ifndef NSURLFileResourceTypeSymbolicLink
#define NSURLFileResourceTypeSymbolicLink @"NSURLFileResourceTypeSymbolicLink"
#define NSURLFileResourceTypeRegular      @"NSURLFileResourceTypeRegular"
#define NSURLFileResourceTypeDirectory    @"NSURLFileResourceTypeDirectory"
#endif

/* CALayer.backgroundColor (on AppKitPolyfill's CALayer stub) */
@interface CALayer (OakGNUstepCompatBatch3)
@property CGColorRef backgroundColor;
@end

/* NSHelpManager.openHelpAnchor:inBook: */
@interface NSHelpManager (OakGNUstepCompatBatch3)
- (void)openHelpAnchor:(NSString*)a inBook:(NSString*)b;
@end


/* ==== Batch 4 ==== */

/* NSAlertStyle 10.12+ aliases */
#ifndef NSAlertStyleWarning
#define NSAlertStyleWarning      NSWarningAlertStyle
#define NSAlertStyleInformational NSInformationalAlertStyle
#define NSAlertStyleCritical     NSCriticalAlertStyle
#endif

@interface NSAlert (OakGNUstepCompatBatch4)
@property (readonly, strong) NSButton* suppressionButton;
@property BOOL showsSuppressionButton;
@end

/* NSImage stock-name constants 10.6+ */
#ifndef NSImageNameApplicationIcon
#define NSImageNameApplicationIcon       @"NSApplicationIcon"
#define NSImageNameStatusAvailable       @"NSStatusAvailable"
#define NSImageNameStatusUnavailable     @"NSStatusUnavailable"
#define NSImageNameStatusPartiallyAvailable @"NSStatusPartiallyAvailable"
#define NSImageNameStatusNone            @"NSStatusNone"
#define NSImageNameRefreshTemplate       @"NSRefreshTemplate"
#define NSImageNameStopProgressTemplate  @"NSStopProgressTemplate"
#endif

/* NSStackView 10.9+ */
@interface NSStackView (OakGNUstepCompatBatch4)
@property (copy) NSArray<NSView*>* views;
- (void)addView:(NSView*)v inGravity:(NSInteger)g;
@end

/* NSAccessibility 10.10+ */
@interface NSObject (OakGNUstepCompatBatch4Accessibility)
@property (copy) NSArray* accessibilitySharedFocusElements;
@property (copy) NSString* accessibilityRoleDescription;
@property BOOL accessibilityElement;
@end

/* NSWorkspace 10.13+ */
@interface NSWorkspace (OakGNUstepCompatBatch4)
@property (readonly) BOOL isVoiceOverEnabled;
- (NSArray<NSURL*>*)mountedVolumeURLsIncludingResourceValuesForKeys:(NSArray<NSString*>*)keys options:(NSUInteger)options;
@end
#ifndef NSWorkspaceDidRenameVolumeNotification
#define NSWorkspaceDidRenameVolumeNotification @"NSWorkspaceDidRenameVolumeNotification"
#endif
#ifndef NSVolumeEnumerationSkipHiddenVolumes
#define NSVolumeEnumerationSkipHiddenVolumes (1<<1)
#endif

/* NSFileManager 10.6/10.12+ */
@interface NSFileManager (OakGNUstepCompatBatch4)
@property (readonly, strong) NSURL* homeDirectoryForCurrentUser;
- (NSArray<NSURL*>*)mountedVolumeURLsIncludingResourceValuesForKeys:(NSArray<NSString*>*)keys options:(NSUInteger)options;
- (BOOL)getRelationship:(NSInteger*)outRelationship ofDirectory:(NSInteger)dir inDomain:(NSInteger)mask toItemAtURL:(NSURL*)url error:(NSError**)err;
- (BOOL)replaceItemAtURL:(NSURL*)originalItem withItemAtURL:(NSURL*)newItem backupItemName:(NSString*)backupItemName options:(NSUInteger)options resultingItemURL:(NSURL**)resultingURL error:(NSError**)err;
@end

#ifndef NSURLRelationship_DEFINED
#define NSURLRelationship_DEFINED
typedef NSInteger NSURLRelationship;
enum {
  NSURLRelationshipContains = 0,
  NSURLRelationshipSame     = 1,
  NSURLRelationshipOther    = 2,
};
#endif

/* NSURL 10.7/10.13+ */
@interface NSURL (OakGNUstepCompatBatch4)
+ (NSURL*)fileURLWithFileSystemRepresentation:(const char*)path isDirectory:(BOOL)isDir relativeToURL:(NSURL*)baseURL;
- (BOOL)setTemporaryResourceValue:(id)value forKey:(NSString*)key;
@end

/* NSTextField factory 10.12+ */
@interface NSTextField (OakGNUstepCompatBatch4)
+ (instancetype)textFieldWithString:(NSString*)s;
+ (instancetype)labelWithString:(NSString*)s;
+ (instancetype)wrappingLabelWithString:(NSString*)s;
@end

/* NSRelativeDateTimeFormatter 10.15+ — stub */
#ifndef NSRelativeDateTimeFormatter_STUB_DEFINED
#define NSRelativeDateTimeFormatter_STUB_DEFINED
@interface NSRelativeDateTimeFormatter : NSFormatter
- (NSString*)localizedStringFromDate:(NSDate*)d relativeToDate:(NSDate*)ref;
- (NSString*)localizedStringForDate:(NSDate*)d relativeToDate:(NSDate*)ref;
@end
#endif

/* os_activity_initiate */
#ifndef os_activity_initiate
#define os_activity_initiate(desc, flags, block) do { (void)(desc); (void)(flags); ((void(^)(void))(block))(); } while(0)
#endif

/* NSOutlineView 10.7+ batched insert/remove */
@interface NSOutlineView (OakGNUstepCompatBatch4)
- (void)insertItemsAtIndexes:(NSIndexSet*)is inParent:(id)parent withAnimation:(NSUInteger)opts;
- (void)removeItemsAtIndexes:(NSIndexSet*)is inParent:(id)parent withAnimation:(NSUInteger)opts;
- (void)beginUpdates;
- (void)endUpdates;
@end

/* NSDraggingSession 10.7+ */
@interface NSDraggingSession (OakGNUstepCompatBatch4)
@property (readonly, strong) NSPasteboard* draggingPasteboard;
@end

/* NSMenuItem.parentItem */
@interface NSMenuItem (OakGNUstepCompatBatch4)
@property (readonly, strong) NSMenuItem* parentItem;
@end

/* NSMenu 10.6+ propertiesToUpdate */
@interface NSMenu (OakGNUstepCompatBatch4)
- (NSUInteger)propertiesToUpdate;
@end
#ifndef NSMenuPropertyItemTitle
#define NSMenuPropertyItemTitle           (1<<0)
#define NSMenuPropertyItemAttributedTitle (1<<1)
#define NSMenuPropertyItemKeyEquivalent   (1<<2)
#define NSMenuPropertyItemImage           (1<<3)
#define NSMenuPropertyItemEnabled         (1<<4)
#define NSMenuPropertyItemAccessibilityDescription (1<<5)
#endif

/* NSButton.imageScaling */
@interface NSButton (OakGNUstepCompatBatch4)
@property NSImageScaling imageScaling;
@end

#ifndef NSImageBinding
#define NSImageBinding @"image"
#define NSValueBinding @"value"
#endif

/* NSTabViewController.fullScreenMinHeight (CommitWindow uses something similar) */
@interface NSViewController (OakGNUstepCompatBatch4)
@property CGFloat fullScreenMinHeight;
@end

/* NSProgressKindFile */
#ifndef NSProgressKindFile
#define NSProgressKindFile @"NSProgressKindFile"
#endif

/* CTL_HW for sysctl on Linux */
#ifndef CTL_HW
#define CTL_HW   6
#define HW_NCPU  3
#define HW_MEMSIZE 24
#endif

/* NSTextStorageDelegate protocol */
#ifndef NSTextStorageDelegate_STUB_DEFINED
#define NSTextStorageDelegate_STUB_DEFINED
@protocol NSTextStorageDelegate <NSObject> @end
#endif

/* NSSearchField wraps property */
@interface NSSearchField (OakGNUstepCompatBatch4)
@property BOOL wraps;
@end

/* NSLayoutConstraint.constant — GNUstep already has it; ensure setter exposed */
@interface NSLayoutConstraint (OakGNUstepCompatBatch4)
@property CGFloat constant;
@end

/* QLPreviewPanel data source / delegate protocol stubs */
#ifndef QLPreviewPanel_STUB_DEFINED
#define QLPreviewPanel_STUB_DEFINED
@protocol QLPreviewPanelDataSource <NSObject> @end
@protocol QLPreviewPanelDelegate <NSObject> @end
@class QLPreviewPanel;
#endif

/* ==== Batch 5 ==== */

@interface NSWindow (OakGNUstepCompatBatch5)
- (void)endSheet:(NSWindow*)sheet returnCode:(NSInteger)code;
- (void)endSheet:(NSWindow*)sheet;
@property NSInteger animationBehavior;
@property NSWindowStyleMask styleMask;
@end
#ifndef NSWindowAnimationBehaviorDefault
#define NSWindowAnimationBehaviorDefault       0
#define NSWindowAnimationBehaviorNone          2
#define NSWindowAnimationBehaviorDocumentWindow 3
#define NSWindowAnimationBehaviorUtilityWindow 4
#define NSWindowAnimationBehaviorAlertPanel    5
#endif

@interface NSFileManager (OakGNUstepCompatBatch5)
- (BOOL)createSymbolicLinkAtURL:(NSURL*)url withDestinationURL:(NSURL*)dest error:(NSError**)err;
@end
#ifndef NSFileManagerItemReplacementUsingNewMetadataOnly
#define NSFileManagerItemReplacementUsingNewMetadataOnly (1<<0)
#define NSFileManagerItemReplacementWithoutDeletingBackupItem (1<<1)
#endif

@interface NSOutlineView (OakGNUstepCompatBatch5)
- (void)moveItemAtIndex:(NSInteger)from inParent:(id)fromParent toIndex:(NSInteger)to inParent:(id)toParent;
- (void)draggingSession:(NSDraggingSession*)s endedAtPoint:(NSPoint)p operation:(NSDragOperation)op;
@end

@interface NSWorkspace (OakGNUstepCompatBatch5)
- (void)activateFileViewerSelectingURLs:(NSArray<NSURL*>*)urls;
@end

@interface NSMenuItem (OakGNUstepCompatBatch5)
@property (strong) NSView* view;
@end

@interface NSObject (OakGNUstepCompatBatch5Restoration)
- (void)invalidateRestorableState;
@end

@interface NSApplication (OakGNUstepCompatBatch5)
@property (readonly) BOOL isFullKeyboardAccessEnabled;
@end

@interface NSObject (OakGNUstepCompatBatch5A11y)
- (BOOL)isAccessibilityElement;
- (void)setAccessibilityElement:(BOOL)b;
@end

#ifndef NSProgressFileOperationKindDownloading
#define NSProgressFileOperationKindDownloading @"NSProgressFileOperationKindDownloading"
#endif

/* sysctl on Linux lives in sys/sysctl.h (deprecated but present) */
#if !defined(__APPLE__)
#include <unistd.h>
extern "C" int sysctl(int* name, unsigned nlen, void* oldp, size_t* oldlenp, void* newp, size_t newlen);
#endif

/* NSWindow.representedURL, isOnActiveSpace */
@interface NSWindow (OakGNUstepCompatBatch6)
@property (copy) NSURL* representedURL;
@property (readonly) BOOL isOnActiveSpace;
@end

/* NSURL.setResourceValue:forKey:error: */
@interface NSURL (OakGNUstepCompatBatch6)
- (BOOL)setResourceValue:(id)value forKey:(NSString*)key error:(NSError**)err;
@end
#ifndef NSURLTagNamesKey
#define NSURLTagNamesKey @"NSURLTagNamesKey"
#endif

/* NSTask.terminationHandler */
@interface NSTask (OakGNUstepCompatBatch6)
@property (copy) void(^terminationHandler)(NSTask*);
@end

/* NSProgress 10.9+ extra keys */
#ifndef NSProgressFileOperationKindKey
#define NSProgressFileOperationKindKey @"NSProgressFileOperationKindKey"
#define NSProgressEstimatedTimeRemainingKey @"NSProgressEstimatedTimeRemainingKey"
#define NSProgressFileTotalCountKey @"NSProgressFileTotalCountKey"
#define NSProgressFileCompletedCountKey @"NSProgressFileCompletedCountKey"
#endif

/* NSTableView 10.7+ */
@interface NSTableView (OakGNUstepCompatBatch6)
- (void)enumerateAvailableRowViewsUsingBlock:(void(^)(id rowView, NSInteger row))block;
@end

/* NSTitlebarAccessoryViewController.hidden */
@interface NSTitlebarAccessoryViewController (OakGNUstepCompatBatch6)
@property (getter=isHidden) BOOL hidden;
@end

/* NSViewController state restoration */
@interface NSViewController (OakGNUstepCompatBatch6Restore)
- (void)restoreStateWithCoder:(NSCoder*)coder;
- (void)encodeRestorableStateWithCoder:(NSCoder*)coder;
@end

/* NSKeyedArchiver 10.13+ */
@interface NSKeyedArchiver (OakGNUstepCompatBatch6)
@property (readonly, copy) NSData* encodedData;
- (instancetype)initRequiringSecureCoding:(BOOL)secure;
@end

/* NSEvent scrolling 10.7+ */
@interface NSEvent (OakGNUstepCompatBatch6)
@property (class, readonly) BOOL isSwipeTrackingFromScrollEventsEnabled;
@property (readonly) CGFloat scrollingDeltaX;
@property (readonly) CGFloat scrollingDeltaY;
@property (readonly) BOOL hasPreciseScrollingDeltas;
@end

/* NSOperatingSystemVersion */
#ifndef NSOperatingSystemVersion_DEFINED
#define NSOperatingSystemVersion_DEFINED
typedef struct {
  NSInteger majorVersion;
  NSInteger minorVersion;
  NSInteger patchVersion;
} NSOperatingSystemVersion;
@interface NSProcessInfo (OakGNUstepCompatBatch6)
@property (readonly) NSOperatingSystemVersion operatingSystemVersion;
@end
#endif

#ifndef NSAnimateBinding
#define NSAnimateBinding @"animate"
#define NSEnabledBinding @"enabled"
#define NSHiddenBinding  @"hidden"
#define NSSelectionIndexesBinding @"selectionIndexes"
#define NSContentArrayBinding     @"contentArray"
#endif

/* NSURLProtocol class methods (10.2+) — declare on class category */
@interface NSURLProtocol (OakGNUstepCompatBatch6)
+ (void)setProperty:(id)value forKey:(NSString*)key inRequest:(NSMutableURLRequest*)request;
+ (void)removePropertyForKey:(NSString*)key inRequest:(NSMutableURLRequest*)request;
+ (id)propertyForKey:(NSString*)key inRequest:(NSURLRequest*)request;
@end

/* NSEvent isFullScreenMode polyfill on NSView (used by tabs) */
@interface NSView (OakGNUstepCompatBatch6FullScreen)
@property (readonly) BOOL isInFullScreenMode;
@end

/* Carbon Icon Services constants */
#ifndef kGenericFolderIcon
#define kGenericFolderIcon   'fldr'
#define kGenericDocumentIcon 'docu'
#define kUnknownFSObjectIcon 'unfs'
#define kGenericApplicationIcon 'APPL'
#endif

/* NSTouchBarItemIdentifier extras */
#ifndef NSTouchBarItemIdentifierOtherItemsProxy
#define NSTouchBarItemIdentifierOtherItemsProxy @"NSTouchBarItemIdentifierOtherItemsProxy"
#define NSTouchBarItemIdentifierFlexibleSpace   @"NSTouchBarItemIdentifierFlexibleSpace"
#define NSTouchBarItemIdentifierFixedSpaceSmall @"NSTouchBarItemIdentifierFixedSpaceSmall"
#define NSTouchBarItemIdentifierFixedSpaceLarge @"NSTouchBarItemIdentifierFixedSpaceLarge"
#endif

/* NSEvent.trackSwipeEvent stub */
@interface NSEvent (OakGNUstepCompatBatch6Swipe)
- (void)trackSwipeEventWithOptions:(NSUInteger)opts dampenAmountThresholdMin:(CGFloat)minD max:(CGFloat)maxD usingHandler:(void(^)(CGFloat gestureAmount, NSEventPhase phase, BOOL isComplete, BOOL* stop))handler;
@end
#ifndef NSEventSwipeTrackingLockDirection
#define NSEventSwipeTrackingLockDirection (1<<0)
#define NSEventSwipeTrackingClampGestureAmount (1<<1)
#endif

/* NSOutlineView delegate optional method declarations as informal protocol */
@interface NSObject (OakGNUstepCompatBatch6OutlineDelegate)
- (BOOL)outlineView:(NSOutlineView*)outlineView isGroupItem:(id)item;
@end

/* WebKit deeper interface — make WebFrame a real class */
@class WebDataSource;
#ifndef WebFrame_CLASS_DEFINED
#define WebFrame_CLASS_DEFINED
@interface WebFrame : NSObject
@property (readonly, strong) WebFrameView* frameView;
@property (readonly, strong) WebDataSource* dataSource;
@property (readonly, strong) WebDataSource* provisionalDataSource;
@property (readonly, copy) NSString* name;
- (void)loadRequest:(NSURLRequest*)request;
- (void)loadHTMLString:(NSString*)html baseURL:(NSURL*)baseURL;
- (void)stopLoading;
- (void)reload;
- (id)windowObject;
- (NSString*)stringByEvaluatingJavaScriptFromString:(NSString*)script;
@end
@interface WebFrameView : NSView
@property (strong) NSView* documentView;
@end
@interface WebDataSource : NSObject
@property (readonly, strong) NSURLRequest* request;
@property (readonly, strong) NSURLResponse* response;
- (NSData*)data;
@end
@interface WebPreferences : NSObject
+ (WebPreferences*)standardPreferences;
@property BOOL javaScriptEnabled;
@property BOOL plugInsEnabled;
@end
#endif

@interface WebView (OakGNUstepCompatBatch6Class)
+ (void)registerURLSchemeAsLocal:(NSString*)scheme;
+ (BOOL)canShowMIMEType:(NSString*)mimeType;
@end

/* NSSegmentedControl factory 10.12+ */
@interface NSSegmentedControl (OakGNUstepCompatBatch7)
+ (instancetype)segmentedControlWithImages:(NSArray<NSImage*>*)images trackingMode:(NSSegmentSwitchTracking)mode target:(id)target action:(SEL)action;
+ (instancetype)segmentedControlWithLabels:(NSArray<NSString*>*)labels trackingMode:(NSSegmentSwitchTracking)mode target:(id)target action:(SEL)action;
@end

#ifndef NSImageNameTouchBarBookmarksTemplate
#define NSImageNameTouchBarBookmarksTemplate @"NSTouchBarBookmarksTemplate"
#define NSImageNameTouchBarHistoryTemplate   @"NSTouchBarHistoryTemplate"
#define NSImageNameTouchBarSearchTemplate    @"NSTouchBarSearchTemplate"
#define NSImageNameTouchBarShareTemplate     @"NSTouchBarShareTemplate"
#define NSImageNameTouchBarComposeTemplate   @"NSTouchBarComposeTemplate"
#endif

@interface NSWindow (OakGNUstepCompatBatch7)
- (void)toggleFullScreen:(id)sender;
@end

@interface NSDraggingItem (OakGNUstepCompatBatch7)
@property (strong) id item;
@end

@interface NSObject (OakGNUstepCompatBatch7DraggingInfo)
@property BOOL animatesToDestination;
@property NSInteger numberOfValidItemsForDrop;
@end

@interface NSAnimation (OakGNUstepCompatBatch7)
+ (id)animation;
+ (id)defaultAnimationForKey:(NSString*)key;
@end
@interface CABasicAnimation (OakGNUstepCompatBatch7)
+ (CABasicAnimation*)animation;
+ (CABasicAnimation*)animationWithKeyPath:(NSString*)keyPath;
@end
@interface NSObject (OakGNUstepCompatBatch7DefaultAnim)
+ (id)defaultAnimationForKey:(NSString*)key;
@end
@protocol NSDraggingInfoCompat
- (void)enumerateDraggingItemsWithOptions:(NSUInteger)opts forView:(NSView*)view classes:(NSArray*)classArray searchOptions:(NSDictionary*)so usingBlock:(void(^)(NSDraggingItem* draggingItem, NSInteger idx, BOOL* stop))block;
@end
@interface NSObject (OakGNUstepCompatBatch7DraggingInfoCalls) <NSDraggingInfoCompat>
@end

/* IconRef + Icon Services stubs */
#ifndef IconRef_DEFINED
#define IconRef_DEFINED
typedef struct OpaqueIconRef* IconRef;
#define kOnSystemDisk          (-32768)
#define kSystemIconsCreator    'macs'
#define kAliasBadgeIcon        'abdg'
#define kLockedBadgeIcon       'lbdg'
static inline OSStatus GetIconRefFromTypeInfo(uint32_t v1, OSType creator, CFStringRef typeInfo, CFStringRef extInfo, uint32_t flags, IconRef* iconRef) {
  (void)v1;(void)creator;(void)typeInfo;(void)extInfo;(void)flags; if(iconRef) *iconRef = NULL; return -1;
}
static inline OSStatus GetIconRef(int16_t vRefNum, OSType creator, OSType iconType, IconRef* iconRef) {
  (void)vRefNum;(void)creator;(void)iconType; if(iconRef) *iconRef = NULL; return -1;
}
static inline OSStatus ReleaseIconRef(IconRef ref) { (void)ref; return 0; }
@interface NSImage (OakGNUstepCompatBatch7Icon)
- (id)initWithIconRef:(IconRef)ref;
@end
#endif

@interface WebDataSource (OakGNUstepCompatBatch7)
@property (readonly, strong) NSURLRequest* initialRequest;
@end
@interface WebView (OakGNUstepCompatBatch7)
@property (readonly, copy) NSString* mainFrameTitle;
@property (readonly, copy) NSString* mainFrameURL;
@end

@interface NSFont (OakGNUstepCompatBatch7)
+ (NSFont*)monospacedDigitSystemFontOfSize:(CGFloat)size weight:(CGFloat)weight;
+ (NSFont*)monospacedSystemFontOfSize:(CGFloat)size weight:(CGFloat)weight;
@end
#ifndef NSFontWeightRegular
#define NSFontWeightUltraLight  -0.8
#define NSFontWeightThin        -0.6
#define NSFontWeightLight       -0.4
#define NSFontWeightRegular      0.0
#define NSFontWeightMedium       0.23
#define NSFontWeightSemibold     0.3
#define NSFontWeightBold         0.4
#define NSFontWeightHeavy        0.56
#define NSFontWeightBlack        0.62
#endif

/* representedObject is on NSCell + NSMenuItem + NSViewController; expose on NSObject */
@interface NSObject (OakGNUstepCompatBatch6Represented)
@property (strong) id representedObject;
- (NSString*)key;
@end

/* QLPreviewPanel — minimal class so [QLPreviewPanel sharedPreviewPanel] parses */
#ifndef QLPreviewPanel_CLASS_DEFINED
#define QLPreviewPanel_CLASS_DEFINED
@interface QLPreviewPanel : NSPanel
+ (QLPreviewPanel*)sharedPreviewPanel;
+ (BOOL)sharedPreviewPanelExists;
@property (weak) id<QLPreviewPanelDataSource> dataSource;
@property (weak) id<QLPreviewPanelDelegate> delegate;
- (void)reloadData;
- (void)refreshCurrentPreviewItem;
@end
#endif

/* NSMenuProperties typedef + NSUserInterfaceItemIdentifier */
#ifndef NSMenuProperties_DEFINED
#define NSMenuProperties_DEFINED
typedef NSUInteger NSMenuProperties;
#endif
#ifndef NSUserInterfaceItemIdentifier_DEFINED
#define NSUserInterfaceItemIdentifier_DEFINED
typedef NSString* NSUserInterfaceItemIdentifier;
#endif

/* NSTableColumnResizingOptions */
#ifndef NSTableColumnResizingOptions_DEFINED
#define NSTableColumnResizingOptions_DEFINED
typedef NSUInteger NSTableColumnResizingOptions;
#endif

/* NSEvent CGEvent bridge stubs */
@interface NSEvent (OakGNUstepCompatBatch5CGEvent)
+ (NSEvent*)eventWithCGEvent:(CGEventRef)e;
@property (readonly) CGEventRef CGEvent;
@end

/* LSSetDefaultHandlerForURLScheme — no-op on non-Apple */
#ifndef LSSetDefaultHandlerForURLScheme
static inline OSStatus LSSetDefaultHandlerForURLScheme(CFStringRef scheme, CFStringRef bundleID) {
  (void)scheme; (void)bundleID; return -1;
}
#endif

#endif /* !__APPLE__ */
#endif

/* Implementation of the GNUstep AppKit polyfills. Link once into libtmcore. */
#if !defined(__APPLE__)
#import "AppKitPolyfill.h"

@implementation NSURL (OakGNUstepCompat)
- (char const*)fileSystemRepresentation
{
	NSString* path = self.isFileURL ? self.path : nil;
	if(!path) return "";
	return [path fileSystemRepresentation];
}
@end

@implementation NSAttributedString (OakGNUstepCompat)
- (void)enumerateAttributesInRange:(NSRange)enumerationRange
                           options:(NSUInteger)opts
                        usingBlock:(void (^)(NSDictionary*, NSRange, BOOL*))block
{
	(void)opts;
	NSUInteger pos = enumerationRange.location;
	NSUInteger end = NSMaxRange(enumerationRange);
	BOOL stop = NO;
	while(pos < end && !stop)
	{
		NSRange effective;
		NSDictionary* attrs = [self attributesAtIndex:pos effectiveRange:&effective];
		NSRange clamped = NSIntersectionRange(effective, enumerationRange);
		block(attrs ?: @{}, clamped, &stop);
		pos = NSMaxRange(effective);
	}
}
@end

@implementation NSIndexSet (OakGNUstepCompat)
- (NSIndexSet*)indexesPassingTest:(BOOL (^)(NSUInteger, BOOL*))predicate
{
	NSMutableIndexSet* result = [NSMutableIndexSet indexSet];
	NSUInteger idx = [self firstIndex];
	BOOL stop = NO;
	while(idx != NSNotFound && !stop)
	{
		if(predicate(idx, &stop)) [result addIndex:idx];
		idx = [self indexGreaterThanIndex:idx];
	}
	return result;
}
@end

@implementation NSBundle (OakGNUstepCompat)
- (NSImage*)imageForResource:(NSString*)name
{
	for(NSString* ext in @[@"png", @"tiff", @"jpg", @"gif", @"pdf", @"icns"])
	{
		NSString* path = [self pathForResource:name ofType:ext];
		if(path)
			return [[NSImage alloc] initWithContentsOfFile:path];
	}
	return nil;
}
@end

@implementation CALayer @end
@implementation CABasicAnimation
+ (instancetype)animationWithKeyPath:(NSString*)path { (void)path; return [self new]; }
@end

@implementation NSLayoutAnchorStub
- (void*)constraintEqualToAnchor:(id)other { (void)other; return (void*)1; }
- (void*)constraintEqualToAnchor:(id)other constant:(CGFloat)c { (void)other; (void)c; return (void*)1; }
- (void*)constraintGreaterThanOrEqualToAnchor:(id)other { (void)other; return (void*)1; }
- (void*)constraintLessThanOrEqualToAnchor:(id)other { (void)other; return (void*)1; }
@end

@implementation NSView (OakGNUstepCompat)
- (BOOL)wantsLayer { return NO; }
- (void)setWantsLayer:(BOOL)w { (void)w; }
- (CALayer*)layer { return nil; }
- (void)setLayer:(CALayer*)l { (void)l; }
@end

@implementation NSView (OakGNUstepCompatAnchors)
- (NSLayoutAnchorStub*)leadingAnchor        { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)trailingAnchor       { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)topAnchor            { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)bottomAnchor         { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)widthAnchor          { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)heightAnchor         { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)centerXAnchor        { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)centerYAnchor        { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)firstBaselineAnchor  { return [NSLayoutAnchorStub new]; }
- (NSLayoutAnchorStub*)lastBaselineAnchor   { return [NSLayoutAnchorStub new]; }
@end

@implementation NSObject (OakGNUstepCompatAccessibility)
- (NSString*)accessibilityLabel { return nil; }
- (void)setAccessibilityLabel:(NSString*)label { (void)label; }
- (NSString*)accessibilityRole  { return nil; }
- (void)setAccessibilityRole:(NSString*)role { (void)role; }
- (NSString*)accessibilityValue { return nil; }
- (void)setAccessibilityValue:(id)value { (void)value; }
@end

@implementation NSButton (OakGNUstepCompat)
+ (NSButton*)buttonWithTitle:(NSString*)title target:(id)target action:(SEL)action {
	NSButton* b = [[NSButton alloc] initWithFrame:NSMakeRect(0,0,100,24)];
	[b setTitle:title ?: @""]; [b setTarget:target]; [b setAction:action];
	[b setBezelStyle:NSRoundedBezelStyle];
	return b;
}
+ (NSButton*)checkboxWithTitle:(NSString*)title target:(id)target action:(SEL)action {
	NSButton* b = [self buttonWithTitle:title target:target action:action];
	[b setButtonType:NSSwitchButton];
	return b;
}
@end

@implementation NSTextField (OakGNUstepCompat)
+ (NSTextField*)labelWithString:(NSString*)stringValue {
	NSTextField* f = [[NSTextField alloc] initWithFrame:NSMakeRect(0,0,100,20)];
	[f setStringValue:stringValue ?: @""]; [f setBezeled:NO]; [f setDrawsBackground:NO];
	[f setEditable:NO]; [f setSelectable:YES];
	return f;
}
+ (NSTextField*)wrappingLabelWithString:(NSString*)stringValue {
	NSTextField* f = [self labelWithString:stringValue];
	[[f cell] setWraps:YES];
	return f;
}
@end

@implementation NSView (OakGNUstepCompatTracking)
- (void)updateTrackingAreas { }
- (void)addTrackingArea:(id)area { (void)area; }
- (void)removeTrackingArea:(id)area { (void)area; }
@end

@implementation NSLayoutAnchorStub
- (NSLayoutConstraint*)constraintEqualToAnchor:(id)other { (void)other; return nil; }
- (NSLayoutConstraint*)constraintEqualToAnchor:(id)other constant:(CGFloat)c { (void)other; (void)c; return nil; }
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToAnchor:(id)other { (void)other; return nil; }
- (NSLayoutConstraint*)constraintLessThanOrEqualToAnchor:(id)other { (void)other; return nil; }
- (NSLayoutConstraint*)constraintEqualToConstant:(CGFloat)c { (void)c; return nil; }
- (NSLayoutConstraint*)constraintGreaterThanOrEqualToConstant:(CGFloat)c { (void)c; return nil; }
@end

extern "C" std::string OakMoveToTrash(std::string const&);
@implementation NSFileManager (OakGNUstepCompatTrash)
- (BOOL)trashItemAtURL:(NSURL*)url resultingItemURL:(NSURL**)outResultingURL error:(NSError**)error {
	std::string src = url.path.fileSystemRepresentation ?: "";
	std::string dst = OakMoveToTrash(src);
	if(dst.empty() || dst == NULL_STR) { if(error) *error = [NSError errorWithDomain:NSPOSIXErrorDomain code:errno userInfo:nil]; return NO; }
	if(outResultingURL) *outResultingURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:dst.c_str()]];
	return YES;
}
@end

@implementation NSObject (OakGNUstepCompatAccessibilityMore)
- (id)accessibilityTitleUIElement { return nil; }
- (void)setAccessibilityTitleUIElement:(id)element { (void)element; }
@end

@implementation NSMenu (OakGNUstepCompat)
static char kMenuFontKey;
- (NSFont*)font { return objc_getAssociatedObject(self, &kMenuFontKey); }
- (void)setFont:(NSFont*)f { objc_setAssociatedObject(self, &kMenuFontKey, f, OBJC_ASSOCIATION_RETAIN); }
@end

@implementation NSWorkspace (OakGNUstepCompat)
- (NSURL*)URLForApplicationToOpenURL:(NSURL*)url { (void)url; return nil; }
- (NSArray<NSURL*>*)URLsForApplicationsToOpenURL:(NSURL*)url { (void)url; return @[]; }
@end

@implementation NSControl (OakGNUstepCompatControlSize)
- (NSControlSize)controlSize { return [[self cell] controlSize]; }
- (void)setControlSize:(NSControlSize)s { [[self cell] setControlSize:s]; }
@end

@implementation NSObject (OakGNUstepCompatAnimator)
- (instancetype)animator { return self; }
@end

@implementation NSTableView (OakGNUstepCompatStyle)
static char kTableStyleKey;
- (NSTableViewStyle)style { return (NSTableViewStyle)[objc_getAssociatedObject(self, &kTableStyleKey) integerValue]; }
- (void)setStyle:(NSTableViewStyle)s { objc_setAssociatedObject(self, &kTableStyleKey, @(s), OBJC_ASSOCIATION_RETAIN); }
@end

@implementation NSView (OakGNUstepCompatIntrinsic)
- (void)invalidateIntrinsicContentSize { [self setNeedsDisplay:YES]; }
- (NSSize)intrinsicContentSize { return NSMakeSize(-1, -1); /* NSViewNoIntrinsicMetric sentinel */ }
- (NSArray*)constraints { return @[]; }
@end

@implementation NSObject (OakGNUstepCompatA11yElement)
static char kA11yElementKey;
- (BOOL)accessibilityElement { return [objc_getAssociatedObject(self, &kA11yElementKey) boolValue]; }
- (void)setAccessibilityElement:(BOOL)f { objc_setAssociatedObject(self, &kA11yElementKey, @(f), OBJC_ASSOCIATION_RETAIN); }
@end

@implementation WebView @end

@implementation NSImage (OakGNUstepCompatTemplate)
static char kTemplateKey;
- (BOOL)isTemplate { return [objc_getAssociatedObject(self, &kTemplateKey) boolValue]; }
- (void)setTemplate:(BOOL)t { objc_setAssociatedObject(self, &kTemplateKey, @(t), OBJC_ASSOCIATION_RETAIN); }
@end

@implementation NSView (OakGNUstepCompatTrackingAreas)
- (NSArray*)trackingAreas { return @[]; }
- (void)removeConstraints:(NSArray*)constraints { (void)constraints; }
- (void)addConstraints:(NSArray*)constraints { (void)constraints; }
@end


@implementation NSColor (OakGNUstepCompatCGColor)
+ (NSColor*)colorWithCGColor:(CGColorRef)cgColor {
	if(!cgColor) return [NSColor blackColor];
	const CGFloat* comps = CGColorGetComponents(cgColor);
	size_t n = CGColorGetNumberOfComponents(cgColor);
	if(n >= 4) return [NSColor colorWithCalibratedRed:comps[0] green:comps[1] blue:comps[2] alpha:comps[3]];
	if(n >= 3) return [NSColor colorWithCalibratedRed:comps[0] green:comps[1] blue:comps[2] alpha:1.0];
	if(n >= 2) return [NSColor colorWithCalibratedWhite:comps[0] alpha:comps[1]];
	return [NSColor blackColor];
}
- (CGColorRef)CGColor {
	CGFloat r = 0, g = 0, b = 0, a = 1;
	NSColor* c = [self colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
	if(c) [c getRed:&r green:&g blue:&b alpha:&a];
	CGFloat comps[4] = { r, g, b, a };
	CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
	CGColorRef result = CGColorCreate(space, comps);
	CGColorSpaceRelease(space);
	return (CGColorRef)CFAutorelease(result);
}
@end

@implementation NSTouchBar (OakGNUstepCompatItems)
static char kDefaultItemsKey, kCustomAllowedKey, kCustomIDKey, kDelegateKey;
- (NSArray*)defaultItemIdentifiers { return objc_getAssociatedObject(self, &kDefaultItemsKey); }
- (void)setDefaultItemIdentifiers:(NSArray*)a { objc_setAssociatedObject(self, &kDefaultItemsKey, a, OBJC_ASSOCIATION_COPY); }
- (NSArray*)customizationAllowedItemIdentifiers { return objc_getAssociatedObject(self, &kCustomAllowedKey); }
- (void)setCustomizationAllowedItemIdentifiers:(NSArray*)a { objc_setAssociatedObject(self, &kCustomAllowedKey, a, OBJC_ASSOCIATION_COPY); }
- (NSString*)customizationIdentifier { return objc_getAssociatedObject(self, &kCustomIDKey); }
- (void)setCustomizationIdentifier:(NSString*)s { objc_setAssociatedObject(self, &kCustomIDKey, s, OBJC_ASSOCIATION_COPY); }
- (id<NSTouchBarDelegate>)delegate { return objc_getAssociatedObject(self, &kDelegateKey); }
- (void)setDelegate:(id<NSTouchBarDelegate>)d { objc_setAssociatedObject(self, &kDelegateKey, d, OBJC_ASSOCIATION_ASSIGN); }
@end

@implementation NSImage (OakGNUstepCompatLockFocus)
- (void)lockFocusFlipped:(BOOL)flipped { (void)flipped; [self lockFocus]; }
@end

@implementation NSView (OakGNUstepCompatInputContext)
- (id)inputContext { return nil; }
@end

@implementation NSObject (OakGNUstepCompatA11yArray)
- (NSUInteger)accessibilityArrayAttributeCount:(NSString*)attribute { (void)attribute; return 0; }
- (NSArray*)accessibilityArrayAttributeValues:(NSString*)attribute index:(NSUInteger)index maxCount:(NSUInteger)maxCount { (void)attribute; (void)index; (void)maxCount; return @[]; }
- (NSUInteger)accessibilityIndexOfChild:(id)child { (void)child; return NSNotFound; }
@end

@implementation NSAccessibilityCustomRotorSearchParameters
@synthesize filterString = _filterString, currentItem = _currentItem, searchDirection = _searchDirection;
@end

@implementation NSAccessibilityCustomRotorItemResult (OakGNUstepCompatInit)
static char kTargetElementKey, kTargetRangeKey;
- (instancetype)initWithTargetElement:(id)target {
	self = [super init];
	if(self) objc_setAssociatedObject(self, &kTargetElementKey, target, OBJC_ASSOCIATION_RETAIN);
	return self;
}
- (id)targetElement { return objc_getAssociatedObject(self, &kTargetElementKey); }
- (void)setTargetElement:(id)t { objc_setAssociatedObject(self, &kTargetElementKey, t, OBJC_ASSOCIATION_RETAIN); }
- (NSRange)targetRange { NSValue* v = objc_getAssociatedObject(self, &kTargetRangeKey); return v ? [v rangeValue] : NSMakeRange(0,0); }
- (void)setTargetRange:(NSRange)r { objc_setAssociatedObject(self, &kTargetRangeKey, [NSValue valueWithRange:r], OBJC_ASSOCIATION_RETAIN); }
static char kCustomLabelKey;
- (NSString*)customLabel { return objc_getAssociatedObject(self, &kCustomLabelKey); }
- (void)setCustomLabel:(NSString*)s { objc_setAssociatedObject(self, &kCustomLabelKey, s, OBJC_ASSOCIATION_COPY); }
@end

@implementation NSSpellChecker (OakGNUstepCompat)
- (BOOL)hasLearnedWord:(NSString*)word { (void)word; return NO; }
- (NSArray*)guessesForWordRange:(NSRange)range inString:(NSString*)str language:(NSString*)lang inSpellDocumentWithTag:(NSInteger)tag {
	(void)range; (void)lang;
	return [self guessesForWord:str] ?: @[];
}
- (void)learnWord:(NSString*)word { (void)word; }
- (void)unlearnWord:(NSString*)word { (void)word; }
@end

@implementation NSCustomTouchBarItem (OakGNUstepCompat)
static char kCTBViewKey, kCTBIdentKey;
- (instancetype)initWithIdentifier:(NSTouchBarItemIdentifier)identifier {
	self = [super init];
	if(self) objc_setAssociatedObject(self, &kCTBIdentKey, [identifier copy], OBJC_ASSOCIATION_RETAIN);
	return self;
}
- (NSView*)view { return objc_getAssociatedObject(self, &kCTBViewKey); }
- (void)setView:(NSView*)v { objc_setAssociatedObject(self, &kCTBViewKey, v, OBJC_ASSOCIATION_RETAIN); }
static char kCTBPriorityKey, kCTBLabelKey;
- (float)visibilityPriority { return [objc_getAssociatedObject(self, &kCTBPriorityKey) floatValue]; }
- (void)setVisibilityPriority:(float)p { objc_setAssociatedObject(self, &kCTBPriorityKey, @(p), OBJC_ASSOCIATION_RETAIN); }
- (NSString*)customizationLabel { return objc_getAssociatedObject(self, &kCTBLabelKey); }
- (void)setCustomizationLabel:(NSString*)s { objc_setAssociatedObject(self, &kCTBLabelKey, s, OBJC_ASSOCIATION_COPY); }
@end

@implementation NSButton (OakGNUstepCompatImageInit)
+ (NSButton*)buttonWithImage:(NSImage*)image target:(id)target action:(SEL)action {
	NSButton* b = [[NSButton alloc] initWithFrame:NSMakeRect(0,0,24,24)];
	[b setImage:image]; [b setTarget:target]; [b setAction:action];
	[b setBordered:NO];
	return b;
}
@end

@implementation NSAlert (OakGNUstepCompatAccessory)
static char kAccessoryViewKey;
- (NSView*)accessoryView { return objc_getAssociatedObject(self, &kAccessoryViewKey); }
- (void)setAccessoryView:(NSView*)v { objc_setAssociatedObject(self, &kAccessoryViewKey, v, OBJC_ASSOCIATION_RETAIN); }
- (void)layout { }
@end

@implementation NSDraggingItem {
  NSRect _draggingFrame;
  id _imageComponentsProvider;
}
- (instancetype)initWithPasteboardWriter:(id)writer { (void)writer; return [super init]; }
- (NSRect)draggingFrame { return _draggingFrame; }
- (void)setDraggingFrame:(NSRect)f { _draggingFrame = f; }
- (void)setDraggingFrame:(NSRect)f contents:(id)contents { (void)contents; _draggingFrame = f; }
- (id)imageComponentsProvider { return _imageComponentsProvider; }
- (void)setImageComponentsProvider:(id)p { _imageComponentsProvider = p; }
@end
@implementation NSDraggingSession @end

@implementation NSView (OakGNUstepCompatDragging)
- (NSDraggingSession*)beginDraggingSessionWithItems:(NSArray<NSDraggingItem*>*)items event:(NSEvent*)event source:(id)source {
	(void)items; (void)event; (void)source; return [NSDraggingSession new];
}
- (void)showDefinitionForAttributedString:(NSAttributedString*)attr atPoint:(NSPoint)point { (void)attr; (void)point; }
@end

@implementation NSEvent (OakGNUstepCompatStage)
- (NSInteger)stage { return 0; }
- (CGFloat)stageTransition { return 0; }
@end

@implementation NSSegmentedControl (OakGNUstepCompatTracking)
static char kTrackingModeKey;
- (NSSegmentSwitchTracking)trackingMode { return (NSSegmentSwitchTracking)[objc_getAssociatedObject(self, &kTrackingModeKey) integerValue]; }
- (void)setTrackingMode:(NSSegmentSwitchTracking)m { objc_setAssociatedObject(self, &kTrackingModeKey, @(m), OBJC_ASSOCIATION_RETAIN); }
@end

@implementation NSImage (OakGNUstepCompatA11yDescription)
static char kA11yImgDescKey;
- (NSString*)accessibilityDescription { return objc_getAssociatedObject(self, &kA11yImgDescKey); }
- (void)setAccessibilityDescription:(NSString*)d { objc_setAssociatedObject(self, &kA11yImgDescKey, d, OBJC_ASSOCIATION_COPY); }
@end

@implementation NSImage (OakGNUstepCompatDrawingHandler)
+ (NSImage*)imageWithSize:(NSSize)size flipped:(BOOL)flipped drawingHandler:(BOOL (^)(NSRect))handler {
	NSImage* image = [[NSImage alloc] initWithSize:size];
	[image lockFocus];
	if(handler) handler(NSMakeRect(0, 0, size.width, size.height));
	[image unlockFocus];
	return image;
}
@end

@implementation NSStringDrawingContext
@synthesize minimumScaleFactor = _minimumScaleFactor, actualScaleFactor = _actualScaleFactor, totalBounds = _totalBounds;
@end

@implementation NSApplication (OakGNUstepCompat)
- (NSAppearance*)effectiveAppearance { return nil; }
@end

@implementation NSObject (OakGNUstepCompatAccessibilityOld)
- (NSArray*)accessibilityAttributeNames { return @[]; }
- (id)accessibilityAttributeValue:(NSString*)attribute { (void)attribute; return nil; }
- (BOOL)accessibilityIsAttributeSettable:(NSString*)attribute { (void)attribute; return NO; }
- (void)accessibilitySetValue:(id)value forAttribute:(NSString*)attribute { (void)value; (void)attribute; }
@end

@implementation NSCell (OakGNUstepCompat)
- (CGFloat)rowHeightForText:(NSString*)text { (void)text; return 17; }
@end

@implementation NSPasteboard (OakGNUstepCompatWrite)
- (NSInteger)clearContents { [self declareTypes:@[] owner:nil]; return 0; }
- (BOOL)writeObjects:(NSArray*)objects { (void)objects; return NO; }
@end

@implementation NSWindow (OakGNUstepCompatTitlebar)
- (void)addTitlebarAccessoryViewController:(NSTitlebarAccessoryViewController*)ctrl { (void)ctrl; }
- (void)removeTitlebarAccessoryViewControllerAtIndex:(NSInteger)idx { (void)idx; }
@end

@implementation NSTableView (OakGNUstepCompat)
static char kAllowsTypeSelectKey;
- (BOOL)allowsTypeSelect { return [objc_getAssociatedObject(self, &kAllowsTypeSelectKey) boolValue]; }
- (void)setAllowsTypeSelect:(BOOL)v { objc_setAssociatedObject(self, &kAllowsTypeSelectKey, @(v), OBJC_ASSOCIATION_RETAIN); }
@end

@implementation NSAppleEventDescriptor (OakGNUstepCompatDescriptors)
+ (NSAppleEventDescriptor*)descriptorWithDescriptorType:(DescType)descriptorType data:(NSData*)data {
	(void)descriptorType; (void)data; return [[self alloc] init];
}
+ (NSAppleEventDescriptor*)appleEventWithEventClass:(AEEventClass)ec eventID:(AEEventID)eid targetDescriptor:(NSAppleEventDescriptor*)target returnID:(AEReturnID)rid transactionID:(AETransactionID)tid {
	(void)ec; (void)eid; (void)target; (void)rid; (void)tid;
	return [[self alloc] init];
}
- (void)setParamDescriptor:(NSAppleEventDescriptor*)desc forKeyword:(AEKeyword)keyword { (void)desc; (void)keyword; }
@end

@implementation NSWorkspace (OakGNUstepCompatLaunch)
- (BOOL)launchApplicationAtURL:(NSURL*)url options:(NSUInteger)opts configuration:(NSDictionary*)config error:(NSError**)err {
	(void)opts; (void)config;
	if(err) *err = nil;
	return [self openURL:url];
}
@end

@implementation NSEvent (OakGNUstepCompatMonitor)
+ (id)addLocalMonitorForEventsMatchingMask:(NSEventMask)mask handler:(NSEvent* (^)(NSEvent*))block { (void)mask; (void)block; return nil; }
+ (id)addGlobalMonitorForEventsMatchingMask:(NSEventMask)mask handler:(void (^)(NSEvent*))block { (void)mask; (void)block; return nil; }
+ (void)removeMonitor:(id)monitor { (void)monitor; }
@end

@implementation NSPasteboard (OakGNUstepCompat)
- (NSArray*)readObjectsForClasses:(NSArray*)classArray options:(NSDictionary*)options {
	(void)classArray; (void)options; return @[];
}
@end

@implementation NSView (OakGNUstepCompatFitting)
- (NSSize)fittingSize { return self.frame.size; }
@end

@implementation NSAppleEventDescriptor (OakGNUstepCompat)
+ (NSAppleEventDescriptor*)listDescriptor { return [[NSAppleEventDescriptor alloc] init]; }
- (void)insertDescriptor:(NSAppleEventDescriptor*)desc atIndex:(NSInteger)idx { (void)desc; (void)idx; }
@end

@implementation NSTitlebarAccessoryViewController {
  NSLayoutAttribute _layoutAttribute;
}
- (NSLayoutAttribute)layoutAttribute { return _layoutAttribute; }
- (void)setLayoutAttribute:(NSLayoutAttribute)a { _layoutAttribute = a; }
@end

@implementation NSUserDefaults (OakGNUstepCompat)
- (instancetype)initWithSuiteName:(NSString*)suiteName {
	(void)suiteName;
	return [self init];
}
@end

#endif

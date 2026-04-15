#ifndef KVDB_STUB_H
#define KVDB_STUB_H
/* Stub for vendor/kvdb (submodule not checked out on Linux). */
#ifdef __OBJC__
#import <Foundation/Foundation.h>
@interface KVDB : NSObject
+ (instancetype)sharedDBUsingFile:(NSString*)file inDirectory:(NSString*)dir;
- (id)objectForKey:(NSString*)key;
- (void)setObject:(id)obj forKey:(NSString*)key;
- (void)removeObjectForKey:(NSString*)key;
- (NSArray*)allKeys;
- (void)enumerateKeysAndObjectsUsingBlock:(void(^)(NSString* key, id obj, BOOL* stop))block;
@end
#endif
#endif

#ifndef USERNOTIFICATIONS_STUB_H
#define USERNOTIFICATIONS_STUB_H
#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, UNAuthorizationOptions) {
  UNAuthorizationOptionBadge = 1<<0,
  UNAuthorizationOptionSound = 1<<1,
  UNAuthorizationOptionAlert = 1<<2,
};
typedef NS_OPTIONS(NSUInteger, UNNotificationPresentationOptions) {
  UNNotificationPresentationOptionBadge = 1<<0,
  UNNotificationPresentationOptionSound = 1<<1,
  UNNotificationPresentationOptionAlert = 1<<2,
};

@class UNNotification, UNNotificationResponse, UNNotificationRequest;
@class UNUserNotificationCenter, UNNotificationContent, UNMutableNotificationContent;
@class UNNotificationTrigger;

@protocol UNUserNotificationCenterDelegate <NSObject>
@optional
- (void)userNotificationCenter:(UNUserNotificationCenter*)c
       didReceiveNotificationResponse:(UNNotificationResponse*)r
                withCompletionHandler:(void(^)(void))handler;
- (void)userNotificationCenter:(UNUserNotificationCenter*)c
       willPresentNotification:(UNNotification*)n
         withCompletionHandler:(void(^)(UNNotificationPresentationOptions))handler;
@end

@interface UNNotificationRequest : NSObject
@property (readonly, copy) NSString* identifier;
+ (instancetype)requestWithIdentifier:(NSString*)id content:(UNNotificationContent*)c trigger:(UNNotificationTrigger*)t;
@property (readonly, strong) UNNotificationContent* content;
@end

@interface UNNotificationContent : NSObject
@property (copy) NSString* title;
@property (copy) NSString* subtitle;
@property (copy) NSString* body;
@property (copy) NSDictionary* userInfo;
@end
@interface UNMutableNotificationContent : UNNotificationContent
@end

@interface UNNotification : NSObject
@property (readonly, strong) UNNotificationRequest* request;
@end

@interface UNNotificationResponse : NSObject
@property (readonly, strong) UNNotification* notification;
@property (readonly, copy) NSString* actionIdentifier;
@end

@interface UNUserNotificationCenter : NSObject
@property (class, readonly, strong) UNUserNotificationCenter* currentNotificationCenter;
@property (weak) id<UNUserNotificationCenterDelegate> delegate;
- (void)requestAuthorizationWithOptions:(UNAuthorizationOptions)opts completionHandler:(void(^)(BOOL granted, NSError* error))h;
- (void)addNotificationRequest:(UNNotificationRequest*)r withCompletionHandler:(void(^)(NSError* error))h;
- (void)removeAllDeliveredNotifications;
- (void)removeDeliveredNotificationsWithIdentifiers:(NSArray<NSString*>*)ids;
@end

#endif

#import "OakFoundation.h"
#import "NSString Additions.h"

std::string OakMoveToTrash (std::string const& path)
{
#if defined(__APPLE__)
	NSURL* resultingItemURL;
	if([NSFileManager.defaultManager trashItemAtURL:[NSURL fileURLWithPath:[NSString stringWithCxxString:path]] resultingItemURL:&resultingItemURL error:nil])
			return resultingItemURL.fileSystemRepresentation ?: NULL_STR;
	else	return NULL_STR;
#else
	/* XDG trash spec: move into ~/.local/share/Trash/files/<basename>.
	 * Minimal — no per-file metadata; good enough for MVP. */
	const char* home = getenv("HOME");
	if(!home) return NULL_STR;
	std::string trashDir = std::string(home) + "/.local/share/Trash/files";
	(void)mkdir((std::string(home) + "/.local").c_str(), 0700);
	(void)mkdir((std::string(home) + "/.local/share").c_str(), 0700);
	(void)mkdir((std::string(home) + "/.local/share/Trash").c_str(), 0700);
	(void)mkdir(trashDir.c_str(), 0700);
	size_t slash = path.find_last_of('/');
	std::string base = (slash == std::string::npos) ? path : path.substr(slash+1);
	std::string dest = trashDir + "/" + base;
	if(rename(path.c_str(), dest.c_str()) == 0)
		return dest;
	return NULL_STR;
#endif
}

BOOL OakIsEmptyString (NSString* str)
{
	return !str || [str isEqualToString:@""];
}

BOOL OakNotEmptyString (NSString* str)
{
	return str && ![str isEqualToString:@""];
}

void OakObserveUserDefaults (id <OakUserDefaultsObserver> obj)
{
	__weak id <OakUserDefaultsObserver> weakObject = obj;
	__weak __block id token = [NSNotificationCenter.defaultCenter addObserverForName:NSUserDefaultsDidChangeNotification object:NSUserDefaults.standardUserDefaults queue:NSOperationQueue.mainQueue usingBlock:^(NSNotification* notification){
		if(id <OakUserDefaultsObserver> strongObject = weakObject)
				[strongObject userDefaultsDidChange:notification];
		else	[NSNotificationCenter.defaultCenter removeObserver:token];
	}];
}

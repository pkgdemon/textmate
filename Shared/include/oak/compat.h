#ifndef COMPAT_H_RD1Z6YZA
#define COMPAT_H_RD1Z6YZA

#if !defined(__APPLE__)
#include <sys/utsname.h>
#include <string>
#endif

namespace oak
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#if defined(__APPLE__)
	inline size_t get_gestalt (OSType selector)
	{
		SInt32 res;
		return Gestalt(selector, &res) == noErr ? res : 0;
	}

	inline size_t os_major () { return get_gestalt(gestaltSystemVersionMajor); }
	inline size_t os_minor () { return get_gestalt(gestaltSystemVersionMinor); }
	inline size_t os_patch () { return get_gestalt(gestaltSystemVersionBugFix); }

	inline OSStatus execute_with_privileges (AuthorizationRef authorization, std::string const& pathToTool, AuthorizationFlags options, char* const* arguments, FILE** communicationsPipe)
	{
		return AuthorizationExecuteWithPrivileges(authorization, pathToTool.c_str(), options, arguments, communicationsPipe);
	}
#else  /* GNUstep / Linux / FreeBSD — parse uname(3), stub privilege escalation */
	inline size_t os_major ()
	{
		struct utsname u; if(uname(&u) != 0) return 0;
		size_t v = 0; for(char const* p = u.release; *p && *p >= '0' && *p <= '9'; ++p) v = v*10 + (*p-'0');
		return v;
	}
	inline size_t os_minor ()
	{
		struct utsname u; if(uname(&u) != 0) return 0;
		char const* p = u.release; while(*p && *p != '.') ++p; if(*p == '.') ++p;
		size_t v = 0; for(; *p && *p >= '0' && *p <= '9'; ++p) v = v*10 + (*p-'0');
		return v;
	}
	inline size_t os_patch () { return 0; }

	/* No AuthorizationExecuteWithPrivileges on GNUstep: stub returns EPERM.
	 * Real port will route to polkit or a sudo helper. */
	inline int32_t execute_with_privileges (void* authorization, std::string const& pathToTool, uint32_t options, char* const* arguments, FILE** communicationsPipe)
	{
		(void)authorization; (void)pathToTool; (void)options; (void)arguments; (void)communicationsPipe;
		return -1;
	}
#endif
#pragma clang diagnostic pop
} /* oak */

#endif /* end of include guard: COMPAT_H_RD1Z6YZA */

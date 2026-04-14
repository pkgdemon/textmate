#ifndef IO_RESOURCE_H_6LC0FV3L
#define IO_RESOURCE_H_6LC0FV3L

#if !defined(__APPLE__)
/* Carbon Resource Manager is macOS-only and deprecated even there.
 * Off-macOS: typedef the ID/type and omit the resource() accessor. */
typedef uint32_t ResType;
typedef int16_t  ResID;
namespace path
{
	bool is_text_clipping (std::string const& path);
	std::string resource (std::string const& path, ResType theType, ResID theID);
} /* path */
#else
namespace path
{
	bool is_text_clipping (std::string const& path);
	std::string resource (std::string const& path, ResType theType, ResID theID);
} /* path */
#endif

#endif /* end of include guard: IO_RESOURCE_H_6LC0FV3L */

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#endif

#include "Filesystem.h"
#include "Logger.h"

std::string Filesystem::GetRootPath()
{
    std::string path;

#if defined(WIN32)
    path = "";
#elif defined(__APPLE__)
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if (!mainBundle)
    {
        LogError("mainBundle == NULL");
        return "";
    }
    
    CFURLRef pathUrl = CFBundleCopyResourcesDirectoryURL(mainBundle);
    if (!pathUrl)
    {
        LogError("pathUrl == NULL");
        return "";
    }
    
    char buf[PATH_MAX];
    memset(buf, 0, PATH_MAX);
    CFURLGetFileSystemRepresentation(pathUrl, true, reinterpret_cast<UInt8*>(buf), PATH_MAX);
    
    path = std::string(buf);
    path.append("/");
#else
#    error implement me
#endif
    
    return path;
}
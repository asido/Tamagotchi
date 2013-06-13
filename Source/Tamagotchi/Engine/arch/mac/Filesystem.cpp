#include <CoreFoundation/CoreFoundation.h>
#include <string>
#include "Filesystem.h"
#include "Logger.h"

std::string Filesystem::GetRootPath()
{
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if (!mainBundle)
    {
        TG_ERROR("mainBundle == NULL");
        return "";
    }
    
    CFURLRef pathUrl = CFBundleCopyResourcesDirectoryURL(mainBundle);
    if (!pathUrl)
    {
        TG_ERROR("pathUrl == NULL");
        return "";
    }
    
    char buf[PATH_MAX];
    memset(buf, 0, PATH_MAX);
    CFURLGetFileSystemRepresentation(pathUrl, true, reinterpret_cast<UInt8*>(buf), PATH_MAX);
    
    std::string path(buf);
    path.append("/");
    
    return path;
}
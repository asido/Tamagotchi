#include "ResourceMgr/Filesystem.h"

/*
 * We have this primarily for iOS since resource loading requires absolute path
 * of the main bundle. On Windows there is nothing to do.
 */

std::string Filesystem::GetRootPath()
{
	return "";
}
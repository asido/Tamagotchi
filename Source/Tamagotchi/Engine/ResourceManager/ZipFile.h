#ifndef __ZIPFILE_H__
#define __ZIPFILE_H__

#include <string>
#include <memory>
#include "ResourceManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class ResourceZipFile
//-----------------------------------------------------------------------------------------------------------

class ResourceZipFile : public IResourceFile
{
public:
    ResourceZipFile(const std::string resFileName);

    virtual bool        Open() override;
    virtual int         GetRawResourceSize(const Resource &r) override;
    virtual int         GetRawResource(const Resource &r, char *buffer) override;
    virtual int         GetNumResources() const override;
    virtual std::string GetResourceName(int num) const override;

private:
    std::shared_ptr<ZipFile>    zipFile;
    std::string                 resFilename;
};


//-----------------------------------------------------------------------------------------------------------
//  class ZipFile
//-----------------------------------------------------------------------------------------------------------

// Maps path to a zip content id.
typedef std::map<std::string, int> ZipContentsMap;

class ZipFile
{
public:
    ZipFile();
    virtual ~ZipFile();

    bool Init(const std::string &resFileName);
    void End();

    int         GetNumFiles() const { return this->entryCnt; }
    std::string GetFilename(int i) const;
    int         GetFileLen(int i) const;
    bool        ReadFile(int i, void *buf);

    int         Find(const std::string &path) const;

private:
    struct TZipDirHeader;
    struct TZipDirFileHeader;
    struct TZipLocalHeader;

    ZipContentsMap zipContentsMap;

    FILE    *file;        // Zip file.
    char    *dirData;    // Raw data buffer.
    int      entryCnt;    // Number of entries.

    // Pointers to the dir entries in dirData.
    const TZipDirFileHeader **papDir;
};

#endif // __ZIPFILE_H__
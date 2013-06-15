#include <cctype>
#include <cstdio>
#include <string>
#include <limits>
#include <memory>
#include <algorithm>

#include <zlib.h>

#include "ZipFile.h"
#include "StringUtilities.h"
#include "Filesystem.h"
#include "defines.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class ResourceZipFile
//-----------------------------------------------------------------------------------------------------------

ResourceZipFile::ResourceZipFile(const std::string resFileName)
	: resFilename(resFileName), zipFile(NULL)
{

}

bool ResourceZipFile::Open()
{
	this->zipFile = std::shared_ptr<ZipFile>(TG_NEW ZipFile());
	LogAssert(this->zipFile);
	return this->zipFile->Init(this->resFilename);
}

int ResourceZipFile::GetRawResourceSize(const Resource &r)
{
	int resourceNum = this->zipFile->Find(r.GetName());
	if (resourceNum == -1)
	{
		return -1;
	}
	return this->zipFile->GetFileLen(resourceNum);
}

int	ResourceZipFile::GetRawResource(const Resource &r, char *buffer)
{
	int size = 0;
	int resourceNum = this->zipFile->Find(r.GetName());
	if (resourceNum != -1)
	{
		size = this->zipFile->GetFileLen(resourceNum);
		this->zipFile->ReadFile(resourceNum, buffer);
	}
	return size;
}

int	ResourceZipFile::GetNumResources() const
{
	return (this->zipFile ? this->zipFile->GetNumFiles() : 0);
}

std::string	ResourceZipFile::GetResourceName(int num) const
{
	std::string resName = "";
	if (this->zipFile && num >= 0 && num < this->zipFile->GetNumFiles())
	{
		resName = this->zipFile->GetFilename(num);
	}
	return resName;
}


//-----------------------------------------------------------------------------------------------------------
//  ZIP file structures. Note these have to be packed.
//-----------------------------------------------------------------------------------------------------------

#pragma  pack(1)

struct ZipFile::TZipLocalHeader
{
	enum
	{
		SIGNATURE = 0x04034b50
	};
	unsigned long	sig;
	unsigned short	version;
	unsigned short	flag;
	unsigned short	compression;	// Z_NO_COMPRESSION or Z_DEFLATED
	unsigned short	modTime;
	unsigned short	modDate;
	unsigned long	crc32;
	unsigned long	cSize;
	unsigned long	ucSize;
	unsigned short	fnameLen;		// Filename string follows header.
	unsigned short	extraLen;		// Extra field follows filename.
};

struct ZipFile::TZipDirHeader
{
	enum
	{
		SIGNATURE = 0x06054B50
	};
	unsigned long	sig;
	unsigned short	nDisk;
	unsigned short	nStartDisk;
	unsigned short	nDirEntries;
	unsigned short	totalDirEntries;
	unsigned long	dirSize;
	unsigned long	dirOffset;
	unsigned short	cmntLen;
};

struct ZipFile::TZipDirFileHeader
{
	enum
	{
		SIGNATURE = 0x02014B50
	};
	unsigned long	sig;
	unsigned short	verMade;
	unsigned short	verNeeded;
	unsigned short	flag;
	unsigned short	compression;	// COMP_xxxx
	unsigned short	modTime;
	unsigned short	modDate;
	unsigned long	crc32;
	unsigned long	cSize;			// Compressed size.
	unsigned long	ucSize;			// Uncompressed size.
	unsigned short	fnameLen;		// Filename string follows header.
	unsigned short	extraLen;		// Extra field follows filename.
	unsigned short	cmntLen;		// Comment field follows extra field.
	unsigned short	diskStart;
	unsigned short	intAttr;
	unsigned long	extAttr;
	unsigned long	hdrOffset;

	char *GetName() const { return (char*)(this + 1); }
	char *GetExtra() const { return this->GetName() + this->fnameLen; }
	char *GetComment() const { return GetExtra() + this->extraLen; }
};

#pragma pack()


//-----------------------------------------------------------------------------------------------------------
//  class ZipFile
//-----------------------------------------------------------------------------------------------------------

ZipFile::ZipFile()
	: entryCnt(0), file(NULL), dirData(NULL)
{

}

ZipFile::~ZipFile()
{
	End();

	if (this->file)
	{
		fclose(this->file);
	}
}

bool ZipFile::Init(const std::string &resFileName)
{
	End();
    
    const std::string absoluteFilename = Filesystem::GetRootPath() + resFileName;
	this->file = fopen(absoluteFilename.c_str(), "rb");
	if (!this->file)
	{
		LogError("Can't open a file: %s", absoluteFilename);
		return false;
	}

	// Assuming no extra comment at the end, read the whole end record.
	TZipDirHeader dh;

	fseek(this->file, -(int)sizeof(dh), SEEK_END);
	long dhOffset = ftell(this->file);
	memset(&dh, 0, sizeof(dh));
	fread(&dh, sizeof(dh), 1, this->file);

	// Check signature
	if (dh.sig != TZipDirHeader::SIGNATURE)
	{
		LogError("dh.sig signature mismatch. Expected: 0x%X. Got: 0x%X", TZipDirHeader::SIGNATURE, dh.sig);
		return false;
	}

	// Go to the beginning of the directory.
	fseek(this->file, dhOffset - dh.dirSize, SEEK_SET);

	// Allocate the data buffer, and read the whole thing.
	this->dirData = TG_NEW char[dh.dirSize + (dh.nDirEntries * sizeof(*this->papDir))];
	if (!this->dirData)
	{
		LogError("ZipFile::dirData == NULL");
		return false;
	}
	memset(this->dirData, 0, dh.dirSize + (dh.nDirEntries * sizeof(*this->papDir)));
	fread(this->dirData, dh.dirSize, 1, this->file);

	// Now process each entry.
	char *pfh = this->dirData;
	this->papDir = (const TZipDirFileHeader **)(this->dirData + dh.dirSize);

	bool success = true;

	for (int i = 0; i < dh.nDirEntries && success; ++i)
	{
		TZipDirFileHeader &fh = *(TZipDirFileHeader*)pfh;

		// Store the address of nth file for quicker access.
		this->papDir[i] = &fh;

		// Check the directory entry integrity.
		if (fh.sig != TZipDirFileHeader::SIGNATURE)
		{
			LogError("fh.sig signature mismatch. Expected: 0x%X. Got: 0x%X", TZipDirFileHeader::SIGNATURE, fh.sig);
			success = false;
		}
		else
		{
			pfh += sizeof(fh);

			// pfh stores filenames terminated by '\n' and so we need to pass fnameLen in order to get rid of it.
			std::string path(pfh, fh.fnameLen);
			StringUtilities::ToLower(path);
			this->zipContentsMap[path] = i;

			// Skip name, extra and comment fields.
			pfh += fh.fnameLen + fh.extraLen + fh.cmntLen;
		}
	}

	if (!success)
	{
		SAFE_DELETE_ARRAY(this->dirData);
	}
	else
	{
		this->entryCnt = dh.nDirEntries;
	}

	return success;
}

void ZipFile::End()
{
	this->zipContentsMap.clear();
	SAFE_DELETE_ARRAY(this->dirData);
	this->entryCnt = 0;
}

std::string ZipFile::GetFilename(int i) const
{
	std::string fileName = "";

	if (i >= 0 && i < this->entryCnt)
	{
		char pszDest[PATH_MAX];
		memcpy(pszDest, this->papDir[i]->GetName(), this->papDir[i]->fnameLen);
		pszDest[this->papDir[i]->fnameLen] = '\0';
		fileName = pszDest;
	}

	return fileName;
}

int ZipFile::GetFileLen(int i) const
{
	if (i < 0 || i >= this->entryCnt)
	{
		return -1;
	}
	else
	{
		return this->papDir[i]->ucSize;
	}
}

bool ZipFile::ReadFile(int i, void *buf)
{
	if (buf == NULL || i < 0 || i >= this->entryCnt)
	{
		LogError("bad args.");
		return false;
	}

	// Quick'n dirty read, the whole file at once.
	// Ungood if the ZIP has huge files inside.

	// Go to the actual file and read the local header.
	fseek(this->file, this->papDir[i]->hdrOffset, SEEK_SET);
	TZipLocalHeader h;

	memset(&h, 0, sizeof(h));
	fread(&h, sizeof(h), 1, this->file);
	if (h.sig != TZipLocalHeader::SIGNATURE)
	{
		LogError("h.sig signature mismatch. Expected: 0x%X. Got: 0x%X", TZipLocalHeader::SIGNATURE, h.sig);
		return false;
	}

	// Skip extra fields.
	fseek(this->file, h.fnameLen + h.extraLen, SEEK_CUR);

	if (h.compression == Z_NO_COMPRESSION)
	{
		// Simply read in raw stored data.
		fread(buf, h.cSize, 1, this->file);
		return true;
	}
	else if (h.compression != Z_DEFLATED)
	{
		LogError("Unsupported ZIP compression: %d", h.compression);
		return false;
	}

	// Alloc compressed data buffer and read the whole stream.
	char *pcData = TG_NEW char[h.cSize];
	if (!pcData)
	{
		LogError("pcData == NULL");
		return false;
	}

	memset(pcData, 0, h.cSize);
	fread(pcData, h.cSize, 1, this->file);

	bool ret = true;

	// Setup the inflate stream.
	z_stream stream;
	int err;

	stream.next_in = (Bytef*)pcData;
	stream.avail_in = (uInt)h.cSize;
	stream.next_out = (Bytef*)buf;
	stream.avail_out = h.ucSize;
	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;

	// Perform inflation. wbits < 0 indicates no zlib header inside the data.
	err = inflateInit2(&stream, -MAX_WBITS);
	if (err == Z_OK)
	{
		err = inflate(&stream, Z_FINISH);
		inflateEnd(&stream);
		if (err == Z_STREAM_END)
		{
			err = Z_OK;
		}
		inflateEnd(&stream);
	}

	if (err != Z_OK)
	{
		LogError("decompression has failed. Error: %d", err);
		ret = false;
	}

	delete[] pcData;
	return ret;
}

int ZipFile::Find(const std::string &path) const
{
	std::string lowerCase = path;
	StringUtilities::ToLower(lowerCase);
	ZipContentsMap::const_iterator it = this->zipContentsMap.find(lowerCase);
	if (it == this->zipContentsMap.end())
	{
		return -1;
	}
	return it->second;
}
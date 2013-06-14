#include <assert.h>

#if defined(WIN32)
#include <windows.h>
#endif

#include <tinyxml2.h>

#include "Logger.h"

static const char *ERRORLOG_FILENAME = "error.log";

#define TAG_FLAG_WRITE_TO_DEBUGGER	0x01
#define TAG_FLAG_WRITE_TO_LOG_FILE	0x02

#if defined(DEBUG) || defined(_DEBUG)
static const unsigned char TAG_FLAGS_DEFAULT_ERROR		= TAG_FLAG_WRITE_TO_DEBUGGER | TAG_FLAG_WRITE_TO_LOG_FILE;
static const unsigned char TAG_FLAGS_DEFAULT_WARNING	= TAG_FLAG_WRITE_TO_DEBUGGER | TAG_FLAG_WRITE_TO_LOG_FILE;
static const unsigned char TAG_FLAGS_DEFAULT_INFO		= TAG_FLAG_WRITE_TO_DEBUGGER | TAG_FLAG_WRITE_TO_LOG_FILE;
#else
static const unsigned char TAG_FLAGS_DEFAULT_ERROR		= 0;
static const unsigned char TAG_FLAGS_DEFAULT_WARNING	= 0;
static const unsigned char TAG_FLAGS_DEFAULT_INFO		= 0;
#endif


static LogMgr *g_logMgr = NULL;

//-----------------------------------------------------------------------------------------------------------
//  class ErrorMessenger
//-----------------------------------------------------------------------------------------------------------

ErrorMessenger::ErrorMessenger()
{
	if (!g_logMgr)
	{
		return;
	}

	g_logMgr->AddErrorMessenger(this);
	this->enabled = true;
}

void ErrorMessenger::Show(const std::string &errorMessage, bool isFatal, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
	if (!g_logMgr)
	{
		return;
	}

	if (this->enabled)
	{
		if (g_logMgr->Error(errorMessage, isFatal, funcName, sourceFile, lineNum) == ErrorDialogChoice::LOGMGR_ERROR_IGNORE)
		{
			this->enabled = false;
		}
	}
}


//-----------------------------------------------------------------------------------------------------------
//  class LogMgr
//-----------------------------------------------------------------------------------------------------------

LogMgr::LogMgr()
{
	SetTagFlags("ERROR", TAG_FLAGS_DEFAULT_ERROR);
	SetTagFlags("WARNING", TAG_FLAGS_DEFAULT_WARNING);
	SetTagFlags("INFO", TAG_FLAGS_DEFAULT_INFO);
}

LogMgr::~LogMgr()
{
	// TODO: Lock messenger critical section.
	for (ErrorMessangerList::iterator it = this->errorMessengers.begin(); it != this->errorMessengers.end(); ++it)
	{
		ErrorMessenger *messenger = *it;
		delete messenger;
	}

	this->errorMessengers.clear();
	// TODO: Unlock messenger critical section.
}

//-----------------------------------------------
// Static
//-----------------------------------------------

bool LogMgr::Init(const std::string *logConfigFilename)
{
	// Rely here on simple assert since no logging can be handled anyway.
	assert(!g_logMgr);

	g_logMgr = TG_NEW LogMgr();

	if (logConfigFilename)
	{
		tinyxml2::XMLDocument xmlDocument;
		
		tinyxml2::XMLError result = xmlDocument.LoadFile(logConfigFilename->c_str());
		if (result)
		{
			return false;
		}

		tinyxml2::XMLElement *root = xmlDocument.RootElement();

		for (tinyxml2::XMLElement *node = root->FirstChildElement(); node; node = node->NextSiblingElement())
		{
			unsigned char flags = 0;
			std::string tag(node->Attribute("tag"));

			char debugger = 0;
			node->Attribute("debugger", &debugger);
			if (debugger)
			{
				flags |= TAG_FLAG_WRITE_TO_DEBUGGER;
			}

			char logFile = 0;
			node->Attribute("file", &logFile);
			if (logFile)
			{
				flags |= TAG_FLAG_WRITE_TO_LOG_FILE;
			}

			g_logMgr->SetTagFlags(tag, flags);
		}
	}

	return true;
}

void LogMgr::Destroy()
{
	SAFE_DELETE(g_logMgr);
	g_logMgr = NULL;
}

//-----------------------------------------------
// Public
//-----------------------------------------------

void LogMgr::Log(const std::string &tag, const std::string &message, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
	// TODO: Tag critical section lock.

	TagMap::iterator it = this->tags.find(tag);
	if (it != this->tags.end())
	{
		// TODO: Tag critical section unlock.

		std::string buffer;
		GetOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
		OutputFinalBufferToLogs(buffer, it->second);
	}
	else
	{
		// TODO: Tag critical section unlock.
	}
}

void LogMgr::AddErrorMessenger(ErrorMessenger *messenger)
{
	// TODO: messenger critical section lock.
	this->errorMessengers.push_back(messenger);
	// TODO: messenger critical section unlock.
}

void LogMgr::SetTagFlags(const std::string &tag, unsigned char flags)
{
	// TODO: tag critical section lock.

	if (flags == 0)
	{
		this->tags.erase(tag);
	}
	else
	{
		TagMap::iterator it = this->tags.find(tag);
		if (it == this->tags.end())
		{
			this->tags.insert(std::make_pair(tag, flags));
		}
		else
		{
			it->second = flags;
		}
	}	

	// TODO: tag critical section unlock.
}

ErrorDialogChoice LogMgr::Error(const std::string &errorMessage, bool isFatal, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
	const std::string tag = (isFatal ? "FATAL" : "ERROR");
	std::string buffer;

	GetOutputBuffer(buffer, tag, errorMessage, funcName, sourceFile, lineNum);

	// TODO: tag critical section lock.
	TagMap::iterator it = this->tags.find(tag);
	if (it != this->tags.end())
	{
		OutputFinalBufferToLogs(buffer, it->second);
	}
	// TODO: tag critical section unlock.

#if defined(WIN32)
	int result = MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_DEFBUTTON3);
	switch (result) {
	case IDIGNORE:		return ErrorDialogChoice::LOGMGR_ERROR_IGNORE;
	case IDABORT:		return ErrorDialogChoice::LOGMGR_ERROR_ABORT;
	case IDRETRY:		return ErrorDialogChoice::LOGMGR_ERROR_RETRY;
	default:			return ErrorDialogChoice::LOGMGR_ERROR_RETRY;
	}
#elif defined(__APPLE__)
#	error implement me
#else
#	error implement me
#endif
}

//-----------------------------------------------
// Private
//-----------------------------------------------

void LogMgr::OutputFinalBufferToLogs(const std::string &finalBuffer, unsigned char flags)
{
	if (flags & TAG_FLAG_WRITE_TO_LOG_FILE)
	{
		WriteToLogFile(finalBuffer);
	}

	if (flags & TAG_FLAG_WRITE_TO_DEBUGGER)
	{
#if defined(WIN32)
		OutputDebugStringA(finalBuffer.c_str());
#elif defined(__APPLE__)
#	error implement me
#else
#	error implement me
#endif
	}
}

bool LogMgr::WriteToLogFile(const std::string &data)
{
	// TODO: is this function thread safe?

	FILE *file = fopen(ERRORLOG_FILENAME, "a+");
	if (!file)
	{
		return false;
	}

	fprintf(file, data.c_str());
	fclose(file);

	return true;
}

void LogMgr::GetOutputBuffer(std::string &outputBuffer, const std::string &tag, const std::string &message, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
	if (!tag.empty())
	{
		outputBuffer = "[" + tag + "]" + message;
	}
	else
	{
		outputBuffer = message;
	}

	if (funcName)
	{
		outputBuffer += "\nFunction: ";
		outputBuffer += funcName;
	}
	if (sourceFile)
	{
		outputBuffer += "\n";
		outputBuffer += sourceFile;
	}
	if (lineNum != 0)
	{
		outputBuffer += "\nLine: ";
		char lineNumBuffer[11];
		memset(lineNumBuffer, 0, sizeof(char));
		outputBuffer += _itoa(lineNum, lineNumBuffer, 10);
	}

	outputBuffer += "\n";
}
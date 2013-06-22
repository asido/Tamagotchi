#include <sstream>
#include <iostream>
#include <assert.h>
#include <time.h>

#if defined(WIN32)
#include <windows.h>
#endif

#include <tinyxml2.h>

#include "Logger.h"
#include "Clock.h"

static const char *ERRORLOG_FILENAME = "error.log";

#define TAG_FLAG_WRITE_TO_DEBUGGER    0x01
#define TAG_FLAG_WRITE_TO_LOG_FILE    0x02

#if defined(DEBUG) || defined(_DEBUG)
static const unsigned char TAG_FLAGS_DEFAULT_ERROR      = TAG_FLAG_WRITE_TO_DEBUGGER | TAG_FLAG_WRITE_TO_LOG_FILE;
static const unsigned char TAG_FLAGS_DEFAULT_WARNING    = TAG_FLAG_WRITE_TO_DEBUGGER | TAG_FLAG_WRITE_TO_LOG_FILE;
static const unsigned char TAG_FLAGS_DEFAULT_INFO       = TAG_FLAG_WRITE_TO_DEBUGGER | TAG_FLAG_WRITE_TO_LOG_FILE;
#else
static const unsigned char TAG_FLAGS_DEFAULT_ERROR      = 0;
static const unsigned char TAG_FLAGS_DEFAULT_WARNING    = 0;
static const unsigned char TAG_FLAGS_DEFAULT_INFO       = 0;
#endif


//-----------------------------------------------------------------------------------------------------------
//  class ErrorMessenger
//-----------------------------------------------------------------------------------------------------------

ErrorMessenger::ErrorMessenger()
    : enabled(true)
{
    LogMgr::Instance().AddErrorMessenger(this);
}

void ErrorMessenger::Show(const std::string &errorMessage, bool isFatal, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
    if (this->enabled)
    {
        if (LogMgr::Instance().Error(errorMessage, isFatal, funcName, sourceFile, lineNum) == ErrorDialogChoice::LOGMGR_ERROR_IGNORE)
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
    this->mutexMessenger.Lock();
    for (ErrorMessangerList::iterator it = this->errorMessengers.begin(); it != this->errorMessengers.end(); ++it)
    {
        ErrorMessenger *messenger = *it;
        delete messenger;
    }
    this->errorMessengers.clear();
    this->mutexMessenger.Unlock();
}

//-----------------------------------------------
// Static
//-----------------------------------------------

LogMgr& LogMgr::Instance()
{
    static LogMgr instance;
    return instance;
}

bool LogMgr::Init(const std::string *logConfigFilename)
{
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

            LogMgr::Instance().SetTagFlags(tag, flags);
        }
    }

    return true;
}

//-----------------------------------------------
// Public
//-----------------------------------------------

void LogMgr::Log(const std::string &tag, const std::string &message, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
    this->mutexTags.Lock();
    TagMap::iterator it = this->tags.find(tag);
    if (it != this->tags.end())
    {
        this->mutexTags.Unlock();

        std::string buffer;
        GetOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
        OutputFinalBufferToLogs(buffer, it->second);
    }
    else
    {
        this->mutexTags.Unlock();
    }
}

void LogMgr::AddErrorMessenger(ErrorMessenger *messenger)
{
    this->mutexMessenger.Lock();
    this->errorMessengers.push_back(messenger);
    this->mutexMessenger.Unlock();
}

void LogMgr::SetTagFlags(const std::string &tag, unsigned char flags)
{
    this->mutexTags.Lock();

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

    this->mutexTags.Unlock();
}

ErrorDialogChoice LogMgr::Error(const std::string &errorMessage, bool isFatal, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
    const std::string tag = (isFatal ? "FATAL" : "ERROR");
    std::string buffer;

    GetOutputBuffer(buffer, tag, errorMessage, funcName, sourceFile, lineNum);

    this->mutexTags.Lock();
    TagMap::iterator it = this->tags.find(tag);
    if (it != this->tags.end())
    {
        OutputFinalBufferToLogs(buffer, it->second);
    }
    this->mutexTags.Unlock();

#if defined(WIN32)
    int result = MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_DEFBUTTON3);

    switch (result) {
    case IDIGNORE:
        return ErrorDialogChoice::LOGMGR_ERROR_IGNORE;
        break;

    case IDABORT:
        __debugbreak();
        return ErrorDialogChoice::LOGMGR_ERROR_ABORT;
        break;

    case IDRETRY:
        return ErrorDialogChoice::LOGMGR_ERROR_RETRY;
        break;

    default:
        return ErrorDialogChoice::LOGMGR_ERROR_RETRY;
        break;
    }
#elif defined(__APPLE__)
    std::cout << buffer;
    //__builtin_trap();
    return ErrorDialogChoice::LOGMGR_ERROR_RETRY;
#else
#    error implement me
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
        std::cout << finalBuffer;
#else
#    error implement me
#endif
    }
}

bool LogMgr::WriteToLogFile(const std::string &data)
{
    this->mutexFile.Lock();

    FILE *file = fopen(ERRORLOG_FILENAME, "a+");
    if (!file)
    {
        this->mutexFile.Unlock();
        return false;
    }

    fprintf(file, "%s", data.c_str());
    fclose(file);

    this->mutexFile.Unlock();

    return true;
}

void LogMgr::GetOutputBuffer(std::string &outputBuffer, const std::string &tag, const std::string &message, const char *funcName, const char *sourceFile, unsigned int lineNum)
{
    time_t currentTime;
    struct tm *ptm;

    time(&currentTime);
    ptm = localtime(&currentTime);
    int millis = ((int)((float)clock() / CLOCKS_PER_SEC * 1000.0f)) % 1000;
    outputBuffer = StringUtilities::Format("<%02d:%02d:%02d.%03d> ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, millis);

    if (!tag.empty())
    {
        outputBuffer += "[" + tag + "] " + message;
    }
    else
    {
        outputBuffer += message;
    }

    if (sourceFile)
    {
        outputBuffer += "\n";
        outputBuffer += sourceFile;
    }
    if (lineNum != 0)
    {
        outputBuffer += ":";
        std::stringstream ss;
        ss << lineNum;
        outputBuffer += ss.str();
    }
    if (funcName)
    {
        outputBuffer += " (";
        outputBuffer += funcName;
        outputBuffer += ")";
    }

    outputBuffer += "\n";
}
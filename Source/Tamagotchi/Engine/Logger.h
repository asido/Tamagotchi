#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <map>
#include <list>

#include "defines.h"
#include "StringUtilities.h"

//-----------------------------------------------------------------------------------------------------------
//  class ErrorMessenger
//-----------------------------------------------------------------------------------------------------------

class ErrorMessenger
{
public:
    ErrorMessenger();
    void Show(const std::string &errorMessage, bool isFatal, const char *funcName, const char *sourceFile, unsigned int lineNum);

    bool IsEnabled() const { return this->enabled; }
    void SetEnabled(bool enabled) { this->enabled = enabled; }

private:
    bool enabled;
};


//-----------------------------------------------------------------------------------------------------------
//  class LogMgr
//-----------------------------------------------------------------------------------------------------------

typedef std::map<std::string, unsigned char>    TagMap;
typedef std::list<ErrorMessenger*>              ErrorMessangerList;

typedef enum 
{
    LOGMGR_ERROR_ABORT,
    LOGMGR_ERROR_RETRY,
    LOGMGR_ERROR_IGNORE
} ErrorDialogChoice;

class LogMgr
{
private:
    LogMgr();
    ~LogMgr();
    
public:
    static LogMgr& Instance();

    static bool Init(const std::string *logConfigFilename=NULL);

    void Log(const std::string &tag, const std::string &message, const char *funcName, const char *sourceFile, unsigned int lineNum);

    void AddErrorMessenger(ErrorMessenger *messenger);
    void SetTagFlags(const std::string &tag, unsigned char flags);
    ErrorDialogChoice Error(const std::string &errorMessage, bool isFatal, const char *funcName, const char *sourceFile, unsigned int lineNum);
    
private:
    void OutputFinalBufferToLogs(const std::string &finalBuffer, unsigned char flags);
    bool WriteToLogFile(const std::string &data);
    void GetOutputBuffer(std::string &outputBuffer, const std::string &tag, const std::string &message, const char *funcName, const char *sourceFile, unsigned int lineNum);

private:
    TagMap                tags;
    ErrorMessangerList    errorMessengers;
};

//-----------------------------------------------------------------------------------------------------------
//  Log defines. Use these instead of classes above directly.
//-----------------------------------------------------------------------------------------------------------

#define LogAssert(expr)                                                             \
    do {                                                                            \
        if (!(expr))                                                                \
        {                                                                           \
            static ErrorMessenger *errorMessenger = TG_NEW ErrorMessenger;          \
            errorMessenger->Show(#expr, false, __FUNCTION__, __FILE__, __LINE__);   \
        }                                                                           \
    } while (0)

#define LogFatal(str, ...)                                                          \
    do {                                                                            \
        static ErrorMessenger *errorMessenger = TG_NEW ErrorMessenger;              \
        const std::string s = StringUtilities::Format(str, __VA_ARGS__);            \
        errorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__);            \
    } while (0)

#define LogError(str, ...)                                                          \
    do {                                                                            \
        static ErrorMessenger *errorMessenger = TG_NEW ErrorMessenger;              \
        const std::string s = StringUtilities::Format(str, __VA_ARGS__);            \
        errorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__);           \
    } while (0)

#define LogWarning(str, ...)                                                        \
    do {                                                                            \
        const std::string s = StringUtilities::Format(str, __VA_ARGS__);            \
        LogMgr::Instance().Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__);              \
    } while (0)

#define LogInfo(str, ...)                                                           \
    do {                                                                            \
        const std::string s = StringUtilities::Format(str, __VA_ARGS__);            \
        LogMgr::Instance().Log("INFO", s, NULL, NULL, NULL);                                 \
    } while (0)

#endif
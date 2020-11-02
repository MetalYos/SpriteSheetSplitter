#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <vector>
#include "frame.h"
#include <string>
#include <utility>

class JsonHelper
{
public:
    static bool ImportJson(const std::string& filepath, std::pair<std::string, std::vector<Frame*>>& output);
    static bool ExportJson(const std::string& sSheetPath, const std::string& sSheetName,
                           const std::string& sSheetFilename, const std::vector<Frame*>& frames);
    static std::string GetStringValue(const std::string& line, char startDelimeter = '"', char endDelimeter = '"');
    static int GetIntValue(const std::string& line);
    static bool GetBoolValue(const std::string& line);
    static float GetFloatValue(const std::string& line);
    static void GetUChar3(const std::string& line, unsigned char* output);
};

#endif // JSONHELPER_H

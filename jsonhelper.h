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
    static bool ExportJson(const std::string& sSheetPath, const std::string& sSheetName, const std::string& sSheetFilename, const std::vector<Frame*>& frames);

private:
    static std::string GetStringValue(const std::string& line);
    static int GetIntValue(const std::string& line);
};

#endif // JSONHELPER_H

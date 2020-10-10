#include "utils.h"
#include <locale>

namespace StringUtils
{
    std::string GetFilepathWithoutFile(const std::string& filepath)
    {
        int endIndex;
        for (endIndex = filepath.size() - 1; endIndex >= 0; endIndex--)
        {
            if (filepath[endIndex] == '\\' || filepath[endIndex] == '/')
                break;
        }

        return filepath.substr(0, endIndex + 1);
    }

    std::string GetFilenameExtension(const std::string& filename)
    {
        int startIndex;
        for (startIndex = filename.size() - 1; startIndex >= 0; startIndex--)
        {
            if (filename[startIndex] == '.')
                break;
        }

        return filename.substr(startIndex + 1);
    }

    std::string GetFilename(const std::string& filepath)
    {
        int startIndex;
        for (startIndex = filepath.size() - 1; startIndex >= 0; startIndex--)
        {
            if (filepath[startIndex] == '\\' || filepath[startIndex] == '/')
                break;
        }

        return filepath.substr(startIndex + 1);
    }

    std::string GetFilenameWithoutExt(const std::string& filepath)
    {
        int startIndex = 0;
        int endIndex = filepath.size() - 1;
        for (int i = filepath.size() - 1; i >= 0; i--)
        {
            if (filepath[i] == '.')
                endIndex = i;

            if (filepath[i] == '\\' || filepath[i] == '/')
            {
                startIndex = i + 1;
                break;
            }
        }

        return filepath.substr(startIndex, endIndex - startIndex);
    }

    std::string ReplaceFilename(const std::string& filepath, const std::string& filename)
    {
        auto path = GetFilepathWithoutFile(filepath);
        return path + filename;
    }
}

std::string StringUtils::ToLower(const std::string& str)
{
    std::string strCopy = str;
    for(char &c : strCopy)
        c = tolower(c);
    return strCopy;
}

std::string& StringUtils::ToLower(std::string& str)
{
    for(char &c : str)
        c = tolower(c);
    return str;
}

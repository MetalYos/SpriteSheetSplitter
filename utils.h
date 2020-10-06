#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <string>

namespace MathUtils
{
    template <typename T>
    T Clamp(T val, T min, T max)
    {
        if (val < min)
            return min;
        if (val > max)
            return max;
        return val;
    }
}

namespace StringUtils
{
    std::string GetFilepathWithoutFile(const std::string& filepath);
    std::string GetFilenameExtension(const std::string& filename);
    std::string GetFilename(const std::string& filepath);
    std::string GetFilenameWithoutExt(const std::string& filepath);
    std::string ReplaceFilename(const std::string& filepath, const std::string& filename);
}

#endif // MATHUTILS_H

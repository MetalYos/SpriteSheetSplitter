#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace GraphicsUtils
{
    struct PixelColor
    {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;

        PixelColor()
            : R(0), G(0), B(0), A(0)
        { }

        PixelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
            : R(r), G(g), B(b), A(a)
        { }

        bool operator ==(const PixelColor& c) const
        {
            return (R == c.R && G == c.G && B == c.B && A == c.A);
        }

        bool operator !=(const PixelColor& c) const
        {
            return !(*this == c);
        }
    };
}

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

    template <typename T>
    T Max(T a, T b)
    {
        if (a > b)
            return a;
        return b;
    }

    template <typename T>
    T Min(T a, T b)
    {
        if (a < b)
            return a;
        return b;
    }
}

namespace StringUtils
{
    std::string GetFilepathWithoutFile(const std::string& filepath);
    std::string GetFilenameExtension(const std::string& filename);
    std::string GetFilename(const std::string& filepath);
    std::string GetFilenameWithoutExt(const std::string& filepath);
    std::string ReplaceFilename(const std::string& filepath, const std::string& filename);
    std::string ToLower(const std::string& str);
    std::string& ToLower(std::string& str);
}

#endif // UTILS_H

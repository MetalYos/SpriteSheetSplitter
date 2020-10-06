#ifndef IMAGE_H
#define IMAGE_H

#include <string>

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

class Image
{
public:
    ~Image();
    static Image* LoadImage(const std::string& filepath);

    // Getters
    std::string Filepath() const;
    int Width() const;
    int Height() const;
    int NumOfChannels() const;
    unsigned char* Pixels() const;
    unsigned int SizeInBytes() const;
    unsigned int BytesPerLine() const;
    PixelColor GetPixelColor(int x, int y) const;
    bool IsInsideImage(int x, int y) const;

private:
    Image();

private:
    std::string filepath;
    int width;
    int height;
    int numChannels;
    unsigned char* data;
};

#endif // IMAGE_H

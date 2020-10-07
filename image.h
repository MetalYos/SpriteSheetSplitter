#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "utils.h"

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
    GraphicsUtils::PixelColor GetPixelColor(int x, int y) const;
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

#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image()
    : filepath(""), width(0), height(0), numChannels(0), data(nullptr)
{

}

Image::~Image()
{
    if (data)
        stbi_image_free(data);
}

Image* Image::LoadImage(const std::string& filepath)
{
    Image* img = new Image();

    img->data = stbi_load(filepath.c_str(), &img->width, &img->height, &img->numChannels, 0);
    if (!img->data)
    {
        delete img;
        return nullptr;
    }
    img->filepath = filepath;

    return img;
}

std::string Image::Filepath() const
{
    return filepath;
}

int Image::Width() const
{
    return width;
}

int Image::Height() const
{
    return height;
}

int Image::NumOfChannels() const
{
    return numChannels;
}

unsigned char* Image::Pixels() const
{
    return data;
}

unsigned int Image::SizeInBytes() const
{
    return BytesPerLine() * height;
}

unsigned int Image::BytesPerLine() const
{
    return width * numChannels;
}

GraphicsUtils::PixelColor Image::GetPixelColor(int x, int y) const
{
    GraphicsUtils::PixelColor color;

    if (IsInsideImage(x, y))
    {
        int index = y * BytesPerLine() + x * numChannels;
        for (int i = 0; i < numChannels; i++)
        {
            ((unsigned char*)&color.R)[i] = data[index + i];
        }
    }

    return color;
}

bool Image::IsInsideImage(int x, int y) const
{
    return (x >= 0 && x < width && y >= 0 && y < height);
}

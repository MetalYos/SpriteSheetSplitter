#ifndef FRAMEDETECTION_H
#define FRAMEDETECTION_H

#include <utility>
#include <vector>
#include <tuple>
#include "frame.h"
#include "image.h"

class FrameDetection
{
public:
    static FrameDetection& Instance()
    {
        static FrameDetection instance;
        return instance;
    }
    ~FrameDetection();

    void SetParameters(const Image* imageData,
                       const std::vector<GraphicsUtils::PixelColor>& backgroundColor,
                       int tolerance);
    Frame* DetectFrame(int x, int y);
    std::vector<Frame*> DetectAllFrames();

private:
    FrameDetection();
    bool DetectFrameLoop(int x, int y, std::pair<int, int>& min, std::pair<int, int>& max, int numBgPixels);
    bool IsBackgroundPixel(int x, int y) const;
    bool IsPixelAlreadyVisited(int x, int y) const;

private:
    const Image* _imageData;
    std::vector<GraphicsUtils::PixelColor> _backgroundColors;
    int _tolerance;

    bool _areParamsSet;
    bool* _pixelsVisited;
    std::pair<int, int> _steps;

    std::vector<std::tuple<int, int, int>> _coordsStack;
};

#endif // FRAMEDETECTION_H

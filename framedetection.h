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
                       const PixelColor& backgroundColor,
                       int tolerance);
    Frame* DetectFrame(int x, int y);
    std::vector<Frame*> DetectAllFrames();

private:
    FrameDetection();
    void DetectFrameLoop(int x, int y, std::pair<int, int>& min, std::pair<int, int>& max, int numBgPixels);
    void CalculateAxesIntersections();
    void CalculateOneAxisIntersections(bool isYAxis);
    void ClearAxesIntersections();

private:
    const Image* _imageData;
    PixelColor _backgroundColor;
    int _tolerance;

    bool _areParamsSet;
    bool* _pixelsVisited;

    std::vector<std::tuple<int, int, int>> _coordsStack;

    // Calculate all frames parameters
    std::vector<std::pair<int, int>>* _xAxisIntersections;
    std::vector<std::pair<int, int>>* _yAxisIntersections;
};

#endif // FRAMEDETECTION_H

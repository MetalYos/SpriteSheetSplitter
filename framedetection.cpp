#include "framedetection.h"
#include "constants.h"

FrameDetection::FrameDetection()
    : _imageData(nullptr),
      _tolerance(0),
      _areParamsSet(false),
      _pixelsVisited(nullptr),
      _xAxisIntersections(nullptr),
      _yAxisIntersections(nullptr)
{

}

FrameDetection::~FrameDetection()
{
    ClearAxesIntersections();
}

void FrameDetection::SetParameters(const Image* imageData,
                     const PixelColor& backgroundColor,
                     int tolerance)
{
    if (!imageData)
        return;

    _imageData = imageData;
    _backgroundColor = backgroundColor;
    _tolerance = tolerance;

    // Create a map that indicates if the pixel was visited or not
    int numPixels = imageData->Width() * imageData->Height();
    _pixelsVisited = new bool[numPixels];
    for (int i = 0; i < numPixels; i++)
        _pixelsVisited[i] = false;

    ClearAxesIntersections();
    CalculateAxesIntersections();
}

Frame* FrameDetection::DetectFrame(int originX, int originY)
{
    if (!_imageData->IsInsideImage(originX, originY))
        return nullptr;

    std::pair<int, int> min(_imageData->Width(), _imageData->Height());
    std::pair<int, int> max(0, 0);
    if (!DetectFrameLoop(originX, originY, min, max, 0))
        return nullptr;

    Frame* frame = new Frame(min.second, min.first, max.first - min.first, max.second - min.second);
    return frame;
}

std::vector<Frame*> FrameDetection::DetectAllFrames()
{
    std::vector<Frame*> frames;

    int step = 5;
    for (int y = step; y < _imageData->Height(); y += step)
    {
        for (int x = step; x < _imageData->Width(); x+= step)
        {
            Frame* frame = DetectFrame(x, y);
            if (frame == nullptr)
                continue;
            else
                frames.push_back(frame);
        }
    }


    return frames;
}

bool FrameDetection::DetectFrameLoop(int x, int y, std::pair<int, int>& min, std::pair<int, int>& max, int numBgPixels)
{
    if (IsBackgroundPixel(x, y))
        return false;
    if (IsPixelAlreadyVisited(x, y))
        return false;

    _coordsStack.push_back({x, y, numBgPixels});
    while (_coordsStack.size() > 0)
    {
        auto coords = _coordsStack.back();
        _coordsStack.pop_back();

        int curX = std::get<0>(coords);
        int curY = std::get<1>(coords);
        int curNumBgPixels = std::get<2>(coords);

        int index = curY * _imageData->Width() + curX;

        // If I am outside the image boundaris, move to the next coordinate
        if (!_imageData->IsInsideImage(curX, curY))
            continue;
        // If I am now at a pixel that was already visited, move to the next coordinate.
        if (IsPixelAlreadyVisited(curX, curY))
            continue;
        // 3. If we are outside the tolerance, move to the next coordinate.
        if (curNumBgPixels > _tolerance)
            continue;

        if (!IsBackgroundPixel(curX, curY))
        {
            if (curX < min.first)
                min.first = curX;
            if (curX > max.first)
                max.first = curX;
            if (curY < min.second)
                min.second = curY;
            if (curY > max.second)
                max.second = curY;
        }
        else
        {
            curNumBgPixels++;
        }

        // Push the next coordinates to the stack
        _coordsStack.push_back({curX + 1, curY, curNumBgPixels});
        _coordsStack.push_back({curX - 1, curY, curNumBgPixels});
        _coordsStack.push_back({curX, curY + 1, curNumBgPixels});
        _coordsStack.push_back({curX, curY - 1, curNumBgPixels});

        // Mark this pixel as visited
        _pixelsVisited[index] = true;
    }

    return true;
}

void FrameDetection::CalculateAxesIntersections()
{
    _xAxisIntersections = new std::vector<std::pair<int, int>>[_imageData->Width()];
    _yAxisIntersections = new std::vector<std::pair<int, int>>[_imageData->Height()];

    CalculateOneAxisIntersections(false);
    CalculateOneAxisIntersections(true);
}

void FrameDetection::CalculateOneAxisIntersections(bool isYAxis)
{
    auto& axis = _xAxisIntersections;
    int majorLen = _imageData->Width();
    int minorLen = _imageData->Height();
    if (isYAxis)
    {
        axis = _yAxisIntersections;
        majorLen = _imageData->Height();
        minorLen = _imageData->Width();
    }

    for (int major = 0; major < majorLen; major++)
    {
        for (int minor = 0; minor < minorLen; minor++)
        {
            bool inside = false;
            auto color = _imageData->GetPixelColor(major, minor);
            if (color.A != 0 && color != _backgroundColor)
            {
                if (!inside)
                {
                    axis[major].push_back({minor, minor});
                    inside = true;
                }
            }
            else
            {
                if (inside)
                {
                    axis[major].back().second = minor - 1;
                    inside = false;
                }
            }
        }
    }
}

void FrameDetection::ClearAxesIntersections()
{
    if (_xAxisIntersections != nullptr)
    {
        for (int i = 0; i < _imageData->Width(); i++)
            _xAxisIntersections[i].clear();
        delete[] _xAxisIntersections;
        _xAxisIntersections = nullptr;
    }

    if (_yAxisIntersections != nullptr)
    {
        for (int i = 0; i < _imageData->Height(); i++)
            _yAxisIntersections[i].clear();
        delete[] _yAxisIntersections;
        _yAxisIntersections = nullptr;
    }
}

bool FrameDetection::IsBackgroundPixel(int x, int y) const
{
    auto color = _imageData->GetPixelColor(x, y);
    return !(color.A > Constants::FRAME_COLOR_TOLERANCE && color != _backgroundColor);
}

bool FrameDetection::IsPixelAlreadyVisited(int x, int y) const
{
    int index = y * _imageData->Width() + x;
    return _pixelsVisited[index];
}

#include "framedetection.h"
#include "settings.h"

FrameDetection::FrameDetection()
    : _imageData(nullptr),
      _tolerance(0),
      _areParamsSet(false),
      _pixelsVisited(nullptr),
      _steps({ Settings::FRAME_DETECTION_ALGO_STEP, Settings::FRAME_DETECTION_ALGO_STEP })
{

}

FrameDetection::~FrameDetection()
{
    if (_pixelsVisited != nullptr)
        delete[] _pixelsVisited;
}

void FrameDetection::SetTolerance(int tolerance)
{
    _tolerance = tolerance;
}

void FrameDetection::SetBgColors(const std::vector<GraphicsUtils::PixelColor>& backgroundColor)
{
    _backgroundColors = backgroundColor;
}

void FrameDetection::SetImageData(Image *image)
{
    if (image == nullptr)
        return;
    _imageData = image;

    CreateAlreadyVisitedMatrix();
}

void FrameDetection::SetParameters(const Image* imageData,
                     const std::vector<GraphicsUtils::PixelColor>& backgroundColors,
                     int tolerance)
{
    if (!imageData)
        return;

    _imageData = imageData;
    _backgroundColors = backgroundColors;
    _tolerance = tolerance;

    CreateAlreadyVisitedMatrix();
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
    CreateAlreadyVisitedMatrix();
    std::vector<Frame*> frames;

    for (int y = _steps.second; y < _imageData->Height(); y += _steps.second)
    {
        for (int x = _steps.first; x < _imageData->Width(); x+= _steps.first)
        {
            Frame* frame = DetectFrame(x, y);
            if (frame == nullptr)
                continue;
            else
            {
                frames.push_back(frame);
                if (Settings::FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP)
                {
                    _steps.first = frame->Width() / 4;
                    _steps.second = frame->Height() / 4;
                }
            }
        }
    }


    return frames;
}

void FrameDetection::CreateAlreadyVisitedMatrix()
{
    if (_pixelsVisited != nullptr)
        delete[] _pixelsVisited;

    // Create a map that indicates if the pixel was visited or not
    int numPixels = _imageData->Width() * _imageData->Height();
    _pixelsVisited = new bool[numPixels];
    for (int i = 0; i < numPixels; i++)
        _pixelsVisited[i] = false;
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

bool FrameDetection::IsBackgroundPixel(int x, int y) const
{
    auto color = _imageData->GetPixelColor(x, y);

    bool isBg = false;
    isBg |= (color.A <= Settings::FRAME_COLOR_TOLERANCE);
    for (auto bgColor : _backgroundColors)
    {
        isBg |= (color == bgColor);
    }

    return isBg;
}

bool FrameDetection::IsPixelAlreadyVisited(int x, int y) const
{
    int index = y * _imageData->Width() + x;
    return _pixelsVisited[index];
}

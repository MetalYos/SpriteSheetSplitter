#ifndef MAINWINDOWVIEWMODEL_H
#define MAINWINDOWVIEWMODEL_H

#include <vector>
#include "frame.h"
#include "image.h"
#include "settings.h"

class FramesWindowViewModel
{
public:
    static FramesWindowViewModel& Instance()
    {
        static FramesWindowViewModel instance;
        return instance;
    }
    ~FramesWindowViewModel();

    // Image Methods
    bool LoadImage(const std::string& filepath);
    const Image* GetImage() const;

    // Frame Methods
    void AddFrame(Frame* frame);
    void RemoveFrame(int index);
    void RemoveSelectedFrames();
    void ClearFrames();
    void SetFrames(const std::vector<Frame*>& frames);
    Frame* GetFrame(int index);
    void SetSelectedFrames(const std::vector<int>& framesIndices);
    std::vector<Frame*> GetSelectedFrames();
    std::vector<Frame*>& GetFrames();
    int GetFrameIndex(Frame* frame) const;

    std::vector<int> GetSelectedFrameIndices() const;
    void SelectFrame(size_t index, bool only = true);
    void DeselectAllFrames();
    void SelectLastFrame(bool only = true);
    bool IsFrameSelected(int index) const;

    // Frame Detection Methods
    void SetFrameDetectionParameters(int tolerance);
    void SetFrameDetectionParameters(const std::vector<GraphicsUtils::PixelColor>& backgroundColor,
                       int tolerance = Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_INIT));
    void DetectSelectedFrames();
    void DetectAllFrames();


private:
    FramesWindowViewModel();

private:
    std::vector<Frame*> frames;
    std::vector<int> selectedFrameIndices;
    Image* imageRaw;
};

#endif // MAINWINDOWVIEWMODEL_H

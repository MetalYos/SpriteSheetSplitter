#ifndef MAINWINDOWVIEWMODEL_H
#define MAINWINDOWVIEWMODEL_H

#include <vector>
#include "frame.h"
#include "image.h"
#include "settings.h"

class MainWindowViewModel
{
public:
    static MainWindowViewModel& Instance()
    {
        static MainWindowViewModel instance;
        return instance;
    }
    ~MainWindowViewModel();

    // Image Methods
    bool LoadImage(const std::string& filepath);
    const Image* GetImage() const;

    // Frame Methods
    void AddFrame(Frame* frame);
    void RemoveFrame(int index);
    void RemoveSelectedFrame();
    void ClearFrames();
    void SetFrames(const std::vector<Frame*>& frames);
    Frame* GetFrame(int index);
    Frame* GetSelectedFrame();
    std::vector<Frame*>& GetFrames();
    int GetFrameIndex(Frame* frame) const;

    int GetSelectedFrameIndex() const;
    void SelectFrame(size_t index);
    void DeselectAllFrames();
    void SelectLastFrame();

    // Frame Detection Methods
    void SetFrameDetectionParameters(int tolerance);
    void SetFrameDetectionParameters(const std::vector<GraphicsUtils::PixelColor>& backgroundColor,
                       int tolerance = Settings::FRAME_TOLERANCE_INIT);
    bool DetectFrame();
    bool DetectAllFrames();


private:
    MainWindowViewModel();

private:
    std::vector<Frame*> frames;
    int selectedFrameIndex;
    Image* imageRaw;
};

#endif // MAINWINDOWVIEWMODEL_H

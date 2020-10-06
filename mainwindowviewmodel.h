#ifndef MAINWINDOWVIEWMODEL_H
#define MAINWINDOWVIEWMODEL_H

#include "frame.h"
#include <vector>

class MainWindowViewModel
{
public:
    static MainWindowViewModel& Instance()
    {
        static MainWindowViewModel instance;
        return instance;
    }
    ~MainWindowViewModel();

    void AddFrame(Frame* frame);
    void RemoveFrame(int index);
    void RemoveSelectedFrame();
    void ClearFrames();
    void SetFrames(const std::vector<Frame*> frames);
    Frame* GetFrame(int index);
    Frame* GetSelectedFrame();
    std::vector<Frame*>& GetFrames();

    int GetSelectedFrameIndex() const;
    void SelectFrame(size_t index);
    void DeselectAllFrames();
    void SelectLastFrame();

private:
    MainWindowViewModel();

private:
    std::vector<Frame*> frames;
    int selectedFrameIndex;
};

#endif // MAINWINDOWVIEWMODEL_H

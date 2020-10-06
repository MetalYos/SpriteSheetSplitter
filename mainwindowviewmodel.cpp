#include "mainwindowviewmodel.h"

MainWindowViewModel::MainWindowViewModel()
    : selectedFrameIndex(-1)
{

}

MainWindowViewModel::~MainWindowViewModel()
{
    ClearFrames();
}

void MainWindowViewModel::AddFrame(Frame* frame)
{
    frames.push_back(frame);
    SelectLastFrame();
}

void MainWindowViewModel::RemoveFrame(int index)
{
    if (index < 0 || index >= (int)frames.size())
        return;

    int counter = 0;
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
        if (counter == index)
        {
            frames.erase(it);
            SelectLastFrame();
            break;
        }
        counter++;
    }
}

void MainWindowViewModel::RemoveSelectedFrame()
{
    RemoveFrame(selectedFrameIndex);
}

void MainWindowViewModel::ClearFrames()
{
    while (frames.size() > 0)
    {
        Frame* frame = frames.back();
        frames.pop_back();
        delete frame;
    }

    selectedFrameIndex = -1;
}

void MainWindowViewModel::SetFrames(const std::vector<Frame*> frames)
{
    if (frames.size() > 0)
        ClearFrames();

    for (size_t i = 0; i < frames.size(); i++)
        AddFrame(frames[i]);
}

Frame* MainWindowViewModel::GetFrame(int index)
{
    if (index < 0 || index >= (int)frames.size())
        return nullptr;

    return frames[index];
}

std::vector<Frame*>& MainWindowViewModel::GetFrames()
{
    return frames;
}

Frame* MainWindowViewModel::GetSelectedFrame()
{
    if (selectedFrameIndex < 0)
        return nullptr;
    return frames[selectedFrameIndex];
}

int MainWindowViewModel::GetSelectedFrameIndex() const
{
    return selectedFrameIndex;
}

void MainWindowViewModel::SelectFrame(size_t index)
{
    if (index < 0 || index > frames.size())
        return;

    selectedFrameIndex = index;
}

void MainWindowViewModel::DeselectAllFrames()
{
    selectedFrameIndex = -1;
}

void MainWindowViewModel::SelectLastFrame()
{
    selectedFrameIndex = frames.size() - 1;
}

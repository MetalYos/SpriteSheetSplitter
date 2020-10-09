#include "mainwindowviewmodel.h"
#include "framedetection.h"

MainWindowViewModel::MainWindowViewModel()
    : selectedFrameIndex(-1)
{

}

MainWindowViewModel::~MainWindowViewModel()
{
    ClearFrames();
}

bool MainWindowViewModel::LoadImage(const std::string& filepath)
{
    if (imageRaw != nullptr)
        delete imageRaw;

    imageRaw = Image::LoadImage(filepath);
    if (imageRaw == nullptr)
        return false;

    FrameDetection::Instance().SetImageData(imageRaw);
    return true;
}

const Image* MainWindowViewModel::GetImage() const
{
    return imageRaw;
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

void MainWindowViewModel::SetFrames(const std::vector<Frame*>& frames)
{
    if (frames.size() == 0)
        return;

    ClearFrames();
    for (size_t i = 0; i < frames.size(); i++)
        AddFrame(frames[i]);
    selectedFrameIndex = 0;
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

int MainWindowViewModel::GetFrameIndex(Frame* frame) const
{
    for (size_t i = 0; i < frames.size(); i++)
    {
        if (frame == frames[i])
            return i;
    }
    return 0;
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

void MainWindowViewModel::SetFrameDetectionParameters(int tolerance)
{
    FrameDetection::Instance().SetImageData(imageRaw);
    FrameDetection::Instance().SetTolerance(tolerance);
}

void MainWindowViewModel::SetFrameDetectionParameters(const std::vector<GraphicsUtils::PixelColor>& backgroundColor, int tolerance)
{
    FrameDetection::Instance().SetParameters(imageRaw, backgroundColor, tolerance);
}

bool MainWindowViewModel::DetectFrame()
{
    auto frame = GetSelectedFrame();
    if (frame == nullptr)
        return false;

    Frame* temp = FrameDetection::Instance().DetectFrame(frame->OriginAtParentCoords().first,
                                                         frame->OriginAtParentCoords().second);
    if (temp != nullptr)
    {
        frame->SetFrame(temp->Top(), temp->Left(), temp->Bottom(), temp->Right());
        delete temp;
        return true;
    }
    else
        return false;
}

bool MainWindowViewModel::DetectAllFrames()
{
    if (imageRaw == nullptr)
        return false;

    auto frames = FrameDetection::Instance().DetectAllFrames();
    SetFrames(frames);
    return true;
}

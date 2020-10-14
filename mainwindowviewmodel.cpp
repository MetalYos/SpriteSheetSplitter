#include "mainwindowviewmodel.h"
#include "framedetection.h"
#include <algorithm>

MainWindowViewModel::MainWindowViewModel()
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

void MainWindowViewModel::RemoveSelectedFrames()
{
    while (selectedFrameIndices.size() > 0)
    {
        int index = selectedFrameIndices.back();
        selectedFrameIndices.pop_back();
        RemoveFrame(index);
    }
}

void MainWindowViewModel::ClearFrames()
{
    while (frames.size() > 0)
    {
        Frame* frame = frames.back();
        frames.pop_back();
        delete frame;
    }

    selectedFrameIndices.clear();
}

void MainWindowViewModel::SetFrames(const std::vector<Frame*>& frames)
{
    if (frames.size() == 0)
        return;

    ClearFrames();
    for (size_t i = 0; i < frames.size(); i++)
        AddFrame(frames[i]);

    selectedFrameIndices.push_back(0);
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

void MainWindowViewModel::SetSelectedFrames(const std::vector<int>& framesIndices)
{
    selectedFrameIndices = framesIndices;
}

std::vector<Frame*> MainWindowViewModel::GetSelectedFrames()
{
    std::vector<Frame*> output;

    for (auto index : selectedFrameIndices)
        output.push_back(GetFrame(index));

    return output;
}

std::vector<int> MainWindowViewModel::GetSelectedFrameIndices() const
{
    return selectedFrameIndices;
}

void MainWindowViewModel::SelectFrame(size_t index, bool only)
{
    if (index < 0 || index > frames.size())
        return;

    if (only)
        selectedFrameIndices.clear();

    if (!IsFrameSelected(index))
        selectedFrameIndices.push_back(index);
}

void MainWindowViewModel::DeselectAllFrames()
{
    selectedFrameIndices.clear();
}

void MainWindowViewModel::SelectLastFrame(bool only)
{
    if (only)
        selectedFrameIndices.clear();
    selectedFrameIndices.push_back(frames.size() - 1);
}

bool MainWindowViewModel::IsFrameSelected(int index) const
{
    for (auto i : selectedFrameIndices)
    {
        if (i == index)
            return true;
    }
    return false;
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

void MainWindowViewModel::DetectSelectedFrames()
{
    if (imageRaw == nullptr)
        return;

    auto frames = GetSelectedFrames();
    for (auto frame : frames)
    {
        Frame* temp = FrameDetection::Instance().DetectFrame(frame->OriginAtParentCoords().first,
                                                             frame->OriginAtParentCoords().second);

        if (temp != nullptr)
        {
            frame->SetFrame(temp->Top(), temp->Left(), temp->Bottom(), temp->Right());
            delete temp;
        }
    }
}

void MainWindowViewModel::DetectAllFrames()
{
    if (imageRaw == nullptr)
        return;

    auto frames = FrameDetection::Instance().DetectAllFrames();
    std::sort(frames.begin(), frames.end(), FrameComparer());

    SetFrames(frames);
}

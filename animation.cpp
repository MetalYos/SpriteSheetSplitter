#include "animation.h"
#include "utils.h"

Animation::Animation()
    : isLooped(true), currentFrameIndex(-1),
      currentState(AnimationStates::STOPPED)
{

}

Animation::Animation(const std::vector<Frame*>& frames)
    : frames(frames), isLooped(true),
      currentState(AnimationStates::STOPPED)
{
    if (frames.size() > 0)
        currentFrameIndex = 0;
    else
        currentFrameIndex = -1;
}

Animation::~Animation()
{

}

bool Animation::IsLooped() const
{
    return isLooped;
}

void Animation::SetLooped(bool isLooped)
{
    isLooped = true;
}

Animation::AnimationStates Animation::GetState() const
{
    return currentState;
}

void Animation::SetState(AnimationStates state)
{
    currentState = state;
}

int Animation::GetCurrentFrameIndex() const
{
    return currentFrameIndex;
}

Frame* Animation::GetCurrentFrame() const
{
    if (currentFrameIndex < 0)
        return nullptr;
    return frames[currentFrameIndex];
}

size_t Animation::GetNumberOfFrames() const
{
    return frames.size();
}

void Animation::GoToStart()
{
    if (frames.size() == 0)
        return;
    currentFrameIndex = 0;
}

void Animation::GoToEnd()
{
    if (frames.size() == 0)
        return;
    currentFrameIndex = frames.size() - 1;
}

void Animation::GoToNextFrame()
{
    if (frames.size() == 0)
        return;

    if (isLooped)
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
    else
        currentFrameIndex = MathUtils::Min<int>(currentFrameIndex + 1, frames.size() - 1);
}

void Animation::GoToPreviousFrame()
{
    if (frames.size() == 0)
        return;

    if (isLooped)
        currentFrameIndex = (currentFrameIndex - 1) % frames.size();
    else
        currentFrameIndex = MathUtils::Max<int>(currentFrameIndex - 1, 0);
}
